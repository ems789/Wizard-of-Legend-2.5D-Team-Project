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
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, D3DXCOLOR(1.f,1.f,1.f,1.f), L"", VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);

	//  Outer Wall Texture
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_LeftWallTex_Outer", Engine::BUFFER_LEFTWALLTEX, Engine::WALL_OUTER), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TopWallTex_Outer", Engine::BUFFER_TOPWALLTEX, Engine::WALL_OUTER), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RightWallTex_Outer", Engine::BUFFER_RIGHTWALLTEX, Engine::WALL_OUTER), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_BottomWallTex_Outer", Engine::BUFFER_BOTTOMWALLTEX, Engine::WALL_OUTER), E_FAIL);

	//  Inner Wall Texture
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_LeftWallTex_Inner", Engine::BUFFER_LEFTWALLTEX, Engine::WALL_INNER), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TopWallTex_Inner", Engine::BUFFER_TOPWALLTEX, Engine::WALL_INNER), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RightWallTex_Inner", Engine::BUFFER_RIGHTWALLTEX, Engine::WALL_INNER), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_BottomWallTex_Inner", Engine::BUFFER_BOTTOMWALLTEX, Engine::WALL_INNER), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_CeilingWallTex", Engine::BUFFER_CEILINGWALLTEX), E_FAIL);

	//	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/TileSet/Grass/TutorialCourseFloorRoom2_9.png"), E_FAIL);

	//  Tile Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Tile/Tile%d.png", 608), E_FAIL);

	//  Wall Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Wall", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Wall/Wall%d.png", 134), E_FAIL);



	//	Player Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleUp.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleDown.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleRight.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_IdleLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardIdleLeft.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunUp/WizardRunUp%d.png", 10), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunDown/WizardRunDown%d.png", 10), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunRight/WizardRunRight%d.png", 10), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_RunLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardRunLeft/WizardRunLeft%d.png", 10), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoEUp/WizardPBAoEUp%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoEDown/WizardPBAoEDown%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoERight/WizardPBAoERight%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_DashLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardPBAoELeft/WizardPBAoELeft%d.png", 9), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Up/WizardForehandUp%d.png", 8), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Down/WizardForehandDown%d.png", 8), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Right/WizardForehandRight%d.png", 8), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_AttackLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardForehand/Left/WizardForehandLeft%d.png", 8), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Up", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Up/WizardBackhandUp%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Down", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Down/WizardBackhandDown%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Right", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Right/WizardBackhandRight%d.png", 9), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Player_Attack2Left", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/WizardBackhand/Left/WizardBackhandLeft%d.png", 9), E_FAIL);

	//	HP Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarBG", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarBG.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarFill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarFill.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_HPBarHurtFill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/HPBarHurtFill.png"), E_FAIL);

	//	AirRing
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_AirRingUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AirRing/Up/AirRing%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_AirRingDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AirRing/Down/AirRing%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_AirRingRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AirRing/Right/AirRing%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_AirRingLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AirRing/Left/AirRing%d.png", 4), E_FAIL);

	//	FireBall Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Fire", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Skill/FireBall/IgnitionOrb_%d.png", 4), E_FAIL);

	//	WindSlash Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_WindSlash", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Skill/WindSlash/WindSlashLarge%d.png", 7), E_FAIL);

	//	Meteor	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Meteor", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Skill/MeteorStrike/MeteorStrike%d.png", 4), E_FAIL);
	//	Explosion Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireExplosion", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FireExplosion/FireExplosion%d.png", 7), E_FAIL);

	//	SlashHitSpark Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_SlashHitSpark", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/SlashHitSpark/SlashHitSpark_%d.png", 7), E_FAIL);

	//	FireParticle Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireParticle1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FireParticle/Particle1/FireParticle1_%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireParticle2", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FireParticle/Particle2/FireParticle2_%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireParticle3", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FireParticle/Particle3/FireParticle3_%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireParticle4", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FireParticle/Particle4/FireParticle4_%d.png", 6), E_FAIL);

	//	Monster
	//	Golem
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_Idle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/Idle/GolemIdle.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_Run", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemRun/GolemRunRight%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_Death", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/Death/GolemDeath%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackDown0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackRight0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackLeft0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackUp0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackFollowThrough", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackFollowThrough.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Golem_AttackFollowThrough_Alt", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Golem/GolemAttackFollowThrough_Alt.png"), E_FAIL);

	//	Cyclops
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Idle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/CyclopsIdle0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_RunRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Run/Right/CyclopsRunRight%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_RunLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Run/Left/CyclopsRunLeft%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Hurt", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Hurt/CyclopsHurt.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Dead", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Dead/CyclopsDead%d.png", 7), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Up", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Up/CyclopsAttackUp0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Attack_Up", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Up/CyclopsAttackUp1.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Down", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Down/CyclopsAttackDown0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Attack_Down", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Down/CyclopsAttackDown1.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Right", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Right/CyclopsAttackRight0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Attack_Right", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Right/CyclopsAttackRight1.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Left", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Left/CyclopsAttackLeft0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Cyclops_Attack_Left", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Cyclops/Attack/Left/CyclopsAttackLeft1.png"), E_FAIL);

	//	RedBeam
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_RedBeam", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/RedBeam.png"), E_FAIL);

	//	FireBoss Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Idle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Idle/FireBossIdle.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashReadyRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/DashReady/FireBossDash0.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashReadyLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/DashReady/FireBossDashLeft.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashFinishRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/DashFinish/FireBossDash2.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashFinishLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/DashFinish/FireBossDashLeft.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/FireBossDashUp.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/FireBossDashDown.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/FireBossDashRight.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_DashLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Dash/FireBossDashLeft.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Heel", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Heel/FireBossHeel%d.png", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Hurt", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Hurt/FireBossHurt%d.png", 4), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_AttackDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Forehand/Down/FireBossForehandDown%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_AttackLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Forehand/Left/FireBossForehandLeft%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_AttackRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Forehand/Right/FireBossForehandRight%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_AttackUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Forehand/Up/FireBossForehandUp%d.png", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_PointDown", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Point/Down/FireBossPointDown%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_PointLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Point/Left/FireBossPointLeft%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_PointRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Point/Right/FireBossPointRight%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_PointUp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Point/Up/FireBossPointUp%d.png", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Stomp", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/Stomp/FireBossStomp%d.png", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_SpinKickRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/SpinKick/Right/FireBossSpinKick%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_SpinKickLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/SpinKick/Left/FireBossSpinKick%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_RoundhouseKickRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/RoundhouseKick/Right/FireBossRoundhouseKick%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_RoundhouseKickLeft", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FireBoss/RoundhouseKick/Left/FireBossRoundhouseKick%d.png", 5), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Taunt", Engine::TEX_NORMAL,	L"../Bin/Resource/Texture/Monster/FireBoss/Taunt/FireBossTaunt%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Squat", Engine::TEX_NORMAL,	L"../Bin/Resource/Texture/Monster/FireBoss/Squat/FireBossSquat%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Dead", Engine::TEX_NORMAL,		L"../Bin/Resource/Texture/Monster/FireBoss/Dead/FireBossDead%d.png", 2), E_FAIL);

	//	Shuriken Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FireBoss_Shuriken", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Skill/Shuriken.png"), E_FAIL);

	//	Floor Crack
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_FloorCrack", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/FloorCrack/FloorCrack3.png"), E_FAIL);


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

