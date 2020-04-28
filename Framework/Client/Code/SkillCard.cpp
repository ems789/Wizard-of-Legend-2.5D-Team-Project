#include "stdafx.h"
#include "SkillCard.h"

#include "Export_Function.h"
#include "Inven.h"
#include "NumberFont.h"

CSkillCard::CSkillCard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSkillCard::~CSkillCard()
{
}

HRESULT CSkillCard::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const wstring& wstrSkillTag, Engine::CSkill* pSkill, const _int& iPrice, const _float& fRadius)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	NULL_CHECK_RETURN(pSkill, E_FAIL);

	m_pSkill = pSkill;
	m_pTransformCom->Set_Pos(pPos);
	m_fScale = fScale;
	Update_Scale();
	m_iPrice = iPrice;
	m_tSphere.fRadius = fRadius;
	m_wstrSkillTag = wstrSkillTag;

	return S_OK;
}

_int CSkillCard::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	const Engine::CComponent* pCom = Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC);

	const _vec3* pTargetPos = dynamic_cast<const Engine::CTransform*>(pCom)->GetInfo(Engine::INFO_POS);
	_vec3 vDist = *pTargetPos - *m_pTransformCom->GetInfo(Engine::INFO_POS);
	_float fDist = D3DXVec3Length(&vDist);

	m_bIsOn = fDist < m_tSphere.fRadius;

	if (m_bIsOn && Engine::KeyDown(DIK_F))
	{
		if (CInven::GetInstance()->Get_Coin() >= m_iPrice)
		{
			m_bIsDead = true;
			CInven::GetInstance()->Add_Skill(m_wstrSkillTag, m_pSkill);
			m_pSkill->AddRef();
			CInven::GetInstance()->Remove_Coin(m_iPrice);
			Engine::PlaySound_(L"ItemActivate.wav", CSoundMgr::EFFECT);
		}
	}
	

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CSkillCard::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	vPos.y += 40.f  * m_fScale;
	CNumberFont::GetInstance()->DrawNumberBill(m_iPrice, &vPos, 0.05f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CSkillCard::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CSkillCard::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CSkillCard::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CSkillCard::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CSkillCard::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CSkillCard::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CSkillCard::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, pTextureTag));
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

CSkillCard * CSkillCard::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pPos, const _float& fScale, const wstring& wstrSkillTag, Engine::CSkill* pSkill, const _int& iPrice, const _float& fRadius)
{
	CSkillCard* pInstance = new CSkillCard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pPos, fScale, wstrSkillTag, pSkill, iPrice, fRadius)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSkillCard::Free()
{
	Engine::Safe_Release(m_pSkill);
	CGameObject::Free();
}

