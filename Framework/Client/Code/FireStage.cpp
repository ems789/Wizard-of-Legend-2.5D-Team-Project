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
#include "Mouse.h"
#include "Golem.h"
#include "Mage.h"
#include "Cyclops.h"
#include "Knight.h"
#include "FireBoss.h"
#include "LightningBoss.h"
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
#include "GoldBox.h"
#include "Coin.h"
#include "Scaffold.h"
#include "LaidObject.h"
#include "BasicEffect.h"
#include "SkeletonMinion.h"

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
	//FAILED_CHECK_RETURN(UI_Setting(), E_FAIL);
	FAILED_CHECK_RETURN(Store_Setting(), E_FAIL);
	CUI::GetInstance()->ShowOnUI();

	Engine::StopAll();

	Engine::PlayBGM(L"Fire.wav");

	return S_OK;
}

_int CFireStage::Update_Scene(const _float& fTimeDelta)
{
	const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

	//FirstMonsterGen(pPlayerPos);
	//SecondMonsterGen(pPlayerPos);
	//ThirdMonsterGen(pPlayerPos);
	//FourthMonsterGen(pPlayerPos);

	if (Engine::KeyDown(DIK_P))
	{
		CSkeletonMinion* cs = CSkeletonMinion::Create(m_pGraphicDev, &_vec3(50.f, 0.5f, 37.5f));
		Add_GameObject(L"Monster", L"SkeletonMinion", cs);

	}

	_int iExit = Room_State_Update(fTimeDelta);

	if (1 == iExit)
	{
		return 1;
	}

	iExit = CScene::Update_Scene(fTimeDelta);

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

	pGameObject = CTerrain::Create(m_pGraphicDev, L"../Bin/Data/FireStageTile.dat", L"../Bin/Data/FireStageWall_5.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Terrain", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CFireStage::Ready_Monster_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

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
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(22.5, 2.f, 18.f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_1;

		RoomBlock_Close();

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
		pGameObject = CBlobSpitter::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		fx = rand() % 9 + 4.f;
		fz = rand() % 7 + 6.f;
		vCardPos = { fx, 1.f, fz };
		pGameObject = CBlobSpitter::Create(m_pGraphicDev, &vCardPos);
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
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(19.f, 2.f, 9.5f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_2;
		RoomBlock_Close();
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
		pGameObject = CGhoulLarge::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);


		vCardPos = { rand() % (_int)(fMaxX - fMinX - 2.f) + fMinX + 1.f, 1.f, rand() % (_int)(fMaxZ - fMinZ - 2.f) + fMinZ + 1.f };
		pGameObject = CGhoulLarge::Create(m_pGraphicDev, &vCardPos);
		NULL_CHECK_RETURN(pGameObject, );
		pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(16.5f, 2.f, 46.f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(9.5f, 2.f, 36.5f), CRoomBlock::BLOCK_UP);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_3;
		RoomBlock_Close();
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

		//_vec3 vCardPos = { 82.f, 1.f, 36.f};
		//Engine::CGameObject* pGameObject = CFireBoss::Create(m_pGraphicDev, &vCardPos);
		//NULL_CHECK_RETURN(pGameObject, );

		//CCardSpawn* pCardSpawn = CCardSpawn::Create(m_pGraphicDev, L"Texture_CardSpawn", 28.f, 20.f, 0.05f, &vCardPos, pGameObject);
		//Add_GameObject(L"Effect", L"CardSpawn", pCardSpawn);



		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(68.5, 2.f, 38.f), CRoomBlock::BLOCK_RIGHT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(93.5f, 2.f, 37.f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_4;
		m_eRoomPhase = RP_1;
		m_fTimer = 0.f;

		m_f4RoomX = fMinX;
		m_f4RoomZ = fMinZ;

		m_fRoomMinX = fMinX;
		m_fRoomMaxX = fMaxX;
		m_fRoomMinZ = fMinZ;
		m_fRoomMaxZ = fMaxZ;


		m_v4RoomPos = { (fMaxX + fMinX) / 2, 1.f, (fMaxZ + fMinZ) / 2 };
		m_f4RoomRadius = max(m_fRoomMaxX - m_fRoomMinX, m_fRoomMaxZ - m_fRoomMinZ) / 2;

		RoomBlock_Close();

		m_b4thMonsterGen = true;
	}


}

void CFireStage::FifthMonsterGen(const _vec3 * pPlayerPos)
{
	if (m_b5thMonsterGen)
		return;

	_float	fMinX = 71.f, fMaxX = 88.f;
	_float	fMinZ = 5.f, fMaxZ = 17.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{
		_float	fMidX = (fMinX + fMaxX) / 2;
		_float	fMidZ = (fMinZ + fMaxZ) / 2;

		_float	fStartX = fMidX - 3.f;
		_float	fStartZ = fMidZ - 3.f;

		_bool	bBarrel[9] = { true, false, };
		for (_int i = 0; i < 20; ++i)
		{
			_int iIndexA = rand() % 9;
			_int iIndexB = rand() % 9;
			swap(bBarrel[iIndexA], bBarrel[iIndexB]);
		}

		for (_int i = 0; i < 3; ++i)
		{
			for (_int j = 0; j < 3; ++j)
			{
				CBarrel* pBarrel = CBarrel::Create(m_pGraphicDev, L"Texture_Barrel", &_vec3(fStartX + j * 3.f, 1.f, fStartZ + i * 3.f), 0.05f, 1.f, 0.f, !bBarrel[i * 3 + j]);
				if (bBarrel[i * 3 + j])
					Engine::Add_GameObject(L"Monster", L"Barrel", pBarrel);
				else
					Engine::Add_GameObject(L"GameLogic", L"BoomBarrel", pBarrel);
			}
		}


		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(69.f, 2.f, 8.f), CRoomBlock::BLOCK_RIGHT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(90.f, 2.f, 8.5f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_5;
		m_eRoomPhase = RP_1;
		m_fTimer = 0.f;

		m_fRoomMinX = fMinX;
		m_fRoomMaxX = fMaxX;
		m_fRoomMinZ = fMinZ;
		m_fRoomMaxZ = fMaxZ;

		RoomBlock_Close();

		m_b5thMonsterGen = true;
	}



}

void CFireStage::SixthRoomStart(const _vec3 * pPlayerPos)
{
	if (m_b6thRoomStart)
		return;

	_float	fMinX = 48.f, fMaxX = 62.f;
	_float	fMinZ = 79.f, fMaxZ = 94.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(46.f, 2.f, 94.f), CRoomBlock::BLOCK_RIGHT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_6;
		m_eRoomPhase = RP_1;
		m_fTimer = 0.f;

		m_fRoomMinX = fMinX;
		m_fRoomMaxX = fMaxX;
		m_fRoomMinZ = fMinZ;
		m_fRoomMaxZ = fMaxZ;

		m_b6thRoomStart = true;
	}
}

void CFireStage::SeventhRoomStart(const _vec3 * pPlayerPos)
{
	if (m_b7thRoomStart)
		return;

	_float	fMinX = 83.f, fMaxX = 97.f;
	_float	fMinZ = 81.f, fMaxZ = 97.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{
		ZeroMemory(m_bScaffoldAnswer, sizeof(_bool) * 16);

		m_bScaffoldAnswer[2] = true;
		m_bScaffoldAnswer[4] = true;
		m_bScaffoldAnswer[7] = true;
		m_bScaffoldAnswer[10] = true;
		m_bScaffoldAnswer[11] = true;
		m_bScaffoldAnswer[14] = true;
		m_bScaffoldAnswer[15] = true;

		m_vecScaffold.reserve(16);

		_float fStartX = (fMinX + fMaxX) / 2 - 5.f;
		_float fStartZ = (fMinZ + fMaxZ) / 2 - 5.f;

		for (_uint i = 0; i < 4; ++i)
		{
			for (_uint j = 0; j < 4; ++j)
			{
				CScaffold* pScaffold = CScaffold::Create(m_pGraphicDev, L"Texture_Scaffold", &_vec3(fStartX + j * 3.0f, 0.f, fStartZ + i * 3.f), 1.f);
				Engine::Add_GameObject(L"GameLogic", L"Scaffold", pScaffold);
				m_vecScaffold.push_back(pScaffold);
			}
		}

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(3.5f, 3.5f, 0.f), &_vec3(80.f, 2.f, 88.5f), CRoomBlock::BLOCK_RIGHT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_7;
		m_eRoomPhase = RP_1;
		m_fTimer = 0.f;

		m_fRoomMinX = fMinX;
		m_fRoomMaxX = fMaxX;
		m_fRoomMinZ = fMinZ;
		m_fRoomMaxZ = fMaxZ;

		m_b7thRoomStart = true;
	}
}

void CFireStage::EighthRoomStart(const _vec3 * pPlayerPos)	//	Boss Room
{
	if (m_b8thRoomStart)
		return;

	_float	fMinX = 4.f, fMaxX = 24.f;
	_float	fMinZ = 70.f, fMaxZ = 96.f;

	if (pPlayerPos->x >= fMinX && pPlayerPos->x <= fMaxX &&
		pPlayerPos->z >= fMinZ && pPlayerPos->z <= fMaxZ)
	{
		_float	fMidX = (fMinX + fMaxX) / 2;
		_float	fMidZ = (fMinZ + fMaxZ) / 2;

		CFireBoss* pBoss = CFireBoss::Create(m_pGraphicDev, &_vec3(fMidX, 1.f, fMidZ));
		Engine::Add_GameObject(L"Monster", L"Boss", pBoss);

		CRoomBlock* pRoomBlock = CRoomBlock::Create(m_pGraphicDev, &_vec3(4.f, 3.5f, 0.f), &_vec3(26.f, 2.f, 77.f), CRoomBlock::BLOCK_LEFT);
		Add_GameObject(L"GameLogic", L"RoomBlock", pRoomBlock);
		m_RoomBlockList.push_back(pRoomBlock);

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_8;
		m_eRoomPhase = RP_1;
		m_fTimer = 0.f;

		m_fRoomMinX = fMinX;
		m_fRoomMaxX = fMaxX;
		m_fRoomMinZ = fMinZ;
		m_fRoomMaxZ = fMaxZ;

		RoomBlock_Close();

		m_b8thRoomStart = true;


	}
}

void CFireStage::EngindRoomStart()
{
	if (m_b9thRoomStart)
		return;

	if (m_b1stMonsterGen && m_b2ndMonsterGen && m_b3rdMonsterGen && m_b4thMonsterGen &&
		m_b5thMonsterGen && m_b6thRoomStart && m_b7thRoomStart &&	m_b8thRoomStart)
	{
		m_b9thRoomStart = true;

		m_eCurState = ROOM_CLOSE;
		m_eRoomNumber = ROOM_NUM_9;
		m_eRoomPhase = RP_1;
		m_fTimer = 1.f;

		CLaidObject*	pLandingPad = CLaidObject::Create(m_pGraphicDev, L"Texture_TeleportLandingPad", 1.f, 0.f, 0.1f, &_vec3(86.5, 0.1f, 65.f), 0.f, false, 0.f);
		Add_GameObject(L"GameLogic", L"Teleport_LandingPad", pLandingPad);
	}
}

_int CFireStage::Room_State_Update(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CFireStage::ROOM_IDLE:
		Room_Idle_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_CLOSE:
		Room_Close_Update(fTimeDelta);
		break;
	}

	return _int();
}

_int CFireStage::Room_Idle_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

	EngindRoomStart();

	FirstMonsterGen(pPlayerPos);
	SecondMonsterGen(pPlayerPos);
	ThirdMonsterGen(pPlayerPos);
	FourthMonsterGen(pPlayerPos);
	FifthMonsterGen(pPlayerPos);
	SixthRoomStart(pPlayerPos);

	SeventhRoomStart(pPlayerPos);

	EighthRoomStart(pPlayerPos);



	return 0;
}

