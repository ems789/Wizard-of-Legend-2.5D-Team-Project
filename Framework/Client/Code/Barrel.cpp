#include "stdafx.h"
#include "Barrel.h"

#include "Export_Function.h"
#include "SphereCollider.h"
#include "FireKick.h"
#include "BasicEffect.h"

CBarrel::CBarrel(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBarrel::~CBarrel()
{
}

HRESULT CBarrel::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed, const _bool& bBoom)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	
	m_fScale = fScale;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = fMaxFrame;
	m_tFrame.fFrameSpeed = fFrameSpeed;

	m_bBoom = bBoom;

	m_d3dColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	Update_Scale();

	return S_OK;
}

_int CBarrel::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Turn_To_Camera_Look();

	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	m_tSphere.vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	return 0;
}

void CBarrel::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Set_Color(m_d3dColor);
	m_pBufferCom->Render_Buffer();
}

void CBarrel::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_bIsDead = true;

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	if (m_bBoom)
	{
		CFireKick* pFireKick = CFireKick::Create(m_pGraphicDev, vPos, 0.2f, 4.f, 20, 0.05f);
		Engine::Add_GameObject(L"GameLogic", L"Barrel_Boom", pFireKick);
	}
	else
	{
		CBasicEffect*  pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_BarrelDestroyed", L"", 4.f, 10.f, 0.05f, &vPos, false, 0.f);
		Engine::Add_GameObject(L"Effect", L"Effect_BarrelDestroyed", pEffect);
	}
}

const _vec3 * CBarrel::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CBarrel::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CBarrel::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CBarrel::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CBarrel::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CBarrel::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CBarrel::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcAlphaTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcColTex"));
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

void CBarrel::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CBarrel::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CBarrel::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CBarrel * CBarrel::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed, const _bool& bBoom)
{
	CBarrel* pInstance = new CBarrel(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pPos, fScale, fMaxFrame, fFrameSpeed, bBoom)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBarrel::Free()
{
	CGameObject::Free();
}

