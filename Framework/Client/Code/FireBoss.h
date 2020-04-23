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
	enum FireBoss_STATE	{	FBS_IDLE, FBS_ATTACK, FBS_DASH_READY, FBS_DASH, FBS_DASH_FINISH, FBS_SPINKICK, FBS_ROUNDHOUSEKICK, 
							FBS_HEEL, FBS_STOMP, FBS_POINT, FBS_HURT, FBS_SQUAT, FBS_TAUNT, FBS_DEAD, FBS_END };
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
	virtual const _vec3*		Get_PrePos() const override { return &m_vPrePos; }

public:		//	Set_Functions
	void	Set_Pos(const _vec3* pPos);
	void	Set_Pos(const _vec3& vPos);
	virtual void Set_PosX(const _float& fx);
	virtual void Set_PosY(const _float& fy);
	virtual void Set_PosZ(const _float& fz);

private:
	HRESULT Add_Component();
	void	Animation(const _float& fTimeDelta);
	void	Change_State();
	void	Update_State(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();
	void	Fitting_Scale_With_Texture(CFireBoss::FireBoss_STATE eState);
	void	Fitting_Scale_With_Texture(CFireBoss::FireBoss_STATE eState, _ulong dwIndex);
	void	FireEffect(const _float& fTimeDelta);

private:
	void Idle_State();
	void Attack_State();
	void DashReady_State();
	void Dash_State();
	void DashFinish_State();
	void RoundKick_State();
	void SpinKick_State();
	void Heel_State();
	void Stomp_State();
	void Point_State();
	void Hurt_State();
	void Squat_State();
	void Taunt_State();
	void Dead_State();


public:
	_int Idle_Update(const _float& fTimeDelta);
	_int Attack_Update(const _float& fTimeDelta);
	void DashReady_Update(const _float& fTimeDelta);
	void Dash_Update(const _float& fTimeDelta);
	void DashFinish_Update(const _float& fTimeDelta);
	void RoundKick_Update(const _float& fTimeDelta);
	void SpinKick_Update(const _float& fTimeDelta);
	void Heel_Update(const _float& fTimeDelta);
	void Stomp_Update(const _float& fTimeDelta);
	void Point_Update(const _float& fTimeDelta);
	void Hurt_Update(const _float& fTimeDelta);
	void Squat_Update(const _float& fTimeDelta);
	void Taunt_Update(const _float& fTimeDelta);
	void Dead_Update(const _float& fTimeDelta);


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
	_vec3			m_vPrePos;

	_int	m_iHP		= 1000;
	_int	m_iHPMax	= 1000;

	_float	m_fScale = 0.05f;
	_float	m_fHeight = 1.f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	CFireBoss::FireBoss_STATE		m_ePreState = CFireBoss::FBS_END;
	CFireBoss::FireBoss_STATE		m_eCurState = CFireBoss::FBS_END;
	CFireBoss::FireBoss_DIR			m_eCurDir	= CFireBoss::FBD_END;

	_float	m_fSpeed = 40.f;

	_float	m_fFireEffectTime = 0.f;

	_float	m_fIdleTime = 0.f;
	_uint	m_uiPattern = 0;

	_bool	m_bAttack = false;
	_uint	m_uiDashCount = 0;
	_vec3	m_vDashDir = {};
	_float	m_fFireRoadItv = 0.f;
	
	_uint	m_uiKickCount = 0;
	_vec3	m_vKickDir = {};
	_float	m_fKickFireTime = 0.f;
	_float	m_fKickAngle = 0.f;

	_bool	m_bHeelInit = false;
	_vec3	m_vHeelTarget	= {};
	_vec3	m_vHeelDir		= {};
	_float	m_fHeelSpeed = 0.f;
	_float	m_fJumpPower	= 0.f;
	_float	m_fFallSpeed	= 5.f;
	_bool	m_bHeelFire = false;

	_bool	m_bFireStomp = false;
	
	_bool	m_bFirePoint = false;

	_float	m_fHurtTime = 0.f;

	_float	m_fDeadTime = 0.f;

public:
	static CFireBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // FireBoss_h__
