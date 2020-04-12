#include "stdafx.h"
#include "FirstViewCamera.h"

#include "Export_Function.h"

CFirstViewCamera::CFirstViewCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CFirstViewCamera::~CFirstViewCamera()
{

}

HRESULT CFirstViewCamera::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	m_pTargetInfo = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	return S_OK;
}

_int CFirstViewCamera::Update_Camera(const _float& fTimeDelta)
{
	Update_EyeAtUp(fTimeDelta);

	_int iExit = Engine::CCamera::Update_Camera(fTimeDelta);

	return 0;
}

void CFirstViewCamera::Update_EyeAtUp(const _float & fTimeDelta)
{
	m_vEye = *m_pTargetInfo->GetInfo(Engine::INFO_POS);

	Mouse_Move(fTimeDelta);

	_matrix matRotX, matRotY, matRotZ, matRotAll;
	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);
	matRotAll = matRotX * matRotY * matRotZ;

	D3DXVec3TransformNormal(&m_vAt, &AXIS_Z, &matRotAll);
	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);
}

void CFirstViewCamera::Mouse_Move(const _float & fTimeDelta)
{
	if (false == m_bFixCursor)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))	//	X 축 회전
		m_vAngle.x += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))	//	Y 축 회전
		m_vAngle.y += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);
}

CFirstViewCamera * CFirstViewCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFirstViewCamera* pInstance = new CFirstViewCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFirstViewCamera::Free(void)
{
	Engine::CCamera::Free();
}
