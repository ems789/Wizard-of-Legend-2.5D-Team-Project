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

	m_fSpeed = 10.f;
	m_tSphere.fRadius = 1.f;
	m_eCurState = GFS_ORBIT;
	m_fAngle = 0.f;
	m_fOrbitSpeed = 180.f;

	return S_OK;
}

_int CGuidedFire::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	switch (m_eCurState)
	{
	case CGuidedFire::GFS_ORBIT:
		Orbit_Target(fTimeDelta);

		if (m_fLifeCount > 0.5f)
			SearchTarget();

		break;
	case CGuidedFire::GFS_FIRE:

		m_pTransformCom->Move_Pos(m_vDir * m_fSpeed * fTimeDelta);

		break;
	}

	m_fTailDelay += fTimeDelta;
	if (m_fTailDelay > 0.05f)
	{
		m_fTailDelay -= 0.05f;
		GuidedFireTail();
	}

	if (m_bIsDead)
		return 0;
	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_GameObject_To_CollisionList(m_pCollisionTag, this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CGuidedFire::Render_GameObject()
{
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

		Engine::Add_GameObject(L"Effect", L"GuidedFireExplosion", pEffect);
	}
	Engine::PlaySound_(L"ImpactFire.wav", CSoundMgr::EFFECT);
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

void CGuidedFire::Orbit_Target(const _float & fTimeDelta)
{
	_vec3 vTargetPos = *m_pRevTarget->GetInfo(Engine::INFO_POS);

	_matrix matRotAxis;

	m_fAngle += fTimeDelta * D3DXToRadian(m_fOrbitSpeed);

	D3DXMatrixRotationAxis(&matRotAxis, &m_vAxis, m_fAngle);

	_vec3 vMyPos;
	D3DXVec3TransformNormal(&vMyPos, &m_vInitialPos, &matRotAxis);
	vMyPos = vMyPos * m_fRevRadius + vTargetPos;

	m_pTransformCom->Set_Pos(vMyPos);
}

void CGuidedFire::SearchTarget()
{
	const Engine::CLayer* pLayer = Engine::Get_Layer(L"Monster");

	if (nullptr == pLayer)
		return;

	const multimap<const _tchar*, Engine::CGameObject*>* pmapObject = pLayer->Get_LayerMap();
	if (nullptr == pmapObject)
		return;

	for (auto& iter : *pmapObject)
	{
		_vec3 vDist = *iter.second->Get_Pos() - *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_float fDist = D3DXVec3Length(&vDist);

		if (fDist < m_fDetectDist)
		{
			D3DXVec3Normalize(&m_vDir, &vDist);
			m_eCurState = GFS_FIRE;
			break;
		}
		
	}
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
		D3DXVec3Normalize(&vDir, &vDir);

		CFireEffect* pEffect = CFireEffect::Create(m_pGraphicDev, pTextureTag, L"FireParticle", 6.f, 15.f, 10.f * m_fSize, &vCreatePos, &vDir, 1.f,
			false, 0.f, D3DXCOLOR(1.f, 0.7f, 0.5f, 1.f), D3DXCOLOR(0.f, 1.f, 2.0f, 0.f));

		Engine::Add_GameObject(L"Effect", L"GuidedFire", pEffect);
	}
}

void CGuidedFire::Setting_InitialPos(const _float& fTiltAngle)
{
	_matrix matRotY;

	D3DXMatrixRotationY(&matRotY, fTiltAngle);
	D3DXVec3TransformNormal(&m_vAxis, &AXIS_X, &matRotY);

	D3DXVec3Cross(&m_vInitialPos, &AXIS_Y, &m_vAxis);
	D3DXVec3Normalize(&m_vInitialPos, &m_vInitialPos);
	_vec3 vPos = *m_pRevTarget->GetInfo(Engine::INFO_POS) + m_vInitialPos * m_fRevRadius;
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Update_Component(0.f);
}

void CGuidedFire::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CGuidedFire* CGuidedFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fTiltAngle, const _float& fLifeTime, const Engine::CTransform* pRevTarget, const _float& fRevRadius,
	const _float fDetectDist, const _uint& uiFireCnt, const _int& iAttack, const _float& fSize, const _tchar* pCollisionTag /*= L"MonsterAttack"*/, const _float& fRadius/* = 1.f*/)
{
	CGuidedFire*	pInstance = new CGuidedFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_pRevTarget = pRevTarget;
	pInstance->m_fRevRadius = fRevRadius;
	pInstance->m_fDetectDist = fDetectDist;
	pInstance->m_uiFireCnt = uiFireCnt;
	pInstance->m_iAttack = iAttack;
	pInstance->m_fSize = fSize;
	pInstance->m_pCollisionTag = pCollisionTag;
	pInstance->m_tSphere.fRadius = fRadius;
	pInstance->Setting_InitialPos(fTiltAngle);

	return pInstance;
}

void CGuidedFire::Free()
{
	CGameObject::Free();
}

