#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev()
{

}

Engine::CInputDev::~CInputDev()
{
	Free();
}

_bool CInputDev::KeyDown(_ubyte byKeyID)
{
	_bool	bCurKeyState = m_byKeyState[byKeyID] & 0x80;

	//	���� ���� �� ���� ���� ������ �� true
	if (!(m_bKeyDown[byKeyID]) && (bCurKeyState))
	{
		m_bKeyDown[byKeyID] = true;
		return true;
	}
	
	//	���� ���� �� �ְ� ���� ������ �ʾ��� �� false
	if (m_bKeyDown[byKeyID] && !bCurKeyState)
	{
		m_bKeyDown[byKeyID] = false;
		return false;
	}

	return false;
}

_bool CInputDev::KeyUp(_ubyte byKeyID)
{
	_bool	bCurKeyState = m_byKeyState[byKeyID] & 0x80;

	//	���� ���� �� �ְ� ���� ������ �ʾ��� �� true;
	if (m_bKeyUp[byKeyID] && !bCurKeyState)
	{
		m_bKeyUp[byKeyID] = false;
		return true;

	}

	if (!m_bKeyUp[byKeyID] && bCurKeyState)
	{
		m_bKeyUp[byKeyID] = true;
		return false;
	}

	return false;
}

_bool CInputDev::KeyPress(_ubyte byKeyID)
{
	return m_byKeyState[byKeyID] & 0x80;
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	FAILED_CHECK_RETURN(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();

	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();
	
	ZeroMemory(m_bKeyDown,	sizeof(_bool) * 256);
	ZeroMemory(m_bKeyUp,	sizeof(_bool) * 256);

	return S_OK;
}

void Engine::CInputDev::Set_InputDev()
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CInputDev::Free()
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}