_int CFireStage::Room_Close_Update(const _float & fTimeDelta)
{

	switch (m_eRoomNumber)
	{
	case CFireStage::ROOM_NUM_1:
		FirstRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_2:
		SecondRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_3:
		ThirdRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_4:
		FourthRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_5:
		FifthRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_6:
		SixthRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_7:
		SeventhRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_8:
		EighthRoom_Update(fTimeDelta);
		break;
	case CFireStage::ROOM_NUM_9:
		NinthRoom_Update(fTimeDelta);
		break;
	}

	return 0;
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

_int CFireStage::FirstRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();

	return 0;
}

_int CFireStage::SecondRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();
	return 0;
}

_int CFireStage::ThirdRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();
	return 0;
}

_int CFireStage::FourthRoom_Update(const _float & fTimeDelta)
{
	switch (m_eRoomPhase)
	{
	case CFireStage::RP_1:
		FourthRoom_Gen(fTimeDelta);
		break;
	case CFireStage::RP_2:
		FourthRoom_Go(fTimeDelta);
		break;
	case CFireStage::RP_3:
		FourthRoom_Gen2(fTimeDelta);
		break;
	case CFireStage::RP_4:
		FourthRoom_Go2(fTimeDelta);
		break;
	default:
		break;
	}


	return _int();
}

