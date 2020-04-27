#ifndef WindBoss_h__
#define WindBoss_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END


class CWindBoss : public Engine::CGameObject
{
private:
	enum WindBoss_STATE { WBS_IDLE, WBS_ATTACK, WBS_ATTACK2, WBS_ATTACK3, WBS_READYCHARGE, WBS_DASH, WBS_SPIN, WBS_DASH_READY, WBS_SPINEND, WBS_FlOAT,
						  WBS_HURT, WBS_TAUNT, WBS_DEAD, WBS_END};

	enum WindBoss_DIR {WBD_UP, WBD_DOWN, WBD_LEFT, WBD_RIGHT, WBD_END};

private:
	explicit CWindBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWindBoss();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual const _vec3*		Get_Pos()	const override;	virtual const _vec3*		Get_PrePos() const override { return &m_vPrePos; }

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
	void	Fitting_Scale_With_Texture(CWindBoss::WindBoss_STATE eState);
	void	Fitting_Scale_With_Texture(CWindBoss::WindBoss_STATE eState, _ulong dwIndex);
	void	WindEffect(const _float& fTimeDelta);

private:
	void Idle_State();
	void Attack_State();
	void Attack_State2();
	void Attack_State3();
	void DashReady_State();
	void Dash_State();
	void Spin_State();
	void SpinEnd_State();

	void Hurt_State();
	void Float_State();
	//void Taunt_State();
	void Dead_State();

public:
	_int Idle_Update(const _float& fTimeDelta);
	_int Attack_Update(const _float& fTimeDelta);
	_int Attack_Update2(const _float& fTimeDelta);
	_int Attack_Update3(const _float& fTimeDelta);
	void DashReady_Update(const _float& fTimeDelta);
	_int Dash_Update(const _float& fTimeDelta);
	void Float_Update(const _float& fTimeDelta);
	_int Spin_Update(const _float& fTimeDelta);
	void SpinEnd_Update(const _float& fTimeDelta);
	void Hurt_Update(const _float& fTimeDelta);
	//void Taunt_Update(const _float& fTimeDelta);
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
	_vec3		m_vPrePos;

	_int	m_iHP = 1000;
	_int	m_iHPMax = 1000;

	_float	m_fScale = 0.05f;
	_float	m_fHeight = 1.f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	CWindBoss::WindBoss_STATE		m_ePreState = CWindBoss::WBS_END;
	CWindBoss::WindBoss_STATE		m_eCurState = CWindBoss::WBS_END;
	CWindBoss::WindBoss_DIR			m_eCurDir = CWindBoss::WBD_END;

	_float	m_fSpeed = 30.f;

	_float	m_fWindEffectTime = 0.f;

	_float	m_fIdleTime = 0.f;
	_float  m_fDashTime = 0.f;
	_float  m_fSpinTime = 0.f;
	_uint	m_uiPattern = 0;
	_bool	m_bSpin = true;
	_bool	m_bFillar = true;
	_bool	m_bAttack = false;
	_vec3	m_vDashDir = {};
	_vec3	m_vFloatDir = {};

	_float	m_fHurtTime = 0.f;

	_float	m_fDeadTime = 0.f;


public:
	static CWindBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // !WindBoss_h__
