#ifndef LightningFalling_h__
#define LightningFalling_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END



class CLightningFalling : public Engine::CGameObject
{
public:
	CLightningFalling(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningFalling();

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
	void Turn_To_Camera_Look();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fLimitDistance = 50.f;
	_float	m_fLifeTime = 0.f;
	_float	m_fLifeCount = 0.f;
	_int	m_iAttack = 35;

	Engine::SPHERE m_tSphere;

	_vec3	m_vTargetDir;

public:
	static CLightningFalling* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vInitialPos, const _float& fLifeTime);


private:
	virtual void Free() override;
};

#endif // LightningFalling_h__
