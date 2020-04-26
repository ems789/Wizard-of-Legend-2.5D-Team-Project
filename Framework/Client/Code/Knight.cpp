#include "stdafx.h"
#include "Knight.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "Coin.h"

CKnight::CKnight(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CKnight::~CKnight()
{

}

HRESULT CKnight::Ready_GameObject()
{FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fHeight = 1.f;

	m_eCurDir = KD_LEFT;
	m_eCurState = KS_IDLE;

	m_fSpeed = 5.f;

	return S_OK;
}

_int CKnight::Update_GameObject(const _float& fTimeDelta)
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

void CKnight::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CKnight::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CKnight::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CKnight::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CKnight::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CKnight::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CKnight::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CKnight::Add_Component()
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

	m_vvTextureCom.resize(CKnight::KS_END, vector<Engine::CTexture*>(CKnight::KD_END));

	//	Texture Component Setting;
	//	Idle
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_IDLE][KD_LEFT] = pTextureCom;
	m_vvTextureCom[KS_IDLE][KD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	//  Run
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Run_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_RUN][KD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Run_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_RUN][KD_RIGHT] = pTextureCom;

	//	Attack
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Attack_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_ATTACK][KD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Attack_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_ATTACK][KD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Attack_Up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_ATTACK][KD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Attack_Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_ATTACK][KD_DOWN] = pTextureCom;

	//  Dash
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Dash_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_DASH][KD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Dash_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_DASH][KD_RIGHT] = pTextureCom;

	//  Dead
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_DEATH][KD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Knight_Dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[KS_DEATH][KD_RIGHT] = pTextureCom;

	return S_OK;
}

void CKnight::Animation(const _float & fTimeDelta)
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

void CKnight::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CKnight::KS_IDLE:
		Idle_State();
		break;
	case CKnight::KS_RUN:
		Run_State();
		break;
	case CKnight::KS_DASH:
		Dash_State();
		break;
	case CKnight::KS_ATTACK:
		Attack_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CKnight::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CKnight::KS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CKnight::KS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CKnight::KS_DASH:
		Dash_Update(fTimeDelta);
		break;
	case CKnight::KS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	}
}

void CKnight::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CKnight::Fitting_Scale_With_Texture(CKnight::Knight_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CKnight::Fitting_Scale_With_Texture(CKnight::Knight_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CKnight::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(KS_IDLE);
}

void CKnight::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(KS_RUN);
}

void CKnight::Dash_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(KS_DASH);
}

void CKnight::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 3.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(KS_ATTACK);
}

_int CKnight::Idle_Update(const _float & fTimeDelta)
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
		m_eCurState = KS_ATTACK;
	}
	if (fDist < 20.f)
	{
		m_eCurState = KS_RUN;
	}

	return 0;
}

