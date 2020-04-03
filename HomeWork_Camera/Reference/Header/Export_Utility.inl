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

// Utility Release
void			Release_Utility(void)
{
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}
