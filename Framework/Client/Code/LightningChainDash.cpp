#include "stdafx.h"
#include "LightningChainDash.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "LaidEffect.h"

CLightningChainDash::CLightningChainDash(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}


CLightningChainDash::~CLightningChainDash()
{
}

HRESULT CLightningChainDash::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 0.8f;

	return S_OK;
}

_int CLightningChainDash::Update_GameObject(const _float & fTimeDelta)
{
	LifeTimer(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	Turn_To_Camera_Look();

	Engine::Add_GameObject_To_CollisionList(L"MonsterAttack", this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	
	return 0;
}

void CLightningChainDash::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CLightningChainDash::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CLightningChainDash::Add_Effect(const _vec3 * pPos)
{
	CBasicEffect* pLightningFist = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningFist", L"", 6.f, 10.f, 0.02f, pPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningFist", pLightningFist);
	Engine::PlaySound_(L"BallLightningLoop.wav", CSoundMgr::EFFECT);
}

HRESULT CLightningChainDash::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CLightningChainDash::LifeTimer(const _float & fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
		m_bIsDead = true;
}

void CLightningChainDash::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CLightningChainDash * CLightningChainDash::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vInitialPos, _float fAngle, const _float& fLifeTime)
{
	CLightningChainDash*	pInstance = new CLightningChainDash(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(pInstance->m_vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fLifeTime = fLifeTime;

	CLaidEffect* pLightningTip1 = CLaidEffect::Create(pGraphicDev, L"Texture_LightningTip", L"", rand() % 4 + 1, 10.f, 0.05f, &pInstance->m_vInitialPos, fAngle, true, 1.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningTip1", pLightningTip1);
	CLaidEffect* pLightningTip2 = CLaidEffect::Create(pGraphicDev, L"Texture_LightningTip", L"", rand() % 4 + 1, 10.f, 0.05f, &pInstance->m_vInitialPos, fAngle, true, 1.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningTip2", pLightningTip2);
	CLaidEffect* pLightningTip3 = CLaidEffect::Create(pGraphicDev, L"Texture_LightningTip", L"", rand() % 4 + 1, 10.f, 0.05f, &pInstance->m_vInitialPos, fAngle, true, 1.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningTip3", pLightningTip3);
	CLaidEffect* pLightningTip4 = CLaidEffect::Create(pGraphicDev, L"Texture_LightningTip", L"", rand() % 4 + 1, 10.f, 0.05f, &pInstance->m_vInitialPos, fAngle, true, 1.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningTip4", pLightningTip4);

	return pInstance;
}

void CLightningChainDash::Free()
{
	CGameObject::Free();
}
