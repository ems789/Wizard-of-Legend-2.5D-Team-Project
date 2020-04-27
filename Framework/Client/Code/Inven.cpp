#include "stdafx.h"
#include "Inven.h"
#include "UIImage.h"
#include "Player.h"
#include "AquaVortex.h"
#include "WaterBall.h"
#include "SharkPool.h"
#include "MeteorStrike.h"
#include "GuidedFireBall.h"
#include "FireBall.h"
#include "Export_Function.h"
#include "UI.h"
#include "NumberFont.h"

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
	m_vSelect = { -774.f, 2.f, 0.f }; //커서 위치

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_Inven", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/SelectBase.png"), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	FAILED_CHECK_RETURN(Setting_SelectBox(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_BasicSkill(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_SignatureSkill(), E_FAIL);
	/// 임시 스킬이미지들
	FAILED_CHECK_RETURN(Setting_SkillImage(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_SlotImage(), E_FAIL);


	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matTrans;

	m_eCurState = CInven::INVEN_OFF;
	m_eCurSelect = CInven::SELECT_BASE;
	
	//스킬 추가 부분 추가된 부분
	CMeteorStrike* pMeteorStrike = CMeteorStrike::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMeteorStrike, E_FAIL);
	CFireBall* pFireBall = CFireBall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pFireBall, E_FAIL);
	CGuidedFireBall* pGuidedFireBall = CGuidedFireBall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGuidedFireBall, E_FAIL);
	CAquaVortex* pAqua = CAquaVortex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pAqua, E_FAIL);
	CWaterBall* pWaterBall = CWaterBall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pWaterBall, E_FAIL);
	CSharkPool* pSharkPool = CSharkPool::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSharkPool, E_FAIL);

	Add_Skill(L"Skill_AquaVortex", pAqua);
	Add_Skill(L"Skill_WaterBall", pWaterBall);
	Add_Skill(L"Skill_SharkPool", pSharkPool);
	Add_Skill(L"Skill_MeteorStrike", pMeteorStrike);
	Add_Skill(L"Skill_GuidedFire", pGuidedFireBall);
	Add_Skill(L"Skill_FireBall", pFireBall);

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
				m_eCurSelect = CInven::SELECT_BASE;
			//m_eCurSkill = CInven::LIST_ON;

		}
		break;
		///////////////////////////////////////////////////////
	case CInven::INVEN_ON:
		if (Engine::KeyDown(DIK_RIGHT))
		{
			if (m_eCurSelect == SELECT_BASE)
			{
				m_vSelect = { -624.f, 2.f, 0.f };
				m_eCurSelect = SELECT_DASH;
				m_pSelectBox->Set_Pos(m_vSelect);
			}
			else if (m_eCurSelect == SELECT_DASH)
			{
				m_vSelect = { -475.f, 2.f, 0.f };
				m_eCurSelect = SELECT_STANDARD;
				m_pSelectBox->Set_Pos(m_vSelect);
			}
			else if (m_eCurSelect == SELECT_STANDARD)
			{
				m_vSelect = { -330.f, 2.f, 0.f };
				m_eCurSelect = SELECT_SIGNATURE;
				m_pSelectBox->Set_Pos(m_vSelect);
			}

			else if (m_eCurSelect == SELECT_SIGNATURE)
			{
				m_vSelect = { -774.f, 2.f, 0.f };
				m_eCurSelect = SELECT_BASE;
				m_pSelectBox->Set_Pos(m_vSelect);
			}

		}
		if (Engine::KeyDown(DIK_LEFT))
		{
			if (m_eCurSelect == SELECT_BASE)
			{
				m_bUpgradedSkillOn = true;
				m_vSelect = { -330.f, 2.f, 0.f };
				m_eCurSelect = SELECT_SIGNATURE;
				m_pSelectBox->Set_Pos(m_vSelect);
			}

			else if (m_eCurSelect == SELECT_DASH)
			{
				m_vSelect = { -774.f, 2.f, 0.f };
				m_eCurSelect = SELECT_BASE;
				m_pSelectBox->Set_Pos(m_vSelect);
			}
			else if (m_eCurSelect == SELECT_STANDARD)
			{
				m_vSelect = { -624.f, 2.f, 0.f };
				m_eCurSelect = SELECT_DASH;
				m_pSelectBox->Set_Pos(m_vSelect);
			}
			else if (m_eCurSelect == SELECT_SIGNATURE)
			{
				m_vSelect = { -475.f, 2.f, 0.f };
				m_eCurSelect = SELECT_STANDARD;
				m_pSelectBox->Set_Pos(m_vSelect);
			}
		}
		if (m_vSelect.x == -330.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
		{
			m_eCurState = SIGNATURE_FIRE;
			// m_vEquipCardPos = {-}
			m_eCurSkill = METEOR;
		}
		else if (m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
		{
			m_eCurState = BASIC_FIRE;
			m_eCurSkill = FIREBALL;
		}
		else if (m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
		{
			m_eCurState = BASIC_FIRE;
			m_eCurSkill = FIREBALL;
		}
		else if (m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
		{
			m_eCurState = BASIC_FIRE;
			m_eCurSkill = FIREBALL;
		}
		if (Engine::KeyDown(DIK_T))
		{
			m_bInvenOn = false;
			m_bListOn = false;
			m_eCurState = CInven::INVEN_OFF;
			m_eCurSkill = CInven::LIST_ON;
			//
			//m_eCurSelect = CInven::SELECT_BASE;
		}
		break;

	case CInven::BASIC_FIRE:
		if (Engine::KeyDown(DIK_LEFT))
		{
			if (m_eCurSkill == CInven::FIREBALL)
				m_eCurSkill = CInven::FIRESKILL1;

			else if (m_eCurSkill == CInven::FIRESKILL1)
				m_eCurSkill = CInven::FIREBALL;
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			if (m_eCurSkill == CInven::FIRESKILL1)
				m_eCurSkill = CInven::FIREBALL;

			else if (m_eCurSkill == CInven::FIREBALL)
				m_eCurSkill = CInven::FIRESKILL1;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_WATER;
			m_eCurSkill = CInven::WATERBALL;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_WIND;
			//
			m_eCurSkill = CInven::LIST_ON;
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
			if (m_eCurSkill == CInven::SHARKPOOL)
				m_eCurSkill = CInven::WATERBALL;

			else if (m_eCurSkill == CInven::WATERBALL)
				m_eCurSkill = CInven::SHARKPOOL;

		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			if (m_eCurSkill == CInven::WATERBALL)
				m_eCurSkill = CInven::SHARKPOOL;

			else if (m_eCurSkill == CInven::SHARKPOOL)
				m_eCurSkill = CInven::WATERBALL;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_LIGHTNING;
			m_eCurSkill = CInven::LIGHTSKILL1;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_FIRE;
			m_eCurSkill = CInven::FIREBALL;
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
			if (m_eCurSkill == CInven::LIGHTSKILL1)
				m_eCurSkill = CInven::LIGHTSKILL2;

			else if (m_eCurSkill == CInven::LIGHTSKILL2)
				m_eCurSkill = CInven::LIGHTSKILL1;
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			if (m_eCurSkill == CInven::LIGHTSKILL2)
				m_eCurSkill = CInven::LIGHTSKILL1;

			else if (m_eCurSkill == CInven::LIGHTSKILL1)
				m_eCurSkill = CInven::LIGHTSKILL2;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_WIND;
			m_eCurSkill = CInven::LIST_ON;
			//m_eCurSkill = CInven::WINDSKILL1;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_WATER;
			m_eCurSkill = WATERBALL;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = LIST_ON;
		}
		break;
	case CInven::BASIC_WIND:
		if (Engine::KeyDown(DIK_LEFT))
		{
			/*	if (m_eCurSkill == CInven::WINDSKILL1)
			m_eCurSkill == CInven::WINDSKILL2;*/
		}
		if (Engine::KeyDown(DIK_RIGHT))
		{
			//if (m_eCurSkill == CInven::WINDSKILL2)
			//	m_eCurSkill = CInven::WINDSKILL1;
		}
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::BASIC_FIRE;
			m_eCurSkill = FIREBALL;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::BASIC_LIGHTNING;
			m_eCurSkill = CInven::LIGHTSKILL1;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = LIST_ON;
		}
		break;

	case CInven::SIGNATURE_FIRE:
		///m_eCurSkill = FIRESKILL1;
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_WATER;
			m_eCurSkill = CInven::AQUAVORTEX;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_WIND;
			// 바람스킬 넣어야함
			m_eCurSkill = CInven::LIST_ON;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = CInven::LIST_ON;
		}
		break;
	case CInven::SIGNATURE_WATER:

		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_LIGHTNING;
			m_eCurSkill = CInven::LIGHTSKILL3;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_FIRE;
			m_eCurSkill = METEOR;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = LIST_ON;
		}

		break;
	case CInven::SIGNATURE_LIGHTNING:
		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_WIND;
			m_eCurSkill = CInven::LIST_ON;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_WATER;
			m_eCurSkill = CInven::AQUAVORTEX;
		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
			m_eCurSkill = LIST_ON;
		}
		break;
	case CInven::SIGNATURE_WIND:

		if (Engine::KeyDown(DIK_UP))
		{
			m_eCurState = CInven::SIGNATURE_FIRE;
			m_eCurSkill = METEOR;
		}
		if (Engine::KeyDown(DIK_DOWN))
		{
			m_eCurState = CInven::SIGNATURE_LIGHTNING;
			m_eCurSkill = CInven::LIGHTSKILL3;

		}
		if (Engine::KeyDown(DIK_BACKSPACE))
		{
			m_eCurState = CInven::INVEN_ON;
		}
		break;
	}
	//스킬 장착시 인벤에나오는 스킬 화면 
	//FIRE----------------------------------------------------------------
	if (m_eCurSkill == CInven::FIREBALL && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_FireBall");
		m_pPlayer->Change_Skill(1, pSkill);
		m_eCurBaseEquip = CInven::EQUIP_FIREBALL;
		CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseFireBall");
		m_vEquipCardPos = { -774.f, 2.f, 0.f };
		m_pEquipFireBall->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::FIREBALL && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_FireBall");
		m_pPlayer->Change_Skill(2, pSkill);
		CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseFireBall");
		m_eCurDashEquip = CInven::EQUIP_FIREBALL;
		m_vEquipCardPos = { -624.f, 0.f, 0.f };
		m_pEquipFireBall->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::FIREBALL && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_FireBall");
		m_pPlayer->Change_Skill(3, pSkill);
		CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseFireBall");
		m_eCurStandardEquip = CInven::EQUIP_FIREBALL;
		m_vEquipCardPos = { -475.f, 0.f, 0.f };
		m_pEquipFireBall->Set_Pos(m_vEquipCardPos);
	}

	if (m_eCurSkill == CInven::FIRESKILL1 && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_GuidedFire");
		m_pPlayer->Change_Skill(1, pSkill);
		CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseFireBlast");
		m_eCurBaseEquip = CInven::EQUIP_FIRESKILL1;
		m_vEquipCardPos = { -774.f, 0.f, 0.f };
		m_pEquipFireSkill1->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::FIRESKILL1 && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_GuidedFire");
		m_pPlayer->Change_Skill(2, pSkill);
		CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseFireBlast");
		m_eCurDashEquip = CInven::EQUIP_FIRESKILL1;
		m_vEquipCardPos = { -624.f, 0.f, 0.f };
		m_pEquipFireSkill1->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::FIRESKILL1 && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_GuidedFire");
		m_pPlayer->Change_Skill(3, pSkill);
		CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseFireBlast");
		m_eCurStandardEquip = CInven::EQUIP_FIRESKILL1;
		m_vEquipCardPos = { -475.f, 0.f, 0.f };
		m_pEquipFireSkill1->Set_Pos(m_vEquipCardPos);
	}
	///////water------------------------------------------------------------
	if (m_eCurSkill == CInven::WATERBALL && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_WaterBall");
		m_pPlayer->Change_Skill(1, pSkill);
		CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseWaterBall");
		m_eCurBaseEquip = CInven::EQUIP_WATERBALL;
		m_vEquipCardPos = { -774.f, 0.f, 0.f };
		m_pEquipWaterBall->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::WATERBALL && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_WaterBall");
		m_pPlayer->Change_Skill(2, pSkill);
		CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseWaterBall");
		m_eCurDashEquip = CInven::EQUIP_WATERBALL;
		m_vEquipCardPos = { -624.f, 0.f, 0.f };
		m_pEquipWaterBall->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::WATERBALL && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_WaterBall");
		m_pPlayer->Change_Skill(3, pSkill);
		CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseWaterBall");
		m_eCurStandardEquip = CInven::EQUIP_WATERBALL;
		m_vEquipCardPos = { -475.f, 0.f, 0.f };
		m_pEquipWaterBall->Set_Pos(m_vEquipCardPos);
	}
	/////////////////////////////
	if (m_eCurSkill == CInven::SHARKPOOL && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_SharkPool");
		m_pPlayer->Change_Skill(1, pSkill);
		CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseWaterArc");
		//책 내려가고 판넬에 스킬 이미지 들어가도록
		m_eCurBaseEquip = CInven::EQUIP_WATERARC;
		m_vEquipCardPos = { -774.f, 0.f, 0.f };
		m_pEquipWaterArc->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::SHARKPOOL && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_SharkPool");
		m_pPlayer->Change_Skill(2, pSkill);
		CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseWaterArc");
		m_eCurDashEquip = CInven::EQUIP_WATERARC;
		m_vEquipCardPos = { -624.f, 0.f, 0.f };
		m_pEquipWaterArc->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::SHARKPOOL && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_SharkPool");
		m_pPlayer->Change_Skill(3, pSkill);
		CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseWaterArc");
		m_eCurStandardEquip = CInven::EQUIP_WATERARC;
		m_vEquipCardPos = { -475.f, 0.f, 0.f };
		m_pEquipWaterArc->Set_Pos(m_vEquipCardPos);
	}
	///////Lightning------------------------------------------------------------
	if (m_eCurSkill == CInven::LIGHTSKILL1 && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseShockLace");
		m_eCurBaseEquip = CInven::EQUIP_LIGHTSKILL1;
		m_vEquipCardPos = { -774.f, 0.f, 0.f };
		m_pEquipLightningSkill1->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::LIGHTSKILL1 && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseShockLace");
		m_eCurDashEquip = CInven::EQUIP_LIGHTSKILL1;
		m_vEquipCardPos = { -624.f, 0.f, 0.f };
		m_pEquipLightningSkill1->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::LIGHTSKILL1 && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseShockLace");
		m_eCurStandardEquip = CInven::EQUIP_LIGHTSKILL1;
		m_vEquipCardPos = { -475.f, 0.f, 0.f };
		m_pEquipLightningSkill1->Set_Pos(m_vEquipCardPos);
	}
	/////////////////////////////
	if (m_eCurSkill == CInven::LIGHTSKILL2 && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{

		//책 내려가고 판넬에 스킬 이미지 들어가도록
		m_eCurBaseEquip = CInven::EQUIP_LIGHTSKILL2;
		CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseCurrentBurst");
		m_vEquipCardPos = { -774.f, 0.f, 0.f };
		m_pEquipLightningSkill2->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::LIGHTSKILL2 && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseCurrentBurst");
		m_eCurDashEquip = CInven::EQUIP_LIGHTSKILL2;
		m_vEquipCardPos = { -624.f, 0.f, 0.f };
		m_pEquipLightningSkill2->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::LIGHTSKILL2 && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseCurrentBurst");
		m_eCurStandardEquip = CInven::EQUIP_LIGHTSKILL2;
		m_vEquipCardPos = { -475.f, 0.f, 0.f };
		m_pEquipLightningSkill2->Set_Pos(m_vEquipCardPos);
	}
	/////////////////////////			WIND           ---------------------------------------------------------
	//if (m_eCurSkill == CInven::WINDSKILL1 && m_vSelect.x == -774.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	//{
	//	CUI::GetInstance()->SlotSkillOn(0, m_vSlotPos1, L"UseAirWave");
	//	m_eCurBaseEquip = CInven::EQUIP_WINDSKILL1;
	//	m_vEquipCardPos = { -774.f, 0.f, 0.f };
	//	m_pEquipWindSkill1->Set_Pos(m_vEquipCardPos);
	//}
	//if (m_eCurSkill == CInven::WINDSKILL1 && m_vSelect.x == -624.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	//{
	//	CUI::GetInstance()->SlotSkillOn(1, m_vSlotPos2, L"UseAirWave");
	//	m_eCurDashEquip = CInven::EQUIP_WINDSKILL1;
	//	m_vEquipCardPos = { -624.f, 0.f, 0.f };
	//	m_pEquipWindSkill1->Set_Pos(m_vEquipCardPos);
	//}
	//if (m_eCurSkill == CInven::WINDSKILL1 && m_vSelect.x == -475.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	//{
	//	CUI::GetInstance()->SlotSkillOn(2, m_vSlotPos3, L"UseAirWave");
	//	m_eCurStandardEquip = CInven::EQUIP_WINDSKILL1;
	//	m_vEquipCardPos = { -475.f, 0.f, 0.f };
	//	m_pEquipWindSkill1->Set_Pos(m_vEquipCardPos);
	//}
	/////////////////////////////singautreeeeee///////////////////////////////////////
	if (m_eCurSkill == CInven::METEOR && m_vSelect.x == -330.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_MeteorStrike");
		m_pPlayer->Change_Skill(4, pSkill);
		CUI::GetInstance()->SlotSkillOn(3, m_vSlotPos4, L"UseMeteor");
		m_eCurSignatureEquip = CInven::EQUIP_METEOR;
		m_vEquipCardPos = { -330.f, 0.f, 0.f };
		m_pEquipMeteor->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::AQUAVORTEX && m_vSelect.x == -330.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		Engine::CSkill* pSkill = Find_Skill(L"Skill_AquaVortex");
		m_pPlayer->Change_Skill(4, pSkill);
		CUI::GetInstance()->SlotSkillOn(3, m_vSlotPos4, L"UseAquaVortex");
		m_eCurSignatureEquip = CInven::EQUIP_AQUAVORTEX;
		m_vEquipCardPos = { -330.f, 0.f, 0.f };
		m_pEquipAquaVortex->Set_Pos(m_vEquipCardPos);
	}
	if (m_eCurSkill == CInven::LIGHTSKILL3 && m_vSelect.x == -330.f && m_vSelect.y == 2 && Engine::KeyDown(DIK_RETURN))
	{
		CUI::GetInstance()->SlotSkillOn(3, m_vSlotPos4, L"UseLightningXStrike");
		m_eCurSignatureEquip = CInven::EQUIP_LIGHTSKILL3;
		m_vEquipCardPos = { -330.f, 0.f, 0.f };
		m_pEquipLightningSkill3->Set_Pos(m_vEquipCardPos);
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
	//////////////////////////////////////
	switch (m_eCurBaseEquip)
	{
	case EQUIP_FIREBALL:
		m_pEquipFireBall->Render_UIImage();
		break;
	case EQUIP_FIRESKILL1:
		m_pEquipFireSkill1->Render_UIImage();
		break;
	case EQUIP_WATERBALL:
		m_pEquipWaterBall->Render_UIImage();
		break;
	case EQUIP_WATERARC:
		m_pEquipWaterArc->Render_UIImage();
		break;
	case EQUIP_LIGHTSKILL1:
		m_pEquipLightningSkill1->Render_UIImage();
		break;
	case EQUIP_LIGHTSKILL2:
		m_pEquipLightningSkill2->Render_UIImage();
		break;
	case EQUIP_WINDSKILL1:
		m_pEquipWindSkill1->Render_UIImage();
		break;
	}
	switch (m_eCurDashEquip)
	{
	case EQUIP_FIREBALL:
		m_pEquipFireBall->Render_UIImage();
		break;
	case EQUIP_FIRESKILL1:
		m_pEquipFireSkill1->Render_UIImage();
		break;
	case EQUIP_WATERBALL:
		m_pEquipWaterBall->Render_UIImage();
		break;
	case EQUIP_WATERARC:
		m_pEquipWaterArc->Render_UIImage();
		break;
	case EQUIP_LIGHTSKILL1:
		m_pEquipLightningSkill1->Render_UIImage();
		break;
	case EQUIP_LIGHTSKILL2:
		m_pEquipLightningSkill2->Render_UIImage();
		break;
	case EQUIP_WINDSKILL1:
		m_pEquipWindSkill1->Render_UIImage();
		break;
	}
	switch (m_eCurStandardEquip)
	{
	case EQUIP_FIREBALL:
		m_pEquipFireBall->Render_UIImage();
		break;
	case EQUIP_FIRESKILL1:
		m_pEquipFireSkill1->Render_UIImage();
		break;
	case EQUIP_WATERBALL:
		m_pEquipWaterBall->Render_UIImage();
		break;
	case EQUIP_WATERARC:
		m_pEquipWaterArc->Render_UIImage();
		break;
	case EQUIP_LIGHTSKILL1:
		m_pEquipLightningSkill1->Render_UIImage();
		break;
	case EQUIP_LIGHTSKILL2:
		m_pEquipLightningSkill2->Render_UIImage();
		break;
	case EQUIP_WINDSKILL1:
		m_pEquipWindSkill1->Render_UIImage();
		break;
	}
	switch (m_eCurSignatureEquip)
	{
	case EQUIP_LIGHTSKILL3:
		m_pEquipLightningSkill3->Render_UIImage();
		break;
	case EQUIP_METEOR:
		m_pEquipMeteor->Render_UIImage();
		break;
	case EQUIP_AQUAVORTEX:
		m_pEquipAquaVortex->Render_UIImage();
		break;
	case EQUIP_WINDSKLL2:
		m_pEquipWindSkill2->Render_UIImage();
		break;
	}
	///////////////////////////////////////////////
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
	case WATERBALL:
		m_pWaterBall->Render_UIImage();
		break;
	case SHARKPOOL:
		m_pWaterArc->Render_UIImage();
		break;
	case AQUAVORTEX:
		m_pAquaVortex->Render_UIImage();
		break;
	case FIREBALL:
		m_pFireBall->Render_UIImage();
		break;
	case METEOR:
		m_pMeteor->Render_UIImage();
		break;
	case FIRESKILL1:
		m_pFireSkill1->Render_UIImage();
		break;
	case WINDSKILL1:
		m_pWindSkill1->Render_UIImage();
		break;
	case WINDSKILL2:
		m_pWindSkill2->Render_UIImage();
		break;
	case LIGHTSKILL1:
		m_pLightningSkill1->Render_UIImage();
		break;
	case LIGHTSKILL2:
		m_pLightningSkill2->Render_UIImage();
		break;
	case LIGHTSKILL3:
		m_pLightningSkill3->Render_UIImage();
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

	CUIImage* pUIImage = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vSelect, L"Cursor");
	//왼쪽위치 Scale : 300, 420  Pos : -680, 10 
	//scale, pos순서

	NULL_CHECK_RETURN(pUIImage, E_FAIL);
	m_pSelectBox = pUIImage;

	return S_OK;
}


