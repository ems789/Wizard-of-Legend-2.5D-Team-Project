#ifndef Store_h__
#define Store_h__

#include "Defines.h"
#include "GameObject.h"

class CSkillCard;
class CStore : public Engine::CGameObject
{
private:
	explicit CStore(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStore();

public:
	HRESULT			Ready_GameObject(const _vec3* pPos, CSkillCard* pSkillCard1, CSkillCard* pSkillCard2, CSkillCard* pSkillCard3);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

private:
	static CStore*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, CSkillCard* pSkillCard1, CSkillCard* pSkillCard2, CSkillCard* pSkillCard3);

private:
	virtual void Free() override;
};


#endif // Store_h__
