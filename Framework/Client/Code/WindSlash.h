#ifndef WindSlash_h__
#define WindSlash_h__

#include "Defines.h"
#include "Skill.h"

class CWindSlash : public Engine::CSkill
{
private:
	explicit CWindSlash(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWindSlash();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;

private:
	_float	m_fSpeed = 0.1f;

public:
	static void WindSlash_Hit_Func();

public:
	static CWindSlash* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // WindSlash_h__
