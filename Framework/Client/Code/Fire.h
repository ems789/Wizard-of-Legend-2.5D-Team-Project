#ifndef Fire_h__
#define Fire_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CFire : public Engine::CGameObject
{
private:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFire();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObjcet() override;

private:
	HRESULT		Add_Component();
	void		Animation(const _float& fTimeDelta);

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos	= { 0.f, 0.f, 0.f };
	_vec3	m_vDir			= { 0.f, 0.f, 0.f };
	_float	m_fSpeed		= 50.f;
	_float	m_fLimitDistance = 50.f;

	FRAME	m_tFrame;

public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vAngle, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed);

private:
	virtual void Free() override;

};

#endif // Fire_h__
