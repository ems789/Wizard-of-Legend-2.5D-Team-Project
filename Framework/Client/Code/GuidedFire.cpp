#include "stdafx.h"
#include "GuidedFire.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "FireEffect.h"

CGuidedFire::CGuidedFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CGuidedFire::~CGuidedFire()
{

}

HRESULT CGuidedFire::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CGuidedFire::Update_GameObject(const _float& fTimeDelta)
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
		GuidedFireTail();
	}

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(m_pCollisionTag, this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CGuidedFire::Render_GameObject()
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	//m_pTextureCom->Render_Texture();
	//m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CGuidedFire::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CGuidedFire::Add_Effect(const _vec3* pPos)
{
	_vec3 vPos = (*m_pTransformCom->GetInfo(Engine::INFO_POS) + *pPos) / 2;
	vPos.y += 0.5f;

	for (_uint i = 0; i < 4; ++i)
	{
		const _tchar* pTextureTag = nullptr;
		switch (rand() % 4)
		{
		case 0:
			pTextureTag = L"Texture_FireParticle1";
			break;
		case 1:
			pTextureTag = L"Texture_FireParticle2";
			break;
		case 2:
			pTextureTag = L"Texture_FireParticle3";
			break;
		case 3:
			pTextureTag = L"Texture_FireParticle4";
			break;
		default:
			break;
		}

		_vec3 vDir = { (rand() % 100 - 50.f) * m_fSize, (rand() % 100 - 50.f) * m_fSize, (rand() % 100 - 50.f) * m_fSize };
		_vec3 vCreatePos = vPos + vDir;
		D3DXVec3Normalize(&vDir, &vDir);

		CFireEffect* pEffect = CFireEffect::Create(m_pGraphicDev, pTextureTag, L"GuidedFireParticle", 6.f, 15.f, 20.f * m_fSize, &vCreatePos, &vDir, 1.f,
			false, 0.f, D3DXCOLOR(1.f, 0.7f, 0.5f, 1.f), D3DXCOLOR(0.f, 1.f, 2.0f, 0.f));

		Engine::Add_GameObject(L"GameLogic", L"GuidedFireExplosion", pEffect);
	}
}

HRESULT CGuidedFire::Add_Component()
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

void CGuidedFire::LifeTimer(const _float & fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
		m_bIsDead = true;
}

void CGuidedFire::GuidedFireTail()
{
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	for (_uint i = 0; i < m_uiFireCnt; ++i)
	{
		const _tchar* pTextureTag = nullptr;
		switch (rand() % 4)
		{
		case 0:
			pTextureTag = L"Texture_FireParticle1";
			break;
		case 1:
			pTextureTag = L"Texture_FireParticle2";
			break;
		case 2:
			pTextureTag = L"Texture_FireParticle3";
			break;
		case 3:
			pTextureTag = L"Texture_FireParticle4";
			break;
		default:
			break;
		}

		_vec3 vDir = { (rand() % 100 - 50.f) * m_fSize, (rand() % 100 - 50.f) * m_fSize, (rand() % 100 - 50.f) * m_fSize };
		_vec3 vCreatePos = vPos + vDir;
		vCreatePos.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		CFireEffect* pEffect = CFireEffect::Create(m_pGraphicDev, pTextureTag, L"FireParticle", 6.f, 15.f, 10.f * m_fSize, &vCreatePos, &vDir, 1.f,
			false, 0.f, D3DXCOLOR(1.f, 0.7f, 0.5f, 1.f), D3DXCOLOR(0.f, 1.f, 2.0f, 0.f));

		Engine::Add_GameObject(L"GameLogic", L"GuidedFire", pEffect);
	}
}

void CGuidedFire::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CGuidedFire* CGuidedFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _float& fLifeTime, const Engine::CTransform* pRevTarget, 
	const _uint& uiFireCnt, const _int& iAttack, const _float& fSize, const _tchar* pCollisionTag /*= L"MonsterAttack"*/, const _float& fRadius/* = 1.f*/)
{
	CGuidedFire*	pInstance = new CGuidedFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_pRevTarget = pRevTarget;
	pInstance->m_uiFireCnt = uiFireCnt;
	pInstance->m_iAttack = iAttack;
	pInstance->m_fSize = fSize;
	pInstance->m_pCollisionTag = pCollisionTag;
	pInstance->m_tSphere.fRadius = fRadius;

	return pInstance;
}

void CGuidedFire::Free()
{
	CGameObject::Free();
}

