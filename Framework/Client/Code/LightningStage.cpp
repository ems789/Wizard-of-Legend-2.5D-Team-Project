#include "stdafx.h"
#include "LightningStage.h"
#include "Player.h"
#include "CastingCircle.h"
#include "FirstViewCamera.h"
#include "ThirdViewCamera.h"
#include "QuaterViewCamera.h"
#include "UI.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Mouse.h"
#include "Mage.h"
#include "Knight.h"
#include "LightningBoss.h"
#include "WindBoss.h"
#include "CardSpawn.h"
#include "RoomBlock.h"
#include "BlobSpitter.h"
#include "GhoulLarge.h"
#include "BlobRoller.h"
#include "SkillCard.h"
#include "FireBall.h"
#include "MeteorStrike.h"
#include "GuidedFireBall.h"
#include "AquaVortex.h"
#include "WaterBall.h"
#include "SharkPool.h"
#include "ImageObject.h"
#include "Barrel.h"
#include "FireStomp.h"
#include "LightningFist.h"
#include "GoldBox.h"
#include "Coin.h"
#include "Scaffold.h"
#include "LaidObject.h"
#include "BasicEffect.h"
#include "SkeletonMinion.h"
#include "Portal.h"
#include "FireStage.h"

CLightningStage::CLightningStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CLightningStage::~CLightningStage()
{

}

HRESULT CLightningStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_StaticLayer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Effect_Layer(L"Effect"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Monster_Layer(L"Monster"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Camera(), E_FAIL);
	//	FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);
	FAILED_CHECK_RETURN(Store_Setting(), E_FAIL);
	CUI::GetInstance()->ShowOnUI();

	Engine::StopAll();

	Engine::PlayBGM(L"Lightning.wav");

	return S_OK;
}

_int CLightningStage::Update_Scene(const _float& fTimeDelta)
{
	if (Engine::KeyDown(DIK_B))
	{
		CFireStage* pFireStage = CFireStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pFireStage, -1);
		Engine::SetUp_Scene(pFireStage);
		return 1;
	}

	if (Engine::KeyDown(DIK_F9))
	{
		// LightningBoss
		_vec3 vCardPos = { 50, 1.f, 37 };
		Engine::CGameObject* pGameObject = CLightningBoss::Create(m_pGraphicDev, &_vec3(50, 1.f, 37));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);
	}

	const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

	/*if (Engine::KeyDown(DIK_O))
	{
		CUI::GetInstance()->ShowOnBossUI();
		CWindBoss* pBoss = CWindBoss::Create(m_pGraphicDev, &_vec3(50.f, 1.f, 37.f));
		Add_GameObject(L"Monster", L"Boss", pBoss);

		CUI::GetInstance()->Render_BossUI();
	}*/

	Room_State_Update(fTimeDelta);

	_int iExit = CScene::Update_Scene(fTimeDelta);

	/// 포탈 씬 교체
	if (pPlayerPos->x >= 85.f && pPlayerPos->x <= 86.f &&
		pPlayerPos->z >= 50.f && pPlayerPos->z <= 51.f)
	{
	}


	return iExit;
}

void CLightningStage::Render_Scene()
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

HRESULT CLightningStage::Ready_GameLogic_Layer(const _tchar * pLayerTag)
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

	pGameObject = CTerrain::Create(m_pGraphicDev, L"../Bin/Data/LightningTile.dat", L"../Bin/Data/LightningWall.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Terrain", pGameObject);

	//	Portal
	CPortal* pPortal = CPortal::Create(m_pGraphicDev, &_vec3(86.f, 1.5f, 51.f));
	pLayer->Add_GameObject(L"Portal", pPortal);

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

HRESULT CLightningStage::Ready_Monster_Layer(const _tchar * pLayerTag)
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

HRESULT CLightningStage::Ready_Effect_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLightningStage::Ready_UI_Layer(const _tchar * pLayerTag)
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

HRESULT CLightningStage::Ready_StaticLayer()
{
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	pPlayer->Set_PosX(52.f);
	pPlayer->Set_PosZ(45.f);
	Engine::Add_GameObjectToStaticLayer(L"Player", pPlayer);
	FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);

	return S_OK;
}

HRESULT CLightningStage::Ready_Camera()
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

HRESULT CLightningStage::UI_Setting()
{


	return S_OK;
}

