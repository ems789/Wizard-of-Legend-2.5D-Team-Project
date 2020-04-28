#ifndef LightningSpearShot_h__
#define LightningSpearShot_h__

#include "Defines.h"
#include "Skill.h"

class CLightningSpearShot : public Engine::CSkill
{
private:
	explicit CLightningSpearShot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningSpearShot();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;

private:
	_float	m_fSpeed = 30.f;
	_bool	m_bFirstShot = true;

public:
	static void LightningSpearShot_Hit_Func();

public:
	static CLightningSpearShot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // LightningSpearShot_h__
