#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID			Get_LoadingID() const	{ return m_eLoading; }
	CRITICAL_SECTION*	Get_Crt()				{ return &m_Crt; }
	_bool				Get_Finish() const		{ return m_bFinish; }
	const _tchar*		Get_LoadString()		{ return m_szLoading; }

public:
	static _uint	CALLBACK Thread_Main(void* pArg);

public:
	HRESULT		Ready_Loading(LOADINGID eLoading);
	_uint		Loading_ForState();

private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eLoading;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	_tchar				m_szLoading[STR_MAX];

public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading);

private:
	virtual void Free();
};

#endif // Loading_h__