#ifndef MeteorStrike_h__
#define MeteorStrike_h__

#include "Defines.h"
#include "Skill.h"

class CMeteorStrike : public Engine::CSkill
{
private:
	explicit CMeteorStrike(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeteorStrike();

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
	static CMeteorStrike* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // MeteorStrike_h__
