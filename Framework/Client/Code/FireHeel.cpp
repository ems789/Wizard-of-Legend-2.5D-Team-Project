#include "stdafx.h"
#include "FireHeel.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "FireEffect.h"
#include "FireKick.h"
#include "LaidEffect.h"

CFireHeel::CFireHeel(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFireHeel::~CFireHeel()
{

}

HRESULT CFireHeel::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CFireHeel::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_vec3 vDistance = m_pTransformCom->GetInfoRef(Engine::INFO_POS) - m_vInitialPos;
	_float fDist = D3DXVec3Length(&vDistance);
	if (fDist > 50.f)
	{
		m_bIsDead = true;
		return 0;
	}

	m_fTailDelay += fTimeDelta;
	if (m_fTailDelay > 0.05f)
	{
		m_fTailDelay -= 0.05f;
		FireHeelTail();
	}

	m_pTransformCom->Move_Pos(m_vDir * m_fSpeed * fTimeDelta);

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	//Engine::Add_GameObject_To_CollisionList(L"MonsterAttack", this);
	//m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CFireHeel::Render_GameObject()
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	//m_pTextureCom->Render_Texture();
	//m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CFireHeel::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CFireHeel::Add_Effect(const _vec3* pPos)
{

}

HRESULT CFireHeel::Add_Component()
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

void CFireHeel::LifeTimer(const _float & fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
		m_bIsDead = true;
}

void CFireHeel::FireHeelTail()
{
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	vPos.y = 0.1f;
	CLaidEffect* pCrack = CLaidEffect::Create(m_pGraphicDev, L"Texture_FloorCrack", L"FloorCrack", 1.f, 0.f, 0.05f, &vPos, 0.f, true, 0.05f);
	Engine::Add_GameObject(L"Effect", L"FloorCrack", pCrack);

	CFireKick* pFireKick = CFireKick::Create(m_pGraphicDev, vPos, 0.1f);
	Engine::Add_GameObject(L"GameLogic", L"FireKick", pFireKick);


}

void CFireHeel::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CFireHeel* CFireHeel::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _float& fSpeed, const _float& fLifeTime)
{
	CFireHeel*	pInstance = new CFireHeel(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_vDir = vDir;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_fSpeed = fSpeed;

	return pInstance;
}

void CFireHeel::Free()
{
	CGameObject::Free();
}

