#include "stdafx.h"
#include "LightningSpearShot.h"
#include "LightningSpearGen.h"
#include "LaidEffect.h"
#include "BasicFollowingEffect.h"
#include "BasicEffect.h"
#include "SphereCollider.h"
#include "RotatedBillEffect.h"

#include "Export_Function.h"

CLightningSpearShot::CLightningSpearShot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CLightningSpearShot::~CLightningSpearShot()
{

}

HRESULT CLightningSpearShot::Ready_Skill()
{
	m_fCoolTime = 3.f;

	Engine::Ready_Timer(L"LightningSpearShot_CoolTime");
	Engine::Set_TimeDelta(L"LightningSpearShot_CoolTime");

	m_fSpeed = 20.f;


	return S_OK;
}

_int CLightningSpearShot::Use_Skill(const _float& fTimeDelta)
{	
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"LightningSpearShot_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"LightningSpearShot_CoolTime");

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	vStartPos += vLook;

	CLightningSpearGen* pLightningSpearGen = CLightningSpearGen::Create(m_pGraphicDev, &vStartPos, &vLook, 0.05f, 12);
	Engine::Add_GameObject(L"GameLogic", L"LightningSpearGen", pLightningSpearGen);

	//Engine::PlaySound_(L"FireballCast.wav", CSoundMgr::EFFECT);

	m_fCurTime = m_fCoolTime;

	return 2;
}

_int CLightningSpearShot::Use_Skill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	float fCumulativeTime = Engine::Get_CummulativeTime(L"LightningSpearShot_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"LightningSpearShot_CoolTime");

	_vec3 vStartPos = *pPos;
	vStartPos += *pDir;

	_vec3 vAngle;
	Engine::CMyMath::DirectionalVectorToAngle(&vAngle, pDir);

	CLightningSpearGen* pLightningSpearGen = CLightningSpearGen::Create(m_pGraphicDev, &vStartPos, pDir, 0.05f, 12);
	Engine::Add_GameObject(L"GameLogic", L"LightningSpearGen", pLightningSpearGen);


	/*CLightningSpear* pLightningSpear = CLightningSpear::Create(m_pGraphicDev, vStartPos, *pDir, m_fSpeed, 7, 30.f, 0.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"LightningSpear", pLightningSpear), -1);*/

	m_fCurTime = m_fCoolTime;

	return 2;
}

_int CLightningSpearShot::Use_UpgradedSkill(const _float & fTimeDelta)
{

	return 3;
}

_int CLightningSpearShot::Use_UpgradedSkill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{

	return 3;
}

void CLightningSpearShot::LightningSpearShot_Hit_Func()
{
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

CLightningSpearShot* CLightningSpearShot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightningSpearShot*	pInstance = new CLightningSpearShot(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLightningSpearShot::Free()
{
	Engine::CSkill::Free();
}

