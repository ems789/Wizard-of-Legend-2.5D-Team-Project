#include "stdafx.h"
#include "UIObject.h"

#include "Export_Function.h"

CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUIObject::~CUIObject()
{
}

HRESULT CUIObject::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pScale, const _vec3* pPos)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_vScale = *pScale;

	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Pos(pPos);
	
	Update_Scale();

	return S_OK;
}

_int CUIObject::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	_int iEixt = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return iEixt;
}

void CUIObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

const _vec3* CUIObject::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CUIObject::Move_Pos(const _vec3 * pMove)
{
	m_pTransformCom->Move_Pos(pMove);
}

void CUIObject::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CUIObject::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

HRESULT CUIObject::Add_Component(const _tchar* pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
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

void CUIObject::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_vScale.x, pTexInfo->tImgInfo.Height * m_vScale.y, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CUIObject * CUIObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pScale, const _vec3 * pPos)
{
	CUIObject* pInstance = new CUIObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pScale, pPos)))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CUIObject::Free()
{
	Engine::CGameObject::Free();
}

