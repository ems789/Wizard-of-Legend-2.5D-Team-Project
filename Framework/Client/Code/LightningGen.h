#ifndef LightningGen_h__
#define LightningGen_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CFire;
class CLightningGen : public Engine::CGameObject
{

private:
	explicit CLightningGen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningGen();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;

private:
	HRESULT		Add_Component();
	void		Generate_Lightning(const _float& fTimeDelta);

private:	//	Components
	Engine::CTransform* m_pTransformCom = nullptr;

private:
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 25.f;
	_float	m_fLimitDistance = 50.f;
	_float	m_fAngle = 0.f;
	_int	m_iAttack = 10;

	Engine::SPHERE m_tSphere;

	_float			m_fTimeCount = 0.f;
	_float			m_fLightningTimeItv = 0.f;
	_uint			m_uiLightningMax = 0;
	_uint			m_uiLightningCnt = 0;

public:
	static CLightningGen* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, const _float& fLightningItv, const _uint& uiLightningMax);

private:
	virtual void Free() override;

};

#endif // LightningGen_h__
