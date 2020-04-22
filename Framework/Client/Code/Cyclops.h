#ifndef Cyclops_h__
#define Cyclops_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CCyclops : public Engine::CGameObject
{
private:
	enum CYCLOPS_STATE	{ CYS_IDLE, CYS_RUN, CYS_HURT, CYS_ATTACK_READY, CYS_ATTACK, CYS_END };
	enum CYCLOPS_DIR	{ CYD_UP, CYD_DOWN, CYD_LEFT, CYD_RIGHT, CYD_END };

private:
	explicit CCyclops(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCyclops();

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
	void	Fitting_Scale_With_Texture(CCyclops::CYCLOPS_STATE eState);
	void	Fitting_Scale_With_Texture(CCyclops::CYCLOPS_STATE eState, _ulong dwIndex);

private:
	void Idle_State();
	void Run_State();
	void Hurt_State();
	void Attack_Ready_State();
	void Attack_State();

public:
	_int Idle_Update(const _float& fTimeDelta);
	_int Run_Update(const _float& fTimeDelta);
	_int Hurt_Update(const _float& fTimeDelta);
	_int Attack_Ready_Update(const _float& fTimeDelta);
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
	_float	m_fHeight = 0.5f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	CCyclops::CYCLOPS_STATE		m_ePreState = CCyclops::CYS_END;
	CCyclops::CYCLOPS_STATE		m_eCurState = CCyclops::CYS_END;
	CCyclops::CYCLOPS_DIR		m_eCurDir = CCyclops::CYD_END;

	_float	m_fSpeed = 2.f;

	_vec3	m_vHurtDir = {0.f, 0.f, 0.f};
	_vec3	m_vAttackTarget = {0.f, 0.f, 0.f};

public:
	static CCyclops* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // Cyclops_h__
