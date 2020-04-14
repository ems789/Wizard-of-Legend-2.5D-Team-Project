#ifndef Skill_h__
#define Skill_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTexture;
class ENGINE_DLL CSkill : public CBase
{
protected:
	explicit CSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkill();

public:	//	Get Functions
	_float			Get_Attack()		const { return m_fAttack; }
	_float			Get_CoolTime()		const { return m_fCoolTime; }
	const wstring&	Get_Name()			const { return m_wstrName; }

public:
	virtual HRESULT		Ready_Skill()	PURE;
	virtual	_int		Use_Skill(const _float& fTimeDelta)PURE;
	virtual	_int		Use_UpgradedSkill(const _float& fTimeDelta)PURE;

	virtual void		Render_SkillCard() {};

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	wstring				m_wstrName = L"";
	_float				m_fCoolTime = 0.f;
	_float				m_fCurTime = 0.f;
	_float				m_fAttack = 0.f;

protected:
	virtual void Free() override;

};

END

#endif // Skill_h__
