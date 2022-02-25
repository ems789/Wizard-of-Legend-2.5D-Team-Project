// Management Instance
// Get
CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}
// Set
// General
HRESULT		Create_Management(CManagement** ppManagement)
{
	CManagement*		pInstance = CManagement::GetInstance();

	if (nullptr == pInstance)
		return E_FAIL;

	*ppManagement = pInstance;

	return S_OK;
}

// Renderer
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}


// Get
CCamera* Get_MainCam()
{
	return CCameraMgr::GetInstance()->Get_MainCam();
}
//General
// Camera
HRESULT	Add_Camera(const _tchar* pCamTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_Camera(pCamTag, pCamera);
}

HRESULT	Remove_Camera(const _tchar* pCamTag)
{
	return CCameraMgr::GetInstance()->Remove_Camera(pCamTag);
}

HRESULT	SetUp_Camera(const _tchar* pCamTag) 
{
	return CCameraMgr::GetInstance()->SetUp_Camera(pCamTag);
}

_int	Update_Camera(const _float& fTimeDelta)
{
	return CCameraMgr::GetInstance()->Update_Camera(fTimeDelta);
}

// Utility Release
void			Release_Utility(void)
{
	CCameraMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}
