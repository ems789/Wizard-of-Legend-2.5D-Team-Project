#include "stdafx.h"
#include "WaterBall.h"
#include "Water.h"

#include "Export_Function.h"

CWaterBall::CWaterBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CWaterBall::~CWaterBall()
{

}

HRESULT CWaterBall::Ready_Skill()
{
	m_fCoolTime = 1.f;

	Engine::Ready_Timer(L"WaterBall_CoolTime");
	Engine::Set_TimeDelta(L"WaterBall_CoolTime");

	m_fSpeed = 20.f;

	return S_OK;
}

_int CWaterBall::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"WaterBall_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"WaterBall_CoolTime");

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	vStartPos += vLook;

	CWater* pWater = CWater::Create(m_pGraphicDev, vStartPos, vLook, m_fSpeed, 3, 15.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Water", pWater), -1);

	m_fCurTime = m_fCoolTime;

	Engine::PlaySound_(L"AquaPrisonLoop.wav", CSoundMgr::EFFECT);

	return 1;
}

_int CWaterBall::Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"WaterBall_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"WaterBall_CoolTime");

	_vec3 vStartPos = *pPos;
	vStartPos += *pDir;

	_vec3 vAngle;
	Engine::CMyMath::DirectionalVectorToAngle(&vAngle, pDir);

	CWater* pWater = CWater::Create(m_pGraphicDev, vStartPos, *pDir, m_fSpeed, 3, 15.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"Water", pWater), -1);

	m_fCurTime = m_fCoolTime;
	Engine::PlaySound_(L"AquaBeamStart.wav", CSoundMgr::EFFECT);
	return 1;
}

_int CWaterBall::Use_UpgradedSkill(const _float& fTimeDelta)
{
	return 0;
}

CWaterBall* CWaterBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBall* pInstance = new CWaterBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CWaterBall::Free()
{
	Engine::CSkill::Free();
}
