#include "stdafx.h"
#include "LightningSpear.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "RotatedBillEffect.h"

CLightningSpear::CLightningSpear(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CLightningSpear::~CLightningSpear()
{

}

HRESULT CLightningSpear::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CLightningSpear::Update_GameObject(const _float& fTimeDelta)
{
	//Turn_To_Camera_Look();

	if (m_bIsDead)
		return 0;

	Animation(fTimeDelta);

	Update_Scale();

	_vec3 vDir = *m_pTransformCom->GetInfo(Engine::INFO_UP);
	D3DXVec3Normalize(&vDir, &vDir);

	if (m_bGo)
		m_pTransformCom->Move_Pos(vDir * m_fSpeed * fTimeDelta);

	_vec3 vDistance = m_pTransformCom->GetInfoRef(Engine::INFO_POS) - m_vInitialPos;
	_float fDist = D3DXVec3Length(&vDistance);
	if (fDist > 50.f)
	{
		m_bIsDead = true;
		return 0;
	}

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	if (m_bGo)
		Engine::Add_GameObject_To_CollisionList(m_pCollisionTag, this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CLightningSpear::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CLightningSpear::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CLightningSpear::Add_Effect(const _vec3* pPos)
{
	CRotatedBillEffect* pEffect = CRotatedBillEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, pPos, false, 0.f, D3DXToRadian(rand() % 367));
	Engine::Add_GameObject(L"GameLogic", L"SlashHitSpark", pEffect);
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

void CLightningSpear::Go()
{
	m_bGo = true;
}

void CLightningSpear::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

HRESULT CLightningSpear::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningSpear"));
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

void CLightningSpear::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CLightningSpear::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CLightningSpear* CLightningSpear::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed, const _float& fAngle, const _tchar* pCollisionTag /*= L"Player_Bullet"*/, _bool bGo/* = true*/)
{
	CLightningSpear*	pInstance = new CLightningSpear(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_vDir = vDir;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos.x , vInitialPos.y, vInitialPos.z);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_tFrame.fCurFrame = 0;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_pCollisionTag = pCollisionTag;
	pInstance->m_bGo = bGo;

	float fAngleY = fAngle;
	if (vDir.z < 0.f)
		fAngleY += D3DXToRadian(180.f);
	pInstance->m_pTransformCom->Set_Angle(_vec3(D3DXToRadian(90.f), fAngleY, 0.f));

	pInstance->m_pTransformCom->Set_Scale(2.f, 2.f, 2.f);
	/*CBasicEffect* pLightningBase = CBasicEffect::Create(pGraphicDev, L"Texture_LightningBase", L"", 5.f, 10.f, 0.1f, &pInstance->m_vInitialPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningBase", pLightningBase);
	Engine::PlaySound_(L"BallLightningEnd.wav", CSoundMgr::EFFECT);*/

	return pInstance;
}

void CLightningSpear::Free()
{

	CGameObject::Free();
}

