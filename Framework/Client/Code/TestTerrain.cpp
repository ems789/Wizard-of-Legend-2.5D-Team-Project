#include "stdafx.h"
#include "TestTerrain.h"

#include "Export_Function.h"

CTestTerrain::CTestTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTestTerrain::~CTestTerrain()
{

}

HRESULT CTestTerrain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_GameObject(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTestTerrain::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return iExit;
}

void CTestTerrain::Render_GameObjcet()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

}

HRESULT CTestTerrain::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_TerrainTex"));
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"TestTerrain Add_Component failed. m_pBufferCom is nullptr.");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"TestTerrain Add_Component failed. m_pTextureCom is nullptr.");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"TestTerrain Add_Component failed. m_pTransformCom is nullptr.");
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"TestTerrain Add_Component failed. m_pRenderer is nullptr");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();

	return S_OK;
}

CTestTerrain* CTestTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestTerrain*	pInstance = new CTestTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTestTerrain::Free()
{
	Engine::CGameObject::Free();
}