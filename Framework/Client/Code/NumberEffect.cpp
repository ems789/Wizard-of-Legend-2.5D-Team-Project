#include "stdafx.h"
#include "NumberEffect.h"

#include "Export_Function.h"
#include "NumberFont.h"

CNumberEffect::CNumberEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CNumberEffect::~CNumberEffect()
{

}

_int CNumberEffect::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	//Update_Scale();
	Move_Update(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CNumberEffect::Render_GameObject()
{
	CNumberFont::GetInstance()->DrawNumberBill(m_iNumber, m_pTransformCom->GetInfo(Engine::INFO_POS), m_fScale, m_d3dColor);

}

HRESULT CNumberEffect::Ready_GameObject(const D3DXCOLOR& d3dColor)
{
	FAILED_CHECK_RETURN(Add_Component(d3dColor), E_FAIL);

	return S_OK;
}

HRESULT CNumberEffect::Add_Component(const D3DXCOLOR & d3dColor)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	m_d3dColor = d3dColor;

	return S_OK;
}

void CNumberEffect::LifeTimer(const _float & fTimeDelta)
{
	if (m_fLifeTime <= 0.f)
		return;

	m_fCumulativeTime += fTimeDelta;

	if (m_fCumulativeTime >= m_fLifeTime)
		m_bIsDead = true;
}

void CNumberEffect::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);

}

void CNumberEffect::Move_Update(const _float & fTimeDelta)
{
	if (m_pTransformCom->GetInfo(Engine::INFO_POS)->y - m_fScale * 10.f < 0.f)
	{
		m_fDownSpeed = 0.f;
		m_fUpForce *= 0.6f;
		m_fSpeed *= 0.6f;
		m_pTransformCom->Set_PosY(m_fScale* 10.f);

	}

	_vec3 vMove = _vec3(0.f, 1.f, 0.f) * fTimeDelta * m_fSpeed;

	m_fDownSpeed += m_fGravity * fTimeDelta;
	vMove.y += (m_fUpForce - m_fDownSpeed) * fTimeDelta;

	m_pTransformCom->Move_Pos(vMove);


}

CNumberEffect* CNumberEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fScale, const _vec3* pPos, _float fLifeTime, const D3DXCOLOR& d3dColor, const _int& iNumber)
{
	CNumberEffect* pInstance = new CNumberEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(d3dColor)))
		Engine::Safe_Release(pInstance);

	pInstance->m_fScale = fScale;
	pInstance->m_pTransformCom->Set_Pos(pPos);
	//pInstance->m_pTransformCom->Set_Angle(pAngle);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_d3dColor = d3dColor;

	pInstance->m_fLifeTime = fLifeTime;
	

	pInstance->m_iNumber = iNumber;

	return pInstance;
}

void CNumberEffect::Free()
{
	CEffect::Free();
}

