#ifndef WaterBall_h__
#define WaterBall_h__

#include "Defines.h"
#include "Skill.h"


class CWaterBall : public Engine::CSkill
{
private:
	explicit CWaterBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterBall();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;

private:
	_float	m_fSpeed = 30.f;
	_bool	m_bFirstShot = true;

public:
	static CWaterBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};
#endif // WaterBall_h__