_int CFireStage::FourthRoom_Gen(const _float & fTimeDelta)
{
	_float	fMinX = 71.f, fMaxX = 90.f;
	_float	fMinZ = 26.f, fMaxZ = 49.f;

	m_fTimer += fTimeDelta;

	if (m_fTimer > 0.2f)
	{
		m_fTimer = 0.f;
		_vec3 vPos = { m_f4RoomX, 0.f, m_f4RoomZ };
		const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

		const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

		_vec3 vDir = *pPlayerPos - vPos;
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);
		switch (m_ui4RoomPattern)
		{
		case 0:
		{
			m_f4RoomX += 3.f;
			if (m_f4RoomX >= fMaxX - 1.f)
			{
				m_f4RoomX = fMaxX - 1.f;
				++m_ui4RoomPattern;
				return 0;
			}
			vPos = { m_f4RoomX, 1.f, m_f4RoomZ };
		}
		break;
		case 1:
			m_f4RoomZ += 3.f;
			if (m_f4RoomZ >= fMaxZ - 1.f)
			{
				m_f4RoomZ = fMaxZ - 1.f;
				++m_ui4RoomPattern;
				return 0;
			}
			vPos = { m_f4RoomX, 1.f, m_f4RoomZ };
			break;
		case 2:
			m_f4RoomX -= 3.f;
			if (m_f4RoomX <= fMinX + 1.f)
			{
				m_f4RoomX = fMinX + 1.f;
				++m_ui4RoomPattern;
				return 0;
			}
			vPos = { m_f4RoomX, 1.f, m_f4RoomZ };
			break;
		case 3:
			m_f4RoomZ -= 3.f;
			if (m_f4RoomZ <= fMinZ + 1.f)
			{
				m_f4RoomX = fMinX;
				m_f4RoomZ = fMinZ;
				m_ui4RoomPattern = 0;
				m_eRoomPhase = RP_2;
				return 0;
			}
			vPos = { m_f4RoomX, 1.f, m_f4RoomZ };
			break;
		}

		CBlobRoller* pBlobRoller = CBlobRoller::Create(m_pGraphicDev, vPos, vDir, 10.f, 7, 10.f, L"MonsterAttack", false);
		Engine::Add_GameObject(L"GameLogic", L"BlobRoller", pBlobRoller);
		m_BlobRollerList.push_back(pBlobRoller);
	}

	return 0;
}

