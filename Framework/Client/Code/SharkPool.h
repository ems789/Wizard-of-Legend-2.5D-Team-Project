#ifndef SharkPool_h__
#define SharkPool_h__

#include "Defines.h"
#include "Skill.h"

class CSharkPool : public Engine::CSkill
{
private:
	explicit CSharkPool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSharkPool();

public:
	virtual HRESULT Ready_Skill() override;
	virtual _int Use_Skill(const _float& fTimeDelta) override;
	virtual _int Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta) override;
	virtual _int Use_UpgradedSkill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir) override;

private:
	_float	 m_fSpeed =30.f;
	_bool	 m_bFirstShot = true;
	_bool	 m_bColl = false;

public:
	static CSharkPool* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
	
};
#endif // SharkPool_h__
