#ifndef SkillCard_h__
#define SkillCard_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CSkill;

END

class CSkillCard : public Engine::CGameObject
{
private:
	explicit CSkillCard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkillCard();

public:
	HRESULT Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, Engine::CSkill* pSkill);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

private:
	HRESULT Add_Component(const _tchar* pTextureTag);
	void	Update_Scale();

private:	//	Components
	Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CTransform*		m_pTransformCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

private:
	_float			m_fScale = 0.05f;
	Engine::CSkill*	m_pSkill = nullptr;

private:
	static CSkillCard*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, Engine::CSkill* pSkill);

private:
	virtual void Free() override;
};


#endif // SkillCard_h__
