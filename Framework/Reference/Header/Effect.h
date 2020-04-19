#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class ENGINE_DLL CEffect : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();
	
public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_Effect(const _tchar* pTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual void			Set_Pos(const _vec3* pPos) override;
	virtual const _vec3*	Get_Pos() const override;


protected:
	HRESULT			Add_Component(const _tchar* pTextureTag);

public:
	const _tchar*	Get_EffectTag() const { return m_pEffectTag; }

protected:
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

protected:
	const _tchar*	m_pEffectTag = nullptr;

protected:
	virtual void Free() override;

};

END

#endif // Effect_h__
