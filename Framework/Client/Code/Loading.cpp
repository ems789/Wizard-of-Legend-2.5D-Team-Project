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
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, L"", VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);

	//	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/TileSet/Grass/TutorialCourseFloorRoom2_9.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Tile/ForestTile/ForestTile%d.png", 321), E_FAIL);

	//	Player Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleUp.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleDown.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleRight.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunUp/WizardRunUp%d.png", 10), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunDown/WizardRunDown%d.png", 10), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunRight/WizardRunRight%d.png", 10), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoEUp/WizardPBAoEUp%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoEDown/WizardPBAoEDown%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoERight/WizardPBAoERight%d.png", 9), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Up/WizardForehandUp%d.png", 8), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Down/WizardForehandDown%d.png", 8), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Right/WizardForehandRight%d.png", 8), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Up", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Up/WizardBackhandUp%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Down", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Down/WizardBackhandDown%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Right", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Right/WizardBackhandRight%d.png", 9), E_FAIL);





	//	HP Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarBG", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarBG.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarFill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarFill.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarHurtFill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarHurtFill.png"), E_FAIL);

	//	FireBall Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Fire", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Skill/FireBall/IgnitionOrb_%d.png", 4), E_FAIL);

	//	WindSlash Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_WindSlash", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Skill/WindSlash/WindSlashLarge%d.png", 7), E_FAIL);

	//	FireBoss Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_FireBoss_Idle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/FireBossIdle.png"), E_FAIL);

	//	Explosion Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireExplosion", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FireExplosion/FireExplosion%d.png", 7), E_FAIL);

	//	SlashHitSpark Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_SlashHitSpark", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/SlashHitSpark/SlashHitSpark_%d.png", 7), E_FAIL);

	//	Monster
	//	Golem
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_Idle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/Idle/GolemIdle.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_Run", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemRun/GolemRunRight%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_Death", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/Death/GolemDeath%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackDown0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackRight0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackUp0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackFollowThrough", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackFollowThrough.png"), E_FAIL);



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

