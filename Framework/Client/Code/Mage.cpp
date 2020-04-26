#include "stdafx.h"
#include "Mage.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "WindAgentSwirl.h"
#include "Coin.h"

CMage::CMage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CMage::~CMage()
{

}

HRESULT CMage::Ready_GameObject()
{FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fHeight = 1.f;

	m_eCurDir = MD_LEFT;
	m_eCurState = MS_IDLE;

	m_fSpeed = 5.f;

	return S_OK;
}

_int CMage::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Turn_To_Camera_Look();

	Update_State(fTimeDelta);
	Change_State();
	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	return 0;
}

void CMage::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CMage::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CMage::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CMage::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CMage::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CMage::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CMage::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CMage::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();


	Engine::CTexture* pTextureCom = nullptr;

	m_vvTextureCom.resize(CMage::MS_END, vector<Engine::CTexture*>(CMage::MD_END));

	//	Texture Component Setting;
	//	Idle
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_IDLE][MD_LEFT] = pTextureCom;
	m_vvTextureCom[MS_IDLE][MD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	//  Run
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Run_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_RUN][MD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Run_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_RUN][MD_RIGHT] = pTextureCom;

	//	Cast
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Cast"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_CAST][MD_LEFT] = pTextureCom;
	m_vvTextureCom[MS_CAST][MD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	
	//	Attack
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_ATTACK][MD_LEFT] = pTextureCom;
	m_vvTextureCom[MS_ATTACK][MD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	//  Dead
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Dead_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_DEATH][MD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Mage_Dead_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[MS_DEATH][MD_RIGHT] = pTextureCom;

	return S_OK;
}

void CMage::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	{
		if (m_bAnimRepeat)
			m_tFrame.fCurFrame = 0.f;
		else
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
			m_bAnimFinish = true;
		}
	}
}

void CMage::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CMage::MS_IDLE:
		Idle_State();
		break;
	case CMage::MS_RUN:
		Run_State();
		break;
	case CMage::MS_CAST:
		Cast_State();
		break;
	case CMage::MS_ATTACK:
		Attack_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CMage::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CMage::MS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CMage::MS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CMage::MS_CAST:
		Cast_Update(fTimeDelta);
		break;
	case CMage::MS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	}
}

void CMage::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CMage::Fitting_Scale_With_Texture(CMage::Mage_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CMage::Fitting_Scale_With_Texture(CMage::Mage_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CMage::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(MS_IDLE);
}

void CMage::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(MS_RUN);
}

void CMage::Cast_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(MS_CAST);
}

void CMage::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(MS_ATTACK);
}

_int CMage::Idle_Update(const _float & fTimeDelta)
{
	m_fIdleTime += fTimeDelta;

	if (m_fIdleTime < 1.f)
		return 0;

	m_fIdleTime = 0.f;

	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos	= *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vMyPos		= *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDist		= vTargetPos - vMyPos;
	_float fDist	= D3DXVec3Length(&vDist);

	if (fDist < 1.f)
	{
		m_eCurState = MS_CAST;
	}
	if (fDist < 20.f)
	{
		m_eCurState = MS_RUN;
	}

	return 0;
}

_int CMage::Run_Update(const _float & fTimeDelta)
{
	if (m_bIsCasting)
		return 0;

	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDir = vTargetPos - vPos;
	_float fDist = D3DXVec3Length(&vDir);

	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	if (fDist < 5.f)
	{
		m_eCurState = MS_CAST;
		m_bIsCasting = true;
	}
	else if (fDist > 20.f)
		m_eCurState = MS_IDLE;

	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
	{
		if (m_eCurState == MS_CAST)
		{
			CBasicEffect* pWindAgentEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindAgentSwirl", L"", 5.f, 5.f, 0.05f, &_vec3(vPos.x, vPos.y, vPos.z + vDir.z / 10), true, 1.5f);
			Engine::Add_GameObject(L"GameLogic", L"Texture_WindAgentSwirl", pWindAgentEffect);
			NULL_CHECK_RETURN(pWindAgentEffect, -1);
		}
	}
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
	{
		if (m_eCurState == MS_CAST)
		{
			CBasicEffect* pWindAgentEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindAgentSwirl", L"", 5.f, 5.f, 0.05f, &_vec3(vPos.x, vPos.y, vPos.z - 0.1f), true, 1.5f);
			Engine::Add_GameObject(L"GameLogic", L"Texture_WindAgentSwirl", pWindAgentEffect);
			NULL_CHECK_RETURN(pWindAgentEffect, -1);
		}

		Mage_DIR eLeftRight	= vDir.x > 0.f ? MD_RIGHT : MD_LEFT;

		m_eCurDir = eLeftRight;
	}
		break;
	}

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CMage::Cast_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = MS_ATTACK;		
		m_bIsAttack = false;
	}
	return 0;
}

_int CMage::Attack_Update(const _float & fTimeDelta)
{
	if (!m_bIsAttack)
	{
		m_bIsAttack = true;

		const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, -1);

		_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vDir = vTargetPos - vPos;
		_float fDist = D3DXVec3Length(&vDir);

		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		CWindAgentSwirl* pWindAgentSwirl = CWindAgentSwirl::Create(m_pGraphicDev, vPos, vDir, 20.f);
		Engine::Add_GameObject(L"Effect", L"Texture_WindAgentSwirl", pWindAgentSwirl);
	}

	if (m_bAnimFinish)
	{
		const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, -1);

		_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_eCurState = MS_IDLE;
		m_bIsCasting = false;
	}

	return 0;
}

void CMage::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_bIsDead = true;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_Mage_Dead_Left", L"MageDieEffect", 6, 10.f, m_fScale, &vPos, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"MageDieEffect", pDieEffect);

		_uint iRepeat = rand() % 10;
		for (_uint i = 0; i < iRepeat; ++i)
		{
			_vec3 vDir = { rand() % 100 / 50.f - 1.f, 0.f ,rand() % 100 / 50.f - 1.f };
			D3DXVec3Normalize(&vDir, &vDir);
			_float fUpForce = rand() % 100 / 10.f + 20.f;
			_float	fSpeed = rand() % 100 / 50.f;
			_int	iCoin = rand() % 20 + 1;
			CCoin* pCoin = CCoin::Create(m_pGraphicDev, m_pTransformCom->GetInfo(Engine::INFO_POS), &vDir, fUpForce, fSpeed, iCoin);
			Engine::Add_GameObject(L"GameLogic", L"Coin", pCoin);
		}
	}

}

void CMage::Mage_Hit_Effect()
{
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

CMage* CMage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CMage* pInstance = new CMage(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CMage::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

