#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Defines.h"
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"

BEGIN(Engine)

// GraphicDev
// Get
// Set
// General
inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);

// TimerMgr
// Get
inline void		Set_TimeDelta(const _tchar* pTimerTag);
// Set
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
// General
inline HRESULT	Ready_Timer(const _tchar* pTimerTag);

// FrameMgr
// Get
// Set
// General
inline _bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);


// Release System
inline void			Release_System(void);

#include "Export_System.inl"

END



#endif // Export_System_h__
