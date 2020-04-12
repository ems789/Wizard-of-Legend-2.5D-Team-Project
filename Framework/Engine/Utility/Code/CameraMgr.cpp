#include "CameraMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCameraMgr)

Engine::CCameraMgr::CCameraMgr()
{

}

Engine::CCameraMgr::~CCameraMgr()
{
	Free();
}

_int Engine::CCameraMgr::Update_MainCamera(const _float& fTimeDelta)
{
	_int iExit = 0;

	if (nullptr == m_pMainCamera)
		iExit = Update_OrthogonalCam(fTimeDelta);
	else
		iExit = m_pMainCamera->Update_Camera(fTimeDelta);
	
	return iExit;
}

HRESULT Engine::CCameraMgr::Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
	m_fWinCX = static_cast<_float>(dwWinCX);
	m_fWinCY = static_cast<_float>(dwWinCY);

	return S_OK;
}

_int CCameraMgr::Update_NullCamera(const _float & fTimeDelta)
{
	_matrix matView, matProj;
	_vec3	vEye(0.f, 5.f, -5.f), vAt(0.f, 0.f, 0.f), vUp;
	D3DXVec3Cross(&vUp, &(vAt - vEye), &_vec3(1.f, 0.f, 0.f));

	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), 1.f, 1.f, 1000.f);
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW,			&matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION,	&matProj);

	return 0;
}

_int CCameraMgr::Update_OrthogonalCam(const _float & fTimeDelta)
{
	_matrix matView, matProj;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixOrthoLH(&matProj, m_fWinCX, m_fWinCY, 0.f, 1.f);

	m_pGraphicDev->SetTransform(D3DTS_VIEW,			&matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION,	&matProj);

	return 0;
}

HRESULT Engine::CCameraMgr::SetUp_MainCamera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	CCamera* pCamera = Get_Camera(dwContainerIdx, pCameraTag);

	if (nullptr == pCamera)
		return E_FAIL;

	Safe_Release(m_pMainCamera);
	m_pMainCamera = pCamera;
	m_pMainCamera->AddRef();

	return S_OK;
}

HRESULT Engine::CCameraMgr::Add_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag, CCamera* pCamera)
{
	if (nullptr != Get_Camera(dwContainerIdx, pCameraTag))
	{
		_tchar szFailText[256] = L"";
		wsprintf(szFailText, L"Add ", pCameraTag, L" failed. ", pCameraTag, L" already exist.");
		MessageBox(NULL, szFailText, L"Add Camera Failed", MB_OK);
		return E_FAIL;
	}

	m_mapCamera[dwContainerIdx].emplace(pCameraTag, pCamera);
}

HRESULT Engine::CCameraMgr::Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	auto iter = find_if(m_mapCamera[dwContainerIdx].begin(), m_mapCamera[dwContainerIdx].end(), CTag_Finder(pCameraTag));

	if (m_mapCamera[dwContainerIdx].end() == iter)
	{
		_tchar szFailText[256] = L"";
		wsprintf(szFailText, L"Remove ", pCameraTag, L" failed. There is no camera that named");
		MessageBox(NULL, szFailText, L"Remove Camera Failed", MB_OK);
		return E_FAIL;
	}

	if (Safe_Release(iter->second))
	{
		_tchar szFailText[256] = L"";
		wsprintf(szFailText, L"Remove ", pCameraTag, L" failed. Reference Counter is not 0");
		MessageBox(NULL, szFailText, L"Remove Camera Failed", MB_OK);
		return E_FAIL;
	}

	if (m_pMainCamera == iter->second)
	{
		Safe_Release(m_pMainCamera);
		m_pMainCamera = nullptr;
	}

	m_mapCamera[dwContainerIdx].erase(iter);

	return S_OK;
}

Engine::CCamera* Engine::CCameraMgr::Get_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	auto iter = find_if(m_mapCamera[dwContainerIdx].begin(), m_mapCamera[dwContainerIdx].end(), CTag_Finder(pCameraTag));

	if (m_mapCamera[dwContainerIdx].end() == iter)
		return nullptr;

	return iter->second;
}

void Engine::CCameraMgr::Free()
{
	Safe_Release(m_pMainCamera);
	for (_uint i = 0; i < CAM_END; ++i)
	{
		for_each(m_mapCamera[i].begin(), m_mapCamera[i].end(), CDeleteMap());
		m_mapCamera[i].clear();
	}
	Safe_Release(m_pGraphicDev);
}