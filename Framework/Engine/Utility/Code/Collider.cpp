#include "Collider.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

Engine::CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
{

}

CCollider::~CCollider()
{

}

Engine::CComponent* CCollider::Clone(void)
{
	return new CCollider(*this);
}


void CCollider::Free()
{
	Safe_Release(m_pGraphicDev);
}