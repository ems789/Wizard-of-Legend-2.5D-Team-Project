#include "stdafx.h"
#include "Inven.h"
#include "UIImage.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CInven)

CInven::CInven()
{

}

CInven::~CInven()
{
	Free();
}

HRESULT CInven::Ready_Inven(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_vSelect = { -680.f, 10.f, 0.f };

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_Inven", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/SelectBase.png"), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	FAILED_CHECK_RETURN(Setting_SelectBox(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_BasicSkill(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_SignatureSkill(), E_FAIL);
	/// 임시 스킬이미지들
	FAILED_CHECK_RETURN(Setting_SkillImage(), E_FAIL);


	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matTrans;

	m_eCurState = CInven::INVEN_OFF;
	//m_eCurSkill = CInven::RISINGDRAGON;

	return S_OK;
}

_int CInven::Update_Inven(const _float & fTImeDelta)
{
	switch (m_eCurState)
	{
	case CInven::INVEN_OFF:
		if (Engine::KeyDown(DIK_T))
		{
			m_bInvenOn = !m_bInvenOn;
			m_bListOn = !m_bListOn;
			if (m_bInvenOn == true)
				m_eCurState = CInven::INVEN_ON;
			if (m_bListOn == true)
				m_eCurSkill = CInven::LIST_ON;
		}
		break;

	case CInven::INVEN_ON:
		if (Engine::KeyDown(DIK_RIGHT))
		{
			m_vSelect = { -325.f, 15.f, 0.f };
			m_bUpgradedSkillOn = true;
			m_pSelectBox->Set_Pos(m_vSelect);
		}
		if (Engine::KeyDown(DIK_LEFT))
		{
			//표준일때만 
			m_vSelect = { -680.f, 10.f, 0.f };
			m_bUpgradedSkillOn = false;
			m_pSelectBox->Set_Pos(m_vSelect);
		}
		if ((m_bUpgradedSkillOn == false) && Engine::KeyDown(DIK_RETURN))
		{
			m_eCurState = CInven::BASIC_FIRE;
			m_eCurSkill = CInven::RISINGDRAGON;
		}
		if ((m_bUpgradedSkillOn == true) && Engine::KeyDown(DIK_RETURN))
		{
			m_eCurState = CInven::SIGNATURE_FIRE;
		}
		if (Engine::KeyDown(DIK_T))
		{
			m_bInvenOn = false;
			m_bListOn = false;
			m_eCurState = CInven::INVEN_OFF;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;

	case CInven::BASIC_FIRE:
		if (Engine::KeyDown(DIK_LEFT))
		{
			m_eCurSkill = CInven::EXPLOSIVECHARGE;
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			m_eCurSkill = CInven::RISINGDRAGON;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_WATER;
			m_eCurSkill = CInven::WATERBALL;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_WIND;
			m_eCurSkill = CInven::AIRWAVE;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;
	case CInven::BASIC_WATER:
		if (Engine::KeyDown(DIK_LEFT))
		{
			m_eCurSkill = CInven::WATERBALL;
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			m_eCurSkill = CInven::WATERBLAST;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_LIGHTNING;
			m_eCurSkill = CInven::SHOCKLACE;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_FIRE;
			m_eCurSkill = CInven::RISINGDRAGON;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;

	case CInven::BASIC_LIGHTNING:
		if (Engine::KeyDown(DIK_LEFT))
		{
			m_eCurSkill = CInven::SHOCKLACE;
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			m_eCurSkill = CInven::SHOCKSPEAR;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_WIND;
			m_eCurSkill = CInven::AIRWAVE;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_WATER;
			m_eCurSkill = CInven::WATERBALL;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;
	case CInven::BASIC_WIND:
		if (Engine::KeyDown(DIK_LEFT))
		{
			m_eCurSkill = CInven::AIRWAVE;
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			m_eCurSkill = CInven::WINDSLAM;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_FIRE;
			m_eCurSkill = CInven::RISINGDRAGON;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_LIGHTNING;
			m_eCurSkill = CInven::SHOCKLACE;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;

	case CInven::SIGNATURE_FIRE:
		if (Engine::KeyDown(DIK_LEFT))
		{

		}
		if (Engine::KeyDown(DIK_RIGHT))
		{

		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_WATER;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_WIND;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;
	case CInven::SIGNATURE_WATER:
		if (Engine::KeyDown(DIK_LEFT))
		{

		}
		if (Engine::KeyDown(DIK_RIGHT))
		{

		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_LIGHTNING;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_FIRE;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
		}
		break;
	case CInven::SIGNATURE_LIGHTNING:
		if (Engine::KeyDown(DIK_LEFT))
		{

		}
		if (Engine::KeyDown(DIK_RIGHT))
		{

		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_WIND;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_WATER;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
		}
		break;
	case CInven::SIGNATURE_WIND:
		if (Engine::KeyDown(DIK_LEFT))
		{

		}
		if (Engine::KeyDown(DIK_RIGHT))
		{

		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_FIRE;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_LIGHTNING;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
		}
		break;
	}

	return 0;
}

void CInven::Render_Inven()
{
	if (false == m_bInvenOn)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	switch (m_eCurState)
	{
	case CInven::INVEN_ON:
		m_pSelectBox->Render_UIImage();
		break;
	case CInven::BASIC_FIRE:
		m_pBasicFire->Render_UIImage();
		break;
	case CInven::BASIC_LIGHTNING:
		m_pBasicLightning->Render_UIImage();
		break;
	case CInven::BASIC_WATER:
		m_pBasicWater->Render_UIImage();
		break;
	case CInven::BASIC_WIND:
		m_pBasicWind->Render_UIImage();
		break;
	case CInven::SIGNATURE_FIRE:
		m_pSignatureFire->Render_UIImage();
		break;
	case CInven::SIGNATURE_WATER:
		m_pSignatureWater->Render_UIImage();
		break;
	case CInven::SIGNATURE_LIGHTNING:
		m_pSignatureLightning->Render_UIImage();
		break;
	case CInven::SIGNATURE_WIND:
		m_pSignatureWind->Render_UIImage();
		break;
	}
	switch (m_eCurSkill)
	{
	case CInven::RISINGDRAGON:
		m_pUseRisingDragon->Render_UIImage();
		break;
	case CInven::WINDSLAM:
		m_pUseWindSlam->Render_UIImage();
		break;
	case CInven::AIRWAVE:
		m_pUseAirWave->Render_UIImage();
		break;
	case CInven::SHOCKLACE:
		m_pUseShockLace->Render_UIImage();
		break;
	case CInven::SHOCKSPEAR:
		m_pUseShokcSpear->Render_UIImage();
		break;
	case CInven::WATERBLAST:
		m_pUseWaterBlast->Render_UIImage();
		break;
	case CInven::WATERBALL:
		m_pUseWaterBall->Render_UIImage();
		break;
	case CInven::EXPLOSIVECHARGE:
		m_pUseExplosiveCharge->Render_UIImage();
		break;
	}



}

HRESULT CInven::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Inven"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

HRESULT CInven::Setting_SelectBox()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Cursor", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/SpellbookUISpellCursor.png"), E_FAIL);

	CUIImage* pUIImage = CUIImage::Create(m_pGraphicDev, _vec3(300.f, 420.f, 0.f), m_vSelect, L"Cursor");
	//왼쪽위치 Scale : 300, 420  Pos : -680, 10 
	//scale, pos순서

	NULL_CHECK_RETURN(pUIImage, E_FAIL);
	m_pSelectBox = pUIImage;

	return S_OK;
}

HRESULT CInven::Setting_BaseSkill()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_BaseSkill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/spellbookui.png"), E_FAIL);

	CUIImage* pUIBaseImage = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Texture_BaseSkill");

	NULL_CHECK_RETURN(pUIBaseImage, E_FAIL);
	m_pBaseBox = pUIBaseImage;

	return S_OK;
}

HRESULT CInven::Setting_UpgradedSkill()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_UpgradeSkill", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/upgradespellbook.png"), E_FAIL);

	CUIImage* pUIUpgradeSpellImage = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Texture_UpgradeSkill");

	NULL_CHECK_RETURN(pUIUpgradeSpellImage, E_FAIL);
	m_pUpgradeBox = pUIUpgradeSpellImage;

	return S_OK;
}

