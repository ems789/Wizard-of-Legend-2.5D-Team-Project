#include "stdafx.h"
#include "FireBall.h"
#include "Fire.h"

#include "Export_Function.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CFireBall::~CFireBall()
{

}

HRESULT CFireBall::Ready_Skill()
{
	Engine::Ready_Timer(L"FireBall_CoolTime");
	Engine::Set_TimeDelta(L"FireBall_CoolTime");

	m_fCoolTime = 3.f;
	return S_OK;
}

_int CFireBall::Use_Skill(const _float& fTimeDelta)
{
	_float fCurTime = Engine::Get_TimeDelta(L"FireBall_CoolTime");

	if (fCurTime < m_fCoolTime)
		return 0;

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	CFire* pFire = CFire::Create(m_pGraphicDev, pTransform->GetInfoRef(Engine::INFO_POS), pTransform->GetAngleRef(), 10.f, 12, 30.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Fire", pFire), -1);

	Engine::Set_TimeDelta(L"FireBall_CoolTime");

	return 0;
}

_int CFireBall::Use_UpgradedSkill(const _float & fTimeDelta)
{
	return 0;
}

CFireBall* CFireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireBall*	pInstance = new CFireBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFireBall::Free()
{
	Engine::CSkill::Free();
}

