#ifndef FireBoss_h__
#define FireBoss_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CFireBoss : public Engine::CGameObject
{
private:
	enum FireBoss_STATE	{ FBS_IDLE, FBS_RUN, FBS_ATTACK, FBS_DASH, FBS_KICK, FBS_HEEL, FBS_BOOM, FBS_POINT, FBS_HURT, FBS_END };
	enum FireBoss_DIR	{ FBD_UP, FBD_DOWN, FBD_LEFT, FBD_RIGHT, FBD_END };

private:
	explicit CFireBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireBoss();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual const _vec3*		Get_Pos()	const override;

private:
	HRESULT Add_Component();
	void	Animation(const _float& fTimeDelta);
	void	Change_State();
	void	Update_State(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();
	void	Fitting_Scale_With_Texture(CFireBoss::FireBoss_STATE eState);
	void	Fitting_Scale_With_Texture(CFireBoss::FireBoss_STATE eState, _ulong dwIndex);

private:
	void Idle_State();
	void Run_State();
	void Attack_State();
	void Attack_FT_State();

public:
	_int Idle_Update(const _float& fTimeDelta);
	_int Run_Update(const _float& fTimeDelta);
	_int Attack_Update(const _float& fTimeDelta);


public:
	virtual void Hit(const _int& iAtk, const _vec3* pAtkPos) override;

public:		//	About HP
	virtual _int	Get_HP()	const { return m_iHP; }
	virtual _int	Get_HPMax() const { return m_iHPMax; }

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

	vector<vector<Engine::CTexture*>>	m_vvTextureCom;

private:
	Engine::SPHERE	m_tSphere;
	_int	m_iHP		= 200;
	_int	m_iHPMax	= 200;

	_float	m_fScale = 0.05f;
	_float	m_fHeight = 1.f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	CFireBoss::FireBoss_STATE		m_ePreState = CFireBoss::FBS_END;
	CFireBoss::FireBoss_STATE		m_eCurState = CFireBoss::FBS_END;
	CFireBoss::FireBoss_DIR			m_eCurDir	= CFireBoss::FBD_END;

	_float	m_fSpeed = 2.f;

public:
	static CFireBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // FireBoss_h__
