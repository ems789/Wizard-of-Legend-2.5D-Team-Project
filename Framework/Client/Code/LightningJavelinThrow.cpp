#include "stdafx.h"
#include "LightningJavelinThrow.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "BasicFollowingEffect.h"

CLightningJavelinThrow::CLightningJavelinThrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}


CLightningJavelinThrow::~CLightningJavelinThrow()
{
}

HRESULT CLightningJavelinThrow::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 0.5f;

	return S_OK;
}

_int CLightningJavelinThrow::Update_GameObject(const _float & fTimeDelta)
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

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"MonsterAttack", this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	
	return 0;
}

void CLightningJavelinThrow::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CLightningJavelinThrow::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

/// TODO : 바닥 충돌 시 파티클 터짐
void CLightningJavelinThrow::Add_Effect(const _vec3 * pPos)
{
	CBasicEffect* pLightningBossTeleportAir = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningFist", L"", 6.f, 10.f, 0.02f, pPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningFist", pLightningBossTeleportAir);
	Engine::PlaySound_(L"BallLightningLoop.wav", CSoundMgr::EFFECT);
}

HRESULT CLightningJavelinThrow::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// 구체를 던지기 전에 손에 들고 있는 이펙트를 표현하기 위함
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_ThunderJavelin")); 
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

void CLightningJavelinThrow::Move_Update(const _float & fTimeDelta)
{	
	m_pTransformCom->Move_Pos(m_vTargetDir * fTimeDelta * m_fSpeed);
}

void CLightningJavelinThrow::Turn_To_Camera_Look()
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

CLightningJavelinThrow * CLightningJavelinThrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vInitialPos, const _vec3 & vTargetDir, const _float & fSpeed)
{
	CLightningJavelinThrow*	pInstance = new CLightningJavelinThrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_vTargetDir = vTargetDir;	

	_vec3 vPos = pInstance->m_vInitialPos;
	D3DXVec3Normalize(&vPos, &vPos);

	CBasicFollowingEffect* pThunderJavelinSpawn = CBasicFollowingEffect::Create(pGraphicDev, L"Texture_ThunderJavelin_Spawn", L"", 4.f, 10.f, 0.035f, &_vec3(0.f, 0.f, 0.f), pInstance->m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
	Engine::Add_GameObject(L"Effect", L"Texture_ThunderJavelin_Spawn", pThunderJavelinSpawn);

	CBasicFollowingEffect* pLightningStageEffect = CBasicFollowingEffect::Create(pGraphicDev, L"Texture_LightningStageEffect", L"", 16.f, 40.f, 0.035f, &_vec3(0.f, 0.f, 0.f), pInstance->m_pTransformCom->GetInfo(Engine::INFO_POS), true, 2.f);
	Engine::Add_GameObject(L"Effect", L"Texture_LightningStageEffect2", pLightningStageEffect);

	return pInstance;
}

void CLightningJavelinThrow::Free()
{
	CGameObject::Free();
}