_int CFireStage::FourthRoom_Go(const _float & fTimeDelta)
{
	if (false == m_bR4Go)
	{
		for (auto& pBlob : m_BlobRollerList)
			pBlob->Go();
		m_bR4Go = true;
		m_BlobRollerList.clear();
	}

	m_fTimer += fTimeDelta;

	if (m_fTimer > 3.f)
	{
		m_fTimer = 0;
		m_bR4Go = false;
		m_eRoomPhase = RP_3;
	}
	return 0;
}

_int CFireStage::FourthRoom_Gen2(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;
	if (m_fTimer > 10.f)
	{
		m_eRoomPhase = RP_4;
		m_fTimer = 0.f;
		return 0;
	}

	m_fTimer2 += fTimeDelta;
	if (m_fTimer2 > 0.3f)
	{
		m_fTimer2 = 0.f;

		_float fAngle = D3DXToRadian(rand() % 360);
		_vec3	vPos = { 0.f, 0.f, m_f4RoomRadius };

		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, fAngle);
		D3DXVec3TransformNormal(&vPos, &vPos, &matRotY);
		vPos += m_v4RoomPos;

		const Engine::CTransform* pPlayerTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

		const _vec3* pPlayerPos = pPlayerTransform->GetInfo(Engine::INFO_POS);

		_vec3 vDir = *pPlayerPos - vPos;
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		CBlobRoller* pBlobRoller = CBlobRoller::Create(m_pGraphicDev, vPos, vDir, 10.f, 7, 10.f, L"MonsterAttack", true);
		Engine::Add_GameObject(L"GameLogic", L"BlobRoller", pBlobRoller);
		m_BlobRollerList.push_back(pBlobRoller);
	}


	return 0;
}

