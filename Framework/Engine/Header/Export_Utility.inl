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
const	CLayer*			Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
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
void Player_Move_Pos(const _vec3* pMove)
{
	CManagement::GetInstance()->Player_Move_Pos(pMove);
}
void Player_Set_Pos(const _vec3* pPos)
{
	CManagement::GetInstance()->Player_Set_Pos(pPos);
}
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

HRESULT	Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pObj)
{
	return CManagement::GetInstance()->Add_GameObject(pLayerTag, pObjTag, pObj);
}

HRESULT Add_GameObjectToStaticLayer(const _tchar* pObjTag, CGameObject* pObj)
{
	return CManagement::GetInstance()->Add_GameObjectToStaticLayer(pObjTag, pObj);
}
HRESULT Create_StaticLayer()
{
	return CManagement::GetInstance()->Create_StaticLayer();
}
HRESULT	Clear_StaticLayer()
{
	return CManagement::GetInstance()->Clear_StaticLayer();
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
CCamera* Get_MainCamera()
{
	return CCameraMgr::GetInstance()->Get_MainCamera();
}
void Get_MainCameraLook(_vec3* pLook)
{
	CCameraMgr::GetInstance()->Get_MainCameraLook(pLook);
}
void Get_MainCameraRight(_vec3* pRight)
{
	CCameraMgr::GetInstance()->Get_MainCameraRight(pRight);
}
void Get_MainCameraUp(_vec3* pUp)
{
	CCameraMgr::GetInstance()->Get_MainCameraUp(pUp);
}

const _vec3* Get_MainCameraAngle(_vec3* pAngle)
{
	return CCameraMgr::GetInstance()->Get_MainCameraAngle(pAngle);
}
CCameraMgr::MAIN_CAM Get_MainCamType()
{
	return CCameraMgr::GetInstance()->Get_MainCamType();
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

HRESULT	Change_MainCamera(const _ubyte& byOrder)
{
	return CCameraMgr::GetInstance()->Change_MainCamera(byOrder);
}

HRESULT	Add_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_Camera(dwContainerIdx, pCameraTag, pCamera);
}
HRESULT	Add_BasicCamera(const _ubyte& byOrder, const _tchar* pCameraTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_BasicCamera(byOrder, pCameraTag, pCamera);
}
HRESULT	Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	return CCameraMgr::GetInstance()->Remove_Camera(dwContainerIdx, pCameraTag);
}



//	CollisionMgr
//	Get
list<CGameObject*>*	Get_CollisionObjectList(const _tchar* pCollisionListTag)
{
	return CCollisionMgr::GetInstance()->Get_CollisionObjectList(pCollisionListTag);
}
//	Set
//	General
void	Add_GameObject_To_CollisionList(const _tchar* pCollisionListTag, CGameObject* pObj)
{
	CCollisionMgr::GetInstance()->Add_GameObject(pCollisionListTag, pObj);
}

void Add_CollisionFunction(const _tchar* pSrcTag, const _tchar* pDestTag, void(pFunc)(const _tchar*, const _tchar*))
{
	CCollisionMgr::GetInstance()->Add_CollisionFunction(pSrcTag, pDestTag, pFunc);
}

//	Utility Release
void Release_Utility(void)
{
	CCollisionMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}