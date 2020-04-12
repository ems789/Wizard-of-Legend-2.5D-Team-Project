#include "TimerMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr()
{

}

Engine::CTimerMgr::~CTimerMgr()
{
	Free();
}

void Engine::CTimerMgr::Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimer* pInst = Find_Timer(pTimerTag);

	if (nullptr == pInst)
		return;

	pInst->Set_TimeDelta();
}

_float Engine::CTimerMgr::Get_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*		pInst = Find_Timer(pTimerTag);

	if (nullptr == pInst)
		return 0.f;

	return pInst->Get_TimeDelta();
}

HRESULT Engine::CTimerMgr::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*		pInst = Find_Timer(pTimerTag);

	if (nullptr != pInst)
		return E_FAIL;

	pInst = CTimer::Create();
	NULL_CHECK_RETURN(pInst, E_FAIL);

	m_mapTimer.emplace(pTimerTag, pInst);

	return S_OK;
}

void Engine::CTimerMgr::Free()
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}

Engine::CTimer* Engine::CTimerMgr::Find_Timer(const _tchar* pTimerTag)
{
	auto iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTag_Finder(pTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}