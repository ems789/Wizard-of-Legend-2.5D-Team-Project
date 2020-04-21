#ifndef GuidedFireBall_h__
#define GuidedFireBall_h__

#include "Defines.h"
#include "Skill.h"

class CGuidedFireBall : public Engine::CSkill
{
private:
	explicit CGuidedFireBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGuidedFireBall();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;

private:
	_float	m_fSpeed = 30.f;
	_bool	m_bFirstShot = true;

public:
	static CGuidedFireBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // GuidedFireBall_h__
