#include "stdafx.h"
#include "UI.h"
#include "UIImage.h"

#include "Export_Function.h"

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

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);


	m_matWorld = matScale * matTrans;
	return S_OK;
}

_int CUI::Update_PlayerUI(const _float & fTimeDelta)
{
	return 0;
}

void CUI::Render_PlayerUI()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	//m_pTextureCom->Render_Texture();
	//m_pBufferCom->Render_Buffer();

	m_pUIPlayer->Render_UIImage();
	m_pUIHpBar->Render_UIImage();
	m_pUIManaBar->Render_UIImage();
	m_pUICoin->Render_UIImage();

	//////
	m_pUISkillSlot->Render_UIImage();
}


HRESULT CUI::Add_Component()
{
	//Engine::CComponent*	pComponent = nullptr;

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

	NULL_CHECK_RETURN(m_pUISkillSlot, E_FAIL);

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
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIPLAYERBAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_PLAYERBAR.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIHPBAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_HPBAR.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"UIMANABAR", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HP/UI_MANABAR.png"), E_FAIL);

	CUIImage* pPlayerUI = CUIImage::Create(m_pGraphicDev, _vec3(328.f, 80.f, 0.f), _vec3(-800.f, 500.f, 0.f), L"UIPLAYERBAR");
	CUIImage* pHpBarUI = CUIImage::Create(m_pGraphicDev, _vec3(244.f, 32.f, 0.f), _vec3(-765.f, 510.f, 0.f), L"UIHPBAR");
	CUIImage* pManaBarUI = CUIImage::Create(m_pGraphicDev, _vec3(192.f, 16.f, 0.f), _vec3(-790.f, 480.f, 0.f), L"UIMANABAR");

	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	NULL_CHECK_RETURN(pHpBarUI, E_FAIL);
	NULL_CHECK_RETURN(pManaBarUI, E_FAIL);

	m_pUIPlayer = pPlayerUI;
	m_pUIHpBar = pHpBarUI;
	m_pUIManaBar = pManaBarUI;

	return S_OK;
}

void CUI::Free()
{
	Engine::Safe_Release(m_pUICoin);
	Engine::Safe_Release(m_pUISkillSlot);
	Engine::Safe_Release(m_pUIHpBar);
	Engine::Safe_Release(m_pUIManaBar);
	Engine::Safe_Release(m_pUIPlayer);
//	Engine::Safe_Release(m_pTextureCom);
//	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pGraphicDev);
}

