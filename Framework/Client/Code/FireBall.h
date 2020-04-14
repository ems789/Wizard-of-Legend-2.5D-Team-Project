#ifndef FireBall_h__
#define FireBall_h__

#include "Defines.h"
#include "Skill.h"

class CFireBall : public Engine::CSkill
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireBall();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // FireBall_h__