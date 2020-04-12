#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual	~CTexture();

public:
	HRESULT		Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt = 1);
	void		Render_Texture(const _uint& iIndex = 0);

public:
	const TEX_INFO*	Get_TexInfo(const _uint& iIndex = 0, D3DXIMAGE_INFO* pOut = nullptr) const;

private:
	vector<TEX_INFO*>				m_vecTexInfo;

public:
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt = 1);
	virtual CResources*	Clone();

private:
	virtual void Free();
};

END

#endif // Texture_h__
