#ifndef SharkGen_h__
#define SharkGen_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CWaterThrow;
class CSharkGen : public  Engine::CGameObject
{
private:
	explicit CSharkGen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSharkGen();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;


private:
	HRESULT		Add_Component();
	void		Generate_Shark(const _float& fTimeDelta);
	void		CalculateBent(const _float& fBentDecel);
	void		Move_Update(const _float& fTimeDelta);
	void		Turn_To_Camera_Look();
private:
	Engine::CTransform* m_pTransformCom = nullptr;

private:
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_float	m_fLimitDistance = 50.f;
	_int	m_iAttack = 50;
	_vec3	m_vTargetPos;
	_vec3	m_vBentDir;
	_float	m_fBent;
	_float	m_fBentDecel;

	Engine::SPHERE m_tSphere;

	_float			m_fTimeCount = 0.f;
	_uint			m_uiSharkMax = 0;
	_uint			m_uiSharkCnt = 0;

public:
	static CSharkGen* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vInitialPos, const _vec3& vTargetPos, const _float& fSpeed, const _uint& uiSharkMax);
};

#endif // SharkGen_h__
