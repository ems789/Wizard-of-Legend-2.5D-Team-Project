#include "stdafx.h"
#include "MeteorStrike.h"
#include "Fire.h"

#include "Export_Function.h"
#include "Meteor.h"
#include "AlphaLaidEffect.h"

CMeteorStrike::CMeteorStrike(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{

}

CMeteorStrike::~CMeteorStrike()
{

}

HRESULT CMeteorStrike::Ready_Skill()
{
	m_fCoolTime = 0.f;

	Engine::Ready_Timer(L"MeteorStrike_CoolTime");
	Engine::Set_TimeDelta(L"MeteorStrike_CoolTime");
	
	m_fSpeed = 20.f;

	return S_OK;
}

_int CMeteorStrike::Use_Skill(const _float& fTimeDelta)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"MeteorStrike_CoolTime");
	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"MeteorStrike_CoolTime");

	_vec3 vPos;

	_vec2 vMousePos = Engine::Get_MainCamera()->Get_MousePos();
	
	_matrix	matProj, matView;
	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);

	D3DXPLANE tPlane = { 0.f, 1.f, 0.f, 0.f };

	Engine::CMyMath::PickingOnPlane(&vPos, &vMousePos, WINCX, WINCY, &matProj, &matView, &tPlane);

	vPos.y = 0.1f;

	CAlphaLaidEffect* pCastingCircle = CAlphaLaidEffect::Create(m_pGraphicDev, L"Texture_CastingCircle", L"MeteorCastingCircle", 27, 20.f, 0.05f, &vPos, 0.f, false, 1.f, D3DXCOLOR(1.f, 0.5f, 0.2f, 1.f));
	Engine::Add_GameObject(L"GameLogic", L"MeteorCastingCircle", pCastingCircle);

	vPos.y = 30.f;

	CMeteor* pMeteor = CMeteor::Create(m_pGraphicDev, vPos, -AXIS_Y, 20.f, 4.f, 20.f, 0.05f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"MeteorSkill", pMeteor), -1);

	m_fCurTime = m_fCoolTime;

	return 1;
}

_int CMeteorStrike::Use_Skill(const _float & fTimeDelta, const _vec3 * pPos, const _vec3 * pDir)
{
	_float fCumulativeTime = Engine::Get_CummulativeTime(L"MeteorStrike_CoolTime");

	if (m_bFirstShot)
	{
		fCumulativeTime = 10000.f;
		m_bFirstShot = false;
	}
	if (fCumulativeTime < m_fCoolTime)
		return 0;
	else
		Engine::Set_TimeDelta(L"MeteorStrike_CoolTime");

	_vec3 vPos;

	_vec2 vMousePos = Engine::Get_MainCamera()->Get_MousePos();

	_matrix	matProj, matView;
	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);

	D3DXPLANE tPlane = { 0.f, 1.f, 0.f, 0.f };

	Engine::CMyMath::PickingOnPlane(&vPos, &vMousePos, WINCX, WINCY, &matProj, &matView, &tPlane);

	vPos.y = 0.1f;

	CAlphaLaidEffect* pCastingCircle = CAlphaLaidEffect::Create(m_pGraphicDev, L"Texture_CastingCircle", L"MeteorCastingCircle", 27, 20.f, 0.05f, &vPos, 0.f, false, 1.f, D3DXCOLOR(1.f, 0.5f, 0.2f, 1.f));
	Engine::Add_GameObject(L"GameLogic", L"MeteorCastingCircle", pCastingCircle);


	vPos.y = 30.f;

	CMeteor* pMeteor = CMeteor::Create(m_pGraphicDev, vPos, -AXIS_Y, 10.f, 4.f, 20.f, 0.05f);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"MeteorSkill", pMeteor), -1);

	m_fCurTime = m_fCoolTime;

	return 1;
}

_int CMeteorStrike::Use_UpgradedSkill(const _float & fTimeDelta)
{
	return 0;
}

CMeteorStrike* CMeteorStrike::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMeteorStrike*	pInstance = new CMeteorStrike(pGraphicDev);

	if (FAILED(pInstance->Ready_Skill()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMeteorStrike::Free()
{
	Engine::CSkill::Free();
}

