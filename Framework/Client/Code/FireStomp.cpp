#include "stdafx.h"
#include "FireStomp.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "FireEffect.h"
#include "FireKick.h"
#include "LaidEffect.h"
#include "AlphaLaidEffect.h"

CFireStomp::CFireStomp(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFireStomp::~CFireStomp()
{

}

HRESULT CFireStomp::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CFireStomp::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	//LifeTimer(fTimeDelta);

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	switch (m_eCurState)
	{
	case CFireStomp::FSS_CASTING_CICLE:
	{
		fCastingTime += fTimeDelta;
		if (fCastingTime > 1.f)
		{
			m_eCurState = FSS_BOOM;


			CFireKick* pFireKick = CFireKick::Create(m_pGraphicDev, vPos, 0.5f, 20, m_iAttack, 0.01f, L"MonsterAttack", 3.f, 0.05f);
			Engine::Add_GameObject(L"GameLogic", L"StompBoom", pFireKick);
			Engine::Get_MainCamera()->CameraShake();
			Engine::PlaySound_(L"Explosion2.wav", CSoundMgr::EFFECT);
		}
	}
		break;
	case CFireStomp::FSS_BOOM:
		m_bIsDead = true;
		break;
	case CFireStomp::FSS_END:
	{
		vPos.y = 0.1f;

		CAlphaLaidEffect* pCastingCircle = CAlphaLaidEffect::Create(m_pGraphicDev, L"Texture_CastingCircle", L"StompCastingCircle", 27.f, 30.f, 0.03f, &vPos, 0.f, false, 1.f, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		Engine::Add_GameObject(L"Effect", L"StompCastingCircle", pCastingCircle);
		m_eCurState = FSS_CASTING_CICLE;
	}
		break;
	}

	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CFireStomp::Render_GameObject()
{

}

const _vec3 * CFireStomp::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CFireStomp::Add_Effect(const _vec3* pPos)
{

}

HRESULT CFireStomp::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CFireStomp::LifeTimer(const _float & fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
		m_bIsDead = true;
}

void CFireStomp::FireStompTail()
{
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	vPos.y = 0.1f;
	CLaidEffect* pCrack = CLaidEffect::Create(m_pGraphicDev, L"Texture_FloorCrack", L"FloorCrack", 1.f, 0.f, 0.05f, &vPos, 0.f, true, 0.05f);
	Engine::Add_GameObject(L"Effect", L"FloorCrack", pCrack);

	CFireKick* pFireKick = CFireKick::Create(m_pGraphicDev, vPos, 0.1f);
	Engine::Add_GameObject(L"GameLogic", L"FireKick", pFireKick);
}

void CFireStomp::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CFireStomp* CFireStomp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _int& iAttack/* = 50*/)
{
	CFireStomp*	pInstance = new CFireStomp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_iAttack = iAttack;

	return pInstance;
}

void CFireStomp::Free()
{
	CGameObject::Free();
}

