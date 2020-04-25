#include "stdafx.h"
#include "UI.h"
#include "UIImage.h"
#include "Inven.h"
#include "Export_Function.h"
#include "NumberFont.h"

IMPLEMENT_SINGLETON(CUI)

CUI::CUI()

{

}

CUI::~CUI()
{
	Free();
}

HRESULT CUI::Ready_PlayerUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();


	//FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_UI", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/UI/Coin/Coin.png"), E_FAIL);
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_PlayerState(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_SkillSlot(), E_FAIL);
	FAILED_CHECK_RETURN(Setting_Coin(), E_FAIL);
	//FAILED_CHECK_RETURN(Setting_SlotImage(), E_FAIL);


	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_fUISpeed = 0.5f;
	m_matWorld = matScale * matTrans;

	m_vecSlotImage.resize(4, nullptr);


	m_pNumberFont = CNumberFont::GetInstance();
	m_pNumberFont->AddRef();

	return S_OK;
}

_int CUI::Update_PlayerUI(const _float & fTimeDelta)
{
	if (false == m_bShowUI)
		return 0;
	//m_pUIManaBar->Set_Scale(m_vManaScale);
	//m_pUIManaBar->Set_Pos(m_vManaPos);

	//m_vManaPos.x -= 0.5f * m_fUISpeed;
	//m_vManaScale.x -= 0.5f * m_fUISpeed;

	_int PlayerHp = 0, PlayerHpMax = 0;
	PlayerHp = Engine::Get_Player()->Get_HP();
	PlayerHpMax = Engine::Get_Player()->Get_HPMax();

	_float	fHPRatio = PlayerHp / (_float)PlayerHpMax;
	
	_vec3 vHpScale = { m_vHpScale.x * fHPRatio, m_vHpScale.y, m_vHpScale.z };
	_vec3 vHpPos = {m_vHpPos.x - (m_vHpScale.x - vHpScale.x) * 0.5f , m_vHpPos.y, m_vHpPos.z };
	m_pUIHpBar->Set_Scale(vHpScale);
	m_pUIHpBar->Set_Pos(vHpPos);
	
	_float fMp = Engine::Get_Player()->Get_MP();
	_float fMpMax = Engine::Get_Player()->Get_MPMax();

	_float fMpRatio = fMp / fMpMax;

	_vec3 vMpScale	= { m_vManaScale.x * fMpRatio, m_vManaScale.y, m_vManaScale.z };
	_vec3 vMpPos	= { m_vManaPos.x + (vMpScale.x) * 0.5f, m_vManaPos.y, m_vManaPos.z };

	m_pUIManaBar->Set_Scale(vMpScale);
	m_pUIManaBar->Set_Pos(vMpPos);

	//if (Engine::MouseUp(Engine::DIM_LB))
	//{
	//	//마나채우는 평타
	//	m_vManaScale.x += 19.2f;
	//	m_vManaPos.x += 10.f;
	//}
	//if (m_vManaScale.x < 0.f)
	//{
	//	m_vManaScale.x = 0.f;
	//}
	//if (m_vManaPos.x < -896.f)
	//{
	//	m_vManaPos.x = -896.f;
	//}
	//if (m_vManaScale.x > 192.f)
	//{
	//	m_vManaScale.x = 192.f;
	//}
	//if (m_vManaPos.x > -795.f)
	//	m_vManaPos.x = -795.f;


	return 0;
}

void CUI::Render_PlayerUI()
{
	if (false == m_bShowUI)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	//m_pTextureCom->Render_Texture();
	//m_pBufferCom->Render_Buffer();

	m_pUIHurtBar->Render_UIImage();
	m_pUIHpBar->Render_UIImage();
	m_pUIManaBar->Render_UIImage();
	m_pUIPlayer->Render_UIImage();

	m_pUICoin->Render_UIImage();

	m_pNumberFont->DrawNumber(CInven::GetInstance()->Get_Coin(), &_vec3(-50.f, 500.f, 0.f), 4.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pUISkillSlot->Render_UIImage();

	for (_uint i = 0; i < 4; ++i)
	{
		//어느 슬롯에 넣을거냐 
		if (nullptr != m_vecSlotImage[i])
			m_vecSlotImage[i]->Render_UIImage();
	}

}


void CUI::SlotSkillOn(_uint uiSlot, const _vec3 & vImagePos, const _tchar * pTextureTag)
{
	if (nullptr != m_vecSlotImage[uiSlot])
		Engine::Safe_Release(m_vecSlotImage[uiSlot]);

	CUIImage* pSkillImage = CUIImage::Create(m_pGraphicDev, _vec3{ 50.f, 50.f, 0.f }, vImagePos, pTextureTag);


	m_vecSlotImage[uiSlot] = pSkillImage;


}

HRESULT CUI::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	//	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	///NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);
	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_UI"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	//NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"UI : Add_Component Failed. m_pBufferCom is nullptr");
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_UI_PLAYERBAR"));
	//NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"UI : Add_Component Failed. m_pTextureCom is nullptr");
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	//pComponent = m_pTransformCom = Engine::CTransform::Create();
	//NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"UI : Add_Component Failed. m_pTransformCom is nullptr");
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	//
	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"UI : Add_Component Failed. m_pRendererCom is nullptr");
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

