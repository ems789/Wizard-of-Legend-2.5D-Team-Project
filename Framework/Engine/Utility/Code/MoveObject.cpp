#include "MoveObject.h"

USING(Engine)

Engine::CMoveObject::CMoveObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CMoveObject::~CMoveObject()
{

}

HRESULT Engine::CMoveObject::Ready_GameObject()
{
	return S_OK;
}

_int Engine::CMoveObject::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void Engine::CMoveObject::Render_GameObject()
{

}

Engine::SPHERE* Engine::CMoveObject::Get_Sphere()
{
	return nullptr;
}

void Engine::CMoveObject::Free()
{
	CGameObject::Free();
}

