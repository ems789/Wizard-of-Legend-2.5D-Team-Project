#include "stdafx.h"
#include "GhoulLarge.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "Coin.h"
#include "Golem.h"

CGhoulLarge::CGhoulLarge(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CGhoulLarge::~CGhoulLarge()
{

}

HRESULT CGhoulLarge::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 3.f;

	m_fHeight = 2.5f;

	m_eCurState = GLS_IDLE;

	m_fSpeed = 5.f;

	return S_OK;
}

_int CGhoulLarge::Update_GameObject(const _float& fTimeDelta)
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

void CGhoulLarge::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CGhoulLarge::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CGhoulLarge::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CGhoulLarge::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CGhoulLarge::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CGhoulLarge::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CGhoulLarge::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CGhoulLarge::Add_Component()
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

	m_vvTextureCom.resize(CGhoulLarge::GLS_END, nullptr);

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_GhoulLarge_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GLS_IDLE]	= pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_GhoulLarge_Run"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GLS_RUN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_GhoulLarge_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GLS_ATTACK] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_GhoulLarge_Attack2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GLS_ATTACK2] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_GhoulLarge_Attack3"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GLS_ATTACK3] = pTextureCom;

	return S_OK;
}

void CGhoulLarge::Animation(const _float & fTimeDelta)
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

void CGhoulLarge::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CGhoulLarge::GLS_IDLE:
		Idle_State();
		break;
	case CGhoulLarge::GLS_RUN:
		Run_State();
		break;
	case CGhoulLarge::GLS_ATTACK:
		Attack_State();
		break;
	case CGhoulLarge::GLS_ATTACK2:
		Attack2_State();
		break;
	case CGhoulLarge::GLS_ATTACK3:
		Attack3_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CGhoulLarge::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CGhoulLarge::GLS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CGhoulLarge::GLS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CGhoulLarge::GLS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	case CGhoulLarge::GLS_ATTACK2:
		Attack2_Update(fTimeDelta);
		break;
	case CGhoulLarge::GLS_ATTACK3:
		Attack3_Update(fTimeDelta);
		break;
	}
}

void CGhoulLarge::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CGhoulLarge::Fitting_Scale_With_Texture(CGhoulLarge::GHOUL_LARGE_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CGhoulLarge::Fitting_Scale_With_Texture(CGhoulLarge::GHOUL_LARGE_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CGhoulLarge::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(GLS_IDLE);
}

void CGhoulLarge::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(GLS_RUN);
}

void CGhoulLarge::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(GLS_ATTACK);
}

void CGhoulLarge::Attack2_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(GLS_ATTACK2);
}

void CGhoulLarge::Attack3_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(GLS_ATTACK3);
}

_int CGhoulLarge::Idle_Update(const _float & fTimeDelta)
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

	if (fDist < 4.f)
	{
		switch (m_uiPattern)
		{
		case 0:
			m_eCurState = GLS_ATTACK;
			break;
		case 1:
			m_eCurState = GLS_ATTACK2;
			break;
		case 2:
			m_eCurState = GLS_ATTACK3;
			break;
		}
		++m_uiPattern;

		if (m_uiPattern > 2)
			m_uiPattern = 0;
	}
	if (fDist < 20.f)
	{
		m_eCurState = GLS_RUN;
	}

	return 0;
}

_int CGhoulLarge::Run_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDir = vTargetPos - vPos;
	_float fDist = D3DXVec3Length(&vDir);

	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	
	if (fDist < 4.f)
	{
		switch (m_uiPattern)
		{
		case 0:
			m_eCurState = GLS_ATTACK;
			break;
		case 1:
			m_eCurState = GLS_ATTACK2;
			break;
		case 2:
			m_eCurState = GLS_ATTACK3;
			break;
		}
		++m_uiPattern;

		if (m_uiPattern > 2)
			m_uiPattern = 0;
	}
	else if (fDist > 20.f)
		m_eCurState = GLS_IDLE;

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CGhoulLarge::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_fAttackDelay += fTimeDelta;

		if (m_fAttackDelay > 0.5f)
		{
			m_fAttackDelay = 0.f;
			m_eCurState = GLS_IDLE;
			m_bAttack = false;
			return 0;
		}
	}

	if (m_tFrame.fCurFrame > 3.f && m_tFrame.fCurFrame < 4.f)
	{
		const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, -1);
		_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vDir = vTargetPos - vPos;	
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Move_Pos(vDir * m_fSpeed* fTimeDelta);
	}
	else if (m_tFrame.fCurFrame > 4.f)
	{
		if (!m_bAttack)
		{
			m_bAttack = true;

			_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

			CBasicEffect* pHitEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vPos, false, 0.f);
			NULL_CHECK_RETURN(pHitEffect, -1);

			CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, this, pHitEffect, 3.f, L"MonsterAttack", 20, 0.5f);
			Engine::Add_GameObject(L"GameLogic", L"MonsterCollider", pCollider);
			pCollider->Add_Hit_Effect(CGolem::Golem_Hit_Effect);
		}
	}

	return 0;
}

_int CGhoulLarge::Attack2_Update(const _float & fTimeDelta)
{
	return Attack_Update(fTimeDelta);
}

_int CGhoulLarge::Attack3_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_fAttackDelay += fTimeDelta;

		if (m_fAttackDelay > 0.5f)
		{
			m_fAttackDelay = 0.f;
			m_eCurState = GLS_IDLE;
			m_bAttack = false;
			return 0;
		}
	}

	if (m_tFrame.fCurFrame > 4.f)
	{
		if (!m_bAttack)
		{
			m_bAttack = true;

			_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
			vPos.y = 0.1f;

			CLaidEffect* pCrack = CLaidEffect::Create(m_pGraphicDev, L"Texture_FloorCrack", L"FloorCrack", 1.f, 0.f, 0.1f, &vPos, 0.f, true, 0.05f);
			Engine::Add_GameObject(L"Effect", L"FloorCrack", pCrack);

			CBasicEffect* pHitEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vPos, false, 0.f);
			NULL_CHECK_RETURN(pHitEffect, -1);

			CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pCrack, pHitEffect, 4.f, L"MonsterAttack", 20, 0.5f);
			Engine::Add_GameObject(L"GameLogic", L"MonsterCollider", pCollider);
			pCollider->Add_Hit_Effect(CGolem::Golem_Hit_Effect);
		}
	}
	
	return _int();
}

void CGhoulLarge::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_bIsDead = true;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_GhoulLarge_Dead", L"GhoulLargeDieEffect", 6, 10.f, m_fScale, &vPos, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"GelemDieEffect", pDieEffect);

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

		Engine::PlaySound_(L"MovingStatueDead.wav", CSoundMgr::EFFECT);
	}

}

void CGhoulLarge::GhoulLarge_Hit_Effect()
{
	Engine::PlaySound_(L"ImpactPhysicalHeavy.wav", CSoundMgr::EFFECT);
}

CGhoulLarge* CGhoulLarge::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CGhoulLarge* pInstance = new CGhoulLarge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CGhoulLarge::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		Engine::Safe_Release(m_vvTextureCom[i]);
	}

	Engine::CGameObject::Free();
}

