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
	m_fCoolTime = 0.0f;

	Engine::Ready_Timer(L"FireBall_CoolTime");
	Engine::Set_TimeDelta(L"FireBall_CoolTime");

	return S_OK;
}

_int CFireBall::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"FireBall_CoolTime");
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"FireBall_CoolTime");

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	
	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	vStartPos += vLook;

	CFire* pFire = CFire::Create(m_pGraphicDev, vStartPos, pTransform->GetAngleRef(), m_fSpeed, 4, 30.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Fire", pFire), -1);

	m_fCurTime = m_fCoolTime;

	return 0;
}

_int CFireBall::Use_Skill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"FireBall_CoolTime");
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"FireBall_CoolTime");
	
	_vec3 vStartPos = *pPos;
	vStartPos += *pDir * 1.5f;

	_vec3 vAngle;
	Engine::CMyMath::DirectionalVectorToAngle(&vAngle, pDir);


	CFire* pFire = CFire::Create(m_pGraphicDev, vStartPos, vAngle, m_fSpeed, 4, 30.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Fire", pFire), -1);

	m_fCurTime = m_fCoolTime;

	return ;
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