void CInven::Set_Player(CPlayer * pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();
}

HRESULT CInven::Setting_BasicSkill()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Fire", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Fire.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Water", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Water.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Lightning", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Lightning.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Basic_Wind", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Basic_Wind.png"), E_FAIL);

	//책위치
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
	//////////
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"WindSkill2", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/VortexWave.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"AquaVortex", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/AquaVortex.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"LightningSkill3", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/LightningXStrike.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Meteor", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Meteor.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipWindSkill2", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/VortexWave.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipAquaVortex", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/AquaVortex.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipLightningSkill3", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/LightningXStrike.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipMeteor", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/Meteor.png"), E_FAIL);

	CUIImage* pUISignatureFire = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Fire");
	CUIImage* pUISignatureWater = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Water");
	CUIImage* pUISignatureLightning = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Lightning");
	CUIImage* pUISignatureWind = CUIImage::Create(m_pGraphicDev, _vec3(850.f, 850.f, 0.f), _vec3(-500.f, 0.f, 0.f), L"Signature_Wind");
	////////////signature
	CUIImage* pUIEquipSignatureAquaVortex = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipAquaVortex");
	CUIImage* pUIEquipSignatureMeteor = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipMeteor");
	CUIImage* pUIEquipSignatureLightningSkill3 = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipLightningSkill3");
	CUIImage* pUIEquipSignatureWindSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipWindSkill2");
	//////////////////////////////////
	CUIImage* pUIMeteor = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"Meteor");
	CUIImage* pUIAquaVortex = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"AquaVortex");
	CUIImage* pUILightningSkill3 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"LightningSkill3");
	CUIImage* pUIWindSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"WindSkill2");
	///////////////////////////////////////////////////////////////////////////////////
	NULL_CHECK_RETURN(pUISignatureFire, E_FAIL);
	NULL_CHECK_RETURN(pUISignatureWater, E_FAIL);
	NULL_CHECK_RETURN(pUISignatureLightning, E_FAIL);
	NULL_CHECK_RETURN(pUISignatureWind, E_FAIL);
	////
	NULL_CHECK_RETURN(pUIMeteor, E_FAIL);
	NULL_CHECK_RETURN(pUIAquaVortex, E_FAIL);
	NULL_CHECK_RETURN(pUILightningSkill3, E_FAIL);
	NULL_CHECK_RETURN(pUIWindSkill2, E_FAIL);
	//
	NULL_CHECK_RETURN(pUIEquipSignatureAquaVortex, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipSignatureMeteor, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipSignatureLightningSkill3, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipSignatureWindSkill2, E_FAIL);

	m_pSignatureFire = pUISignatureFire;
	m_pSignatureWater = pUISignatureWater;
	m_pSignatureLightning = pUISignatureLightning;
	m_pSignatureWind = pUISignatureWind;
	//////////////////////////////////////////////////
	m_pAquaVortex = pUIAquaVortex;
	m_pMeteor = pUIMeteor;
	m_pLightningSkill3 = pUILightningSkill3;
	m_pWindSkill2 = pUIWindSkill2;
	////////////////////////////////////////
	m_pEquipAquaVortex = pUIEquipSignatureAquaVortex;
	m_pEquipMeteor = pUIEquipSignatureMeteor;
	m_pEquipWindSkill2 = pUIEquipSignatureWindSkill2;
	m_pEquipLightningSkill3 = pUIEquipSignatureLightningSkill3;
	////////////////////////////////////////////////


	return S_OK;
}

