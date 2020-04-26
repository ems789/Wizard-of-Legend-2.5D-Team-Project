#ifndef WindTornado_h__
#define WindTornado_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CWindTornado : public Engine::CGameObject
{
private:
	explicit CWindTornado(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWindTornado();

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
	void		LifeTimer(const _float& fTimeDelta);

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;

	//_float	m_fTailDelay = 0.f;

	_float	m_fLifeTime = 0.f;
	_float	m_fLifeCount = 0.f;

	Engine::SPHERE m_tSphere;

private:
	void Turn_To_Camera_Look();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	//Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;


	static CWindTornado* Create(LPDIRECT3DDEVICE9, const _vec3& vInitialPos, const _float& fSpeed, const _float& fLifeTime);

private:
	virtual void Free() override;
};
#endif // WindTornado_h__
