#include "stdafx.h"
#include "WindBall.h"

#include "Export_Function.h"
#include "BasicEffect.h"

CWindBall::CWindBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CWindBall::~CWindBall()
{

}

HRESULT CWindBall::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 2.f;
	//m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

	return S_OK;
}

_int CWindBall::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	if (m_bIsDead)
		return 0;


	Animation(fTimeDelta);

	m_vDir = m_vTargetPos - m_vInitialPos;
	_float fDist = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pTransformCom->Move_Pos(m_vDir * m_fSpeed * fTimeDelta);

	if (fDist > 50.f)
	{
		m_bIsDead = true;
		return 0;
	}

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"MonsterAttack", this);
	
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;

}

void CWindBall::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_int>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3* CWindBall::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CWindBall::Add_Effect(const _vec3* pPos)
{

}

HRESULT CWindBall::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBall"));
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

void CWindBall::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CWindBall::Go()
{
	m_bGo = true;
}



void CWindBall::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	//vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CWindBall * CWindBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vInitialPos, const _vec3& vTargetPos, const _vec3 & vDir, const _float & fSpeed, const _float & fMaxFrame, const _float & fFrameSpeed)
{
	CWindBall* pInstance = new CWindBall(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_vTargetPos = vTargetPos;
	pInstance->m_tFrame.fCurFrame = 0;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;


	return pInstance;
}

void CWindBall::Free()
{
	CGameObject::Free();
}

