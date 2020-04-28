#include "stdafx.h"
#include "LightningGen.h"

#include "Export_Function.h"
#include "AlphaLaidEffect.h"
#include "LightningSpear.h"

CLightningGen::CLightningGen(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CLightningGen::~CLightningGen()
{

}

HRESULT CLightningGen::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CLightningGen::Update_GameObject(const _float& fTimeDelta)
{
	Generate_Lightning(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}


const _vec3 * CLightningGen::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CLightningGen::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CLightningGen::Generate_Lightning(const _float & fTimeDelta)
{
	m_fTimeCount += fTimeDelta;

	if (m_fTimeCount > m_fLightningTimeItv)
	{
		m_fTimeCount -= m_fLightningTimeItv;
		++m_uiLightningCnt;

		if (m_uiLightningCnt > m_uiLightningMax)
		{
			m_bIsDead = true;
			return;
		}
		
		/*CAlphaLaidEffect* pCastingCircle = CAlphaLaidEffect::Create(m_pGraphicDev, L"Texture_CastingCircle", L"MeteorCastingCircle", 27, 20.f, 0.05f, &vPos, 0.f, false, 1.f, D3DXCOLOR(1.f, 0.5f, 0.2f, 1.f));
		Engine::Add_GameObject(L"Effect", L"MeteorCastingCircle", pCastingCircle);*/

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

		_vec3 vStartPos = pTransform->GetInfoRef(Engine::INFO_POS);
		_vec3 vLook = pTransform->GetInfoRef(Engine::INFO_LOOK);

		float fAngle = Engine::CMyMath::YAngleTransformFromVec(&m_vDir);

		CLightningSpear* pLightningSpear = CLightningSpear::Create(m_pGraphicDev, vStartPos, m_vDir, m_fSpeed, 7, 30.f, fAngle + m_fAngle);

		Engine::Add_GameObject(L"GameLogic", L"LightningSpear", pLightningSpear);

		Engine::PlaySound_(L"LightningPinball.wav", CSoundMgr::EFFECT);
	}
}

CLightningGen * CLightningGen::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, const _float & fLightningTimeItv, const _uint & uiLightningMax)
{
	CLightningGen*	pInstance = new CLightningGen(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(*pPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fLightningTimeItv = fLightningTimeItv;
	pInstance->m_uiLightningMax = uiLightningMax;
	pInstance->m_vDir = *pDir;

	return pInstance;
}

void CLightningGen::Free()
{

	CGameObject::Free();
}

