#ifndef Lightning_h__
#define Lightning_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CLightning : public Engine::CGameObject
{
private:
	explicit CLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightning();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;
	virtual void				Add_Effect(const _vec3* pPos) override;

private:
	HRESULT		Add_Component();
	void		Animation(const _float& fTimeDelta);

private:
	void Turn_To_Camera_Look();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_float	m_fCurTime = 0.f;
	_float	m_fLifeTime = 0.f;
	_float	m_fSoundTime = 0.f;
	_float	m_fRadius = 1.f;
	_float	m_fAttckDelay = 0.5f;
	_int	m_iAttack = 50;

	FRAME	m_tFrame;
	Engine::SPHERE m_tSphere;

	const _tchar*	m_pCollisionTag = nullptr;

public:
	static CLightning* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _float& fMaxFrame, const _float& fFrameSpeed, const _float& fLifeTime, const _float& fRadius = 1.f, const _float& fAttackDelay = 0.5f, const _tchar* pCollisionTag = L"Player_Bullet");

private:
	virtual void Free() override;

};

#endif // Lightning_h__
