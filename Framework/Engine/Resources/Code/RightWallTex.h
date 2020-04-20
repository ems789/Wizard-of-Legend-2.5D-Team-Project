#ifndef RightWallTex_h__
#define RightWallTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRightWallTex : public CVIBuffer
{
private:
	explicit CRightWallTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRightWallTex(const CRightWallTex& rhs);
	virtual ~CRightWallTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
public:
	HRESULT Ready_Buffer(const _ulong dwVtxItv = 1);

private:
	HANDLE				m_hFile;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CRightWallTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwVtxItv);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // RightWallTex_h__
