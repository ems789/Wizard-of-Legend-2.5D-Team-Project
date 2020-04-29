#include "CameraMgr.h"
#include "InputDev.h"

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

	Key_Input();

	if (nullptr == m_pMainCamera)
		iExit = Update_OrthogonalCam();
	else
		iExit = m_pMainCamera->Update_Camera(fTimeDelta);
	
	return iExit;
}

HRESULT CCameraMgr::SetUp_Alpha()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000008);


	return S_OK;

}

HRESULT CCameraMgr::Finish_Alpha()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CCameraMgr::SetUp_RenderUI()
{
	_int iExit = Update_OrthogonalCam();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (-1 == iExit)
		return E_FAIL;

	return S_OK;
}

HRESULT CCameraMgr::Finish_RenderUI()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//_int iExit = Update_MainCamera(0.f);

	return S_OK;
}

HRESULT Engine::CCameraMgr::Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
	m_fWinCX = static_cast<_float>(dwWinCX);
	m_fWinCY = static_cast<_float>(dwWinCY);

	for (_uint i = 0; i < MAIN_CAM_END; ++i)
		m_szBasicCamTag[i] = nullptr;

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

_int CCameraMgr::Update_OrthogonalCam()
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
	m_pMainCamera->Reset_Camera();
	m_pMainCamera->AddRef();

	return S_OK;
}

HRESULT CCameraMgr::Change_MainCamera(const _ubyte & byOrder)
{
	if (!m_szBasicCamTag[byOrder])
		return E_FAIL;

	FAILED_CHECK_RETURN(SetUp_MainCamera(CAM_STATIC, m_szBasicCamTag[byOrder]), E_FAIL);
	m_eMainCam = static_cast<MAIN_CAM>(byOrder);

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

	return S_OK;
}

HRESULT CCameraMgr::Add_BasicCamera(const _ubyte & byOrder, const _tchar * pCameraTag, CCamera * pCamera)
{
	if (byOrder > 3)
	{
		MSG_BOX("Add_BasicCamera Failed. byOrder is bigger than 3.");
		return E_FAIL;
	}

	FAILED_CHECK_RETURN(Add_Camera(CAM_STATIC, pCameraTag, pCamera), E_FAIL);
	
	m_szBasicCamTag[byOrder] = pCameraTag;
	
	return S_OK;
}

HRESULT Engine::CCameraMgr::Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag)
{
	auto iter = find_if(m_mapCamera[dwContainerIdx].begin(), m_mapCamera[dwContainerIdx].end(), CTag_Finder(pCameraTag));

	if (m_mapCamera[dwContainerIdx].end() == iter)
	{
		wstring wstrText = L"";
		wstrText = L"Remove";
		wstrText += pCameraTag;
		wstrText += L" failed. There is no camera that named";

		MessageBox(NULL, wstrText.c_str(), L"Remove Camera Failed", MB_OK);
		return E_FAIL;
	}

	if (m_pMainCamera == iter->second)
	{
		Safe_Release(m_pMainCamera);
		m_pMainCamera = nullptr;
	}

	if (Safe_Release(iter->second))
	{
		wstring wstrText = L"";
		wstrText = L"Remove";
		wstrText += pCameraTag;
		wstrText += L" failed. Reference Counter is not 0";

		MessageBox(NULL, wstrText.c_str(), L"Remove Camera Failed", MB_OK);
		return E_FAIL;
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

void CCameraMgr::Key_Input()
{
	if (false == m_bCanKeyInput)
		return;
	if (CInputDev::GetInstance()->KeyDown(DIK_F1))
	{
		m_eMainCam = MAIN_CAM_1ST;
		SetUp_MainCamera(CAM_STATIC, m_szBasicCamTag[m_eMainCam]);
	}
	if (CInputDev::GetInstance()->KeyDown(DIK_F2))
	{
		m_eMainCam = MAIN_CAM_3RD;
		SetUp_MainCamera(CAM_STATIC, m_szBasicCamTag[m_eMainCam]);
	}
	if (CInputDev::GetInstance()->KeyDown(DIK_F3))
	{
		m_eMainCam = MAIN_CAM_QUATER;
		SetUp_MainCamera(CAM_STATIC, m_szBasicCamTag[m_eMainCam]);
	}
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