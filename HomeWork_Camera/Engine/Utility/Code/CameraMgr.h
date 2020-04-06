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
	HRESULT			Add_Camera(const _tchar* pCamTag, CCamera* pCamera);
	HRESULT			Remove_Camera(const _tchar* pCamTag);

	HRESULT			SetUp_Camera(const _tchar* pCamTag);
	_int			Update_Camera(const _float& fTimeDelta);

public:
	const _tchar*	Get_MainCamTag() const	{ return m_pMainCamTag; }
	CCamera*		Get_MainCam()			{ return m_pMainCam; }

private:
	CCamera*		Find_Camera(const _tchar* pCamTag);

private:
	map<const _tchar*, CCamera*>	m_mapCamera;
	const _tchar*					m_pMainCamTag = nullptr;
	CCamera*						m_pMainCam = nullptr;

public:
	virtual void Free(void) override;

};

END

#endif // CameraMgr_h__
