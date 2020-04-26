#include "Management.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement()
{

}

Engine::CManagement::~CManagement()
{
	Free();
}

HRESULT Engine::CManagement::SetUp_Scene(CScene* pScene)
{
	if (nullptr == m_pStaticLayer)
		m_pStaticLayer = CLayer::Create();
	
	NULL_CHECK_RETURN(m_pStaticLayer, E_FAIL);

	Safe_Release(m_pScene);
	
	Get_Renderer()->Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	_int iExit = m_pScene->Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = m_pStaticLayer->Update_Layer(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void Engine::CManagement::Render_Scene()
{
	Get_Renderer()->Render_GameObject();

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
	//m_pStaticLayer->Render_Layer();
}

HRESULT CManagement::Add_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pObj)
{
	if (nullptr == m_pScene)
		E_FAIL;

	return m_pScene->Add_GameObject(pLayerTag, pObjTag, pObj);
}

HRESULT CManagement::Add_GameObjectToStaticLayer(const _tchar * pObjTag, CGameObject * pObj)
{
	NULL_CHECK_RETURN_MSG(m_pStaticLayer, E_FAIL, L"Add GameObject to static layer failed, m_pStaticLayer is nullptr");

	FAILED_CHECK_RETURN(m_pStaticLayer->Add_GameObject(pObjTag, pObj), E_FAIL);

	return S_OK;
}

HRESULT CManagement::Create_StaticLayer()
{
	if (nullptr != m_pStaticLayer)
		return E_FAIL;
	
	m_pStaticLayer = CLayer::Create();

	return S_OK;
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pStaticLayer);
	Safe_Release(m_pScene);
}

const Engine::CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)	const
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

const CGameObject * CManagement::Get_GameObjcet(const _tchar * pLayerTag, const _tchar * pObjTag)	const
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObjcet(pLayerTag, pObjTag);
}

const CLayer * CManagement::Get_Layer(const _tchar * pLayerTag) const
{
	if (nullptr == m_pScene)
		return nullptr;
	return m_pScene->Get_Layer(pLayerTag);
}

const CComponent * CManagement::Get_Component_From_StaticLayer(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID) const
{
	return m_pStaticLayer->Get_Component(pObjTag, pComponentTag, eID);
}

const CGameObject * CManagement::Get_GameObject_From_StaticLayer(const _tchar * pObjTag) const
{
	return m_pStaticLayer->Get_GameObjcet(pObjTag);
}

const CComponent * CManagement::Get_Component_of_Player(const _tchar* pComponentTag, COMPONENTID eID) const
{
	return m_pStaticLayer->Get_Component(L"Player", pComponentTag, eID);
}

const CGameObject * CManagement::Get_Player() const
{
	return m_pStaticLayer->Get_GameObjcet(L"Player");
}

void CManagement::Player_Move_Pos(const _vec3 * pMove)
{
	m_pStaticLayer->Get_GameObject_NC(L"Player")->Move_Pos(pMove);
}

void CManagement::Player_Set_Pos(const _vec3 * pPos)
{
	m_pStaticLayer->Get_GameObject_NC(L"Player")->Set_Pos(pPos);
}
