#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"

#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT		Reserve_ContainerSize(const _ushort& wSize);

	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ushort& wContainerIdx, 
							const _tchar* pBufferTag,
							BUFFERID eID, 
							const _ulong& dwCntX, 
							const _ulong& dwCntZ, 
							const _ulong& dwVtxItv);

	HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ushort& wContainerIdx,
							const _tchar* pTextureTag,
							TEXTURETYPE eType, 
							const _tchar* pPath, 
							const _uint& iCnt = 1);


	void		Render_Buffer(const _ushort& wContainerIdx, 
								const _tchar* pBufferTag);

	CComponent*	Clone(const _ushort& wContainerIdx, 
						const _tchar* pResourceTag);


private:
	CResources*		Find_Resources(const _ushort& wContainerIdx, 
									const _tchar* pResourceTag);

private:
	map<const _tchar*, CResources*>*			m_pMapResources = nullptr;
	_ushort										m_wSize = 0;

public:
	virtual void Free(void);

};

END
#endif // ResourcesMgr_h__