_int CKnight::Run_Update(const _float & fTimeDelta)
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
		m_eCurState = KS_DASH;
	}
	else if (fDist > 20.f)
		m_eCurState = KS_IDLE;

	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
	{
		if (m_eCurState == KS_ATTACK)
		{
			const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
			NULL_CHECK_RETURN(pTargetTransform, -1);

			_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
			_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

			_vec3 vDir = vTargetPos - vPos;
			_float fDist = D3DXVec3Length(&vDir);

			vDir.y = 0.f;
			D3DXVec3Normalize(&vDir, &vDir);

			Knight_DIR eUpDown = vDir.z > 0.f ? KD_UP : KD_DOWN;
			Knight_DIR eLeftRight = vDir.x > 0.f ? KD_RIGHT : KD_LEFT;

			m_eCurDir = fabs(vDir.z) > fabs(vDir.x) ? eUpDown : eLeftRight;
		}
		else
		{
			Knight_DIR eLeftRight = vDir.x > 0.f ? KD_RIGHT : KD_LEFT;

			m_eCurDir = eLeftRight;
		}
	}
		break;
	}

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CKnight::Dash_Update(const _float & fTimeDelta)
{
	if (!m_bIsDash)
	{
		m_bIsDash = true;

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);

		_vec3 vDir = vTargetPos - vPos;
		vDir.y = 0.f;

		D3DXVec3Normalize(&m_vTargetDir, &vDir);

		_float fAngleY = Engine::CMyMath::YAngleTransformFromVec(&vDir);
		if (vDir.z < 0.f)
			fAngleY += D3DXToRadian(180.f);

		CLaidEffect* pAirRing = CLaidEffect::Create(m_pGraphicDev, L"Texture_AirRingUp", L"KnightDash", 4, 10.f, 0.1f,
			m_pTransformCom->GetInfo(Engine::INFO_POS), fAngleY, false, 0.f);
		Engine::Add_GameObject(L"Effect", L"PlayerDash", pAirRing);
		Engine::PlaySound_(L"StandardDash.wav", CSoundMgr::EFFECT);
	}

	if (m_bAnimFinish)
	{
		m_eCurState = KS_ATTACK;		
		m_bIsDash = false;

		// 공격으로 넘어갈때
		m_vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		// 공격은 Up Down 모션도 있음
		switch (Engine::Get_MainCamType())
		{
		case Engine::CCameraMgr::MAIN_CAM_3RD:
		{
			_vec3 vCamLook;
			Engine::Get_MainCameraLook(&vCamLook);
			vCamLook.y = 0.f;
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			_float fDot = D3DXVec3Dot(&m_vTargetDir, &vCamLook);

			if (fDot > 0.f)
				m_eCurDir = KD_UP;
			else
				m_eCurDir = KD_DOWN;
		}
			break;	
		case Engine::CCameraMgr::MAIN_CAM_QUATER:
		{
			Knight_DIR eUpDown = m_vTargetDir.z > 0.f ? KD_UP : KD_DOWN;
			Knight_DIR eLeftRight = m_vTargetDir.x > 0.f ? KD_RIGHT : KD_LEFT;

			m_eCurDir = fabs(m_vTargetDir.z) > fabs(m_vTargetDir.x) ? eUpDown : eLeftRight;
		}
			break;
		}
	}

	m_pTransformCom->Move_Pos(m_vTargetDir * fTimeDelta * 10.f);
	return 0;
}

_int CKnight::Attack_Update(const _float & fTimeDelta)
{
	if (m_tFrame.fCurFrame > 1.f && !m_bIsAttack)
	{
		m_bIsAttack = true;

		_float fAngleY = Engine::CMyMath::YAngleTransformFromVec(&m_vTargetDir);
		if (m_vTargetDir.z < 0.f)
			fAngleY += D3DXToRadian(180.f);

		CLaidEffect* pEffect = CLaidEffect::Create(m_pGraphicDev, L"Texture_Slash", L"KnightSlash", 1.f, 20.f, 0.05f, &m_vPos, fAngleY, true, 0.3f);
		Engine::Add_GameObject(L"GameLogic", L"KnightSlash", pEffect);

		CBasicEffect* pHitEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &m_vPos, false, 0.f);
		NULL_CHECK_RETURN(pHitEffect, -1);

		CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pEffect, pHitEffect, 1.5f, L"MonsterAttack", 20);
		Engine::Add_GameObject(L"GameLogic", L"MonsterCollider", pCollider);
		pCollider->Add_Hit_Effect(CKnight::Knight_Hit_Effect);

	}

	if (m_bAnimFinish)
	{
		m_bIsAttack = false;
		m_eCurState = KS_IDLE;

		const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, -1);

		_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		Knight_DIR eLeftRight = vDir.x > 0.f ? KD_RIGHT : KD_LEFT;

		m_eCurDir = eLeftRight;		
	}

	return 0;
}

void CKnight::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_bIsDead = true;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_Knight_Dead_Left", L"KnightDieEffect", 6, 10.f, m_fScale, &vPos, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"KnightDieEffect", pDieEffect);

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

void CKnight::Knight_Hit_Effect()
{
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

CKnight* CKnight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CKnight* pInstance = new CKnight(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CKnight::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

