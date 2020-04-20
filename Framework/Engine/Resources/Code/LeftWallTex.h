#ifndef LeftWallTex_h__
#define LeftWallTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CLeftWallTex : public CVIBuffer
{
private:
	explicit CLeftWallTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeftWallTex(const CLeftWallTex& rhs);
	virtual ~CLeftWallTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
public:
	HRESULT Ready_Buffer(const _ulong dwVtxItv = 1);

private:
	HANDLE				m_hFile;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CLeftWallTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwVtxItv);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // LeftWallTex_h__
