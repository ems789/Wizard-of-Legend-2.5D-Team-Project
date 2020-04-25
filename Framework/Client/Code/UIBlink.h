#ifndef UIBlink_h__
#define UIBlink_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcAlphaTex;
class CTransform;
class CRenderer;

END

class CUIBlink : public Engine::CGameObject
{
private:
	explicit CUIBlink(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIBlink();


public:
	HRESULT Ready_GameObject(const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual const _vec3* Get_Pos() const override;
	void		Move_Pos(const _vec3* pMove);
	void		Set_Pos(const _vec3* pPos);
	void		Set_PosY(const _float& fy);

private:
	HRESULT		Add_Component(const _tchar* pTextureTag);
	void		Update_Scale();
	void		Change_Color(const _float& fTimeDelta);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRcAlphaTex*	m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

	_vec3		m_vScale;
	D3DXCOLOR	m_d3dColor;
	_float		m_fColorSpeed;

public:
	static CUIBlink* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos);


private:
	virtual void Free() override;

};

#endif // UIBlink_h__
