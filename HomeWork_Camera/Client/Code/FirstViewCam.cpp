#include "stdafx.h"
#include "FirstViewCam.h"

#include "Export_Function.h"

CFirstViewCam::CFirstViewCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CFirstViewCam::~CFirstViewCam()
{

}

HRESULT CFirstViewCam::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN_MSG(CCamera::Ready_GameObject(), E_FAIL, L"Ready_Camera Failed");

	return S_OK;
}

_int CFirstViewCam::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CTransform* pTarget_Transform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTarget_Transform, -1);
	
	m_pTransformCom->Set_Pos(pTarget_Transform->Get_Info(Engine::INFO_POS));

	Rotation(fTimeDelta);

	_int iExit = CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFirstViewCam::Rotation(const _float & fTimeDelta)
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

CFirstViewCam * CFirstViewCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fFovy/* = D3DXToRadian(60.f)*/,
									const _float& fAspect/* = _float(WINCX) / WINCY*/, const _float& fNearZ/* = 1.f*/, const _float& fFarZ/* = 1000.f*/)
{
	CFirstViewCam* pInst = new CFirstViewCam(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	pInst->m_fFovy = fFovy;
	pInst->m_fAspect = fAspect;
	pInst->m_fNearZ = fNearZ;
	pInst->m_fFarZ = fFarZ;

	return pInst;
}


void CFirstViewCam::Free(void)
{
	CCamera::Free();
}

