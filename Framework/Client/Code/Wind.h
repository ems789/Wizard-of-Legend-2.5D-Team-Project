#ifndef Wind_h__
#define Wind_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CWind : public Engine::CGameObject
{
private:
	explicit CWind(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWind();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }

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
	_float	m_fSpeed		= 10.f;
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 10;

	_float	m_fScale = 0.2f;

	FRAME	m_tFrame;
	Engine::SPHERE m_tSphere;

public:
	static CWind* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vAngle, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed, _float fDir);

private:
	virtual void Free() override;

};

#endif // Wind_h__