_int CFireStage::FourthRoom_Go2(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;
	if (m_fTimer > 3.f)
	{
		m_eRoomPhase = RP_1;
		m_fTimer = 0.f;
		RoomBlock_Open();

		_vec3 vPos = { (m_fRoomMaxX + m_fRoomMinX) / 2, 1.f, (m_fRoomMaxZ + m_fRoomMinZ) / 2 };

		_uint iRepeat = rand() % 10 + 10;
		for (_uint i = 0; i < iRepeat; ++i)
		{
			_vec3 vDir = { rand() % 100 / 50.f - 1.f, 0.f ,rand() % 100 / 50.f - 1.f };
			D3DXVec3Normalize(&vDir, &vDir);
			_float fUpForce = rand() % 100 / 10.f + 20.f;
			_float	fSpeed = rand() % 100 / 50.f;
			_int	iCoin = rand() % 20 + 1;
			CCoin* pCoin = CCoin::Create(m_pGraphicDev, &vPos, &vDir, fUpForce, fSpeed, iCoin);
			Engine::Add_GameObject(L"GameLogic", L"Coin", pCoin);
		}

		return 0;
	}

	return 0;
}

_int CFireStage::FifthRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
	{
		RoomBlock_Open();

		_vec3 vPos = { (m_fRoomMaxX + m_fRoomMinX) / 2, 1.f, (m_fRoomMaxZ + m_fRoomMinZ) / 2 };

		_uint iRepeat = rand() % 10 + 10;
		for (_uint i = 0; i < iRepeat; ++i)
		{
			_vec3 vDir = { rand() % 100 / 50.f - 1.f, 0.f ,rand() % 100 / 50.f - 1.f };
			D3DXVec3Normalize(&vDir, &vDir);
			_float fUpForce = rand() % 100 / 10.f + 20.f;
			_float	fSpeed = rand() % 100 / 50.f;
			_int	iCoin = rand() % 20 + 1;
			CCoin* pCoin = CCoin::Create(m_pGraphicDev, &vPos, &vDir, fUpForce, fSpeed, iCoin);
			Engine::Add_GameObject(L"GameLogic", L"Coin", pCoin);
		}
	}

	return 0;
}

_int CFireStage::SixthRoom_Update(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer >= 20.f)
	{
		RoomBlock_Open();

		_vec3 vPos = { (m_fRoomMaxX + m_fRoomMinX) / 2, 1.f, (m_fRoomMaxZ + m_fRoomMinZ) / 2 };

		_uint iRepeat = rand() % 10 + 10;
		for (_uint i = 0; i < iRepeat; ++i)
		{
			_vec3 vDir = { rand() % 100 / 50.f - 1.f, 0.f ,rand() % 100 / 50.f - 1.f };
			D3DXVec3Normalize(&vDir, &vDir);
			_float fUpForce = rand() % 100 / 10.f + 20.f;
			_float	fSpeed = rand() % 100 / 50.f;
			_int	iCoin = rand() % 20 + 1;
			CCoin* pCoin = CCoin::Create(m_pGraphicDev, &vPos, &vDir, fUpForce, fSpeed, iCoin);
			Engine::Add_GameObject(L"GameLogic", L"Coin", pCoin);
		}

		m_fTimer = 0.f;
	}

	m_fTimer2 += fTimeDelta;

	if (m_fTimer2 > 0.5f)
	{
		m_fTimer2 = 0.f;
		_int iWidth = static_cast<_int>(m_fRoomMaxX - m_fRoomMinX);
		_int iDepth = static_cast<_int>(m_fRoomMaxZ - m_fRoomMinZ);

		_vec3 vPos = { rand() % iWidth + m_fRoomMinX, 0.1f, rand() % iDepth + m_fRoomMinZ };

		CFireStomp* pFireStomp = CFireStomp::Create(m_pGraphicDev, vPos, 50);
		Engine::Add_GameObject(L"GameLogic", L"FireBomb", pFireStomp);
	}

	return 0;
}

