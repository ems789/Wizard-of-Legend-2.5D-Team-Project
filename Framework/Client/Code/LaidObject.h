#ifndef LaidObject_h__
#define LaidObject_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CLaidObject : public Engine::CGameObject
{
private:
	explicit CLaidObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaidObject();

public:
	HRESULT		 Ready_GameObject(const _tchar* pTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT Add_Component(const _tchar* pTextureTag);

private:
	void	LifeTimer(const _float& fTimeDelta);
	void	Animation(const _float& fTimeDelta);
	void	Update_Scale();

private:
	Engine::CRcTex*		m_pBufferCom	= nullptr;
	Engine::CTexture*	m_pTextureCom	= nullptr;
	Engine::CTransform*	m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom	= nullptr;

private:
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fScale = 1.f;

public:
	static CLaidObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, const _float& fAngleY, _bool bAnimRepeat, _float fLifeTime);

private:
	virtual void Free() override;
};

#endif // LaidObject_h__
