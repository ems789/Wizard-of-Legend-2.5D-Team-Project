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
	enum PLAYER_STATE { P_IDLE, P_RUN, P_ATTACK, P_DASH, P_SKILL1, P_SKILL2, P_END };

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
	void	Key_Input(const _float& fTimeDelta);
	void	Key_Input_For_Attack(const _float& fTimeDelta);
	void	Key_Input_Attack_For_QuaterView(const _float& fTimeDelta);
	void	Key_Input_For_Move(const _float& fTimeDelta);
	void	Key_Input_Move_For_QuaterView(const _float& fTimeDelta);
	void	Key_Input_For_1stAnd3rdView(const _float& fTimeDelta);

private:	//	State Setting
	void	Idle_State();
	void	Run_State();
	void	Attack_State();
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

public:		//	Get Functions
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual const _vec3*		Get_Pos()	const override;

public:		//	About Collision
	virtual void	Hit(const _int& iAtk, const _vec3* pAtkPos) {}
	
public:		//	About HP
	virtual _int	Get_HP()	const { return m_iHP; }
	virtual _int	Get_HPMax() const { return m_iHPMax; }


private:	//	Components
	Engine::CRcTex*		m_pBufferCom	= nullptr;
	Engine::CTexture*	m_pTextureCom	= nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom	= nullptr;

private:
	FRAME	m_tFrame;
	CPlayer::PLAYER_STATE m_ePreState = CPlayer::P_END;
	CPlayer::PLAYER_STATE m_eCurState = CPlayer::P_END;
	_float	m_fSpeed = 10.f;

	Engine::SPHERE	m_tSphere;

	vector<Engine::CSkill*>		m_vecEquipSkill;

	_int	m_iHP		= 500;
	_int	m_iHPMax	= 500;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // Player_h__