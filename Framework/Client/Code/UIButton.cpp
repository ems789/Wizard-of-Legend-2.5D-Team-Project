#include "stdafx.h"
#include "UIButton.h"

#include "Export_Function.h"

CUIButton::CUIButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUIButton::~CUIButton()
{
}

HRESULT CUIButton::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos, const D3DXCOLOR& d3dColor)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_vOnScale = *pScale;
	m_vOffScale = m_vOnScale * 0.8f;

	m_d3dOnColor = d3dColor;

	for (_int i = 0; i < 4; ++i)
		*((_float*)&m_d3dOffColor + i) = (*((_float*)&m_d3dOffColor + i)) * 0.8f;

	m_pTransformCom->Set_Pos(pPos);
	
	Update_Scale();

	return S_OK;
}

_int CUIButton::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Update_Scale();

	_int iEixt = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return iEixt;
}

void CUIButton::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();

	m_pBufferCom->Set_Color(m_bIsOn ? m_d3dOnColor : m_d3dOffColor);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIButton::Add_Component(const _tchar* pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcAlphaTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcColTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, pTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CUIButton::Update_Scale()
{
	_vec3* pScale = m_bIsOn ? &m_vOnScale : &m_vOffScale;

	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * pScale->x, pTexInfo->tImgInfo.Height * pScale->y, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CUIButton * CUIButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pScale, const _vec3 * pPos, const D3DXCOLOR& d3dColor)
{
	CUIButton* pInstance = new CUIButton(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pScale, pPos, d3dColor)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CUIButton::Free()
{
	Engine::CGameObject::Free();
}

