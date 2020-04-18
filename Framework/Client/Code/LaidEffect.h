#ifndef LaidEffect_h__
#define LaidEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CLaidEffect : public Engine::CEffect
{
private:
	explicit CLaidEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaidEffect();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;
	HRESULT		Ready_GameObject(const _tchar* pTextureTag);


private:
	HRESULT	Add_Component(const _tchar* pTextureTag);
	void	LifeTimer(const _float& fTimeDelta);
	void	Animation(const _float& fTimeDelta);
	void	Update_Scale();
	void	Turn_To_Camera_Look();

private:
	Engine::CRcTex*		m_pBufferCom	= nullptr;
	Engine::CTexture*	m_pTextureCom	= nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom	= nullptr;

private:
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fScale = 1.f;

public:
	static CLaidEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, const _float& fAngleY, _bool bAnimRepeat, _float fLifeTime);

private:
	virtual void Free() override;
};

#endif // LaidEffect_h__
