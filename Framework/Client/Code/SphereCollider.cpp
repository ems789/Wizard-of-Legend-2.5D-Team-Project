#include "stdafx.h"
#include "SphereCollider.h"

#include "Export_Function.h"

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCollider(pGraphicDev)
{

}

CSphereCollider::~CSphereCollider()
{

}


HRESULT CSphereCollider::Ready_Collider(CGameObject * pParent, Engine::CEffect* pHitEffect, const _float & fRadius, const _tchar* pColliderTag, const _int& iAttack)
{
	FAILED_CHECK_RETURN(CCollider::Ready_Collider(pParent), E_FAIL);

	m_eType = Engine::CT_SPHERE;
	m_tSphere.fRadius = fRadius;
	m_pColliderTag = pColliderTag;
	m_iAttack = iAttack;
	m_pHitEffect = pHitEffect;
	m_bAddEffect = false;

	return S_OK;
}


_int CSphereCollider::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_pParent->IsDead())
	{
		m_bIsDead = true;
		return 0;
	}
	m_tSphere.vPos = *m_pParent->Get_Pos();

	_int iExit = CCollider::Update_GameObject(fTimeDelta);

	Engine::Add_GameObject_To_CollisionList(m_pColliderTag, this);
	
	return iExit;
}

void CSphereCollider::Render_GameObject()
{
	
}

void CSphereCollider::Add_Effect(const _vec3 * pPos)
{
	if (true == m_bAddEffect)
		return;

	m_bAddEffect = true;

	if (nullptr == m_pHitEffect)
		return;

	_vec3 vHitPoint = (*pPos + m_tSphere.vPos) / 2;

	m_pHitEffect->Set_Pos(&vHitPoint);

	Engine::Add_GameObject(L"GameLogic", m_pHitEffect->Get_EffectTag(), m_pHitEffect);

	if (nullptr != m_pHitFunc)
		m_pHitFunc();

}

CSphereCollider* CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent, Engine::CEffect* pHitEffect, const _float& fRadius, const _tchar* pColliderTag, const _int& iAttack)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pParent, pHitEffect, fRadius, pColliderTag, iAttack)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSphereCollider::Free()
{
	if (false == m_bAddEffect)
		Engine::Safe_Release(m_pHitEffect);
	CCollider::Free();
}

