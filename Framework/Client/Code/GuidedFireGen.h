#ifndef GuidedFireGen_h__
#define GuidedFireGen_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CFire;
class CGuidedFireGen : public Engine::CGameObject
{

private:
	explicit CGuidedFireGen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGuidedFireGen();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;

private:
	HRESULT		Add_Component();
	void		Generate_GuidedFire(const _float& fTimeDelta);

private:	//	Components
	Engine::CTransform* m_pTransformCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;

	Engine::SPHERE m_tSphere;

	_float			m_fTimeCount = 0.f;
	_float			m_fFireTimeItv = 0.f;
	_float			m_fFireAngleItv = 0.f;
	_float			m_fFireAngle = 0.f;
	_uint			m_uiFireMax = 0;
	_uint			m_uiFireCnt = 0;
	_float			m_fRevRadius = 1.f;
	_float			m_fDetectDist = 10.f;

	const Engine::CTransform* m_pRevTarget = nullptr;

public:
	static CGuidedFireGen* Create(LPDIRECT3DDEVICE9 pGraphicDev, const Engine::CTransform* pRevTarget, const _float& fFireTimeItv, const _float& fFireAngleItv, 
		const _uint& uiFireMax, const _float& fRevRadius, const _float& fDetectDist);

private:
	virtual void Free() override;

};

#endif // GuidedFireGen_h__