HRESULT CInven::Setting_BasicSkill()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Fire", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Fire.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Water", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Water.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Lightning", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Lightning.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Wind", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Wind.png"), E_FAIL);

	CUIImage* pUIBasicFire = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Basic_Fire");
	CUIImage* pUIBasicWater = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Basic_Water");
	CUIImage* pUIBasicLightning = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Basic_Lightning");
	CUIImage* pUIBasicWind = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Basic_Wind");

	NULL_CHECK_RETURN(pUIBasicFire, E_FAIL);
	NULL_CHECK_RETURN(pUIBasicWater, E_FAIL);
	NULL_CHECK_RETURN(pUIBasicLightning, E_FAIL);
	NULL_CHECK_RETURN(pUIBasicWind, E_FAIL);

	m_pBasicFire = pUIBasicFire;
	m_pBasicWater = pUIBasicWater;
	m_pBasicLightning = pUIBasicLightning;
	m_pBasicWind = pUIBasicWind;


	return S_OK;
};

HRESULT CInven::Setting_SignatureSkill()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Signature_Fire", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Signature_Fire.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Signature_Water", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Signature_Water.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Signature_Lightning", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Signature_Lightning.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Signature_Wind", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Signature_Wind.png"), E_FAIL);

	CUIImage* pUISignatureFire = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Fire");
	CUIImage* pUISignatureWater = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Water");
	CUIImage* pUISignatureLightning = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Lightning");
	CUIImage* pUISignatureWind = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Wind");

	NULL_CHECK_RETURN(pUISignatureFire, E_FAIL);
	NULL_CHECK_RETURN(pUISignatureWater, E_FAIL);
	NULL_CHECK_RETURN(pUISignatureLightning, E_FAIL);
	NULL_CHECK_RETURN(pUISignatureWind, E_FAIL);

	m_pSignatureFire = pUISignatureFire;
	m_pSignatureWater = pUISignatureWater;
	m_pSignatureLightning = pUISignatureLightning;
	m_pSignatureWind = pUISignatureWind;

	return S_OK;
}

