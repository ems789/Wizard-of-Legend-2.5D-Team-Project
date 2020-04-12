#include "Effect.h"

USING(Engine)

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CEffect::~CEffect()
{

}

HRESULT CEffect::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

_int CEffect::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;
	
	return iExit;
}

void CEffect::Render_GameObjcet()
{
	
}

void CEffect::Free()
{
	CGameObject::Free();
}