_int CFireStage::SeventhRoom_Update(const _float & fTimeDelta)
{
	for (_uint i = 0; i < 16; ++i)
	{
		if (m_bScaffoldAnswer[i] != m_vecScaffold[i]->Get_Down())
			return 0;
	}

	RoomBlock_Open();
	for (auto& pScaffold : m_vecScaffold)
	{
		pScaffold->Die();
	}
	m_vecScaffold.clear();


	return 0;
}

_int CFireStage::EighthRoom_Update(const _float & fTimeDelta)
{
	if (false == Check_Monster())
		RoomBlock_Open();



	return 0;
}

_int CFireStage::NinthRoom_Update(const _float & fTimeDelta)
{
		switch (m_eRoomPhase)
		{
		case CFireStage::RP_1:
		{
			const Engine::CComponent* pComponent = Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC);
			const _vec3* pPlayerPos = dynamic_cast<const Engine::CTransform*>(pComponent)->GetInfo(Engine::INFO_POS);

			_vec3 vLandingPadPos = { 86.5, 0.1f, 65.f };
			_vec3 vDist = *pPlayerPos - vLandingPadPos;
			_float fDist = D3DXVec3Length(&vDist);
			if (fDist < 1.f)
			{
				if (Engine::KeyDown(DIK_F))
				{
					CBasicEffect* pTeleEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_TeleportEffect", L"", 9.f, 10.f, 0.05f, &vLandingPadPos, false, 0.f);
					Add_GameObject(L"Effect", L"TeleportEffect", pTeleEffect);
					m_eRoomPhase = RP_2;
					return 0;
				}
			}
		}
		break;
		case CFireStage::RP_2:
			m_fTimer += fTimeDelta;
			if (m_fTimer > 0.9f)
			{
				m_fTimer = 0.f;
				m_eCurState = ROOM_IDLE;
			}
			break;
		}

	return 0;
}

_bool CFireStage::Check_Monster()
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

void CFireStage::RoomBlock_Close()
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

void CFireStage::RoomBlock_Open()
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

HRESULT CFireStage::Store_Setting()
{
	_vec3 vPos = { 45.f, 1.f, 10.f };

	_vec3 vMerchantPos = vPos;
	vMerchantPos.x += 5.f;
	vMerchantPos.z += 3.f;

	CImageObject* pMerchant = CImageObject::Create(m_pGraphicDev, L"Texture_SkillMerchant", &vMerchantPos, 0.05f, 1.f, 0.f);
	Add_GameObject(L"GameLogic", L"NPC_SkillMerchant", pMerchant);

	Engine::CSkill* pSkill = nullptr;

	pSkill = CFireBall::Create(m_pGraphicDev);
	CSkillCard* pSkillCard = CSkillCard::Create(m_pGraphicDev, L"FireBall", &vPos, 0.025f, L"Skill_FireBall", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	vPos.x += 2.f;
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
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	vPos.x += 2.f;
	pSkill = CWaterBall::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"WaterBall", &vPos, 0.025f, L"Skill_WaterBall", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard);

	vPos.x += 2.f;
	pSkill = CSharkPool::Create(m_pGraphicDev);
	pSkillCard = CSkillCard::Create(m_pGraphicDev, L"WaterArc", &vPos, 0.025f, L"Skill_SharkPool", pSkill, 100, 1.f);
	Add_GameObject(L"GameLogic", L"FireBall_SkillCard", pSkillCard); 


	return S_OK;
}
