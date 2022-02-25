#include "stdafx.h"
#include "QuaterViewCam.h"

#include "Export_Function.h"

CQuaterViewCam::CQuaterViewCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CQuaterViewCam::~CQuaterViewCam()
{

}

HRESULT CQuaterViewCam::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN_MSG(CCamera::Ready_GameObject(), E_FAIL, L"Ready_Camera Failed");

	return S_OK;
}

_int CQuaterViewCam::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CTransform*	pTarget_Transform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_MSG(pTarget_Transform, -1, L"Tartget Transform Failed");

	_vec3 vTargetPos, vUp, vLook;
	pTarget_Transform->Get_Info(Engine::INFO_POS,	&vTargetPos);
	m_pTransformCom->Set_Info(Engine::INFO_POS,		&vTargetPos);
	m_pTransformCom->Set_Angle_X(D3DXToRadian(m_fAngle_Degree));

	_matrix matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fAngle_Degree));
	D3DXVec3TransformNormal(&vLook, &_vec3(0.f, 0.f, 1.f), &matRotX);
	m_pTransformCom->Move_Pos(&(vLook * -m_fDistance));

	//_int iExit = CCamera::Update_GameObject(fTimeDelta);
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;
	
	D3DXMatrixLookAtLH(&m_matView, m_pTransformCom->Get_Info(Engine::INFO_POS), &vTargetPos, m_pTransformCom->Get_Info(Engine::INFO_UP));
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovy, m_fAspect, m_fNearZ, m_fFarZ);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return 0;
}

CQuaterViewCam * CQuaterViewCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fFovy, const _float & fAspect, const _float & fNearZ, const _float & fFarZ)
{
	CQuaterViewCam* pInst = new CQuaterViewCam(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	pInst->m_fFovy = fFovy;
	pInst->m_fAspect = fAspect;
	pInst->m_fNearZ = fNearZ;
	pInst->m_fFarZ = fFarZ;

	return pInst;
}

void CQuaterViewCam::Free(void)
{
	CCamera::Free();
}