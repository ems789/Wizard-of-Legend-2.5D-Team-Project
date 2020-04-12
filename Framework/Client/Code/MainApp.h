#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"

#include "Engine_Defines.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END
class CMainApp : public CBase
{
private:
	explicit CMainApp(void);
	virtual ~CMainApp(void);

public:
	HRESULT Ready_MainApp(void);
	_int Update_MainApp(const _float& fTimeDelta);
	void Render_MainApp(void);

private:
	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagement);

private:
	Engine::CGraphicDev*	m_pDeviceClass = nullptr;
	Engine::CManagement*	m_pManagement = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;;

	_int					m_iExit = 0;

public:
	static CMainApp* Create(void);

private:
	virtual void Free(void);	

};



#endif // MainApp_h__
