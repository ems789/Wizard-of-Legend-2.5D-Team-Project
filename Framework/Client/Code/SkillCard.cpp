#include "stdafx.h"
#include "SkillCard.h"

#include "Export_Function.h"

CSkillCard::CSkillCard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSkillCard::~CSkillCard()
{
}

HRESULT CSkillCard::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, Engine::CSkill* pSkill)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	NULL_CHECK_RETURN(pSkill, E_FAIL);

	m_pSkill = pSkill;
	m_pTransformCom->Set_Pos(pPos);
	m_fScale = fScale;
	Update_Scale();

	return S_OK;
}

_int CSkillCard::Update_GameObject(const _float& fTimeDelta)
{

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CSkillCard::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CSkillCard::Add_Component(const _tchar * pTextureTag)
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

void CSkillCard::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CSkillCard * CSkillCard::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pPos, const _float& fScale, Engine::CSkill* pSkill)
{
	CSkillCard* pInstance = new CSkillCard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pPos, fScale, pSkill)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSkillCard::Free()
{
	CGameObject::Free();
}

