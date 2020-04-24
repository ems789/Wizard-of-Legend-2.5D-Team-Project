#include "stdafx.h"
#include "MeteorGen.h"

#include "Export_Function.h"
#include "AlphaLaidEffect.h"
#include "Meteor.h"

CMeteorGen::CMeteorGen(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMeteorGen::~CMeteorGen()
{

}

HRESULT CMeteorGen::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMeteorGen::Update_GameObject(const _float& fTimeDelta)
{
	Generate_Meteor(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}


const _vec3 * CMeteorGen::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CMeteorGen::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CMeteorGen::Generate_Meteor(const _float & fTimeDelta)
{
	m_fTimeCount += fTimeDelta;

	if (m_fTimeCount > m_fMeteorTimeItv)
	{
		m_fTimeCount -= m_fMeteorTimeItv;
		++m_uiMeteorCnt;

		if (m_uiMeteorCnt > m_uiMeteorMax)
		{
			m_bIsDead = true;
			return;
		}
		
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		vPos.x += rand() % 10 - 5.f;
		vPos.y = 0.1f;
		vPos.z += rand() % 10 - 5.f;

		CAlphaLaidEffect* pCastingCircle = CAlphaLaidEffect::Create(m_pGraphicDev, L"Texture_CastingCircle", L"MeteorCastingCircle", 27, 20.f, 0.05f, &vPos, 0.f, false, 1.f, D3DXCOLOR(1.f, 0.5f, 0.2f, 1.f));
		Engine::Add_GameObject(L"Effect", L"MeteorCastingCircle", pCastingCircle);

		vPos.y = 30.f;

		CMeteor* pMeteor = CMeteor::Create(m_pGraphicDev, vPos, -AXIS_Y, 10.f, 4.f, 20.f, 0.05f);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"GameLogic", L"MeteorSkill", pMeteor), );

		Engine::PlaySound_(L"Meteor.wav", CSoundMgr::EFFECT);
	}
}

CMeteorGen * CMeteorGen::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _float & fMeteorTimeItv, const _uint & uiMeteorMax)
{
	CMeteorGen*	pInstance = new CMeteorGen(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fMeteorTimeItv = fMeteorTimeItv;
	pInstance->m_uiMeteorMax = uiMeteorMax;

	return pInstance;
}

void CMeteorGen::Free()
{

	CGameObject::Free();
}

