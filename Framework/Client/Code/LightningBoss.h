#ifndef LightningBoss_h__
#define LightningBoss_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CLightningBoss : public Engine::CGameObject
{
private:
	enum LightningBoss_STATE	{	FBS_IDLE, FBS_JUMP, FBS_FLOAT, FBS_READY_JAVELIN, FBS_THROW_JAVELIN, FBS_DEAD, FBS_END };
	enum LightningBoss_DIR	{ FBD_UP, FBD_DOWN, FBD_LEFT, FBD_RIGHT, FBD_END };

private:
	explicit CLightningBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningBoss();

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
	void	Fitting_Scale_With_Texture(CLightningBoss::LightningBoss_STATE eState);
	void	Fitting_Scale_With_Texture(CLightningBoss::LightningBoss_STATE eState, _ulong dwIndex);
	void	FireEffect(const _float& fTimeDelta);

private:
	void Idle_State();
	void Jump_State();
	void Float_State();
	void Dead_State();


public:
	_int Idle_Update(const _float& fTimeDelta);
	void Jump_Update(const _float& fTimeDelta);
	void Float_Update(const _float& fTimeDelta);
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
	_int	m_iHP		= 1000;
	_int	m_iHPMax	= 1000;

	_float	m_fScale = 0.05f;
	_float	m_fHeight = 1.f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	CLightningBoss::LightningBoss_STATE		m_ePreState = CLightningBoss::FBS_END;
	CLightningBoss::LightningBoss_STATE		m_eCurState = CLightningBoss::FBS_END;
	CLightningBoss::LightningBoss_DIR		m_eCurDir	= CLightningBoss::FBD_END;

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
	static CLightningBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // LightningBoss_h__
