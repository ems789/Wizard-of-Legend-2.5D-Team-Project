#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "Logo.h"
#include "Mouse.h"

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	::ShowCursor(FALSE);

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagement), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_CameraMgr(m_pGraphicDev, WINCX, WINCY), E_FAIL);
	FAILED_CHECK_RETURN(Mouse_Setting(), E_FAIL);
	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	m_iExit = 0;

	Engine::Set_InputDev();

	if (nullptr != m_pManagement)
		m_iExit = m_pManagement->Update_Scene(fTimeDelta);

	Engine::Update_MainCamera(fTimeDelta);
	m_pMouse->Update_Mouse(fTimeDelta);

	return m_iExit;
}

void CMainApp::Render_MainApp(void)
{
	if (m_iExit == 1)
		return;

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	
	if (nullptr != m_pManagement)
		m_pManagement->Render_Scene();

	m_pMouse->Render_Mouse();

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	// FONT
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"±Ã¼­", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Second", L"¹ÙÅÁ", 10, 10, FW_THIN), E_FAIL);

	// INPUTDEV
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagement)
{
	Engine::CScene*		pScene = nullptr;

	FAILED_CHECK_RETURN(Engine::Create_Management(ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	Engine::Create_StaticLayer();

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagement)->SetUp_Scene(pScene), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Mouse_Setting()
{
	m_pMouse = CMouse::GetInstance();

	m_pMouse->Ready_Mouse(m_pGraphicDev);

	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp* pInst = new CMainApp;

	if (FAILED(pInst->Ready_MainApp()))
		Safe_Release(pInst);

	return pInst;
}

void CMainApp::Free(void)
{
	::ShowCursor(TRUE);
	Safe_Release(m_pMouse);
	_ulong dwRef = Engine::Safe_Release(m_pGraphicDev);
	dwRef = Engine::Safe_Release(m_pDeviceClass);
	dwRef = Engine::Safe_Release(m_pManagement);

	Engine::Release_Resources();
	Engine::Release_Utility();
	Engine::Release_System();
}