#ifndef BlobSpitter_h__
#define BlobSpitter_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CBlobSpitter : public Engine::CGameObject
{
private:
	enum BLOB_SPITTER_STATE	{ BSS_IDLE, BSS_RUN, BSS_ATTACK, BSS_HURT, BSS_END };
	enum BLOB_SPITTER_DIR	{ BSD_LEFT, BSD_RIGHT, BSD_END };

private:
	explicit CBlobSpitter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBlobSpitter();

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
	void	Fitting_Scale_With_Texture(CBlobSpitter::BLOB_SPITTER_STATE eState);
	void	Fitting_Scale_With_Texture(CBlobSpitter::BLOB_SPITTER_STATE eState, _ulong dwIndex);

private:
	void Idle_State();
	void Run_State();
	void Attack_State();
	void Hurt_State();

public:
	_int Idle_Update(const _float& fTimeDelta);
	_int Run_Update(const _float& fTimeDelta);
	_int Attack_Update(const _float& fTimeDelta);
	_int Hurt_Update(const _float& fTimeDelta);

public:
	virtual void Hit(const _int& iAtk, const _vec3* pAtkPos) override;

public:		//	About HP
	virtual _int	Get_HP()	const { return m_iHP; }
	virtual _int	Get_HPMax() const { return m_iHPMax; }

public:
	static void BlobSpitter_Hit_Effect();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

	vector<vector<Engine::CTexture*>>	m_vvTextureCom;

private:
	Engine::SPHERE	m_tSphere;
	_vec3			m_vPrePos;

	_int	m_iHP		= 200;
	_int	m_iHPMax	= 200;

	_float	m_fScale = 0.08f;
	_float	m_fHeight = 1.f;

	_int	m_iAttack = 20;

	FRAME	m_tFrame;
	_bool	m_bAnimRepeat = true;
	_bool	m_bAnimFinish = false;

	CBlobSpitter::BLOB_SPITTER_STATE		m_ePreState = CBlobSpitter::BSS_END;
	CBlobSpitter::BLOB_SPITTER_STATE		m_eCurState = CBlobSpitter::BSS_END;
	CBlobSpitter::BLOB_SPITTER_DIR			m_eCurDir	= CBlobSpitter::BSD_END;

	_float	m_fSpeed = 2.f;

	_bool	m_bFire = false;
	_uint	m_uiFireCnt = 0;
	_uint	m_uiFireMax = 5;

	_bool	m_bFireCool = false;
	_float	m_fFireCool = 0.f;

	_vec3	m_vHurtDir = { 0.f, 0.f, 0.f };

public:
	static CBlobSpitter* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // BlobSpitter_h__
