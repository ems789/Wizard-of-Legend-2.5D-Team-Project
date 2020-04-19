//	GraphicDev
//	Get
//	Set
//	General
HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void	Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void	Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}

void	Render_End(HWND hWnd)
{
	CGraphicDev::GetInstance()->Render_End(hWnd);
}

//	TimerMgr
//	Get
//	Set
//	General
void Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
_float Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
_float	Get_CummulativeTime(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_CummulativeTime(pTimerTag);
}

HRESULT Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

//	FrameMgr
//	Get
//	Set
//	General
_bool IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

//	FontMgr
//	Get
//	Set
//	General
HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}
void	Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	Render_Font(pFontTag, pString, pPos, Color);
}

//	InputDev
//	Get
_byte Get_DIKeyState(_ubyte byKeyID) 
{ 
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte Get_DIMouseState(MOUSEKEYSTATE eMouse) 
{ 
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{ 
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}
// Set
void	Set_InputDev()
{
	CInputDev::GetInstance()->Set_InputDev();
}
// General
HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}

_bool	KeyDown(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyDown(byKeyID);
}
_bool	KeyUp(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyUp(byKeyID);
}
_bool	KeyPress(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyPress(byKeyID);
}

_bool	MouseDown(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MouseDown(eMouse);
}
_bool	MouseUp(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MouseUp(eMouse);
}
_bool	MousePress(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MousePress(eMouse);
}

//	Release System
void Release_System()
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}