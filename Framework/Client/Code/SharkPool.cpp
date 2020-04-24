#include "stdafx.h"
#include "SharkPool.h"
#include "Shark.h"
#include "WaterThrow.h"
#include "Export_Function.h"
#include "SphereCollider.h"
#include "BasicEffect.h"

CSharkPool::CSharkPool(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CSharkPool::~CSharkPool()
{

}

HRESULT CSharkPool::Ready_Skill()
{
	m_fCoolTime = 0.f;

	Engine::Ready_Timer(L"SharkPool_CoolTime");
	Engine::Set_TimeDelta(L"SharkPool_CoolTime");

	m_fSpeed = 20.f;

	return S_OK;
}

_int CSharkPool::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"SharkPool_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}

	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"SharkPool_CoolTime");

	_vec3 vPos;

	_vec2 vMousePos = Engine::Get_MainCamera()->Get_MousePos();

	_matrix matProj, matView;
	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);

	D3DXPLANE tPlane = { 0.f, 1.f, 0.f, 0.f };

	Engine::CMyMath::PickingOnPlane(&vPos, &vMousePos, WINCX, WINCY, &matProj, &matView, &tPlane);

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vAngle = pTransform->GetAngleRef();
	vAngle.x += D3DXToRadian(90.f);

	vPos.y = 0.1f;

	// throw
	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	vStartPos += vLook;

	_vec3 vRight = pTransform->GetInfoRef(Engine::INFO_RIGHT);
	_vec3 vUp = pTransform->GetInfoRef(Engine::INFO_UP);
	_float fBentDecal = 1.5f;

	CWaterThrow* pWaterThrow = CWaterThrow::Create(m_pGraphicDev, vStartPos, vPos, vUp, fBentDecal, 20.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"WaterThrow", pWaterThrow), -1);
	
	m_fCurTime = m_fCoolTime;
	return 1;
}

_int CSharkPool::Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"SharkPool_CoolTime");

	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"SharkPool_CoolTime");

	_vec3 vPos;

	_vec2 vMousePos = Engine::Get_MainCamera()->Get_MousePos();

	_matrix matProj, matView;
	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);

	D3DXPLANE tPlane = { 0.f, 1.f, 0.f, 0.f };

	Engine::CMyMath::PickingOnPlane(&vPos, &vMousePos, WINCX, WINCY, &matProj, &matView, &tPlane);

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vAngle = pTransform->GetAngleRef();
	vAngle.x += D3DXToRadian(90.f);

	vPos.y = 0.1f;

	// throw
	_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
	_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	vStartPos += vLook;

	_vec3 vRight = pTransform->GetInfoRef(Engine::INFO_RIGHT);
	_vec3 vUp = pTransform->GetInfoRef(Engine::INFO_UP);
	_float fBentDecal = 1.5f;

	CWaterThrow* pWaterThrow = CWaterThrow::Create(m_pGraphicDev, vStartPos, vPos, vUp, fBentDecal, 20.f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"WaterThrow", pWaterThrow), -1);

	m_fCurTime = m_fCoolTime;
	return 1;
}

_int CSharkPool::Use_UpgradedSkill(const _float& fTimeDelta)
{
	return 0;
}

CSharkPool* CSharkPool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSharkPool* pInstance = new CSharkPool(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSharkPool::Free()
{
	Engine::CSkill::Free();
}