HRESULT CInven::Setting_SkillImage()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"WaterBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/WaterBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"WaterArc", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/WaterArc.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"FireSkill1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/FireBlast.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"FireBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/FireBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"WindSkill1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/AirWave.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"LightningSkill1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/ShockLace.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"LightningSkill2", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/CurrentBurst.png"), E_FAIL);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////equipimage
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipFireBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/FireBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipFireSkill1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/FireBlast.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipWaterBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/WaterBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipWaterArc", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/WaterArc.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipWindSkill1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/AirWave.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipLightningSkill1", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/ShockLace.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"EquipLightningSkill2", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/CurrentBurst.png"), E_FAIL);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CUIImage* pUIWaterBall = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"WaterBall");
	CUIImage* pUIWaterArc = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"WaterArc");
	CUIImage* pUIFireBall = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"FireBall");
	CUIImage* pUIFireSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"FireSkill1");
	CUIImage* pUILightningSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"LightningSkill1");
	CUIImage* pUILightningSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"LightningSkill2");
	CUIImage* pUIWindSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"WindSkill1");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////equip image
	CUIImage* pUIEquipFireBall = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipFireBall");
	CUIImage* pUIEquipFireSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipFireSkill1");
	CUIImage* pUIEquipWaterBall = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipWaterBall");
	CUIImage* pUIEquipWaterArc = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipWaterArc");
	CUIImage* pUIEquipLightningSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipLightningSkill1");
	CUIImage* pUIEquipLightningSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipLightningSkill2");
	CUIImage* pUIEquipWindSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(105.f, 200.f, 0.f), m_vEquipCardPos, L"EquipWindSkill1");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	NULL_CHECK_RETURN(pUIWaterBall, E_FAIL);
	NULL_CHECK_RETURN(pUIWaterArc, E_FAIL);
	NULL_CHECK_RETURN(pUIFireSkill1, E_FAIL);
	NULL_CHECK_RETURN(pUIFireBall, E_FAIL);
	NULL_CHECK_RETURN(pUILightningSkill1, E_FAIL);
	NULL_CHECK_RETURN(pUILightningSkill2, E_FAIL);
	NULL_CHECK_RETURN(pUIWindSkill1, E_FAIL);
	/////////////////////////////////////////////// equip image
	NULL_CHECK_RETURN(pUIEquipFireBall, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipFireSkill1, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipWaterBall, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipWaterArc, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipLightningSkill1, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipLightningSkill2, E_FAIL);
	NULL_CHECK_RETURN(pUIEquipWindSkill1, E_FAIL);
	/////////////////////////////////////////////////
	m_pWaterBall = pUIWaterBall;
	m_pWaterArc = pUIWaterArc;
	m_pFireBall = pUIFireBall;
	m_pFireSkill1 = pUIFireSkill1;
	m_pWindSkill1 = pUIWindSkill1;
	m_pLightningSkill1 = pUILightningSkill1;
	m_pLightningSkill2 = pUILightningSkill2;
	///////////////////////////////////////////////////equi\p
	m_pEquipFireBall = pUIEquipFireBall;
	m_pEquipFireSkill1 = pUIEquipFireSkill1;
	m_pEquipWaterBall = pUIEquipWaterBall;
	m_pEquipWaterArc = pUIEquipWaterArc;
	m_pEquipLightningSkill1 = pUIEquipLightningSkill1;
	m_pEquipLightningSkill2 = pUIEquipLightningSkill2;
	m_pEquipWindSkill1 = pUIEquipWindSkill1;

	return S_OK;
}

