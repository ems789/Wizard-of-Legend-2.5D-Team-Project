#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr()
{

}

Engine::CResourcesMgr::~CResourcesMgr()
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pMapResources)
		return E_FAIL;

	m_pMapResources = new map<const _tchar*, CResources*>[wSize];

	m_wSize = wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const _tchar* pPath, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case Engine::BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;
	case Engine::BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;
	case Engine::BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;
	case Engine::BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, pPath, dwCntX, dwCntZ, dwVtxItv);
		break;
	case Engine::BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt /*= 1*/)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pPath, eType, iCnt);

	m_pMapResources[wContainerIdx].emplace(pTextureTag, pResources);

	return S_OK;
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{

}

HRESULT CResourcesMgr::Remove_Resource(const _ushort & wContainerIdx, const _tchar * pResourceTag)
{
	auto iter = find_if(m_pMapResources[wContainerIdx].begin(), m_pMapResources[wContainerIdx].end(), CTag_Finder(pResourceTag));

	if (m_pMapResources[wContainerIdx].end() == iter)
	{
		_tchar szFailMsg[256];
		wsprintf(szFailMsg, L"Remove_Resource failed, There is no resource that tagged ", pResourceTag);
		MessageBox(NULL, szFailMsg, L"System Message", MB_OK);
		return E_FAIL;
	}

	Safe_Release(iter->second);
	m_pMapResources[wContainerIdx].erase(iter);

	return S_OK;
}

Engine::CComponent* Engine::CResourcesMgr::Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	auto iter = find_if(m_pMapResources[wContainerIdx].begin(), m_pMapResources[wContainerIdx].end(), CTag_Finder(pResourceTag));

	if (m_pMapResources[wContainerIdx].end() == iter)
		return nullptr;

	return iter->second->Clone();
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourcesTag)
{
	auto iter = find_if(m_pMapResources[wContainerIdx].begin(), m_pMapResources[wContainerIdx].end(), CTag_Finder(pResourcesTag));

	if (m_pMapResources[wContainerIdx].end() == iter)
		return nullptr;

	return iter->second;
}

void Engine::CResourcesMgr::Free()
{
	for (_uint i = 0; i < m_wSize; ++i)
	{
		for_each(m_pMapResources[i].begin(), m_pMapResources[i].end(), CDeleteMap());
		m_pMapResources[i].clear();
	}

	Safe_Delete_Array(m_pMapResources);
}

