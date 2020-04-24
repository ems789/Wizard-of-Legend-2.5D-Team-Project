#include "stdafx.h"
#include "Aqua.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "SphereCollider.h"

CAqua::CAqua(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CAqua::~CAqua()
{

}

HRESULT CAqua::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CAqua::Update_GameObject(const _float& fTimeDelta)
{
	//Turn_To_Camera_Look();

	if (m_bIsDead)
		return 0;

	Animation(fTimeDelta);

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);



	return 0;
}

void CAqua::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3* CAqua::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CAqua::Add_Effect(const _vec3* pPos)
{
	_vec3 vPos = (*m_pTransformCom->GetInfo(Engine::INFO_POS) + *pPos) / 2;
	vPos.y += 0.5f;

	CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WaterExplosion", L"WaterExplosion", 7.f, 20.f, 0.05f, &vPos, false, 0.f);

	Engine::Add_GameObject(L"GameLogic", L"WaterExplosion", pEffect);
}

HRESULT CAqua::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_AquaVortex"));
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

void CAqua::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_bIsDead = true;

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		vPos.y += 0.1f;
		CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WaterExplosion", L"WaterExplosion", 7.f, 15.f, 0.1f, &vPos, false, 0.f);

		Engine::Add_GameObject(L"GameLogic", L"WaterExplosion", pEffect);

		CSphereCollider* WaterSphere = CSphereCollider::Create(m_pGraphicDev, pEffect, nullptr, 2.f, L"Player_Bullet", 70);

		Engine::Add_GameObject(L"GameLogic", L"Player_Bullet", WaterSphere);

	}
}

void CAqua::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CAqua::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CAqua* CAqua::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vAngle, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed, const _float& fScale)
{

	CAqua*	pInstance = new CAqua(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_pTransformCom->Set_Angle(vAngle);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_tFrame.fCurFrame = 0;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale = fScale;
	pInstance->Update_Scale();

	return pInstance;
}

void CAqua::Free()
{
	CGameObject::Free();
}

