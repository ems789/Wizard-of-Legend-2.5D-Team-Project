#include "stdafx.h"
#include "FirePoint.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "FireEffect.h"
#include "Fire.h"

CFirePoint::CFirePoint(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFirePoint::~CFirePoint()
{

}

HRESULT CFirePoint::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eCurState = FPS_READY;
	m_fFireAngle = 0.f;
	m_fSpeed = 20.f;
	return S_OK;
}

_int CFirePoint::Update_GameObject(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case CFirePoint::FPS_READY:
	{
		m_fStateTime += fTimeDelta;
		if (m_fStateTime > 1.f)
		{
			m_eCurState = FPS_GO;
		}
		else
		{
			m_fFireIntervalTime += fTimeDelta;
			
			m_fFireAngle += fTimeDelta * D3DXToRadian(180.f);

			if (m_fFireIntervalTime >= 0.15f)
			{
				m_fFireIntervalTime -= 0.15f;

				_vec3 vPos, vUp;
				_matrix matRotAxis;

				D3DXVec3Cross(&vUp, &m_vRight, &m_vDir);
				D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_fFireAngle);

				D3DXVec3TransformNormal(&vPos, &m_vRight, &matRotAxis);

				vPos *= 2.f;
				vPos += *m_pTransformCom->GetInfo(Engine::INFO_POS);

				CFire* pFire = CFire::Create(m_pGraphicDev, vPos,  m_vDir, m_fSpeed, 4.f, 30.f, L"MonsterAttack", false);
				m_FireList.push_back(pFire);
				Engine::Add_GameObject(L"GameLogic", L"BossPointFire", pFire);

				Engine::PlaySound_(L"FireChargeup.wav", CSoundMgr::EFFECT);
			}
		}
	}
		break;
	case CFirePoint::FPS_GO:
	{
		for (auto& pFire : m_FireList)
			pFire->Go();
		

		m_bIsDead = true;
	}
		break;
	}
	if (m_bIsDead)
		return 0;

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CFirePoint::Render_GameObject()
{
}

const _vec3 * CFirePoint::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CFirePoint::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

CFirePoint* CFirePoint::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _vec3& vRight, const _float& fSpeed)
{
	CFirePoint*	pInstance = new CFirePoint(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_vDir = vDir;
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_vRight = vRight;

	return pInstance;
}

void CFirePoint::Free()
{

	CGameObject::Free();
}

