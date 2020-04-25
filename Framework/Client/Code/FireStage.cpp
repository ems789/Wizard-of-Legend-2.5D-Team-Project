#include "stdafx.h"
#include "FireStage.h"
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
#include "LightningBoss.h"
#include "CardSpawn.h"
#include "RoomBlock.h"

CFireStage::CFireStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CFireStage::~CFireStage()
{

}

HRESULT CFireStage::Ready_Scene()
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

	Engine::StopAll();

	Engine::PlayBGM(L"Fire.wav");

	return S_OK;
}

_int CFireStage::Update_Scene(const _float& fTimeDelta)
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

	//if (Engine::KeyDown(DIK_F6))
	//{
	//	//	Cyclops
	//	_vec3 vCardPos = { rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f };
	//	Engine::CGameObject* pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	//	CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
	//	Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	//}

	//if (Engine::KeyDown(DIK_F8))
	//{
	//	//	Golem
	//	_vec3 vCardPos = { rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f };
	//	Engine::CGameObject* pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	//	CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
	//	Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	//}

	//if (Engine::KeyDown(DIK_F9))
	//{
	//	// LightningBoss
	//	_vec3 vCardPos = { rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f };
	//	Engine::CGameObject* pGameObject = CLightningBoss::Create(m_pGraphicDev, &_vec3(rand() % 20 + 10.f, 1.f, rand() % 20 + 10.f));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	//	CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
	//	Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	//}

	const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

	FirstMonsterGen(pPlayerPos);
	SecondMonsterGen(pPlayerPos);
	ThirdMonsterGen(pPlayerPos);
	FourthMonsterGen(pPlayerPos);

	_int iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CFireStage::Render_Scene()
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

HRESULT CFireStage::Ready_GameLogic_Layer(const _tchar * pLayerTag)
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

HRESULT CFireStage::Ready_Monster_Layer(const _tchar * pLayerTag)
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

HRESULT CFireStage::Ready_Effect_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CFireStage::Ready_UI_Layer(const _tchar * pLayerTag)
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

HRESULT CFireStage::Ready_StaticLayer()
{
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	pPlayer->Set_PosX(50.f);
	pPlayer->Set_PosZ(37.5f);
	Engine::Add_GameObjectToStaticLayer(L"Player", pPlayer);
	FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);
	return S_OK;
}

HRESULT CFireStage::Ready_Camera()
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

HRESULT CFireStage::UI_Setting()
{


	return S_OK;
}

void CFireStage::FirstMonsterGen(const _vec3* pPlayerPos)
{
	if (m_b1stMonsterGen)
		return;
	
	if (pPlayerPos->x >= 22.f && pPlayerPos->x <= 35.f &&
		pPlayerPos->z >= 22.f && pPlayerPos->z <= 27.f)
	{
		Engine::CGameObject* pGameObject = nullptr;
		CCardSpawn* pCardSpawn = nullptr;

		_vec3 vCardPos = { 23.f, 1.f, 25.f };
		pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 27.f, 1.f, 25.f };
		pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 23.f, 1.f, 22.f };
		pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);


		vCardPos = { 27.f, 1.f, 22.f };
		pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(34., 2.f, 29.f), CRoomBlock::BLOCK_DOWN);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(22.5, 2.f, 18.f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);

		

		m_b1stMonsterGen = true;
	}


}

void CFireStage::SecondMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b2ndMonsterGen)
		return;

	if (pPlayerPos->x >= 3.f && pPlayerPos->x <= 16.f &&
		pPlayerPos->z >= 5.f && pPlayerPos->z <= 16.f)
	{
		Engine::CGameObject* pGameObject = nullptr;
		CCardSpawn* pCardSpawn = nullptr;

		_float	fx = rand() % 9 + 4.f;
		_float	fz = rand() % 7 + 6.f;
		_vec3 vCardPos = { fx, 1.f, fz };
		pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		fx = rand() % 9 + 4.f;
		fz = rand() % 7 + 6.f;
		vCardPos = { fx, 1.f, fz };
		pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		fx = rand() % 9 + 4.f;
		fz = rand() % 7 + 6.f;
		vCardPos = { fx, 1.f, fz };
		pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		fx = rand() % 9 + 4.f;
		fz = rand() % 7 + 6.f;
		vCardPos = { fx, 1.f, fz };
		pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(9.5f, 2.f, 18.5f), CRoomBlock::BLOCK_DOWN);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(19.f, 2.f, 9.5f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);



		m_b2ndMonsterGen = true;
	}
}

void CFireStage::ThirdMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b3rdMonsterGen)
		return;

	_float	fMinX = 2.5f, fMaxX = 14.5f;
	_float	fMinZ = 40.f, fMaxZ = 54.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{
		Engine::CGameObject* pGameObject = nullptr;
		CCardSpawn* pCardSpawn = nullptr;

		//_float	fx = rand() % 9 + 4.f;
		//_float	fz = rand() % 7 + 6.f;
		_vec3 vCardPos = { rand() % (_int)(fMaxX - fMinX - 2.f) + fMinX + 1.f, 1.f, rand() % (_int)(fMaxZ - fMinZ - 2.f) + fMinZ + 1.f };
		pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { rand() % (_int)(fMaxX - fMinX - 2.f) + fMinX + 1.f, 1.f, rand() % (_int)(fMaxZ - fMinZ - 2.f) + fMinZ + 1.f };
		pGameObject = CCyclops::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { rand() % (_int)(fMaxX - fMinX - 2.f) + fMinX + 1.f, 1.f, rand() % (_int)(fMaxZ - fMinZ - 2.f) + fMinZ + 1.f };
		pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);


		vCardPos = { rand() % (_int)(fMaxX - fMinX - 2.f) + fMinX + 1.f, 1.f, rand() % (_int)(fMaxZ - fMinZ - 2.f) + fMinZ + 1.f };
		pGameObject = CGolem::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(16.5f, 2.f, 46.f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(9.5f, 2.f, 36.5f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);



		m_b3rdMonsterGen = true;
	}
}

void CFireStage::FourthMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b4thMonsterGen)
		return;


	_float	fMinX = 71.f, fMaxX = 90.f;
	_float	fMinZ = 26.f, fMaxZ = 49.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{

		_vec3 vCardPos = { 82.f, 1.f, 36.f};
		Engine::CGameObject* pGameObject = CFireBoss::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(68.5, 2.f, 38.f), CRoomBlock::BLOCK_RIGHT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(93.5f, 2.f, 37.f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);

		m_b4thMonsterGen = true;
	}


}

void CFireStage::FifthMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b5thMonsterGen)
		return;
}

CFireStage* CFireStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireStage* pInstance = new CFireStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFireStage::Free()
{
	CScene::Free();
}

