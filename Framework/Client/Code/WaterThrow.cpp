#include "stdafx.h"
#include "WaterThrow.h"
#include "Shark.h"
#include "Export_Function.h"
#include "BasicEffect.h"


CWaterThrow::CWaterThrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CWaterThrow::~CWaterThrow()
{
}

HRESULT CWaterThrow::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fBent = 5.f;

	return S_OK;
}

_int CWaterThrow::Update_GameObject(const _float & fTimeDelta)
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
	
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"PlayerAttack", this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	_vec3 vAngle = {0.f, 0.f, 0.f};
	vAngle.x += D3DXToRadian(90.f);

	_vec3 vWaterPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);
	vWaterPos.y = 0.1f;
	if (m_pTransformCom->GetInfoRef(Engine::INFO_POS).y < 0.f)
	{
		m_bIsDead = true;
		CShark* pShark = CShark::Create(m_pGraphicDev, vWaterPos, vAngle, 20.f, 8.f, 5.f, 0.1f);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"SharkPool", pShark), -1);
	}

	return 0;
}

void CWaterThrow::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3 * CWaterThrow::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CWaterThrow::Add_Effect(const _vec3 * pPos)
{

}

void CWaterThrow::Go()
{
}

HRESULT CWaterThrow::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WaterThrow"));
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

void CWaterThrow::CalculateBent(const _float & fBentDecel)
{
	m_vDir = m_vTargetPos - m_vInitialPos;
	_float fDist = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_float fTime = fDist / m_fSpeed;

	m_fBentDecel = fBentDecel;

	m_fBent = 2.f * fBentDecel / fTime;
}

void CWaterThrow::Move_Update(const _float & fTimeDelta)
{
	_vec3 vDir = m_vDir + m_fBentDecel * m_vBentDir;

	m_fBentDecel -= m_fBent * fTimeDelta;

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	m_pTransformCom->Rotation(Engine::ROT_Z, fTimeDelta * m_fSpeed);
}

void CWaterThrow::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CWaterThrow * CWaterThrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vTargetPos, const _vec3& vBentDir, const _float& fBentDecel, const _float& fSpeed)
{
	CWaterThrow* pInstance = new CWaterThrow(pGraphicDev);

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

void CWaterThrow::Free()
{
	CGameObject::Free();
}



