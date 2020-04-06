#include "CameraMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{

}

CCameraMgr::~CCameraMgr()
{
	Free();
}

HRESULT CCameraMgr::Add_Camera(const _tchar* pCamTag, CCamera* pCamera)
{
	CCamera* pCam = Find_Camera(pCamTag);
	if (nullptr != pCam)
		return E_FAIL;

	m_mapCamera.emplace(pCamTag, pCamera);
	return S_OK;
}

HRESULT CCameraMgr::Remove_Camera(const _tchar* pCamTag)
{
	auto iter = find_if(m_mapCamera.begin(), m_mapCamera.end(), CTag_Finder(pCamTag));

	if (m_mapCamera.end() == iter)
		return E_FAIL;

	if (m_pMainCam == iter->second)
		return E_FAIL;

	m_mapCamera.erase(iter);

	return S_OK;
}

HRESULT CCameraMgr::SetUp_Camera(const _tchar* pCamTag)
{
	CCamera*	pNewCam = Find_Camera(pCamTag);
	NULL_CHECK_RETURN(pNewCam, E_FAIL);

	Safe_Release(m_pMainCam);

	m_pMainCamTag = pCamTag;
	m_pMainCam = pNewCam;
	m_pMainCam->AddRef();

	return S_OK;
}

_int CCameraMgr::Update_Camera(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pMainCam, -1);

	return m_pMainCam->Update_GameObject(fTimeDelta);
}

CCamera * CCameraMgr::Find_Camera(const _tchar * pCamTag)
{
	auto iter = find_if(m_mapCamera.begin(), m_mapCamera.end(), CTag_Finder(pCamTag));
	if (m_mapCamera.end() == iter)
		return nullptr;

	return iter->second;
}


void CCameraMgr::Free(void)
{
	Safe_Release(m_pMainCam);
	for_each(m_mapCamera.begin(), m_mapCamera.end(), CDeleteMap());
	m_mapCamera.clear();
}
