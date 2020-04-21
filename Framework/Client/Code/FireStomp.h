#ifndef FireStomp_h__
#define FireStomp_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CFireStomp : public Engine::CGameObject
{
private:
	enum FIRE_STOMP_STATE
	{
		FSS_CASTING_CICLE,
		FSS_BOOM,
		FSS_END
	};
	
private:
	explicit CFireStomp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireStomp();

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
	void		FireStompTail();

private:
	void Turn_To_Camera_Look();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	//Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_int	m_iAttack = 50;

	_float	m_fTailDelay = 0.f;

	_float	m_fLifeTime = 0.f;
	_float	m_fLifeCount = 0.f;
	FIRE_STOMP_STATE m_eCurState = FSS_END;

	Engine::SPHERE m_tSphere;

	_float fCastingTime = 0.f;

public:
	static CFireStomp* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _int& iAttack = 50);

private:
	virtual void Free() override;

};

#endif // FireStomp_h__
