#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "GameObject.h"
#include "Player.h"
#include "Inven.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUIImage;
class CNumberFont;
class CUI : public CBase
{
	DECLARE_SINGLETON(CUI)

	enum SLOT_LIST
	{
		LOT_OFF, LOT_ON, FIREBALL, METEOR, WINDSLAM, AIRWAVE, AQUAVORTEX, WATERBALL, WATERARC, ICEDOWN, SLOT_END
	};
private:
	explicit CUI();
	virtual ~CUI();

public:
	HRESULT Ready_PlayerUI(LPDIRECT3DDEVICE9 pGraphicDev);
	_int	Update_PlayerUI(const _float& fTimeDelta);
	void	Render_PlayerUI();

	void	ShowOnUI() { m_bShowUI = true; }
	void	ShowOffUI() { m_bShowUI = false; }

	//void	SlotSkillOn(_uint uiSlot, CUIImage* pImage);
	void	SlotSkillOn(_uint uiSlot, const _vec3& vImagePos, const _tchar* pTextureTag);

private:
	HRESULT Add_Component();
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
	CUIImage*	m_pUIHurtBar;
	CUIImage*	m_pUISkillSlot;
	CUIImage*	m_pUICoin;

	_matrix		m_matWorld;
	_vec3		m_vPos = { 100.f, 100.f, 0.f };
	_vec3		m_vScale = { 100.f, 100.f, 0.f };
	_vec3		m_vHpScale = { 244.f, 32.f, 0.f };
	_vec3		m_vManaScale = { 200.f, 16.f, 0.f };//²ËÂû½Ã192
	_vec3		m_vHurtScale = { 244.f, 32.f, 0.f };
	_vec3		m_vHurtPos = { -765.f, 510.f, 0.f };
	_vec3		m_vHpPos = { -765.f, 510.f, 0.f };
	_vec3		m_vManaPos = { -880.f, 480.f, 0.f };


	//½½·Ô½ºÅ³ ÀÌ¹ÌÁö À§Ä¡
	_vec3		m_vSlotScale = { 65.f, 65.f, 0.f };


	_float				m_fUISpeed;
	_bool				m_bShowUI = false;
	CInven::SKILL_LIST	m_SkillList;

	vector<CUIImage*>	m_vecSlotImage;

	CNumberFont*	m_pNumberFont = nullptr;

private:
	virtual void Free();

};

#endif // UI_h__
