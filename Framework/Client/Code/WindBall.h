#ifndef WindBall_h__
#define WindBall_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CWindBall : public Engine::CGameObject
{
private:
	explicit CWindBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWindBall();

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
	void		Go();
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
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;

	Engine::SPHERE m_tSphere;
	_vec3	m_vTargetPos;

	FRAME	m_tFrame;

	_bool	m_bGo = false;
	const _tchar*	m_pCollisionTag = nullptr;

public:
	static CWindBall* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vTargetPos,  const _vec3& vDir, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed);

private:
	virtual void Free() override;
};

#endif // WindBall_h__

