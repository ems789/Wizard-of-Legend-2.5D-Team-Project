#ifndef MeteorGen_h__
#define MeteorGen_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CFire;
class CMeteorGen : public Engine::CGameObject
{

private:
	explicit CMeteorGen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeteorGen();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;

private:
	HRESULT		Add_Component();
	void		Generate_Meteor(const _float& fTimeDelta);

private:	//	Components
	Engine::CTransform* m_pTransformCom = nullptr;

private:
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;

	Engine::SPHERE m_tSphere;

	_float			m_fTimeCount = 0.f;
	_float			m_fMeteorTimeItv = 0.f;
	_uint			m_uiMeteorMax = 0;
	_uint			m_uiMeteorCnt = 0;

public:
	static CMeteorGen* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _float& fMeteorTimeItv, const _uint& uiMeteorMax);

private:
	virtual void Free() override;

};

#endif // MeteorGen_h__