HRESULT CInven::Setting_SkillImage()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseWaterBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseWaterBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseWaterBlast", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseWaterBlast.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseWindSlam", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseWindSlam.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseAirWave", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseAirWave.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseExplosiveCharge", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseExplosiveCharge.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseRisingDragon", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseRisingDragon.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseShockLace", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseShockLace.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseShockSpear", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseShockSpear.png"), E_FAIL);

	CUIImage* pUIWaterBall = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseWaterBall");
	CUIImage* pUIWaterBlast = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseWaterBlast");
	CUIImage* pUIWindSlam = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseWindSlam");
	CUIImage* pUIAirWave = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseAirWave");
	CUIImage* pUIExplosiveCharge = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseExplosiveCharge");
	CUIImage* pUIRisingDragon = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseRisingDragon");
	CUIImage* pUIShockLace = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseShockLace");
	CUIImage* pUIShockSpear = CUIImage::Create(m_pGraphicDev, _vec3(50.f, 50.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseShockSpear");
	NULL_CHECK_RETURN(pUIWaterBall, E_FAIL);
	NULL_CHECK_RETURN(pUIWaterBlast, E_FAIL);
	NULL_CHECK_RETURN(pUIWindSlam, E_FAIL);
	NULL_CHECK_RETURN(pUIAirWave, E_FAIL);
	NULL_CHECK_RETURN(pUIExplosiveCharge, E_FAIL);
	NULL_CHECK_RETURN(pUIRisingDragon, E_FAIL);
	NULL_CHECK_RETURN(pUIShockLace, E_FAIL);
	NULL_CHECK_RETURN(pUIShockSpear, E_FAIL);
	m_pUseWaterBall = pUIWaterBall;
	m_pUseWaterBlast = pUIWaterBlast;
	m_pUseWindSlam = pUIWindSlam;
	m_pUseAirWave = pUIAirWave;
	m_pUseExplosiveCharge = pUIExplosiveCharge;
	m_pUseRisingDragon = pUIRisingDragon;
	m_pUseShockLace = pUIShockLace;
	m_pUseShokcSpear = pUIShockSpear;

	return S_OK;
}

void CInven::Free()
{
	Engine::Safe_Release(m_pUseRisingDragon);
	Engine::Safe_Release(m_pUseShockLace);
	Engine::Safe_Release(m_pUseShokcSpear);
	Engine::Safe_Release(m_pUseExplosiveCharge);
	Engine::Safe_Release(m_pUseWaterBall);
	Engine::Safe_Release(m_pUseWaterBlast);
	Engine::Safe_Release(m_pUseWindSlam);
	Engine::Safe_Release(m_pUseAirWave);

	Engine::Safe_Release(m_pSignatureFire);
	Engine::Safe_Release(m_pSignatureLightning);
	Engine::Safe_Release(m_pSignatureWater);
	Engine::Safe_Release(m_pSignatureWind);
	Engine::Safe_Release(m_pBasicFire);
	Engine::Safe_Release(m_pBasicWater);
	Engine::Safe_Release(m_pBasicLightning);
	Engine::Safe_Release(m_pBasicWind);
	Engine::Safe_Release(m_pBaseBox);
	Engine::Safe_Release(m_pUpgradeBox);
	Engine::Safe_Release(m_pSelectBox);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	for (_uint i = 0; i < SKILL_END; ++i)
	{
		for_each(m_vecSkill[i].begin(), m_vecSkill[i].end(), Engine::CDeleteObj());
		m_vecSkill[i].clear();
	}

	Engine::Safe_Release(m_pGraphicDev);
}
