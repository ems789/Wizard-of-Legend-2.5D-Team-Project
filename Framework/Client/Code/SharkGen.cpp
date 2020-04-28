#include "stdafx.h"
#include "SharkGen.h"
#include "WaterThrow.h"
#include "Export_Function.h"
#include "SharkPool.h"
#include "BasicEffect.h"
#include "FireEffect.h"

CSharkGen::CSharkGen(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSharkGen::~CSharkGen()
{

}

HRESULT CSharkGen::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 3.f;

	m_fBent = 2.f;
}

_int CSharkGen::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();


	return 0;
}

const _vec3* CSharkGen::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CSharkGen::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CSharkGen::Generate_Shark(const _float& fTimeDelta)
{
	
}

void CSharkGen::CalculateBent(const _float& fBentDecel)
{

}

void CSharkGen::Move_Update(const _float& fTimeDelta)
{

}

void CSharkGen::Turn_To_Camera_Look()
{
}

CSharkGen* CSharkGen::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vInitialPos, const _vec3& vTargetPos, const _float& fSpeed, const _uint& uiSharkMax)
{
	CSharkGen* pInstance = new CSharkGen(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	//pInstance->m_pTransformCom->

	return pInstance;
}

