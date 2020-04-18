#include "stdafx.h"
#include "Tile.h"

#include "Export_Function.h"

CTile::CTile(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTile::~CTile(void)
{

}

HRESULT CTile::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTile::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix			matWorld, matView;

	m_pTransformCom->GetWorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	return 0;
}

void CTile::Render_GameObject(void)
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());
		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pTextureCom->Render_Texture();
		m_pBufferCom->Render_Buffer();

		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

const _vec3* CTile::Get_Pos()
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

const bool CTile::Get_Render()
{
	return m_bIsRender;
}

void CTile::Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CTile::Set_Pos(const _vec3& vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CTile::Set_Render(const bool& bIsRender)
{
	m_bIsRender = bIsRender;
}


HRESULT CTile::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTileTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TileTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Tile"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);
	
	return S_OK;
}



CTile* CTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTile*	pInstance = new CTile(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTile::Free(void)
{
	Engine::CGameObject::Free();
}


