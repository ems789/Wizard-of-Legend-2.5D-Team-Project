#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CTerrain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	return S_OK;
}

void CTerrain::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(Engine::INFO_UP, &m_vDir);

	/*if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	}*/

	
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();
}