void CLightningStage::FirstMonsterGen(const _vec3* pPlayerPos)
{
	if (m_b1stMonsterGen)
		return;
	
	if (pPlayerPos->x >= 20.f && pPlayerPos->x <= 27.f &&
		pPlayerPos->z >= 48.f && pPlayerPos->z <= 53.f)
	{
		Engine::CGameObject* pGameObject = nullptr;
		CCardSpawn* pCardSpawn = nullptr;

		_vec3 vCardPos = { 23.f, 1.f, 51.f };
		pGameObject = CKnight::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 19.f, 1.f, 45.f };
		pGameObject = CKnight::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 25.f, 1.f, 45.f };
		pGameObject = CKnight::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);


		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(31.f, 1.5f, 51.5f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(24.5, 1.5f, 41.f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_1;

		RoomBlock_Close();

		m_b1stMonsterGen = true;
	}


}

void CLightningStage::SecondMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b2ndMonsterGen)
		return;

	if (pPlayerPos->x >= 10.f && pPlayerPos->x <= 16.f &&
		pPlayerPos->z >= 27.f && pPlayerPos->z <= 32.f)
	{
		Engine::CGameObject* pGameObject = nullptr;
		CCardSpawn* pCardSpawn = nullptr;

		_vec3 vCardPos = { 8.f, 1.f, 31.f };
		pGameObject = CMage::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 8.f, 1.f, 26.f };
		pGameObject = CMage::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 13.f, 1.f, 31.f };
		pGameObject = CKnight::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		vCardPos = { 13.f, 1.f, 26.f };
		pGameObject = CKnight::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(18.5f, 1.5f, 29.5f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_2;

		RoomBlock_Close();

		m_b2ndMonsterGen = true;
	}
}

void CLightningStage::ThirdMonsterGen(const _vec3 * pPlayerPos)
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

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(16.5f, 1.5f, 46.f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(9.5f, 1.5f, 36.5f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_b3rdMonsterGen = true;
	}
}

void CLightningStage::FourthMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b4thMonsterGen)
		return;


	_float	fMinX = 71.f, fMaxX = 90.f;
	_float	fMinZ = 26.f, fMaxZ = 49.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{

		_vec3 vCardPos = { 82.f, 1.f, 36.f};
		Engine::CGameObject* pGameObject = CWindBoss::Create(m_pGraphicDev, &vCardPos);
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

void CLightningStage::FifthMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b5thMonsterGen)
		return;

	_float	fMinX = 28.f, fMaxX = 34.f;
	_float	fMinZ = 75.f, fMaxZ = 85.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{

		_vec3 vCardPos = { 23.f, 1.f, 80.5f };
		Engine::CGameObject* pGameObject = CWindBoss::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(38.5f, 1.5f, 80.5f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);


		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_5;

		RoomBlock_Close();

		m_b5thMonsterGen = true;
	}
}

void CLightningStage::SixMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b6thMonsterGen)
		return;

	_float	fMinX = 69.f, fMaxX = 76.f;
	_float	fMinZ = 82.f, fMaxZ = 87.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{

		_vec3 vCardPos = { 72.5f, 1.f, 88.f };
		Engine::CGameObject* pGameObject = CLightningBoss::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );

		CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.f, 0.f), &_vec3(72.5f, 1.5f, 80.5f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);


		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_6;

		RoomBlock_Close();

		m_b6thMonsterGen = true;
	}
}

_int CLightningStage::Room_State_Update(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CLightningStage::ROOM_IDLE:
		Room_Idle_Update(fTimeDelta);
		break;
	case CLightningStage::ROOM_CLOSE:
		Room_Close_Update(fTimeDelta);
		break;
		//case CFireStage::ROOM_OPEN:
		//	Room_Open_Update(fTimeDelta);
		//	break;
	}

	return _int();
}

_int CLightningStage::Room_Idle_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

	FirstMonsterGen(pPlayerPos);
	SecondMonsterGen(pPlayerPos);
	ThirdMonsterGen(pPlayerPos);
	FourthMonsterGen(pPlayerPos);
	FifthMonsterGen(pPlayerPos);
	SixMonsterGen(pPlayerPos);

	return _int();
}

