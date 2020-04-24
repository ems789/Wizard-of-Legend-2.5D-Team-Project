#ifndef AquaVortex_h__
#define AquaVortex_h__

#include "Defines.h"
#include "Skill.h"

class CAquaVortex : public Engine::CSkill
{
private:
	explicit CAquaVortex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAquaVortex();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;


private:
	_float	m_fSpeed = 30.f;
	_bool	m_bFirstShot = true;

public:
	static CAquaVortex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};
#endif // AquaVortex_h__
