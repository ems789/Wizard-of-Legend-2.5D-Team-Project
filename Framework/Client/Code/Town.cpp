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
#include "Cyclops.h"
#include "FireBoss.h"
#include "CardSpawn.h"

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
	FAILED_CHECK_RETURN(Ready_Effect_Layer(L"Effect"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Monster_Layer(L"Monster"), E_FAIL);
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
		//	FireBoss
		_vec3 vCardPos = { rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f };
		Engine::CGameObject* pGameObject = CFireBoss::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	}

	if (Engine::KeyDown(DIK_F6))
	{
		//	Cyclops
		_vec3 vCardPos = { rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f };
		Engine::CGameObject* pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	}

	if (Engine::KeyDown(DIK_F8))
	{
		//	Golem
		_vec3 vCardPos = { rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f };
		Engine::CGameObject* pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	}


	_int iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CTown::Render_Scene()
{
#ifdef _DEBUG
	const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vPlayerPos = *pPlayerTransform->GetInfo(Engine::INFO_POS);

	wstring wstrPos = L"Player X : " + to_wstring(vPlayerPos.x);
	Engine::Render_Font(L"Font_Default", wstrPos.c_str(), &_vec2(10.f, 100.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	wstrPos = L"Player Y : " + to_wstring(vPlayerPos.y);
	Engine::Render_Font(L"Font_Default", wstrPos.c_str(), &_vec2(10.f, 120.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	wstrPos = L"Player Z : " + to_wstring(vPlayerPos.z);
	Engine::Render_Font(L"Font_Default", wstrPos.c_str(), &_vec2(10.f, 140.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

#endif // _DEBUG
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

	pGameObject = CTerrain::Create(m_pGraphicDev, L"../Bin/Data/FireStageTile.dat", L"../Bin/Data/FireStageWall_5.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Terrain", pGameObject);

	//	TestMonster
	//pGameObject = CTestMonster::Create(m_pGraphicDev, &_vec3(20.f, 1.f, 20.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster", pGameObject);

	//pGameObject = CGolem::Create(m_pGraphicDev, &_vec3(20.f, 1.f, 25.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster", pGameObject);

	//pGameObject = CFireBoss::Create(m_pGraphicDev, &_vec3(10.f, 1.f, 10.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster", pGameObject);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTown::Ready_Monster_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;
	//pGameObject = CFireBoss::Create(m_pGraphicDev, &_vec3(10.f, 1.f, 10.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster", pGameObject);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTown::Ready_Effect_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTown::Ready_UI_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTown::Ready_StaticLayer()
{
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	pPlayer->Set_PosX(50.f);
	pPlayer->Set_PosZ(37.5f);
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

	Engine::SetUp_MainCamera(Engine::CAM_STATIC, L"Quater_View_Camera");
	//CMouse::GetInstance()->AnimingPointOff();

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