HRESULT CInven::Setting_SlotImage()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseFireBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseFireBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseFireBlast", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseFireBlast.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseMeteor", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseMeteor.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseWaterBall", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseWaterBall.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseWaterArc", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseWaterArc.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseAquaVortex", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseAquaVortex.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseShockLace", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseShockLace.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseCurrentBurst", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseCurrentBurst.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseLightningXStrike", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseLightningXStrike.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseAirWave", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseAirWave.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UseVortexWave", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UseVortexWave.png"), E_FAIL);

	CUIImage* pFireSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseFireBall");
	CUIImage* pFireSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseFireBlast");
	CUIImage* pFireSkill3 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseMeteor");
	CUIImage* pWaterSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseWaterBall");
	CUIImage* pWaterSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseWaterArc");
	CUIImage* pWaterSkill3 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseAquaVortex");
	CUIImage* pLightningSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseShockLace");
	CUIImage* pLightningSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseCurrentBurst");
	CUIImage* pLightningSkill3 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseLightningXStrike");
	CUIImage* pWindSkill1 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseAirWave");
	CUIImage* pWindSkill2 = CUIImage::Create(m_pGraphicDev, _vec3(115.f, 160.f, 0.f), _vec3(-505.f, 115.f, 0.f), L"UseVortexWave");

	NULL_CHECK_RETURN(pFireSkill1, E_FAIL);
	NULL_CHECK_RETURN(pFireSkill2, E_FAIL);
	NULL_CHECK_RETURN(pFireSkill3, E_FAIL);
	NULL_CHECK_RETURN(pWaterSkill1, E_FAIL);
	NULL_CHECK_RETURN(pWaterSkill2, E_FAIL);
	NULL_CHECK_RETURN(pWaterSkill3, E_FAIL);
	NULL_CHECK_RETURN(pLightningSkill1, E_FAIL);
	NULL_CHECK_RETURN(pLightningSkill2, E_FAIL);
	NULL_CHECK_RETURN(pLightningSkill3, E_FAIL);
	NULL_CHECK_RETURN(pWindSkill1, E_FAIL);
	NULL_CHECK_RETURN(pWindSkill2, E_FAIL);

	m_pSlotFireBall = pFireSkill1;
	m_pSlotFireSkill1 = pFireSkill2;
	m_pSlotMeteor = pFireSkill3;
	m_pSlotWaterBall = pWaterSkill1;
	m_pSlotWaterArc = pWaterSkill2;
	m_pSlotAquaVortex = pWaterSkill3;
	m_pSlotLightningSkill1 = pLightningSkill1;
	m_pSlotLightningSkill2 = pLightningSkill2;
	m_pSlotLightningSkill3 = pLightningSkill3;
	m_pSlotWindSkill1 = pWindSkill1;
	m_pSlotWindSkill2 = pWindSkill2;

	return S_OK;
}

