#include "Scene.h"
#include "CollisionMgr.h"
USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene()
{

}

const CComponent * CScene::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID) const
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

const CGameObject * CScene::Get_GameObjcet(const _tchar * pLayerTag, const _tchar * pObjTag) const
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second->Get_GameObjcet(pObjTag);
}

const CLayer * CScene::Get_Layer(const _tchar * pLayerTag) const
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second;
}



HRESULT Engine::CScene::Ready_Scene()
{
	m_pCollisionMgr = CCollisionMgr::GetInstance();
	NULL_CHECK_RETURN(m_pCollisionMgr, E_FAIL);
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = 0;
	
	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(fTimeDelta);

		if (iExit & 0x80000000)
			return -1;
	}

	m_pCollisionMgr->Collision(fTimeDelta);

	return 0;
}

void Engine::CScene::Render_Scene()
{
	//for (auto& iter : m_mapLayer)
	//	iter.second->Render_Layer();
}

HRESULT CScene::Add_Layer(const _tchar * pLayerTag, CLayer * pLayer)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (m_mapLayer.end() != iter)
	{
		MSG_BOX("Add_Layer Failed");
		return E_FAIL;
	}

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CScene::Add_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject* pObj)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (m_mapLayer.end() == iter)
	{
		MSG_BOX("Add_GameObject Failed. Layer does not exist");
		return E_FAIL;
	}

	iter->second->Add_GameObject(pObjTag, pObj);

	return S_OK;
}

void Engine::CScene::Free()
{
	//m_pCollisionMgr->DestroyInstance();

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}

