#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject()
{

}

const CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID) const
{
	auto iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (m_mapComponent[eID].end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT Engine::CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void Engine::CGameObject::Render_GameObject()
{

}

void Engine::CGameObject::Free()
{
	for (_int i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

