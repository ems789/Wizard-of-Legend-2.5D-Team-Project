#ifndef ThunderFall_h__
#define ThunderFall_h__

#include "Defines.h"
#include "Skill.h"

class CThunderFall : public Engine::CSkill
{
private:
	explicit CThunderFall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThunderFall();

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
	static void ThunderFall_Hit_Func();

public:
	static CThunderFall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // ThunderFall_h__
