#include "stdafx.h"
#include "AquaVortex.h"
#include "Water.h"

#include "Export_Function.h"
#include "Aqua.h"
#include "BasicEffect.h"

CAquaVortex::CAquaVortex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CAquaVortex::~CAquaVortex()
{

}

HRESULT CAquaVortex::Ready_Skill()
{
	m_fCoolTime = 0.f;

	Engine::Ready_Timer(L"AquaVortex_CoolTime");
	Engine::Set_TimeDelta(L"AquaVortex_CoolTime");

	m_fSpeed = 20.f;

	return S_OK;
}

_int CAquaVortex::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"AquaVortex_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}

	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"AquaVortex_CoolTime");

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

	vPos.y = 0.2f;
	CAqua* pAqua = CAqua::Create(m_pGraphicDev, vPos, vAngle, 20.f, 8.f, 15.f, 0.1f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"AquaVortex", pAqua), -1);

	m_fCurTime = m_fCoolTime;
	
	return 1;
}

_int CAquaVortex::Use_Skill(const _float& fTimeDelta, const _vec3* pPos, const _vec3* pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"AquaVortex_CoolTime");

	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"AquaVortex_CoolTime");

	_vec3 vPos;

	_vec2 vMousePos = Engine::Get_MainCamera()->Get_MousePos();

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vAngle = pTransform->GetAngleRef();
	vAngle.x += D3DXToRadian(90.f);

	_matrix	matProj, matView;
	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);

	D3DXPLANE tPlane = { 0.f, 1.f, 0.f, 0.f };

	Engine::CMyMath::PickingOnPlane(&vPos, &vMousePos, WINCX, WINCY, &matProj, &matView, &tPlane);

	vPos.y = 0.2f;
	CAqua* pAqua = CAqua::Create(m_pGraphicDev, vPos, vAngle, 10.f, 14.f, 15.f, 0.1f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"AquaVortex", pAqua), -1);


	m_fCurTime = m_fCoolTime;

	return 1;
}

_int CAquaVortex::Use_UpgradedSkill(const _float& fTimeDelta)
{
	return 0;
}

CAquaVortex* CAquaVortex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAquaVortex* pInstance = new CAquaVortex(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CAquaVortex::Free()
{
	Engine::CSkill::Free();
}

