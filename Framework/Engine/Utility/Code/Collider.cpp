#include "Collider.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

Engine::CCollider::CCollider(const CCollider& rhs)
	: CGameObject(rhs)
{

}

CCollider::~CCollider()
{

}


HRESULT CCollider::Ready_Collider(CGameObject * pParent)
{
	NULL_CHECK_RETURN(pParent, E_FAIL);
	m_pParent = pParent;
	m_pParent->AddRef();
	return S_OK;
}

HRESULT Engine::CCollider::Ready_GameObject()
{
	

	return S_OK;
}

_int Engine::CCollider::Update_GameObject(const _float& fTimeDelta)
{
	const _vec3* vPos = m_pParent->Get_Pos();



	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void Engine::CCollider::Render_GameObject()
{

}


void CCollider::Free()
{
	Safe_Release(m_pParent);
	CGameObject::Free();
}