Engine::CSkill * CInven::Find_Skill(const wstring & SkillTag)
{
	auto iter = m_MapSkill.find(SkillTag);

	if (iter == m_MapSkill.end())
		return nullptr;

	return iter->second;
}

HRESULT CInven::Add_Skill(const wstring & SkillTag, Engine::CSkill * pSkill)
{
	NULL_CHECK_RETURN(pSkill, E_FAIL);
	auto iterator = m_MapSkill.find(SkillTag);

	if (iterator == m_MapSkill.end())
	{
		m_MapSkill.emplace(SkillTag, pSkill);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

void CInven::Free()
{

	// Map Skill Release
	for (auto& iter : m_MapSkill)
	{
		Engine::Safe_Release(iter.second);
	}
	m_MapSkill.clear();

	Engine::Safe_Release(m_pPlayer);


	//slot image
	Engine::Safe_Release(m_pSlotFireSkill1);
	Engine::Safe_Release(m_pSlotLightningSkill1);
	Engine::Safe_Release(m_pSlotLightningSkill2);
	Engine::Safe_Release(m_pSlotLightningSkill3);
	Engine::Safe_Release(m_pSlotFireBall);
	Engine::Safe_Release(m_pSlotAquaVortex);
	Engine::Safe_Release(m_pSlotMeteor);
	Engine::Safe_Release(m_pSlotWaterArc);
	Engine::Safe_Release(m_pSlotWaterBall);
	Engine::Safe_Release(m_pSlotWindSkill1);
	Engine::Safe_Release(m_pSlotWindSkill2);
	/////////////////////////
	//////////////////////equip image
	Engine::Safe_Release(m_pEquipFireBall);
	Engine::Safe_Release(m_pEquipFireSkill1);
	Engine::Safe_Release(m_pEquipWaterBall);
	Engine::Safe_Release(m_pEquipWaterArc);
	Engine::Safe_Release(m_pEquipLightningSkill1);
	Engine::Safe_Release(m_pEquipLightningSkill2);
	Engine::Safe_Release(m_pEquipWindSkill1);
	////시그니처///
	Engine::Safe_Release(m_pEquipMeteor);
	Engine::Safe_Release(m_pEquipWindSkill2);
	Engine::Safe_Release(m_pEquipAquaVortex);
	Engine::Safe_Release(m_pEquipLightningSkill3);
	/////////////
	Engine::Safe_Release(m_pWindSkill1);
	Engine::Safe_Release(m_pWindSkill2);
	Engine::Safe_Release(m_pLightningSkill1);
	Engine::Safe_Release(m_pLightningSkill2);
	Engine::Safe_Release(m_pLightningSkill3);
	Engine::Safe_Release(m_pFireBall);
	Engine::Safe_Release(m_pFireSkill1);
	Engine::Safe_Release(m_pMeteor);
	Engine::Safe_Release(m_pWaterBall);
	Engine::Safe_Release(m_pWaterArc);
	Engine::Safe_Release(m_pAquaVortex);

	///////////////////////////////////////////////////////////////
	Engine::Safe_Release(m_pSignatureFire);
	Engine::Safe_Release(m_pSignatureLightning);
	Engine::Safe_Release(m_pSignatureWater);
	Engine::Safe_Release(m_pSignatureWind);
	Engine::Safe_Release(m_pBasicFire);
	Engine::Safe_Release(m_pBasicWater);
	Engine::Safe_Release(m_pBasicLightning);
	Engine::Safe_Release(m_pBasicWind);
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
