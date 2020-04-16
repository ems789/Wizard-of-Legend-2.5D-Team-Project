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

class CInven : public CBase
{
	DECLARE_SINGLETON(CInven)
	enum INVEN_STATE {
		INVEN_OFF, INVEN_ON, INVEN_NORMALBOOK, INVEN_UPGRADEBOOK,
		BASIC_FIRE, BASIC_WATER, BASIC_LIGHTNING, BASIC_WIND,
		SIGNATURE_FIRE, SIGNATURE_WIND, SIGNATURE_WATER, SIGNATURE_LIGHTNING,
		INVEN_END
	};
	enum SKILL_LIST
	{
		LIST_OFF, LIST_ON, RISINGDRAGON, WINDSLAM, AIRWAVE, SHOCKLACE, SHOCKSPEAR, WATERBLAST, WATERBALL, EXPLOSIVECHARGE, LIST_END
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
	//HRESULT Setting_BaseSkill();
	//HRESULT Setting_UpgradedSkill();
	HRESULT Setting_BasicSkill();
	HRESULT Setting_SignatureSkill();
	HRESULT Setting_SkillImage();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;

	CInven::INVEN_STATE m_eCurState = INVEN_END;
	CInven::SKILL_LIST	m_eCurSkill = LIST_END;
	//CInven::INVEN_STATE m_ePrestate = INVEN_END;

	_bool	m_bInvenOn = false;
	_bool	m_bListOn = false;
	_bool	m_bBookOn = false;
	_bool	m_bUpgradedSkillOn = false;
	_vec3		m_vSelect;
	_vec3		m_vScale = { 800.f, 800.f, 0.f };
	_vec3		m_vPos = { -500.f, 0.f, 0.f };
	_matrix		m_matWorld;

	vector<Engine::CSkill*> m_vecSkill[SKILL_END];
	CUIImage*	m_pSelectBox;
	CUIImage*	m_pBaseBox;
	CUIImage*	m_pUpgradeBox;

	CUIImage*	m_pBasicFire;
	CUIImage*	m_pBasicWater;
	CUIImage*	m_pBasicLightning;
	CUIImage*	m_pBasicWind;

	CUIImage*	m_pSignatureFire;
	CUIImage*	m_pSignatureWater;
	CUIImage*	m_pSignatureLightning;
	CUIImage*	m_pSignatureWind;

	CUIImage*	m_pUseWaterBall;
	CUIImage*	m_pUseWaterBlast;
	CUIImage*	m_pUseWindSlam;
	CUIImage*	m_pUseAirWave;
	CUIImage*	m_pUseShokcSpear;
	CUIImage*	m_pUseShockLace;
	CUIImage*	m_pUseRisingDragon;
	CUIImage*	m_pUseExplosiveCharge;

public:
	virtual void Free() override;

};

#endif // Inven_h__
