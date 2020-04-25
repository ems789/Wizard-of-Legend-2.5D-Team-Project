#include "stdafx.h"
#include "LightningFalling.h"

#include "Export_Function.h"
#include "BasicEffect.h"

CLightningFalling::CLightningFalling(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}


CLightningFalling::~CLightningFalling()
{
}

HRESULT CLightningFalling::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 0.8f;

	return S_OK;
}

_int CLightningFalling::Update_GameObject(const _float & fTimeDelta)
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

void CLightningFalling::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CLightningFalling::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CLightningFalling::Add_Effect(const _vec3 * pPos)
{
	CBasicEffect* pLightningFist = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningFist", L"", 6.f, 10.f, 0.02f, pPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningFist", pLightningFist);	
	Engine::PlaySound_(L"BallLightningLoop.wav", CSoundMgr::EFFECT);
}

HRESULT CLightningFalling::Add_Component()
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

void CLightningFalling::LifeTimer(const _float & fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
		m_bIsDead = true;
}

void CLightningFalling::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CLightningFalling * CLightningFalling::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vInitialPos, const _float& fLifeTime)
{
	CLightningFalling*	pInstance = new CLightningFalling(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	_vec3 vRand{ float(rand() % 10 - 5), 0.f, float(rand() % 10 - 5) };

	pInstance->m_vInitialPos = vInitialPos + vRand;
	pInstance->m_pTransformCom->Set_Pos(pInstance->m_vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fLifeTime = fLifeTime;

	CBasicEffect* pLightningBase = CBasicEffect::Create(pGraphicDev, L"Texture_LightningBase", L"", 5.f, 10.f, 0.1f, &pInstance->m_vInitialPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningBase", pLightningBase);
	Engine::PlaySound_(L"BallLightningEnd.wav", CSoundMgr::EFFECT);

	return pInstance;
}

void CLightningFalling::Free()
{
	CGameObject::Free();
}
