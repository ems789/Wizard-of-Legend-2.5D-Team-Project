#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, 
						const _ulong& dwCntZ,	
						const _ulong dwVtxItv = 1) ;


public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwCntX, 
							const _ulong& dwCntZ, 
							const _ulong& dwVtxItv);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // TerrainTex_h__
