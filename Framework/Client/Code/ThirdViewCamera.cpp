#include "stdafx.h"
#include "ThirdViewCamera.h"

#include "Export_Function.h"

CThirdViewCamera::CThirdViewCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CThirdViewCamera::~CThirdViewCamera()
{

}

HRESULT CThirdViewCamera::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	m_pTargetInfo = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetInfo, E_FAIL);

	return S_OK;
}

_int CThirdViewCamera::Update_Camera(const _float& fTimeDelta)
{
	//	Eye, At, Up 벡터 업데이트
	Update_EyeAtUp(fTimeDelta);
	Key_Input();

	//	matView, matProj 완성 후 적용
	_int	iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
}

void CThirdViewCamera::Update_EyeAtUp(const _float& fTimeDelta)
{
	Mouse_Move(fTimeDelta);

	m_vAt = *m_pTargetInfo->GetInfo(Engine::INFO_POS);

	_matrix matRotX, matRotY, matRotZ, matRotAll;

	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	matRotAll = matRotX * matRotY * matRotZ;

	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);

	m_vEye = _vec3(0.f, 0.f, -m_fDistance);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAll);
	m_vEye += m_vAt;
}

void CThirdViewCamera::Mouse_Move(const _float& fTimeDelta)
{
	if (false == m_bFixCursor)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))	//	X 축 회전
		m_vAngle.x += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))	//	Y 축 회전
		m_vAngle.y += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);
	
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Z))	//	Zoom In & Out
		m_fDistance += fTimeDelta * m_fSpeed * 0.3f * dwMouseMove;

	if (m_fDistance < 0.f)
		m_fDistance = 0.f;

	POINT pt = {WINCX / 2, WINCY / 2};
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CThirdViewCamera::Key_Input()
{
	if (Engine::KeyDown(DIK_TAB))
	{
		m_bFixCursor = !m_bFixCursor;

		if (m_bFixCursor)
			::ShowCursor(FALSE);
		else
			::ShowCursor(TRUE);
	}
}

CThirdViewCamera * CThirdViewCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThirdViewCamera* pInstance = new CThirdViewCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CThirdViewCamera::Free(void)
{
	CCamera::Free();
}
