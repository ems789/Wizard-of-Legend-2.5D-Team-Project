#ifndef Inven_h__
#define Inven_h__

#include "Defines.h"
#include "Skill.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;

END

class CUIImage;
class CNumberFont;

class CInven : public CBase
{
	DECLARE_SINGLETON(CInven)
	enum INVEN_STATE {
		INVEN_OFF, INVEN_ON, INVEN_NORMALBOOK, INVEN_UPGRADEBOOK,
		BASIC_FIRE, BASIC_WATER, BASIC_LIGHTNING, BASIC_WIND,
		SIGNATURE_FIRE, SIGNATURE_WIND, SIGNATURE_WATER, SIGNATURE_LIGHTNING,
		INVEN_END
	};
	enum INVEN_SELECT
	{
		SELECT_START, SELECT_BASE, SELECT_DASH, SELECT_STANDARD, SELECT_SIGNATURE, SELECT_END
	};

	enum EQUIP_SKILLIMAGE
	{
		EQUIP_START, EQUIP_FIREBALL, EQUIP_METEOR, EQUIP_WATERBALL, EQUIP_WATERARC, EQUIP_AQUAVORTEX, EQUIP_LIGHTSKILL1, EQUIP_LIGHTSKILL2, EQUIP_LIGHTSKILL3,
		EQUIP_WINDSKILL1, EQUIP_WINDSKLL2,
		/*시그니처*/
		EQUIP_ICEDOWN, EQUIP_FIRESKILL1, EQUIP_END
	};

public:
	enum SKILL_LIST
	{
		LIST_OFF, LIST_ON, FIREBALL, METEOR, AQUAVORTEX, WATERBALL, SHARKPOOL, WINDSKILL1, WINDSKILL2,
		LIGHTSKILL1, LIGHTSKILL2, LIGHTSKILL3, FIRESKILL1, LIST_END
	};

private:
	explicit CInven();
	virtual ~CInven();

public:
	HRESULT		Ready_Inven(LPDIRECT3DDEVICE9 pGraphicDev);
	_int		Update_Inven(const _float& fTImeDelta);
	void		Render_Inven();

private:
	HRESULT Add_Component();
	HRESULT	Setting_SelectBox();

	HRESULT Setting_BasicSkill();
	HRESULT Setting_SignatureSkill();
	HRESULT Setting_SkillImage();
	HRESULT Setting_SlotImage();
	Engine::CSkill* Find_Skill(const wstring& SkillTag);

public:
	HRESULT Add_Skill(const wstring& SkillTag, Engine::CSkill* pSkill);
	void Add_Coin(const _uint& iCoin)	{ m_iCoin += iCoin; }
	void Remove_Coin(const _uint& iCoin) { m_iCoin -= iCoin; }
	const _int& Get_Coin() const { return m_iCoin; }
	void Set_Player(CPlayer* pPlayer);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;

	CInven::INVEN_STATE			m_eCurState = INVEN_END;
	CInven::INVEN_STATE			m_ePreState = INVEN_END;
	CInven::SKILL_LIST			m_eCurSkill = LIST_END;
	CInven::INVEN_SELECT		m_eCurSelect = SELECT_END;
	CInven::EQUIP_SKILLIMAGE	m_eCurBaseEquip = EQUIP_END;
	CInven::EQUIP_SKILLIMAGE	m_eCurDashEquip = EQUIP_END;
	CInven::EQUIP_SKILLIMAGE	m_eCurStandardEquip = EQUIP_END;
	CInven::EQUIP_SKILLIMAGE	m_eCurSignatureEquip = EQUIP_END;

	_bool	m_bSlotOn = false;
	_bool	m_bInvenOn = false;
	_bool	m_bListOn = false;
	_bool	m_bBookOn = false;
	_bool	m_bUpgradedSkillOn = false;
	_vec3		m_vSelect;
	_vec3		m_vScale = { 650.f, 400.f, 0.f };
	_vec3		m_vPos = { -550.f, -30.f, 0.f };
	_vec3		m_vEquipCardPos = { 0.f, 0.f, 0.f };
	_vec3		m_vSlotPos1 = { 608.f, 440.f, 0.f };
	_vec3		m_vSlotPos2 = { 682.f, 440.f, 0.f };
	_vec3		m_vSlotPos3 = { 758.f, 440.f, 0.f };
	_vec3		m_vSlotPos4 = { 834.f, 440.f, 0.f };


	_matrix		m_matWorld;

	vector<Engine::CSkill*> m_vecSkill[SKILL_END];
	CUIImage*	m_pSelectBox;
	CUIImage*	m_pBasicFire;
	CUIImage*	m_pBasicWater;
	CUIImage*	m_pBasicLightning;
	CUIImage*	m_pBasicWind;

	////////////	SPELLBOOK IMAGE
	CUIImage*	m_pSignatureFire;
	CUIImage*	m_pSignatureWater;
	CUIImage*	m_pSignatureLightning;
	CUIImage*	m_pSignatureWind;
	/////////////////////////////////
	CUIImage*	m_pWaterBall;
	CUIImage*	m_pWaterArc;
	CUIImage*	m_pAquaVortex;
	CUIImage*	m_pMeteor;
	CUIImage*	m_pFireBall;
	CUIImage*	m_pFireSkill1;
	CUIImage*	m_pLightningSkill1;
	CUIImage*	m_pLightningSkill2;
	CUIImage*	m_pLightningSkill3;
	CUIImage*	m_pWindSkill1;
	CUIImage*	m_pWindSkill2;
	////////////////////////////Slot Image
	CUIImage*	m_pSlotFireSkill1;
	CUIImage*	m_pSlotFireBall;
	CUIImage*	m_pSlotMeteor;
	CUIImage*	m_pSlotWaterBall;
	CUIImage*	m_pSlotWaterArc;
	CUIImage*	m_pSlotAquaVortex;
	CUIImage*	m_pSlotLightningSkill1;
	CUIImage*	m_pSlotLightningSkill2;
	CUIImage*	m_pSlotLightningSkill3;
	CUIImage*	m_pSlotWindSkill1;
	CUIImage*	m_pSlotWindSkill2;

	///////////////////////////////// Equip Image
	CUIImage*	m_pEquipFireBall;
	CUIImage*	m_pEquipFireSkill1;
	CUIImage*	m_pEquipWaterBall;
	CUIImage*	m_pEquipWaterArc;
	CUIImage*	m_pEquipLightningSkill1;
	CUIImage*	m_pEquipLightningSkill2;
	CUIImage*	m_pEquipWindSkill1;

	//////////Signature Equipimage
	CUIImage*	m_pEquipMeteor;
	CUIImage*	m_pEquipLightningSkill3;
	CUIImage*	m_pEquipWindSkill2;
	CUIImage*	m_pEquipAquaVortex;

	/////////////////
	_int			m_iCoin = 100;

	map<wstring, Engine::CSkill*>  m_MapSkill;
	CPlayer*	m_pPlayer = nullptr;

public:
	virtual void Free() override;

};

#endif // Inven_h__
