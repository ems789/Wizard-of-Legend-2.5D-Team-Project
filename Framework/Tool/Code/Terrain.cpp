#include "stdafx.h"
#include "Terrain.h"
#include "Tile.h"

#include "Export_Function.h"

USING(Engine)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain(void)
{
}

//HRESULT CTerrain::Ready_GameObject(void)
//{
//	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
//
//	return S_OK;
//}

HRESULT CTerrain::Ready_GameObject(::_ulong dwTileX, ::_ulong dwTileZ, ::_ulong dwItv)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	float fX = 0.f, fZ = 0.f;

	m_dwTileX = dwTileX;
	m_dwTileZ = dwTileZ;
	m_dwItv = dwItv;
	m_vecTile.reserve(m_dwTileX * m_dwTileZ);

	CTile* pTile = nullptr;
	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for(::_ulong j=0; j<m_dwTileX; ++j)
		{			
			fX = (j * m_dwItv) + (m_dwItv * 0.5f);
			fZ = (i * m_dwItv) + (m_dwItv * 0.5f);

			pTile = CTile::Create(m_pGraphicDev);
			pTile->Set_Pos(fX, 0.f, fZ);
			pTile->Set_Render(false);

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

::_int CTerrain::Update_GameObject(const ::_float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for (::_ulong j = 0; j < m_dwTileX; ++j)
			m_vecTile[i * m_dwTileX + j]->Update_GameObject(fTimeDelta);
	}

	return 0;
}

void CTerrain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for (::_ulong j = 0; j < m_dwTileX; ++j)
			m_vecTile[i * m_dwTileX + j]->Render_GameObject();
	}

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTerrain::Release_GameObject(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

void CTerrain::TileChange(const ::_vec3& vPos, const ::_tchar* tileTag, bool bIsRender)
{
	m_vecTile[(int)vPos.z * m_dwTileX + (int)vPos.x]->Set_Render(bIsRender);
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}



CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, ::_ulong dwTileX, ::_ulong dwTileZ, ::_ulong dwItv)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(dwTileX, dwTileZ, dwItv)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

