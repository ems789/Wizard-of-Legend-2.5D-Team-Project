#include "stdafx.h"
#include "GuidedFireBall.h"

#include "Export_Function.h"
#include "GuidedFireGen.h"

CGuidedFireBall::CGuidedFireBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CGuidedFireBall::~CGuidedFireBall()
{

}

HRESULT CGuidedFireBall::Ready_Skill()
{
	m_fCoolTime = 3.f;

	Engine::Ready_Timer(L"GuidedFireBall_CoolTime");
	Engine::Set_TimeDelta(L"GuidedFireBall_CoolTime");
	
	m_fSpeed = 20.f;

	return S_OK;
}

_int CGuidedFireBall::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"GuidedFireBall_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"GuidedFireBall_CoolTime");

	const Engine::CTransform* pPlayerTrans = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	CGuidedFireGen* pGuidedFireGen = CGuidedFireGen::Create(m_pGraphicDev, pPlayerTrans, 0.1f, D3DXToRadian(30.f), 12, 4.f, 10.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"GuidedFireGen", pGuidedFireGen), -1);

	m_fCurTime = m_fCoolTime;

	return 1;
}

_int CGuidedFireBall::Use_Skill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"GuidedFireBall_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"GuidedFireBall_CoolTime");

	const Engine::CTransform* pPlayerTrans = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	CGuidedFireGen* pGuidedFireGen = CGuidedFireGen::Create(m_pGraphicDev, pPlayerTrans, 0.1f, D3DXToRadian(30.f), 10, 2.f, 10.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"GuidedFireGen", pGuidedFireGen), -1);

	m_fCurTime = m_fCoolTime;

	return 1;
}

_int CGuidedFireBall::Use_UpgradedSkill(const _float & fTimeDelta)
{
	return 0;
}

CGuidedFireBall* CGuidedFireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGuidedFireBall*	pInstance = new CGuidedFireBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CGuidedFireBall::Free()
{
	Engine::CSkill::Free();
}

