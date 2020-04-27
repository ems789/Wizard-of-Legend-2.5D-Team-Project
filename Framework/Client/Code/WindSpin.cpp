#include "stdafx.h"
#include "WindSpin.h"

#include "Export_Function.h"
#include "BasicEffect.h"

CWindSpin::CWindSpin(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CWindSpin::~CWindSpin()
{

}

HRESULT CWindSpin::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CWindSpin::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_GameObject_To_CollisionList(m_pCollisionTag, this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CWindSpin::Render_GameObject()
{

}

const _vec3* CWindSpin::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CWindSpin::Add_Effect(const _vec3* pPos)
{
	_vec3 vPos = (*m_pTransformCom->GetInfo(Engine::INFO_POS) + *pPos) / 2;
	vPos.y += 0.5f;

	////basiceffect√ﬂ∞°
	//CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindSpin", L"WindSpin", 6.f, 20.f, 0.05f, &vPos, true, 2.f);
	//Engine::Add_GameObject(L"Effect", L"WindSpin", pEffect);
	////6¿Â
}

HRESULT CWindSpin::Add_Component()
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

void CWindSpin::LifeTimer(const _float& fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
		m_bIsDead = true;
}

void CWindSpin::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CWindSpin* CWindSpin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _float& fLifeTime, const _int& iAttack /*= 10*/, const _float& fSize /*= 0.01f*/, const _tchar* pCollisionTag /*= L"MonsterAttack"*/, const _float& fRadius /*= 1.f*/)
{
	CWindSpin*	pInstance = new CWindSpin(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_iAttack = iAttack;
	pInstance->m_fSize = fSize;
	pInstance->m_pCollisionTag = pCollisionTag;
	pInstance->m_tSphere.fRadius = fRadius;

	return pInstance;
}

void CWindSpin::Free()
{
	CGameObject::Free();
}

