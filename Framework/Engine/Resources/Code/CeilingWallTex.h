#ifndef CeilingWallTex_h__
#define CeilingWallTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCeilingWallTex : public CVIBuffer
{
private:
	explicit CCeilingWallTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCeilingWallTex(const CCeilingWallTex& rhs);
	virtual ~CCeilingWallTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
public:
	HRESULT Ready_Buffer(const _ulong dwVtxItv = 1);

private:
	HANDLE				m_hFile;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CCeilingWallTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwVtxItv);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // CeilingWallTex_h__