HRESULT CUI::Setting_SkillSlot()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UI_SKILLBAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Slot/UI_SKILLBAR.png"), E_FAIL);
	CUIImage* pSlotUI = CUIImage::Create(m_pGraphicDev, _vec3(468.f, 160.f, 0.f), _vec3(650.f, 450.f, 0.f), L"UI_SKILLBAR");

	NULL_CHECK_RETURN(pSlotUI, E_FAIL);

	m_pUISkillSlot = pSlotUI;

	return S_OK;
}

HRESULT CUI::Setting_Coin()
{
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Coin", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Coin/Coin.png"), E_FAIL);
	CUIImage* pCoinUI = CUIImage::Create(m_pGraphicDev, _vec3(24.f, 24.f, 0.f), _vec3(-100.f, 500.f, 0.f), L"Coin");

	NULL_CHECK_RETURN(m_pUISkillSlot, E_FAIL);

	m_pUICoin = pCoinUI;

	return S_OK;
}

HRESULT CUI::Setting_PlayerState()
{

	m_vPos = { 100.f, 100.f, 0.f };
	m_vScale = { 100.f, 100.f, 0.f };
	m_vHpScale = { 244.f, 32.f, 0.f };
	m_vManaScale = { 192.f, 16.f, 0.f };//꽉찰시192
	m_vHurtScale = { 244.f, 32.f, 0.f };
	m_vHurtPos = { -765.f, 510.f, 0.f };
	m_vHpPos = { -765.f, 510.f, 0.f };
	m_vManaPos = { -896.f, 480.f, 0.f };




	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIPLAYERBAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_PLAYERBAR.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIHPBAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_HPBAR.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIMANABAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_MANABAR.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIHURTBAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_HURTBAR.png"), E_FAIL);

	CUIImage* pPlayerUI = CUIImage::Create(m_pGraphicDev, _vec3(328.f, 80.f, 0.f), _vec3(-800.f, 500.f, 0.f), L"UIPLAYERBAR");
	CUIImage* pHpBarUI = CUIImage::Create(m_pGraphicDev, m_vHpScale, m_vHpPos, L"UIHPBAR");
	CUIImage* pManaBarUI = CUIImage::Create(m_pGraphicDev, m_vManaScale, m_vManaPos, L"UIMANABAR");
	CUIImage* pHurtBarUI = CUIImage::Create(m_pGraphicDev, m_vHurtScale, m_vHurtPos, L"UIHURTBAR");

	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	NULL_CHECK_RETURN(pHpBarUI, E_FAIL);
	NULL_CHECK_RETURN(pManaBarUI, E_FAIL);
	NULL_CHECK_RETURN(pHurtBarUI, E_FAIL);

	m_pUIPlayer = pPlayerUI;
	m_pUIHpBar = pHpBarUI;
	m_pUIManaBar = pManaBarUI;
	m_pUIHurtBar = pHurtBarUI;

	return S_OK;
}

void CUI::Free()
{
	Engine::Safe_Release(m_pNumberFont);


	for (auto& pUIImage : m_vecSlotImage)
	{
		if (pUIImage)
			Engine::Safe_Release(pUIImage);
	}
	m_vecSlotImage.clear();


	Engine::Safe_Release(m_pUIHurtBar);
	Engine::Safe_Release(m_pUICoin);
	Engine::Safe_Release(m_pUISkillSlot);
	Engine::Safe_Release(m_pUIHpBar);
	Engine::Safe_Release(m_pUIManaBar);
	Engine::Safe_Release(m_pUIPlayer);
	//	Engine::Safe_Release(m_pTextureCom);
	//	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pGraphicDev);
}

