#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	ZeroMemory(&m_vUp, sizeof(_vec3));
}

Engine::CCamera::~CCamera()
{

}

HRESULT Engine::CCamera::Ready_Camera()
{
	m_vEye	= { 0.f, 0.f, 0.f };
	m_vAt	= { 0.f, 0.f, 1.f };
	m_vUp	= { 0.f, 1.f, 0.f };

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW,	&m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int Engine::CCamera::Update_Camera(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return 0;
}

void CCamera::Get_Right(_vec3 * pRight) const
{
	_vec3 vLook = m_vAt - m_vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(pRight, &m_vUp, &vLook);
	D3DXVec3Normalize(pRight, pRight);
}

void CCamera::Get_Look(_vec3 * pLook) const
{
	*pLook = m_vAt - m_vEye;
	D3DXVec3Normalize(pLook, pLook);
}

const _vec3 * CCamera::Get_Angle(_vec3 * pAngle)
{
	_vec3 vAngle;



	return nullptr;
}

void Engine::CCamera::Free(void)
{
	Safe_Release(m_pGraphicDev);
}