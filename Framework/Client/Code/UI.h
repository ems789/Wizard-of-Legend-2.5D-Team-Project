#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUIImage;

class CUI : public CBase
{
	DECLARE_SINGLETON(CUI)
private:
	explicit CUI();
	virtual ~CUI();

public:
	HRESULT Ready_PlayerUI(LPDIRECT3DDEVICE9 pGraphicDev);
	_int	Update_PlayerUI(const _float& fTimeDelta);
	void	Render_PlayerUI();

private:
	HRESULT Add_Component();
	//HRESULT SetUp_Scale_Pos(const _vec3* pScale);
	HRESULT Setting_PlayerState();
	HRESULT Setting_SkillSlot();
	HRESULT Setting_Coin();


private:	//	Componenets
	//Engine::CRcTex*		m_pBufferCom = nullptr;
	//Engine::CTexture*	m_pTextureCom = nullptr;
	//Engine::CTransform* m_pTransformCom	= nullptr;
	//Engine::CRenderer*	m_pRendererCom	= nullptr;

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	CUIImage*	m_pUIPlayer;
	CUIImage*	m_pUIHpBar;
	CUIImage*	m_pUIManaBar;
	CUIImage*	m_pUISkillSlot;
	CUIImage*	m_pUICoin;
	

	_matrix		m_matWorld;
	_vec3		m_vPos = { 100.f, 100.f, 0.f };
	_vec3		m_vScale = { 100.f, 100.f, 0.f };

private:
	virtual void Free();

};

#endif // UI_h__
