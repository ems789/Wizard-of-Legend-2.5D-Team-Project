#include "stdafx.h"
#include "WindAgentSwirl.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "BasicFollowingEffect.h"

CWindAgentSwirl::CWindAgentSwirl(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}


CWindAgentSwirl::~CWindAgentSwirl()
{
}

HRESULT CWindAgentSwirl::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 0.5f;

	return S_OK;
}

_int CWindAgentSwirl::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Move_Update(fTimeDelta);

	_vec3 vDistance = m_pTransformCom->GetInfoRef(Engine::INFO_POS) - m_vInitialPos;
	_float fDist = D3DXVec3Length(&vDistance);
	if (fDist > 50.f)
	{
		m_bIsDead = true;
		return 0;
	}

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	Turn_To_Camera_Look();

	Engine::Add_GameObject_To_CollisionList(L"MonsterAttack", this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	
	return 0;
}

void CWindAgentSwirl::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	//m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CWindAgentSwirl::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CWindAgentSwirl::Add_Effect(const _vec3 * pPos)
{
	CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"", 7.f, 20.f, 0.05f, pPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_SlashHitSpark", pEffect);
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

HRESULT CWindAgentSwirl::Add_Component()
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

void CWindAgentSwirl::Move_Update(const _float & fTimeDelta)
{	
	m_pTransformCom->Move_Pos(m_vTargetDir * fTimeDelta * m_fSpeed);
}

void CWindAgentSwirl::Turn_To_Camera_Look()
{
	_matrix			matBill, matWorld, matView;
	D3DXMatrixIdentity(&matBill);

	m_pTransformCom->GetWorldMatrix(&matWorld);
	Engine::Get_MainCamera()->Get_View(&matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->Set_WorldMatrix(matBill * matWorld);
	//vAngle.x = 0.f;


}

CWindAgentSwirl * CWindAgentSwirl::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vInitialPos, const _vec3 & vTargetDir, const _float & fSpeed)
{
	CWindAgentSwirl*	pInstance = new CWindAgentSwirl(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_vTargetDir = vTargetDir;	

	_vec3 vPos = pInstance->m_vInitialPos;
	D3DXVec3Normalize(&vPos, &vPos);

	CBasicFollowingEffect* pThunderJavelinSpawn = CBasicFollowingEffect::Create(pGraphicDev, L"Texture_WindAgentSwirl", L"", 5.f, 10.f, 0.04f, &_vec3(0.f, 0.f, 0.f), pInstance->m_pTransformCom->GetInfo(Engine::INFO_POS), true, 5.f);
	Engine::Add_GameObject(L"Effect", L"Texture_WindAgentSwirl", pThunderJavelinSpawn);

	return pInstance;
}

void CWindAgentSwirl::Free()
{
	CGameObject::Free();
}
