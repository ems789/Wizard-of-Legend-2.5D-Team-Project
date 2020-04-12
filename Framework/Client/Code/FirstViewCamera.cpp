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
	Mouse_Input(fTimeDelta);

	_int iExit = Engine::CCamera::Update_Camera(fTimeDelta);

	return 0;
}

void CFirstViewCamera::Update_EyeAtUp(const _float & fTimeDelta)
{
}

void CFirstViewCamera::Mouse_Input(const _float & fTimeDelta)
{
	
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

