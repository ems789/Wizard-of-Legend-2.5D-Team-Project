#ifndef CastingCircle_h__
#define CastingCircle_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CRenderer;

END

class CCastingCircle : public Engine::CEffect
{
private:
	explicit CCastingCircle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCastingCircle();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObjcet() override;

private:
	HRESULT			Add_Component();
	void			Animation(const _float& fTimeDelta);

private:
	Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;
	FRAME					m_tFrame;

public:
	static CCastingCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fScale, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // CastingCircle_h__
