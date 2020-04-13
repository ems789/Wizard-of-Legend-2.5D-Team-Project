#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	HRESULT Ready_Buffer(const _tchar* pPath, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3*				m_pPos;
	_bool				m_bClone;

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);
	virtual CResources*	Clone() override;
	
private:
	virtual void Free();
};

END

#endif // TerrainTex_h__
