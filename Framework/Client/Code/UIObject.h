#ifndef UIObject_h__
#define UIObject_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CRenderer;

END

class CUIObject : public Engine::CGameObject
{
private:
	explicit CUIObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIObject();


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
 

private:
	Engine::CTransform*	m_pTransformCom = nullptr;
	Engine::CRcTex*		m_pBufferCom	= nullptr;
	Engine::CTexture*	m_pTextureCom	= nullptr;
	Engine::CRenderer*	m_pRendererCom	= nullptr;

	_vec3	m_vScale;

public:
	static CUIObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos);


private:
	virtual void Free() override;

};

#endif // UIObject_h__
