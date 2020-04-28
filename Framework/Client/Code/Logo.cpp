#include "stdafx.h"
#include "Logo.h"
#include "LoadingScene.h"

#include "Export_Function.h"
#include "Mouse.h"
#include "UIObject.h"
#include "UIButton.h"
#include "UIBlink.h"

_bool CLogo::m_bResourceLoading = false;

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_StaticLayer(), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);
	
	Engine::PlayBGM(L"TitleScreen.wav");
	Engine::SetVolume(CSoundMgr::BGM, 1.f);

	m_eCurState = PRESS_ENTER;

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;


	switch (m_eCurState)
	{
	case CLogo::PRESS_ENTER:
		Update_PressEnter(fTimeDelta);
		break;
	case CLogo::LOGO_MOVING:
		Update_Moving(fTimeDelta);
		break;
	case CLogo::LOGO_BUTTON:
		Update_Button(fTimeDelta);
		break;
	}


	//	Temporary
	if (Engine::KeyDown(DIK_RETURN))
	{
		Engine::CScene* pScene = CLoadingScene::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);

		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), -1);

		return 1;
	}

	return iExit;
}

void CLogo::Render_Scene()
{

}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = nullptr;

	pGameObject = CUIObject::Create(m_pGraphicDev, L"Texture_Logo", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"LogoTitle", pGameObject), E_FAIL, L"Failed to add BackGround");


	//	UIObject
	pGameObject = m_pTitleLogo = CUIObject::Create(m_pGraphicDev, L"Texture_TitleLogo", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"LogoTitle", pGameObject), E_FAIL, L"Failed to add TitleLogo");

	pGameObject = m_pPressEnterKey = CUIBlink::Create(m_pGraphicDev, L"Texture_PressEnterKey", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, -300.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"PressEnterKey", pGameObject), E_FAIL, L"Failed to add PressEnterKey");

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_UI_Layer(const _tchar* pLayerTag)
{

	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9 & pGraphicDev, RESOURCEID eMax)
{
	if (m_bResourceLoading)
		return S_OK;

	m_bResourceLoading = true;

	Engine::Reserve_ContainerSize(eMax);

	//	Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcColTex", Engine::BUFFER_RCCOLTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX), E_FAIL);

	//	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_Logo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/BG/Background%d.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_TitleLogo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/BG/TitleLogo.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_PressEnterKey", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/String/Press_Enter_Key.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_PlayButton", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/String/Play.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_ExitButton", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/String/Exit.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_CastingCircle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/CastingCircle/CastingCircle_%d.png", 27), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Ready_StaticLayer()
{

	return S_OK;
}

_int CLogo::Update_PressEnter(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_RETURN))
	{
		m_eCurState = LOGO_MOVING;
		m_pPressEnterKey->Die();
	}

	return _int();
}

_int CLogo::Update_Moving(const _float & fTimeDelta)
{
	_vec3 vPos = *m_pTitleLogo->Get_Pos();
	if (vPos.y > 150.f)
	{
		m_eCurState = LOGO_BUTTON;
		m_pTitleLogo->Set_PosY(150.f);


		CUIButton* pButton = CUIButton::Create(m_pGraphicDev, L"Texture_PlayButton", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, -200.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		NULL_CHECK_RETURN(pButton, E_FAIL);
		FAILED_CHECK_RETURN_MSG(Engine::Add_GameObject(L"GameLogic", L"PlayButton", pButton), -1, L"Failed to add PlayButton");
		m_vecButtons.push_back(pButton);

		pButton = CUIButton::Create(m_pGraphicDev, L"Texture_ExitButton", &_vec3(1.f, 1.f, 1.f), &_vec3(0.f, -300.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		NULL_CHECK_RETURN(pButton, E_FAIL);
		FAILED_CHECK_RETURN_MSG(Engine::Add_GameObject(L"GameLogic", L"ExitButton", pButton), -1, L"Failed to add ExitButton");
		m_vecButtons.push_back(pButton);


		return 0;
	}

	m_pTitleLogo->Move_Pos(&_vec3(0.f, m_fSpeed * fTimeDelta, 0.f));

	return 0;
}

_int CLogo::Update_Button(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_UP))
	{
		m_iOnButton -= 1;
		if (m_iOnButton < 0)
			m_iOnButton = m_vecButtons.size() - 1;
	}

	if (Engine::KeyDown(DIK_DOWN))
	{
		m_iOnButton += 1;
		if (m_iOnButton >= m_vecButtons.size())
			m_iOnButton = 0;
	}

	for (_int i = 0; i < m_vecButtons.size(); ++i)
	{
		if (m_iOnButton == i)
			m_vecButtons[i]->ButtonOn();
		else
			m_vecButtons[i]->ButtonOff();
	}

	if (Engine::KeyDown(DIK_RETURN))
	{
		switch (m_iOnButton)
		{
		case 0:
		{
			Engine::CScene* pScene = CLoadingScene::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), -1);
			return 1;
		}
			break;
		case 1:
		{
			PostQuitMessage(0);
			return 0;
		}
			break;
		}
	}

	return 0;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo* pInst = new CLogo(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}


void CLogo::Free()
{
	Engine::StopAll();
	CScene::Free();
}

