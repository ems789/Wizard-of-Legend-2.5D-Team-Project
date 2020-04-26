#ifndef Mage_h__
#define Mage_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CMage : public Engine::CGameObject
{
private:
	enum Mage_STATE	{ MS_IDLE, MS_RUN, MS_CAST, MS_ATTACK, MS_DEATH, MS_END };
	enum Mage_DIR		{ MD_LEFT, MD_RIGHT, MD_END };

private:
	explicit CMage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMage();

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
	void	Fitting_Scale_With_Texture(CMage::Mage_STATE eState);
	void	Fitting_Scale_With_Texture(CMage::Mage_STATE eState, _ulong dwIndex);

private:
	void Idle_State();
	void Run_State();
	void Cast_State();
	void Attack_State();

public:
	_int Idle_Update(const _float& fTimeDelta);
	_int Run_Update(const _float& fTimeDelta);
	_int Cast_Update(const _float& fTimeDelta);
	_int Attack_Update(const _float& fTimeDelta);


public:
	virtual void Hit(const _int& iAtk, const _vec3* pAtkPos) override;

public:		//	About HP
	virtual _int	Get_HP()	const { return m_iHP; }
	virtual _int	Get_HPMax() const { return m_iHPMax; }

public:
	static void Mage_Hit_Effect();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

	vector<vector<Engine::CTexture*>>	m_vvTextureCom;

private:
	Engine::SPHERE	m_tSphere;
	_vec3			m_vPrePos;

	_int	m_iHP		= 100;
	_int	m_iHPMax	= 100;

	_float	m_fScale = 0.05f;
	_float	m_fHeight = 1.f;

	_float	m_fCastingTime = 0.f;
	_float	m_fIdleTime = 0.f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	_bool	m_bIsCasting = false;
	_bool	m_bIsAttack = false;

	CMage::Mage_STATE		m_ePreState = CMage::MS_END;
	CMage::Mage_STATE		m_eCurState = CMage::MS_END;
	CMage::Mage_DIR		m_eCurDir	= CMage::MD_END;

	_float	m_fSpeed = 2.f;

public:
	static CMage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // Mage_h__
