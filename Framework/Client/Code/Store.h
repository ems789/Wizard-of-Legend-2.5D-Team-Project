#ifndef Store_h__
#define Store_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CSkill;

END

class CStore : public Engine::CGameObject
{
private:
	explicit CStore(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStore();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:
	HRESULT Add_Skill(const wstring& wstrSkillTag, Engine::CSkill* pSkill);


private:
	map<wstring, Engine::CSkill*> m_mapSkill;

private:
	static CStore*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // Store_h__
