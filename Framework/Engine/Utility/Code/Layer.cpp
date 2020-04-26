#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer()
{

}

Engine::CLayer::~CLayer()
{

}

const CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID) const
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (m_mapObject.end() == iter)
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

const CGameObject * CLayer::Get_GameObjcet(const _tchar * pObjTag) const
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (m_mapObject.end() == iter)
		return nullptr;

	return iter->second;
}

CGameObject * CLayer::Get_GameObject_NC(const _tchar * pObjTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (m_mapObject.end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT Engine::CLayer::Ready_Layer()
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int iExit = 0;

	auto iter		= m_mapObject.begin();
	auto iterEnd	= m_mapObject.end();

	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end(); )
	{
		iExit = iter->second->Update_GameObject(fTimeDelta);

		if (iExit & 0x80000000)
			return -1;

		if (iter->second->IsDead())
		{
			//if (Safe_Release(iter->second))
			//	iter->second->Set_IsDead(true);
			//else
			//	iter = m_mapObject.erase(iter);
			Safe_Release(iter->second);
			iter = m_mapObject.erase(iter);
		}
		else
			++iter;
	}

	return iExit;
}

void Engine::CLayer::Render_Layer()
{
	for (auto& iter : m_mapObject)
		iter.second->Render_GameObject();
}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	//auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	//if (m_mapObject.end() != iter)
	//	return E_FAIL;

	m_mapObject.emplace(pObjTag, pGameObject);

	return S_OK;
}

Engine::CLayer* Engine::CLayer::Create()
{
	CLayer* pInst = new CLayer;

	if (FAILED(pInst->Ready_Layer()))
		Safe_Release(pInst);

	return pInst;
}

void Engine::CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}

