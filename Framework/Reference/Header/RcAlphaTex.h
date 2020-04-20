#ifndef RcAlphaTex_h__
#define RcAlphaTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcAlphaTex : public CVIBuffer
{
private:
	explicit CRcAlphaTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcAlphaTex(const CRcAlphaTex& rhs);
	virtual ~CRcAlphaTex();

public:
	HRESULT		Ready_Buffer(const D3DXCOLOR& d3dColor);
	void		Set_Color	(const D3DXCOLOR& d3dColor);

private:
	D3DXCOLOR	 m_d3dColor;


public:
	static CRcAlphaTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXCOLOR& d3dColor);
	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // RcAlphaTex_h__
