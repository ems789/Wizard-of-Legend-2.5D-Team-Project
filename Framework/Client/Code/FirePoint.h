#ifndef FirePoint_h__
#define FirePoint_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CFire;
class CFirePoint : public Engine::CGameObject
{
private:
	enum FIRE_POINT_STATE { FPS_READY, FPS_GO, FPS_END };

private:
	explicit CFirePoint(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirePoint();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;

private:
	HRESULT		Add_Component();

private:	//	Components
	Engine::CTransform* m_pTransformCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;

	Engine::SPHERE m_tSphere;
	_vec3			m_vRight;
	FIRE_POINT_STATE	m_eCurState = FPS_END;
	_float			m_fStateTime = 0.f;

	_float			m_fFireIntervalTime = 0.f;
	_float			m_fFireAngle = 0.f;
	list<CFire*>	m_FireList;

public:
	static CFirePoint* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _vec3& vRight, const _float& fSpeed);

private:
	virtual void Free() override;

};

#endif // FirePoint_h__
