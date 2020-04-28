#ifndef NumberFont_h__
#define NumberFont_h__

#include "Defines.h"
#include "Base.h"

#include "Export_Function.h"

BEGIN(Engine)

class CRcAlphaTex;
class CTexture;
class CTransform;

END

class CNumberFont : public CBase
{
	DECLARE_SINGLETON(CNumberFont)

private:
	explicit CNumberFont();
	virtual ~CNumberFont();

public:
	HRESULT Ready_NumberFont(LPDIRECT3DDEVICE9 pGraphicDev);
	void DrawNumber(const _int& iNum, const _vec3* pPos, const _float& fScale, D3DXCOLOR d3dColor);
	void DrawNumberBill(const _int& iNum, const _vec3* pPos, const _float& fScale, D3DXCOLOR d3dColor);

private:
	HRESULT		Add_Component();

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	
	Engine::CRcAlphaTex*	m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

private:
	_float	m_fScale;
	D3DXCOLOR	m_d3dColor;

public:
	virtual void Free();

};


#endif // NumberFont_h__
