#include "UI.h"

USING(Engine)

Engine::CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CUI::~CUI()
{

}

HRESULT Engine::CUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

_int Engine::CUI::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void Engine::CUI::Render_GameObjcet()
{
	
}

void Engine::CUI::Free()
{
	CGameObject::Free();
}
