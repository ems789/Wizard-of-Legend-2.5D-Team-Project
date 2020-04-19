#include "stdafx.h"
#include "Town.h"
#include "Player.h"
#include "CastingCircle.h"
#include "FirstViewCamera.h"
#include "ThirdViewCamera.h"
#include "QuaterViewCamera.h"
#include "UI.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "TestTerrain.h"
#include "TestMonster.h"
#include "Mouse.h"
#include "Golem.h"

CTown::CTown(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CTown::~CTown()
{

}

HRESULT CTown::Ready_Scene()
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_StaticLayer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Camera(), E_FAIL);
	//	FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);
	CUI::GetInstance()->ShowOnUI();

	return S_OK;
}

_int CTown::Update_Scene(const _float& fTimeDelta)
{
	if (Engine::KeyDown(DIK_F7))
	{
		//	TestMonster
		Engine::CGameObject* pGameObject = CTestMonster::Create(m_pGraphicDev, &_vec3(rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		Add_GameObject(L"GameLogic", L"Monster", pGameObject);

	}


	_int iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CTown::Render_Scene()
{

}

HRESULT CTown::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), Engine::CTag_Finder(pLayerTag));

	if (m_mapLayer.end() != iter)
	{
		_tchar szText[256] = L"";
		wsprintf(szText, pLayerTag, L" already exist.");
		MessageBox(g_hWnd, szText, L"Ready_GameLogic_Layer Failed", MB_OK);
		return E_FAIL;
	}

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//	Terrain
	/*pGameObject = CTestTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"TestTerrain", pGameObject);*/

	pGameObject = CTerrain::Create(m_pGraphicDev, L"../../Data/MapData.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Terrain", pGameObject);

	//	TestMonster
	pGameObject = CTestMonster::Create(m_pGraphicDev, &_vec3(20.f, 1.f, 20.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Monster", pGameObject);

	pGameObject = CGolem::Create(m_pGraphicDev, &_vec3(20.f, 1.f, 25.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Monster", pGameObject);




	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTown::Ready_UI_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);

	//pGameObject = CPlayerHP::Create(m_pGraphicDev, &_vec3(2.f, 2.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"PlayerHP", pGameObject);

	//pGameObject = CCastingCircle::Create(m_pGraphicDev, 200.f, &_vec3(0.f, 0.f, 0.f), Engine::RENDER_UI);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"CastingCircle", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTown::Ready_StaticLayer()
{
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	Engine::Add_GameObjectToStaticLayer(L"Player", pPlayer);
	FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);
	return S_OK;
}

HRESULT CTown::Ready_Camera()
{
	Engine::CCamera* pCamera = nullptr;

	pCamera = CFirstViewCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	Engine::Add_BasicCamera(0, L"First_View_Camera", pCamera);

	pCamera = CThirdViewCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	Engine::Add_BasicCamera(1, L"Third_View_Camera", pCamera);

	pCamera = CQuaterViewCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	Engine::Add_BasicCamera(2, L"Quater_View_Camera", pCamera);

	Engine::SetUp_MainCamera(Engine::CAM_STATIC, L"Third_View_Camera");
	CMouse::GetInstance()->AnimingPointOn();

	return S_OK;
}

HRESULT CTown::UI_Setting()
{


	return S_OK;
}

HRESULT CTown::Loading_Data(const _tchar * pPath)
{



	return S_OK;
}

CTown* CTown::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTown* pInstance = new CTown(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTown::Free()
{
	CScene::Free();
}

