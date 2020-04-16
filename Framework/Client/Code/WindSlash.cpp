#include "stdafx.h"
#include "WindSlash.h"
#include "Fire.h"

#include "Export_Function.h"
#include "Wind.h"

CWindSlash::CWindSlash(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CWindSlash::~CWindSlash()
{

}

HRESULT CWindSlash::Ready_Skill()
{
	m_fCoolTime = 0.0f;

	Engine::Ready_Timer(L"WindSlash_CoolTime");
	Engine::Set_TimeDelta(L"WindSlash_CoolTime");

	return S_OK;
}

_int CWindSlash::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"WindSlash_CoolTime");
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"WindSlash_CoolTime");

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	vStartPos += vLook;

	_vec3 vAngle = pTransform->GetAngleRef();
	vAngle.x += D3DXToRadian(90.f);

	CWind* pWind = CWind::Create(m_pGraphicDev, vStartPos, vAngle, m_fSpeed, 7, 30.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Wind", pWind), -1);

	m_fCurTime = m_fCoolTime;

	return 0;
}

_int CWindSlash::Use_Skill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"WindSlash_CoolTime");
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"WindSlash_CoolTime");

	_vec3 vStartPos = *pPos;
	vStartPos += *pDir * 1.5f;

	_vec3 vAngle;
	Engine::CMyMath::DirectionalVectorToAngle(&vAngle, pDir);

	vAngle.x = D3DXToRadian(90.f);

	CWind* pWind = CWind::Create(m_pGraphicDev, vStartPos, vAngle, m_fSpeed, 7, 30.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Wind", pWind), -1);

	m_fCurTime = m_fCoolTime;

	return;
}

_int CWindSlash::Use_UpgradedSkill(const _float & fTimeDelta)
{
	return 0;
}

CWindSlash* CWindSlash::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWindSlash*	pInstance = new CWindSlash(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CWindSlash::Free()
{
	Engine::CSkill::Free();
}

