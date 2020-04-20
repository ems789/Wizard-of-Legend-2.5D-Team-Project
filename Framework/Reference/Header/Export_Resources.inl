#include "Export_Resources.h"

const TEX_INFO* Get_TexInfo(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint & iIndex, D3DXIMAGE_INFO* pOut)
{
	return CResourcesMgr::GetInstance()->Get_TexInfo(wContainerIdx, pTextureTag, iIndex, pOut);
}
//	ResourcesMgr
//	Get
//	Set
//	General
HRESULT	Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}
HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const D3DXCOLOR d3dColor, const _tchar* pPath, const _ulong dwCntX, const _ulong dwCntZ, const _ulong dwVtxItv)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, pBufferTag, eID, d3dColor, pPath, dwCntX, dwCntZ, dwVtxItv);
}
HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt/* = 1*/)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}

void Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
}

CComponent* Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone(wContainerIdx, pResourceTag);
}

void Release_Resources()
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}