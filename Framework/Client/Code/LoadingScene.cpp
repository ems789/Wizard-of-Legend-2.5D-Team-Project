#include "stdafx.h"
#include "LoadingScene.h"

#include "Export_Function.h"
#include "CastingCircle.h"
#include "Town.h"

CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

CLoadingScene::~CLoadingScene()
{

}

HRESULT CLoadingScene::Ready_Scene()
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	FAILED_CHECK_RETURN(Ready_Effect_Layer(L"Effect"), E_FAIL);

	return S_OK;
}

_int CLoadingScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (Engine::KeyDown(DIK_RETURN))
		{
			CTown* pScene = CTown::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);

			Engine::SetUp_Scene(pScene);

			return 1;
		}
	}
	return iExit;
}

void CLoadingScene::Render_Scene()
{
	
}

HRESULT CLoadingScene::Ready_Effect_Layer(const _tchar * pLayerTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), Engine::CTag_Finder(pLayerTag));
	if (m_mapLayer.end() != iter)
	{
		MSG_BOX("Duplicated Layer Name.");
		return E_FAIL;
	}

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//	CastingCircle
	pGameObject = CCastingCircle::Create(m_pGraphicDev, 200.f, &_vec3(WINCX / 2 - 200.f, -WINCY / 2 + 200.f, 0.f), Engine::RENDER_UI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CastingCircle", pGameObject), E_FAIL);
	
	m_mapLayer.emplace(L"Effect", pLayer);

	return S_OK;
}

CLoadingScene* CLoadingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingScene* pInstance = new CLoadingScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLoadingScene::Free()
{
	Engine::Safe_Release(m_pLoading);
	CScene::Free();
}