#include "FrameMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr()
{

}

Engine::CFrameMgr::~CFrameMgr()
{
	Free();
}

_bool Engine::CFrameMgr::IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	CFrame* pInst = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pInst, false);

	return pInst->IsPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrameMgr::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	CFrame* pInst = Find_Frame(pFrameTag);
	if (nullptr != pInst)
		return E_FAIL;

	pInst = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pInst, E_FAIL);

	m_mapFrame.emplace(pFrameTag, pInst);

	return S_OK;
}

Engine::CFrame* Engine::CFrameMgr::Find_Frame(const _tchar* pFrameTag)
{
	auto iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTag_Finder(pFrameTag));

	if (m_mapFrame.end() == iter)
		return nullptr;

	return iter->second;
}

void Engine::CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}