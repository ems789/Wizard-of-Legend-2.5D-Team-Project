#ifndef BottomWallTex_h__
#define BottomWallTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBottomWallTex : public CVIBuffer
{
private:
	explicit CBottomWallTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBottomWallTex(const CBottomWallTex& rhs);
	virtual ~CBottomWallTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
public:
	HRESULT Ready_Buffer(const _ulong dwVtxItv = 1, WALLTYPE eWallType = WALL_OUTER);

private:
	HANDLE				m_hFile;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CBottomWallTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwVtxItv, 
							WALLTYPE eWallType);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // BottomWallTex_h__
