#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

class CSkill;

END

class CPlayer : public Engine::CGameObject
{
public:
	enum PLAYER_STATE { P_IDLE, P_RUN, P_ATTACK, P_ATTACK2, P_DASH, P_SKILL1, P_SKILL2, P_END };
	enum PLAYER_DIR { PD_UP, PD_DOWN, PD_RIGHT, PD_LEFT, PD_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	_int	Change_Normal_Skill(Engine::CSkill* pSkill);
	_int	Change_Upgrade_Skill(Engine::CSkill* pSkill);

private:
	HRESULT Add_Component();
	void	Animation(const _float& fTimeDelta);
	void	Change_State();
	_int	Update_State(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();
	void	Fitting_Scale_With_Texture(CPlayer::PLAYER_STATE eState);
	void	Fitting_Scale_With_Texture(CPlayer::PLAYER_STATE eState, _ulong dwIndex);
	void	PickingPlane(_vec3* pOut);

private:	//	Key_Input Functions
	void	Key_Input(const _float& fTimeDelta);

	void	Key_Input_Attack(const _float& fTimeDelta);
	void	Key_Input_Attack_For_1stAnd3rdView(const _float& fTimeDelta);
	void	Key_Input_Attack_For_QuaterView(const _float& fTimeDelta);

	void	Key_Input_Move(const _float& fTimeDelta);
	void	Key_Input_Move_For_QuaterView(const _float& fTimeDelta);
	void	Key_Input_Move_For_1stAnd3rdView(const _float& fTimeDelta);

	void	Key_Input_Dash(const _float& fTimeDelta);
	void	Key_Input_Dash_For_1stAnd3rdView(const _float& fTimeDelta);
	void	Key_Input_Dash_For_QuaterView(const _float& fTimeDleta);

	void	Key_Input_Skill1(const _float& fTimeDelta);
	void	Key_Input_Skill1_For_1stAnd3rdView(const _float& fTimeDelta);
	void	Key_Input_Skill1_For_QuaterView(const _float& fTimeDelta);

	void	Key_Input_Skill2(const _float& fTimeDelta);
	void	Key_Input_Skill2_For_1stAnd3rdView(const _float& fTimeDelta);
	void	Key_Input_Skill2_For_QuaterView(const _float& fTimeDelta);

private:	//	State Setting
	void	Idle_State();
	void	Run_State();
	void	Attack_State();
	void	Attack2_State();
	void	Dash_State();
	void	Skill1_State();
	void	Skill2_State();

private:	//	State Udate
	_int	Idle_Update(const _float& fTimeDelta);
	_int	Run_Update(const _float& fTimeDelta);
	_int	Attack_Update(const _float& fTimeDelta);
	_int	Dash_Update(const _float& fTimeDelta);
	_int	Skill1_Update(const _float& fTimeDelta);
	_int	Skill2_Update(const _float& fTimeDelta);

public:		//	Set_Functions
	void	Set_Pos(const _vec3* pPos);
	void	Set_Pos(const _vec3& vPos);
	virtual void Set_PosX(const _float& fx);
	virtual void Set_PosY(const _float& fy);
	virtual void Set_PosZ(const _float& fz);

public:		//	Get Functions
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual const _vec3*		Get_Pos()	const override;
	virtual const _vec3*		Get_PrePos() const override { return &m_vPrePos; }
	virtual	const _vec3*		Get_Move() const override { return &m_vMove; }

public:		//	About Collision
	virtual void	Hit(const _int& iAtk, const _vec3* pAtkPos) override;

public:		//	About HP
	virtual _int	Get_HP()	const { return m_iHP; }
	virtual _int	Get_HPMax() const { return m_iHPMax; }
	virtual _float	Get_MP()	const { return m_fMP; }
	virtual _float	Get_MPMax() const { return m_fMPMax; }

private:
	void	FullMP_Effect(const _float& fTimeDelta);
	void	TimeSlowCounter(const _float& fTimeDelta);


private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom	= nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

	vector<vector<Engine::CTexture*>>	m_vvTextureCom;
	vector<vector<const _tchar*>>		m_vvpTextureTag;

private:
	FRAME	m_tFrame;
	_bool	m_bAnimFinish = false;
	_bool	m_bAnimRepeat = true;

	CPlayer::PLAYER_STATE m_ePreState = CPlayer::P_END;
	CPlayer::PLAYER_STATE m_eCurState = CPlayer::P_END;
	CPlayer::PLAYER_DIR	  m_eCurDir = CPlayer::PD_END;
	_uint	m_iPreCamState = 0;

	//_bool		m_bDir = true;	//	true : right, false : left

	_float	m_fSpeed = 10.f;

	Engine::SPHERE	m_tSphere;
	_vec3			m_vPrePos;
	_vec3			m_vMove;

	vector<Engine::CSkill*>		m_vecEquipSkill;

	_int	m_iHP = 500;
	_int	m_iHPMax = 500;
	_float  m_fMP = 0.f;
	_float	m_fMPMax = 100.f;

	_float	m_fScale = 0.2f;

	//	Variable about dash
	_float	m_fDashSpeed = 20.f;
	_vec3	m_vDashDir = { 0.f, 0.f, 0.f };

	_ulong	m_dwAttackCnt = 0;
	_vec3	m_vAttackDir = { 0.f, 0.f, 0.f };

	_float	m_fFootStepSound = 0.f;
	_bool	m_bFootStepSound = false;

	_float	m_fManaEffectTime = 0.f;
	_float	m_fSlowCount = 0.f;
	_float	m_fSlowTime = 0.f;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // Player_h__