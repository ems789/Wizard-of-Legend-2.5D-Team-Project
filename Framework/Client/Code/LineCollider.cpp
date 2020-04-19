#include "stdafx.h"
#include "LineCollider.h"

#include "Export_Function.h"

CLineCollider::CLineCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCollider(pGraphicDev)
{

}

CLineCollider::~CLineCollider()
{

}


HRESULT CLineCollider::Ready_Collider(CGameObject * pParent, Engine::CEffect* pHitEffect, const _float & fLength, const _vec3* pAngle, const _tchar* pColliderTag, const _int& iAttack)
{
	FAILED_CHECK_RETURN(CCollider::Ready_Collider(pParent), E_FAIL);

	m_eType = Engine::CT_LINE;

	_matrix matRotX, matRotY, matRotZ;
	D3DXMatrixRotationX(&matRotX, pAngle->x);
	D3DXMatrixRotationY(&matRotY, pAngle->y);
	D3DXMatrixRotationZ(&matRotZ, pAngle->z);
	_vec3 vInit = { 0.f, 0.f, fLength };
	D3DXVec3TransformNormal(&m_vPos2, &vInit, &(matRotX * matRotY * matRotZ));
	m_tLine.vPos1 = *m_pParent->Get_Pos();
	m_tLine.vPos2 = m_tLine.vPos1 + m_vPos2;

	m_pColliderTag = pColliderTag;
	m_iAttack = iAttack;
	m_pHitEffect = pHitEffect;
	m_bAddEffect = false;

	return S_OK;
}


_int CLineCollider::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_pParent->IsDead())
	{
		m_bIsDead = true;
		return 0;
	}
	m_tLine.vPos1 = *m_pParent->Get_Pos();
	m_tLine.vPos2 = m_tLine.vPos1 + m_vPos2;

	_int iExit = CCollider::Update_GameObject(fTimeDelta);

	Engine::Add_GameObject_To_CollisionList(m_pColliderTag, this);
	
	return iExit;
}

void CLineCollider::Render_GameObject()
{
	
}

void CLineCollider::Add_Effect(const _vec3 * pPos)
{
	if (true == m_bAddEffect)
		return;

	m_bAddEffect = true;

	if (nullptr == m_pHitEffect)
		return;

	_vec3 vHitPoint = *pPos;

	m_pHitEffect->Set_Pos(&vHitPoint);

	Engine::Add_GameObject(L"GameLogic", m_pHitEffect->Get_EffectTag(), m_pHitEffect);

}

CLineCollider* CLineCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent, Engine::CEffect* pHitEffect, const _float& fLength, const _vec3* pAngle, const _tchar* pColliderTag, const _int& iAttack)
{
	CLineCollider* pInstance = new CLineCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pParent, pHitEffect, fLength, pAngle, pColliderTag, iAttack)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLineCollider::Free()
{
	if (false == m_bAddEffect)
		Engine::Safe_Release(m_pHitEffect);
	CCollider::Free();
}

