#include "stdafx.h"
#include "QuaterViewCamera.h"
#include "Mouse.h"

#include "Export_Function.h"

CQuaterViewCamera::CQuaterViewCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CQuaterViewCamera::~CQuaterViewCamera()
{

}

HRESULT CQuaterViewCamera::Reset_Camera()
{
	CMouse::GetInstance()->AnimingPointOff();
	CMouse::GetInstance()->CursorRenderOn();


	m_fShakingDir = 0.2f;

	return S_OK;
}
 
HRESULT CQuaterViewCamera::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	m_pTargetInfo = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetInfo, E_FAIL);
	m_fAspect = static_cast<_float>(WINCX) / WINCY;
	return S_OK;
}

_int CQuaterViewCamera::Update_Camera(const _float& fTimeDelta)
{
	Update_EyeAtUp(fTimeDelta);

	_int iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
}

_int CQuaterViewCamera::Update_EyeAtUp(const _float & fTimeDelta)
{
	m_vAt = *m_pTargetInfo->GetInfo(Engine::INFO_POS);

	//Key_Input(fTimeDelta);

	if (m_bShaking)
	{
		m_fShakingTime += fTimeDelta;
		m_vAt.y += m_fShakingDir;
		m_fShakingDir *= -1.f;
		if (m_fShakingTime >= 0.5f)
		{
			m_bShaking = false;
			m_fShakingTime = 0.f;
		}
	}

	_matrix matRotX, matRotY, matRotZ, matRotAll;
	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	matRotAll = matRotX * matRotY * matRotZ;

	m_vEye = _vec3(0.f, 0.f, -m_fDistance);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAll);
	m_vEye += m_vAt;
	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);

	return 0;
}

_int CQuaterViewCamera::Key_Input(const _float & fTimeDelta)
{
	if (Engine::KeyPress(DIK_UP))
		m_vAngle.x -= m_fSpeed * fTimeDelta;
	if (Engine::KeyPress(DIK_DOWN))
		m_vAngle.x += m_fSpeed * fTimeDelta;
	if (Engine::KeyPress(DIK_LEFT))
		m_vAngle.y -= m_fSpeed * fTimeDelta;
	if (Engine::KeyPress(DIK_RIGHT))
		m_vAngle.y += m_fSpeed * fTimeDelta;

	if (Engine::KeyDown(DIK_BACKSPACE))
		m_vAngle = INITIAL_ANGLE;

	return 0;
}

_int CQuaterViewCamera::Reset_Angle()
{
	return _int();
}

void CQuaterViewCamera::CameraShake()
{
	m_bShaking = true;
}

_vec2 CQuaterViewCamera::Get_MousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return _vec2(static_cast<_float>(pt.x), static_cast<_float>(pt.y));
}

CQuaterViewCamera * CQuaterViewCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuaterViewCamera* pInstance = new CQuaterViewCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CQuaterViewCamera::Free(void)
{
	CCamera::Free();
}

