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
	HRESULT Ready_BossUI(LPDIRECT3DDEVICE9 pGraphicDev);
	_int	Update_PlayerUI(const _float& fTimeDelta);
	_int	Update_BossUI(const _float& fTimeDelta);
	//추가
	void	Render_PlayerUI();
	//추가
	void	Render_BossUI();
	void	ShowOnUI() { m_bShowUI = true; }
	void	ShowOffUI() { m_bShowUI = false; }
	///추가
	void	ShowOnBossUI() { m_bBossUIOn = true; }
	void	ShowOffBossUI() { m_bBossUIOn = false; }
	//void	SlotSkillOn(_uint uiSlot, CUIImage* pImage);
	void	SlotSkillOn(_uint uiSlot, const _vec3& vImagePos, const _tchar* pTextureTag);

private:
	HRESULT Add_Component();
	HRESULT Setting_PlayerState();
	HRESULT Setting_SkillSlot();
	HRESULT Setting_Coin();
	//추가
	HRESULT Setting_BossUI();

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
	CUIImage*	m_pEnemyHpBarBG;
	CUIImage*	m_pEnemyHpBarFill;


	_matrix		m_matWorld;
	_vec3		m_vPos = { 100.f, 100.f,0.f };
	_vec3		m_vScale = { 100.f, 100.f, 0.f };
	_vec3		m_vHpScale = { 244.f, 32.f, 0.f };
	_vec3		m_vManaScale = { 200.f, 16.f, 0.f };//꽉찰시192
	_vec3		m_vHurtScale = { 244.f, 32.f, 0.f };
	_vec3		m_vHurtPos = { -765.f, 510.f, 0.f };
	_vec3		m_vHpPos = { -765.f, 510.f, 0.f };
	_vec3		m_vManaPos = { -880.f, 480.f, 0.f };
	_vec3		m_vBossUI = { 0.f, 700.f, 0.f };
	_vec3		m_vBossHpScale = { 267.f, 21.f, 0.f };
	_vec3		m_vBossHpPos = { 0.f, 400.f, 0.f };

	_bool		m_bBossUIOn = false;

	//슬롯스킬 이미지 위치
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
