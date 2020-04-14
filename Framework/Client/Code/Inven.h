#ifndef Inven_h__
#define Inven_h__

#include "Defines.h"
#include "Skill.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;

END

class CInven : public CBase
{
	DECLARE_SINGLETON(CInven)

private:
	explicit CInven();
	virtual ~CInven();

public:
	HRESULT		Ready_Inven(LPDIRECT3DDEVICE9 pGraphicDev);
	_int		Update_Inven(const _float& fTImeDelta);
	void		Render_Inven();

private:
	HRESULT Add_Component();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;

	_bool	m_bInvenOn = false;

	_vec3		m_vScale = { 300.f, 300.f, 0.f };
	_vec3		m_vPos = { 0.f, 0.f, 0.f };
	_matrix		m_matWorld;

	vector<Engine::CSkill*> m_vecSkill[SKILL_END];

public:
	virtual void Free() override;

};

#endif // Inven_h__
