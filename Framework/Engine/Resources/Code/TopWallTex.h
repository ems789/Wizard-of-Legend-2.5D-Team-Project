#ifndef TopWallTex_h__
#define TopWallTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTopWallTex : public CVIBuffer
{
private:
	explicit CTopWallTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTopWallTex(const CTopWallTex& rhs);
	virtual ~CTopWallTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
public:
	HRESULT Ready_Buffer(const _ulong dwVtxItv = 1, WALLTYPE eWallType = WALL_OUTER);

private:
	HANDLE				m_hFile;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CTopWallTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwVtxItv,
							WALLTYPE eWallType);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // TopWallTex_h__