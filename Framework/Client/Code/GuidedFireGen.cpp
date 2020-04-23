#include "stdafx.h"
#include "GuidedFireGen.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "FireEffect.h"
#include "Fire.h"
#include "GuidedFire.h"

CGuidedFireGen::CGuidedFireGen(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CGuidedFireGen::~CGuidedFireGen()
{

}

HRESULT CGuidedFireGen::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fFireAngle = 0.f;
	m_fSpeed = 20.f;
	return S_OK;
}

_int CGuidedFireGen::Update_GameObject(const _float& fTimeDelta)
{
	Generate_GuidedFire(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}


const _vec3 * CGuidedFireGen::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CGuidedFireGen::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CGuidedFireGen::Generate_GuidedFire(const _float & fTimeDelta)
{
	m_fTimeCount += fTimeDelta;

	if (m_fTimeCount > m_fFireTimeItv)
	{
		m_fTimeCount -= m_fFireTimeItv;
		m_fFireAngle += m_fFireAngleItv;
		++m_uiFireCnt;

		if (m_uiFireCnt > m_uiFireMax)
		{
			m_bIsDead = true;
			return;
		}
		CGuidedFire* pGuidedFire = CGuidedFire::Create(m_pGraphicDev, m_fFireAngle, 3.f, m_pRevTarget, m_fRevRadius, m_fDetectDist, 2, 20, 0.005f, L"Player_Bullet", 0.5f);
		NULL_CHECK_RETURN(pGuidedFire, );
		Engine::Add_GameObject(L"GameLogic", L"GuidedFireGen", pGuidedFire);

		Engine::PlaySound_(L"FlameLight.wav", CSoundMgr::EFFECT);
	}
}

CGuidedFireGen* CGuidedFireGen::Create(LPDIRECT3DDEVICE9 pGraphicDev, const Engine::CTransform* pRevTarget, const _float& fFireTimeItv, const _float& fFireAngleItv, 
	const _uint& uiFireMax, const _float& fRevRadius, const _float& fDetectDist)
{
	CGuidedFireGen*	pInstance = new CGuidedFireGen(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pRevTarget = pRevTarget;
	pInstance->m_fFireTimeItv = fFireTimeItv;
	pInstance->m_fFireAngleItv = fFireAngleItv;
	pInstance->m_uiFireMax = uiFireMax;
	pInstance->m_fRevRadius = fRevRadius;
	pInstance->m_fDetectDist = fDetectDist;

	return pInstance;
}

void CGuidedFireGen::Free()
{

	CGameObject::Free();
}

