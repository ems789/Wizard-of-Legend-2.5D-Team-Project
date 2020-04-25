#include "stdafx.h"
#include "UIBlink.h"

#include "Export_Function.h"

CUIBlink::CUIBlink(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUIBlink::~CUIBlink()
{
}

HRESULT CUIBlink::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_vScale = *pScale;

	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Pos(pPos);
	
	Update_Scale();

	m_d3dColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_fColorSpeed = 1.f;

	return S_OK;
}

_int CUIBlink::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;
	
	Change_Color(fTimeDelta);

	_int iEixt = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return iEixt;
}

void CUIBlink::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Set_Color(m_d3dColor);
	m_pBufferCom->Render_Buffer();
}

const _vec3* CUIBlink::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CUIBlink::Move_Pos(const _vec3 * pMove)
{
	m_pTransformCom->Move_Pos(pMove);
}

void CUIBlink::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CUIBlink::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

HRESULT CUIBlink::Add_Component(const _tchar* pTextureTag)
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

void CUIBlink::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_vScale.x, pTexInfo->tImgInfo.Height * m_vScale.y, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CUIBlink::Change_Color(const _float & fTimeDelta)
{
	m_d3dColor.a += fTimeDelta * m_fColorSpeed;
	m_d3dColor.r += fTimeDelta * m_fColorSpeed;
	m_d3dColor.g += fTimeDelta * m_fColorSpeed;
	m_d3dColor.b += fTimeDelta * m_fColorSpeed;

	if (m_d3dColor.a >= 1.f || m_d3dColor.a <= 0.f || 
		m_d3dColor.r >= 1.f || m_d3dColor.r <= 0.f ||
		m_d3dColor.g >= 1.f || m_d3dColor.g <= 0.f ||
		m_d3dColor.b >= 1.f || m_d3dColor.b <= 0.f)
		m_fColorSpeed *= -1.f;

}

CUIBlink * CUIBlink::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pScale, const _vec3 * pPos)
{
	CUIBlink* pInstance = new CUIBlink(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pScale, pPos)))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CUIBlink::Free()
{
	Engine::CGameObject::Free();
}

