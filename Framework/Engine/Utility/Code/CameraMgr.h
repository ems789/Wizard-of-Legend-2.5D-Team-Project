#ifndef CameraMgr_h__
#define CameraMgr_h__

#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	HRESULT		Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY);
	_int		Update_MainCamera(const _float& fTimeDelta);
	HRESULT		SetUp_RenderUI();
	HRESULT		Finish_RenderUI();
	
private:
	_int		Update_NullCamera(const _float& fTimeDelta);
	_int		Update_OrthogonalCam();

public:
	HRESULT		SetUp_MainCamera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);

	HRESULT		Add_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag, CCamera* pCamera);
	HRESULT		Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);

	CCamera*	Get_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	map<const _tchar*, CCamera*>	m_mapCamera[CAM_END];

	CCamera*						m_pMainCamera = nullptr;
	_float							m_fWinCX = 1.f;
	_float							m_fWinCY = 1.f;

private:
	virtual void Free();
};

END

#endif // CameraMgr_h__
