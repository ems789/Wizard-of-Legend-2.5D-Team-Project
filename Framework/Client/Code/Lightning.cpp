#include "stdafx.h"
#include "Lightning.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "LightningFist.h"
#include "RotatedBillEffect.h"

CLightning::CLightning(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CLightning::~CLightning()
{

}

HRESULT CLightning::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = m_fRadius;

	return S_OK;
}

_int CLightning::Update_GameObject(const _float& fTimeDelta)
{
	m_fGenTime += fTimeDelta;

	if (m_fGenTime > 0.2f && !m_bIsPlaySound && m_bIsUpgrade)
	{
		m_bIsPlaySound = true;
		Engine::PlaySound_(L"LightningShuriken.wav", CSoundMgr::EFFECT);
	}

	if (m_fGenTime < 0.3f)
		return 0;

	Turn_To_Camera_Look();

	if (m_bIsDead)
		return 0;
		
	if (!m_bSpawn)
	{
		if (m_bIsUpgrade)
		{			
			CLaidEffect* pWindShuriken = CLaidEffect::Create(m_pGraphicDev, L"Texture_WindShuriken", L"", 3.f, 60.f, 0.1f, &m_pTransformCom->GetInfoRef(Engine::INFO_POS), 0.f, true, 3.f);
			Engine::Add_GameObject(L"GameLogic", L"Texture_WindShuriken", pWindShuriken);
		}
		CBasicEffect* pLightningStageEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningStageEffect", L"", 16.f, 20.f, 0.04f, &m_pTransformCom->GetInfoRef(Engine::INFO_POS), true, 3.f);
		Engine::Add_GameObject(L"Effect", L"Texture_LightningStageEffect2", pLightningStageEffect);
		m_bSpawn = true;
	}

	// 시간 지나면 삭제
	m_fCurTime += fTimeDelta;

	if (m_fCurTime > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	m_fSoundTime += fTimeDelta;
	if (m_fSoundTime >= m_fAttckDelay)
	{
		m_fSoundTime = 0.f;
		Engine::PlaySound_(L"LightningPinball.wav", CSoundMgr::EFFECT);
		CRotatedBillEffect* pEffect = CRotatedBillEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &m_pTransformCom->GetInfoRef(Engine::INFO_POS), false, 0.f, D3DXToRadian(rand() % 367));
		NULL_CHECK_RETURN(pEffect, -1);
		CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, this, pEffect, m_tSphere.fRadius, L"Player_Bullet", 15, m_fAttckDelay);
		Engine::Add_GameObject(L"GameLogic", L"PlayerCollider", pCollider);
		pCollider->Add_Hit_Effect(CLightningFist::LightningFist_Hit_Func);
	}

	Animation(fTimeDelta);

	_vec3 vDistance = m_pTransformCom->GetInfoRef(Engine::INFO_POS) - m_vInitialPos;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	//Engine::Add_GameObject_To_CollisionList(m_pCollisionTag, this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CLightning::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CLightning::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CLightning::Add_Effect(const _vec3* pPos)
{
	CBasicEffect* pLightningBossTeleportAir = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningFist", L"", 6.f, 10.f, 0.03f, pPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningFist", pLightningBossTeleportAir);
	Engine::PlaySound_(L"BallLightningLoop.wav", CSoundMgr::EFFECT);
}

HRESULT CLightning::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningFistAttackLarge"));
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

void CLightning::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CLightning::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CLightning* CLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _float& fMaxFrame, const _float& fFrameSpeed,const _float& fLifeTime, const _float& fRadius /*= 1.f*/, const _float& fAttackDelay /*= 0.5f*/, bool bIsUpgrade /*= false*/, const _tchar* pCollisionTag /*= L"Player_Bullet"*/)
{
	CLightning*	pInstance = new CLightning(pGraphicDev);

	pInstance->m_fRadius = fRadius;

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_vDir = vDir;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_tFrame.fCurFrame = 0;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_pCollisionTag = pCollisionTag;
	pInstance->m_fAttckDelay = fAttackDelay;
	pInstance->m_bIsUpgrade = bIsUpgrade;

	pInstance->m_pTransformCom->Set_Scale(1.5f, 1.5f, 1.5f);

	//if(pInstance->m_bIsUpgrade)
		//Engine::PlaySound_(L"LightningShuriken.wav", CSoundMgr::EFFECT);

	return pInstance;
}

void CLightning::Free()
{

	CGameObject::Free();
}

