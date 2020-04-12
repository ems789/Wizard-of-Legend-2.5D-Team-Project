#include "stdafx.h"
#include "QuaterViewCamera.h"

#include "Export_Function.h"

CQuaterViewCamera::CQuaterViewCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CQuaterViewCamera::~CQuaterViewCamera()
{

}

HRESULT CQuaterViewCamera::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	return S_OK;
}

_int CQuaterViewCamera::Update_Camera(const _float& fTimeDelta)
{
	_int iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
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

