#ifndef Timer_h__
#define Timer_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase 
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	HRESULT Ready_Timer();
	void	Set_TimeDelta();
	_float	Get_TimeDelta() { return m_fTimeDelta; }

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_CpuTick;

	_float				m_fTimeDelta;

public:
	static CTimer*	Create();
	virtual void Free();


};

END

#endif // Timer_h__
