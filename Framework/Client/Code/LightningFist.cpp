#include "stdafx.h"
#include "LightningFist.h"
#include "Lightning.h"
#include "LightningGen.h"
#include "LaidEffect.h"
#include "BasicFollowingEffect.h"
#include "BasicEffect.h"
#include "SphereCollider.h"
#include "RotatedBillEffect.h"

#include "Export_Function.h"

CLightningFist::CLightningFist(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CLightningFist::~CLightningFist()
{

}

HRESULT CLightningFist::Ready_Skill()
{
	m_fCoolTime = 3.f;

	Engine::Ready_Timer(L"LightningFist_CoolTime");
	Engine::Set_TimeDelta(L"LightningFist_CoolTime");

	return S_OK;
}

_int CLightningFist::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"LightningFist_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"LightningFist_CoolTime");

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	vStartPos += vLook;

	CLightning* pLightning = CLightning::Create(m_pGraphicDev, vStartPos, vLook, 3.f, 30.f, 3.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Lightning", pLightning), -1);

	CBasicEffect* pLightningStageEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningStageEffect", L"", 16.f, 20.f, 0.04f, &vStartPos, true, 3.f);
	Engine::Add_GameObject(L"GameLogic", L"Texture_LightningStageEffect2", pLightningStageEffect);

	CRotatedBillEffect* pEffect = CRotatedBillEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vStartPos, false, 0.f, D3DXToRadian(rand() % 367));
	NULL_CHECK_RETURN(pEffect, -1);

	CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pLightning, pEffect, 1.f, L"Player_Bullet", 10, 0.5f);
	Engine::Add_GameObject(L"GameLogic", L"PlayerCollider", pCollider);
	pCollider->Add_Hit_Effect(LightningFist_Hit_Func);

	Engine::PlaySound_(L"ThunderBounce.wav", CSoundMgr::EFFECT);

	m_fCurTime = m_fCoolTime;

	return 3;
}

_int CLightningFist::Use_Skill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"LightningFist_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"LightningFist_CoolTime");

	_vec3 vStartPos = *pPos;
	vStartPos += *pDir;
	vStartPos.z += pDir->z * 0.2f;

	CLightning* pLightning = CLightning::Create(m_pGraphicDev, vStartPos, *pDir, 3.f, 30.f, 3.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Lightning", pLightning), -1);

	CBasicEffect* pLightningStageEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningStageEffect", L"", 16.f, 20.f, 0.04f, &vStartPos, true, 3.f);
	Engine::Add_GameObject(L"GameLogic", L"Texture_LightningStageEffect2", pLightningStageEffect);

	CRotatedBillEffect* pEffect = CRotatedBillEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vStartPos, false, 0.f, D3DXToRadian(rand() % 367));
	NULL_CHECK_RETURN(pEffect, -1);

	CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pLightning, pEffect, 1.f, L"Player_Bullet", 10, 0.5f);
	Engine::Add_GameObject(L"GameLogic", L"PlayerCollider", pCollider);
	pCollider->Add_Hit_Effect(LightningFist_Hit_Func);

	Engine::PlaySound_(L"ThunderBounce.wav", CSoundMgr::EFFECT);
	m_fCurTime = m_fCoolTime;

	return 3;
}

_int CLightningFist::Use_UpgradedSkill(const _float & fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"LightningFist_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"LightningFist_CoolTime");

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	vStartPos += vLook;

	CLightning* pLightning = CLightning::Create(m_pGraphicDev, vStartPos, vLook, 3.f, 30.f, 3.f, 3.f, 0.15f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Lightning", pLightning), -1);

	CLaidEffect* pWindShuriken = CLaidEffect::Create(m_pGraphicDev, L"Texture_WindShuriken", L"", 3.f, 60.f, 0.1f, &vStartPos, 0.f, true, 3.f);
	Engine::Add_GameObject(L"GameLogic", L"Texture_WindShuriken", pWindShuriken);

	CBasicEffect* pLightningStageEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningStageEffect", L"", 16.f, 20.f, 0.04f, &vStartPos, true, 3.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningStageEffect2", pLightningStageEffect);

	CRotatedBillEffect* pEffect = CRotatedBillEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vStartPos, false, 0.f, D3DXToRadian(rand() % 367));
	NULL_CHECK_RETURN(pEffect, -1);

	CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pLightning, pEffect, 3.f, L"Player_Bullet", 10, 0.15f);
	Engine::Add_GameObject(L"GameLogic", L"PlayerCollider", pCollider);
	pCollider->Add_Hit_Effect(LightningFist_Hit_Func);

	Engine::PlaySound_(L"LightningShuriken.wav", CSoundMgr::EFFECT);
	m_fCurTime = m_fCoolTime;

	return 3;
}

_int CLightningFist::Use_UpgradedSkill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"LightningFist_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"LightningFist_CoolTime");

	_vec3 vStartPos = *pPos;
	vStartPos += *pDir;
	vStartPos.z += pDir->z * 0.2f;

	_vec3 vAngle;
	Engine::CMyMath::DirectionalVectorToAngle(&vAngle, pDir);

	CLightning* pLightning = CLightning::Create(m_pGraphicDev, vStartPos, *pDir, 3.f, 30.f, 3.f, 3.f, true, 0.15f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Lightning", pLightning), -1);

	// CLaidEffect* pWindShuriken = CLaidEffect::Create(m_pGraphicDev, L"Texture_WindShuriken", L"", 3.f, 60.f, 0.1f, &vStartPos, 0.f, true, 3.f);
	// Engine::Add_GameObject(L"GameLogic", L"Texture_WindShuriken", pWindShuriken);

	CRotatedBillEffect* pEffect = CRotatedBillEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vStartPos, false, 0.f, D3DXToRadian(rand() % 367));
	NULL_CHECK_RETURN(pEffect, -1);

// 	CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pLightning, pEffect, 3.f, L"Player_Bullet", 10, 0.15f);
// 	Engine::Add_GameObject(L"GameLogic", L"PlayerCollider", pCollider);
// 	pCollider->Add_Hit_Effect(LightningFist_Hit_Func);

	//Engine::PlaySound_(L"LightningShuriken.wav", CSoundMgr::EFFECT);
	m_fCurTime = m_fCoolTime;

	return 3;
}

void CLightningFist::LightningFist_Hit_Func()
{
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

CLightningFist* CLightningFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightningFist*	pInstance = new CLightningFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLightningFist::Free()
{
	Engine::CSkill::Free();
}

