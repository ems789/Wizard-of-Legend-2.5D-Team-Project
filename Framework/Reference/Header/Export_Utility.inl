//	Management Instance
//	Get
const CComponent*	Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}
const CGameObject*	Get_GameObjcet(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObjcet(pLayerTag, pObjTag);
}
CLayer*			Get_StaticLayer()
{
	return CManagement::GetInstance()->Get_StaticLayer();
}

const CComponent*	Get_Component_From_StaticLayer(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component_From_StaticLayer(pObjTag, pComponentTag, eID);
}
const CGameObject*	Get_GameObject_From_StaticLayer(const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject_From_StaticLayer(pObjTag);
}

const CComponent*	Get_Component_of_Player(const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component_of_Player(pComponentTag, eID);
}
const CGameObject*	Get_Player()
{
	return CManagement::GetInstance()->Get_Player();
}

//	Set
//	General
HRESULT Create_Management(CManagement** ppManagemet)
{
	(*ppManagemet) = CManagement::GetInstance();

	if (nullptr == (*ppManagemet))
		return E_FAIL;

	return S_OK;
}

HRESULT SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}

HRESULT Add_GameObjectToStaticLayer(const _tchar* pObjTag, CGameObject* pObj)
{
	return CManagement::GetInstance()->Add_GameObjectToStaticLayer(pObjTag, pObj);
}

//	Renderer
CRenderer* Get_Renderer()
{
	return CRenderer::GetInstance();
}


//	Camera Manager
//	Get
CCamera* Get_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	return CCameraMgr::GetInstance()->Get_Camera(dwContainerIdx, pCameraTag);
}
//	Set
//	General
HRESULT	Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY)
{
	return CCameraMgr::GetInstance()->Ready_CameraMgr(pGraphicDev, dwWinCX, dwWinCY);
}
_int	Update_MainCamera(const _float& fTimeDelta)
{
	return CCameraMgr::GetInstance()->Update_MainCamera(fTimeDelta);
}

HRESULT	SetUp_MainCamera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	return CCameraMgr::GetInstance()->SetUp_MainCamera(dwContainerIdx, pCameraTag);
}

HRESULT	Add_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_Camera(dwContainerIdx, pCameraTag, pCamera);
}
HRESULT	Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	return CCameraMgr::GetInstance()->Remove_Camera(dwContainerIdx, pCameraTag);
}




//	Utility Release
void Release_Utility(void)
{
	CCameraMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}