_int CLightningStage::Room_Close_Update(const _float & fTimeDelta)
{
	switch (m_eRoomNumber)
	{
	case CLightningStage::ROOM_NUM_1:
		FirstRoom_Update(fTimeDelta);
		break;
	case CLightningStage::ROOM_NUM_2:
		SecondRoom_Update(fTimeDelta);
		break;
	case CLightningStage::ROOM_NUM_3:
		ThirdRoom_Update(fTimeDelta);
		break;
	case CLightningStage::ROOM_NUM_5:
		FirstRoom_Update(fTimeDelta);
		break;
	case CLightningStage::ROOM_NUM_6:
		SixRoom_Update(fTimeDelta);
		break;		
	}

	return 0;
}

_int CLightningStage::FirstRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();

	return 0;
}

_int CLightningStage::SecondRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();
	return 0;
}

_int CLightningStage::ThirdRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();
	return 0;
}

_int CLightningStage::FifthRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();
	return 0;
}

_int CLightningStage::SixRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();
	return 0;
}

void CLightningStage::RoomBlock_Close()
{
	if (Engine::Get_MainCamType() == Engine::CCameraMgr::MAIN_CAM_QUATER)
	{
		_vec3 vAngle;
		Engine::Get_MainCamera()->Get_Angle(&vAngle);

		vAngle.x = 0.f;
		Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_3RD);
		Engine::Get_MainCamera()->Set_Angle(&vAngle);
	}
}

_bool CLightningStage::Check_Monster()
{
	const Engine::CLayer* pLayer = Engine::Get_Layer(L"Monster");

	if (false == m_bMonsterGen && 0 != pLayer->Get_ObjectMapSize())
	{
		m_bMonsterGen = true;
		return true;
	}

	if (true == m_bMonsterGen && 0 == pLayer->Get_ObjectMapSize())
	{
		m_bMonsterGen = false;

		return false;
	}

	return true;
}

void CLightningStage::RoomBlock_Open()
{
	m_eCurState = ROOM_IDLE;
	Engine::PlaySound_(L"RoomVictoryUnlock.wav", CSoundMgr::EFFECT);

	if (Engine::Get_MainCamType() != Engine::CCameraMgr::MAIN_CAM_QUATER)
	{
		Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_QUATER);
	}

	for (auto& pRoomBlock : m_RoomBlockList)
		pRoomBlock->Open();
	m_RoomBlockList.clear();

}

HRESULT CLightningStage::Store_Setting()
{
	_vec3 vPos = { 52.f, 1.f, 18.f };

	_vec3 vMerchantPos = vPos;
	vMerchantPos.x += 5.f;
	vMerchantPos.z += 3.f;

	CImageObject* pMerchant = CImageObject::Create(m_pGraphicDev, L"Texture_SkillMerchant", &vMerchantPos, 0.05f, 1.f, 0.f);
	Add_GameObject(L"GameLogic", L"NPC_SkillMerchant", pMerchant);

	Engine::CSkill* pSkill = nullptr;

	pSkill = CFireBall::Create(m_pGraphicDev);
	CSkillCard* pSkillCard = CSkillCard::Create(m_pGraphicDev, L"FireBall", &vPos, 0.025f, L"Skill_FireBall", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	/*vPos.x += 2.f;
	pSkill = CMeteorStrike::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"Meteor", &vPos, 0.025f, L"Skill_MeteorStrike", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	vPos.x += 2.f;
	pSkill = CGuidedFireBall::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"FireSkill1", &vPos, 0.025f, L"Skill_GuidedFire", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	vPos.x += 2.f;
	pSkill = CAquaVortex::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"AquaVortex", &vPos, 0.025f, L"Skill_AquaVortex", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);*/

	/// TODO: 번개 스킬 추가
	vPos.x += 2.f;
	pSkill = CLightningFist::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"LightningSkill3", &vPos, 0.025f, L"Skill_LightningSkill3", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"LightningSkill3_SkillCard", pSkillCard);

	vPos.x += 2.f;
	pSkill = CWaterBall::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"WaterBall", &vPos, 0.025f, L"Skill_WaterBall", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	// 	vPos.x += 2.f;
	// 	pSkill = CSharkPool::Create(m_pGraphicDev);
	// 	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"WaterArc", &vPos, 0.025f, L"Skill_SharkPool", pSkill, 100, 1.f);
	// 	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);


	return S_OK;
}

CLightningStage* CLightningStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightningStage* pInstance = new CLightningStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLightningStage::Free()
{
	CScene::Free();
}

