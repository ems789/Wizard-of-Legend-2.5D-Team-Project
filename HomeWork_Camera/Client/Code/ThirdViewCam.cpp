#include "stdafx.h"
#include "ThirdViewCam.h"

#include "Export_Function.h"

CThirdViewCam::CThirdViewCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CThirdViewCam::~CThirdViewCam()
{

}

HRESULT CThirdViewCam::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN_MSG(CCamera::Ready_GameObject(), E_FAIL, L"Ready_Camera Failed");

	return S_OK;
}

_int CThirdViewCam::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CTransform* pTarget_Transform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTarget_Transform, -1);

	_vec3 vTargetPos, vLook, vUp;
	pTarget_Transform->Get_Info(Engine::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransformCom->Set_Info(Engine::INFO_POS, &(vTargetPos - vLook * m_fTargetDistance));

	Rotation(fTimeDelta);
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

void CThirdViewCam::Rotation(const _float & fTimeDelta)
{
	::POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	_vec2 vDifference = _vec2(pt.x - (WINCX / 2), pt.y - (WINCY / 2));

	if (vDifference.x != 0.f)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(vDifference.x * m_fSpeed * fTimeDelta));

	if (vDifference.y != 0.f)
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(vDifference.y * m_fSpeed * fTimeDelta));

	pt = { WINCX / 2, WINCY / 2 };
	::ClientToScreen(g_hWnd, &pt);
	::SetCursorPos(pt.x, pt.y);
}

CThirdViewCam * CThirdViewCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fFovy, const _float & fAspect, const _float & fNearZ, const _float & fFarZ, const _float& fTargetDistance/* = 10.f*/)
{
	CThirdViewCam* pInst = new CThirdViewCam(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	pInst->m_fFovy				= fFovy;
	pInst->m_fAspect			= fAspect;
	pInst->m_fNearZ				= fNearZ;
	pInst->m_fFarZ				= fFarZ;
	pInst->m_fTargetDistance	= fTargetDistance;

	return pInst;
}

void CThirdViewCam::Free(void)
{
	CCamera::Free();
}

