#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{

}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForState();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForState()
{
	//lstrcpy(m_szLoading, L"");

	//	Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex",		Engine::BUFFER_TERRAINTEX, L"", VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);

	//	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain",		Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/TileSet/Grass/TutorialCourseFloorRoom2_9.png"), E_FAIL);
	
	//	Player Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Idle",	Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleUp.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunUp",	Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunUp/WizardRunUp%d.png", 10), E_FAIL);


	//	HP Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarBG",		Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarBG.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarFill",		Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarFill.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarHurtFill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarHurtFill.png"), E_FAIL);



	m_bFinish = true;

	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

