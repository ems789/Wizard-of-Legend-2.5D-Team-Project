#include "stdafx.h"
#include "Wall.h"

#include "Export_Function.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CWall::~CWall(void)
{

}

HRESULT CWall::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CWall::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix			matWorld, matView;

	m_pTransformCom->GetWorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	return 0;
}

void CWall::Render_GameObject(void)
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

		m_pTextureCom->Render_Texture(m_iDrawID);
		if (m_bHasWall[WALL_LEFT])
			m_pLeftWallBufferCom->Render_Buffer();
		if (m_bHasWall[WALL_TOP])
			m_pTopWallBufferCom->Render_Buffer();
		if (m_bHasWall[WALL_RIGHT])
			m_pRightWallBufferCom->Render_Buffer();
		if (m_bHasWall[WALL_BOTTOM])
			m_pBottomWallBufferCom->Render_Buffer();
	}
}

const _vec3* CWall::Get_Pos()
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

const bool CWall::Get_Render()
{
	return m_bIsRender;
}

const int CWall::Get_DrawID()
{
	return m_iDrawID;
}

void CWall::Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CWall::Set_Pos(const _vec3& vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CWall::Set_Render(const bool& bIsRender)
{
	m_bIsRender = bIsRender;
}

void CWall::Set_WallRender(bool bHasLeftWall, bool bHasTopWall, bool bHasRightWall, bool bHasBottomWall)
{
	m_bHasWall[WALL_LEFT] = bHasLeftWall;
	m_bHasWall[WALL_TOP] = bHasTopWall;
	m_bHasWall[WALL_RIGHT] = bHasRightWall;
	m_bHasWall[WALL_BOTTOM] = bHasBottomWall;
}

void CWall::Set_DrawID(const int iDrawID)
{
	m_iDrawID = iDrawID;
}

HRESULT CWall::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// 상, 하, 좌, 우를 이루는 버퍼 4개
	if (m_bHasWall[WALL_LEFT])
	{
		pComponent = m_pLeftWallBufferCom = dynamic_cast<Engine::CLeftWallTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LeftWallTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_LeftWallBuffer", pComponent);
	}

	if (m_bHasWall[WALL_TOP])
	{
		pComponent = m_pTopWallBufferCom = dynamic_cast<Engine::CTopWallTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TopWallTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_TopWallBuffer", pComponent);
	}

	if (m_bHasWall[WALL_RIGHT])
	{
		pComponent = m_pRightWallBufferCom = dynamic_cast<Engine::CRightWallTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RightWallTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_RightWallBuffer", pComponent);
	}

	if (m_bHasWall[WALL_BOTTOM])
	{
		pComponent = m_pBottomWallBufferCom = dynamic_cast<Engine::CBottomWallTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_BottomWallTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_BottomWallBuffer", pComponent);
	}

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Wall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);
	
	return S_OK;
}


CWall* CWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, bool bHasLeftWall, bool bHasTopWall, bool bHasRightWall, bool bHasBottomWall)
{
	CWall*	pInstance = new CWall(pGraphicDev);
	
	pInstance->Set_WallRender(bHasLeftWall, bHasTopWall, bHasRightWall, bHasBottomWall);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CWall::Free(void)
{
	Engine::CGameObject::Free();
}


