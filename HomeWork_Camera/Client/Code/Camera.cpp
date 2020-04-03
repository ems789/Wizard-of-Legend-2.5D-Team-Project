#include "stdafx.h"
#include "Camera.h"

#include "Export_Function.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	ZeroMemory(&m_vUp, sizeof(_vec3));
}

CCamera::~CCamera()
{

}

HRESULT CCamera::Ready_GameObject(void)
{
	m_eCurState = CCamera::CAM_1ST;

	return S_OK;
}

_int CCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = 0;

	Key_Input();

	ChangeCameraView();

	switch (m_eCurState)
	{
	case CCamera::CAM_1ST:
		iExit = Cam_First_View_Update(fTimeDelta);
		break;
	case CCamera::CAM_3RD:
		iExit = Cam_Third_View_Update(fTimeDelta);
		break;
	case CCamera::CAM_TOP:
		iExit = Cam_Top_View_Update(fTimeDelta);
		break;
	}

	if (iExit & 0x80000000)
		return -1;

	iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CCamera::Render_GameObject(void)
{
	
}

HRESULT CCamera::Add_Component()
{
	Engine::CComponent* pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent->emplace(L"Com_Transform", pComponent);
	

	return S_OK;
}

_int CCamera::Cam_First_View_Update(const _float & fTimeDelta)
{
	Engine::CTransform* pTarget_Transform_Com = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTarget_Transform_Com, -1);
	
	pTarget_Transform_Com->Get_Info(Engine::INFO_POS,	&m_vEye);
	pTarget_Transform_Com->Get_Info(Engine::INFO_LOOK,	&m_vAt);
	pTarget_Transform_Com->Get_Info(Engine::INFO_UP,	&m_vUp);
	m_vAt += m_vEye;
	


	Cam_Update();

	return 0;
}

_int CCamera::Cam_Third_View_Update(const _float & fTimeDelta)
{
	Engine::CTransform* pTarget_Transform_Com = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTarget_Transform_Com, -1);

	_vec3 vTargetPos, vTargetLook;
	pTarget_Transform_Com->Get_Info(Engine::INFO_POS, &vTargetPos);
	pTarget_Transform_Com->Get_Info(Engine::INFO_LOOK, &vTargetLook);
	pTarget_Transform_Com->Get_Info(Engine::INFO_UP, &m_vUp);
	m_vEye = vTargetPos - vTargetLook * m_fCamToTarget_Dist;
	m_vAt = vTargetPos;
	
	Cam_Update();

	return 0;
}

_int CCamera::Cam_Top_View_Update(const _float & fTimeDelta)
{
	Engine::CTransform* pTarget_Transform_Com = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTarget_Transform_Com, -1);
	
	_vec3 vTargetPos, vTarget_To_Cam_Dir = { 0.f, 1.f, -1.f };
	pTarget_Transform_Com->Get_Info(Engine::INFO_POS, &vTargetPos);
	
	m_vEye = vTargetPos + vTarget_To_Cam_Dir * m_fCamToTarget_Dist;
	m_vAt = vTargetPos;
	D3DXVec3Cross(&m_vUp, &vTarget_To_Cam_Dir, &_vec3(1.f, 0.f, 0.f));

	Cam_Update();

	return 0;
}

void CCamera::Cam_Update()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fField_Of_View_Y, _float(WINCX) / WINCY, m_fNearZ, m_fFarZ);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCamera::Key_Input()
{
	if (GetAsyncKeyState('1') & 0x8000)
		m_eCurState = CCamera::CAM_1ST;
	if (GetAsyncKeyState('2') & 0x8000)
		m_eCurState = CCamera::CAM_3RD;
	if (GetAsyncKeyState('3') & 0x8000)
		m_eCurState = CCamera::CAM_TOP;

}

void CCamera::ChangeCameraView()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CCamera::CAM_1ST:
		::ShowCursor(FALSE);
		break;
	case CCamera::CAM_3RD:
		::ShowCursor(FALSE);
		break;
	case CCamera::CAM_TOP:
		::ShowCursor(TRUE);
		break;
	}

	m_ePreState = m_eCurState;
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCamera* pInst = new CCamera(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CCamera::Free(void)
{
	CGameObject::Free();
}

