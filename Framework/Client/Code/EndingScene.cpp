#include "stdafx.h"
#include "EndingScene.h"
#include "LoadingScene.h"

#include "Export_Function.h"
#include "Mouse.h"
#include "UIObject.h"
#include "UIButton.h"
#include "UIBlink.h"
#include "Logo.h"
#include "UI.h"

_bool	CEndingScene::m_bResourceLoading = false;

CEndingScene::CEndingScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CEndingScene::~CEndingScene()
{
}

HRESULT CEndingScene::Ready_Scene()
{
	Engine::Clear_StaticLayer();

	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	
	Engine::PlayBGM(L"Credits.wav");
	Engine::SetVolume(CSoundMgr::BGM, 1.f);

	m_eCurState = ENDING_MOVING;

	CUI::GetInstance()->ShowOffUI();
	CUI::GetInstance()->ShowOffBossUI();

	return S_OK;
}

_int CEndingScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CEndingScene::ENDING_MOVING:
		iExit = Update_Moving(fTimeDelta);
		break;
	case CEndingScene::THANKS_FOR_WATCHING:
		iExit = Update_Thanks(fTimeDelta);
		break;
	}

	if (1 == iExit)
		return 1;

	iExit = CScene::Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CEndingScene::Render_Scene()
{

}

HRESULT CEndingScene::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = nullptr;

	pGameObject = m_pCredits = CUIObject::Create(m_pGraphicDev, L"Texture_Credits", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, -1200.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"EndingSceneTitle", pGameObject), E_FAIL, L"Failed to add BackGround");


	//	UIObject
	
	//	Button
	//CUIButton* pButton = CUIButton::Create(m_pGraphicDev, L"Texture_PlayButton", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, -200.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//NULL_CHECK_RETURN(pButton, E_FAIL);
	//FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"PlayButton", pButton), E_FAIL, L"Failed to add PlayButton");
	//m_vecButtons.push_back(pButton);

	//pButton =  CUIButton::Create(m_pGraphicDev, L"Texture_ExitButton", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, -300.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//NULL_CHECK_RETURN(pButton, E_FAIL);
	//FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"ExitButton", pButton), E_FAIL, L"Failed to add PlayButton");
	//m_vecButtons.push_back(pButton);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CEndingScene::Ready_Resource(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	if (m_bResourceLoading)
		return S_OK;

	m_bResourceLoading = true;

	//	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_Credits", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Credits/Credits.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_Thanks_for_watching", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Credits/Thanks_for_watching.png"), E_FAIL);

	return S_OK;
}

_int CEndingScene::Update_Moving(const _float & fTimeDelta)
{
	if (m_pCredits->Get_Pos()->y > 1200.f)
	{
		m_eCurState = THANKS_FOR_WATCHING;

		CUIObject* pThanks = CUIObject::Create(m_pGraphicDev, L"Texture_Thanks_for_watching", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, 0.f, 0.f));
		Add_GameObject(L"GameLogic", L"Thanks", pThanks);

		return 0;
	}

	m_pCredits->Move_Pos(&_vec3(0.f, m_fSpeed * fTimeDelta, 0.f));

	return 0;
}

_int CEndingScene::Update_Thanks(const _float & fTimeDelta)
{
	//	Temporary
	if (Engine::KeyDown(DIK_RETURN))
	{

	//	Engine::Clear_StaticLayer();

		Engine::Remove_Camera(Engine::CAM_STATIC, L"First_View_Camera");
		Engine::Remove_Camera(Engine::CAM_STATIC, L"Third_View_Camera");
		Engine::Remove_Camera(Engine::CAM_STATIC, L"Quater_View_Camera");

		//CInven::GetInstance()->DestroyInstance();
		//CUI::GetInstance()->DestroyInstance();


		Engine::CScene* pScene = CLogo::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);

		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), -1);

		return 1;
	}


	return 0;
}

CEndingScene * CEndingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEndingScene* pInst = new CEndingScene(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}


void CEndingScene::Free()
{
	Engine::StopAll();
	CScene::Free();
}


