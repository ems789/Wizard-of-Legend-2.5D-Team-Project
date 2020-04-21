#ifndef FireKick_h__
#define FireKick_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CFireKick : public Engine::CGameObject
{
private:
	explicit CFireKick(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireKick();

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
	void		FireKickTail();

private:
	void Turn_To_Camera_Look();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	//Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;

	_float	m_fTailDelay = 0.f;

	_float	m_fLifeTime = 0.f;
	_float	m_fLifeCount = 0.f;
	_uint	m_uiFireCnt = 4;
	_float	m_fSize = 50.f;

	Engine::SPHERE m_tSphere;
	const _tchar* m_pCollisionTag = L"";

public:
	static CFireKick* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _float& fLifeTime, const _uint& uiFireCnt = 4, const _int& iAttack = 10, const _float& fSize = 0.01f, const _tchar* pCollisionTag = L"MonsterAttack", const _float& fRadius = 1.f);

private:
	virtual void Free() override;

};

#endif // FireKick_h__
