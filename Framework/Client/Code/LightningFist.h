#ifndef LightningFist_h__
#define LightningFist_h__

#include "Defines.h"
#include "Skill.h"

class CLightningFist : public Engine::CSkill
{
private:
	explicit CLightningFist(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningFist();

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
	static void LightningFist_Hit_Func();

public:
	static CLightningFist* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // LightningFist_h__
