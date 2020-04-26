#ifndef UIButton_h__
#define UIButton_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcAlphaTex;
class CTransform;
class CRenderer;

END

class CUIButton : public Engine::CGameObject
{
private:
	explicit CUIButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIButton();


public:
	HRESULT Ready_GameObject(const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos, const D3DXCOLOR& d3dColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	void	ButtonOn()		{ m_bIsOn = true; }
	void	ButtonOff()		{ m_bIsOn = false; }
	void	ButtonToggle()	{ m_bIsOn = !m_bIsOn; }

private:
	HRESULT		Add_Component(const _tchar* pTextureTag);
	void		Update_Scale();

private:
	Engine::CTransform*		m_pTransformCom		= nullptr;
	Engine::CRcAlphaTex*	m_pBufferCom		= nullptr;
	Engine::CTexture*		m_pTextureCom		= nullptr;
	Engine::CRenderer*		m_pRendererCom		= nullptr;

	_vec3		m_vOnScale;
	_vec3		m_vOffScale;

	_bool		m_bIsOn = false;
	D3DXCOLOR	m_d3dOnColor;
	D3DXCOLOR	m_d3dOffColor;

public:
	static CUIButton* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos, const D3DXCOLOR& d3dColor);


private:
	virtual void Free() override;

};

#endif // UIButton_h__
