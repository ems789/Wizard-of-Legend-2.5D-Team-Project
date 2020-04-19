#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

//	ResourcesMgr
//	Get
inline const TEX_INFO*	Get_TexInfo(const _ushort& wContainerIdx, const _tchar* pTextureTag, const _uint& iIndex = 0, D3DXIMAGE_INFO* pOut = nullptr);
//	Set
//	General
inline HRESULT	Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const _tchar* pPath = L"", const _ulong dwCntX = 129, const _ulong dwCntZ = 129, const _ulong dwVtxItv = 1);
inline HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt = 1);

inline void Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);

inline CComponent* Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag);

inline void Release_Resources();

#include "Export_Resources.inl"
END
#endif // Export_Resources_h__
