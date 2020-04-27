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
	HRESULT Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const wstring& wstrSkillTag, 
		Engine::CSkill* pSkill, const _int& iPrice, const _float& fRadius);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:
	Engine::CSkill*		Get_Skill(Engine::CSkill* pSkill = nullptr) { return m_pSkill; }

public:
	void			Set_Pos(const _vec3* pPos);
	void			Set_Pos(const _vec3& vPos);
	virtual void	Set_PosX(const _float& fx);
	virtual void	Set_PosY(const _float& fy);
	virtual void	Set_PosZ(const _float& fz);


private:
	HRESULT Add_Component(const _tchar* pTextureTag);
	void	Update_Scale();
	void Turn_To_Camera_Look();


private:	//	Components
	Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CTransform*		m_pTransformCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

private:
	_float			m_fScale = 0.05f;
	Engine::CSkill*	m_pSkill = nullptr;
	_int			m_iPrice = 100;
	_bool			m_bIsOn = false;
	Engine::SPHERE	m_tSphere;
	wstring			m_wstrSkillTag;

	

public:
	static CSkillCard*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const wstring& wstrSkillTag, Engine::CSkill* pSkill, const _int& iPrice, const _float& fRadius);

private:
	virtual void Free() override;
};


#endif // SkillCard_h__
