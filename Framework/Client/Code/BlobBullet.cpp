#include "stdafx.h"
#include "BlobBullet.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "FireEffect.h"

CBlobBullet::CBlobBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBlobBullet::~CBlobBullet()
{

}

HRESULT CBlobBullet::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fBent = 5.f;

	m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);

	return S_OK;
}

_int CBlobBullet::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

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

	if (m_pTransformCom->GetInfo(Engine::INFO_POS)->y < 0.f)
	{
		m_bIsDead = true;
		m_pTransformCom->Set_PosY(0.f);
		Add_Effect(m_pTransformCom->GetInfo(Engine::INFO_POS));
		return 0;
	}

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"MonsterAttack", this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CBlobBullet::Render_GameObject()
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CBlobBullet::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CBlobBullet::Add_Effect(const _vec3* pPos)
{
	_vec3 vPos = (*m_pTransformCom->GetInfo(Engine::INFO_POS) + *pPos) / 2;
	vPos.y += 0.5f;

	CBasicEffect* pBasicEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_BlobDead", L"", 5, 10.f, 0.05f, &vPos, false, 0.f);
	Engine::Add_GameObject(L"Effect", L"BlobDeadEffect", pBasicEffect);

	Engine::PlaySound_(L"WaterExplode.wav", CSoundMgr::EFFECT);
}

HRESULT CBlobBullet::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Blob"));
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

void CBlobBullet::CalculateBent(const _float& fBentDecel)
{
	m_vDir = m_vTargetPos - m_vInitialPos;
	_float fDist = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_float fTime = fDist / m_fSpeed;

	m_fBentDecel = fBentDecel;

	m_fBent = 2.f * fBentDecel / fTime;
}

void CBlobBullet::Move_Update(const _float & fTimeDelta)
{
	_vec3 vDir = m_vDir + m_fBentDecel * m_vBentDir;

	m_fBentDecel -= m_fBent * fTimeDelta;

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	m_pTransformCom->Rotation(Engine::ROT_Z, fTimeDelta * m_fSpeed);
}

void CBlobBullet::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CBlobBullet* CBlobBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vTargetPos, const _vec3& vBentDir, const _float& fBentDecel, const _float& fSpeed)
{
	CBlobBullet*	pInstance = new CBlobBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_vTargetPos = vTargetPos;
	D3DXVec3Normalize(&pInstance->m_vBentDir, &vBentDir);
	pInstance->CalculateBent(fBentDecel);

	return pInstance;
}

void CBlobBullet::Free()
{

	CGameObject::Free();
}

