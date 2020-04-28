#include "stdafx.h"
#include "Cyclops.h"

#include "Export_Function.h"
#include "BasicEffect.h"
#include "LaidEffect.h"
#include "BeamEffect.h"
#include "LineCollider.h"
#include "Coin.h"
#include "NumberEffect.h"

CCyclops::CCyclops(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CCyclops::~CCyclops()
{

}

HRESULT CCyclops::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_eCurDir = CYD_DOWN;
	m_eCurState = CYS_IDLE;

	m_fSpeed = 5.f;
	return S_OK;
}

_int CCyclops::Update_GameObject(const _float& fTimeDelta)
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

void CCyclops::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CCyclops::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}


void CCyclops::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CCyclops::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CCyclops::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CCyclops::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CCyclops::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CCyclops::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_FireBoss_Idle"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();



	Engine::CTexture* pTextureCom = nullptr;

	m_vvTextureCom.resize(CCyclops::CYS_END, vector<Engine::CTexture*>(CCyclops::CYD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_IDLE][CYD_UP] = pTextureCom;
	m_vvTextureCom[CYS_IDLE][CYD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[CYS_IDLE][CYD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[CYS_IDLE][CYD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_RunRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_RUN][CYD_UP] = pTextureCom;
	m_vvTextureCom[CYS_RUN][CYD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_RunLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_RUN][CYD_DOWN] = pTextureCom;
	m_vvTextureCom[CYS_RUN][CYD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_Hurt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_HURT][CYD_UP] = pTextureCom;
	m_vvTextureCom[CYS_HURT][CYD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[CYS_HURT][CYD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[CYS_HURT][CYD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK_READY][CYD_UP] = pTextureCom;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK_READY][CYD_DOWN] = pTextureCom;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK_READY][CYD_RIGHT] = pTextureCom;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_AttackReady_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK_READY][CYD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_Attack_Up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK][CYD_UP] = pTextureCom;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_Attack_Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK][CYD_DOWN] = pTextureCom;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_Attack_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK][CYD_RIGHT] = pTextureCom;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Cyclops_Attack_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[CYS_ATTACK][CYD_LEFT] = pTextureCom;




	return S_OK;
}

void CCyclops::Animation(const _float & fTimeDelta)
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

void CCyclops::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CCyclops::CYS_IDLE:
		Idle_State();
		break;
	case CCyclops::CYS_RUN:
		Run_State();
		break;
	case CCyclops::CYS_HURT:
		Hurt_State();
		break;
	case CCyclops::CYS_ATTACK_READY:
		Attack_Ready_State();
		break;
	case CCyclops::CYS_ATTACK:
		Attack_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CCyclops::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CCyclops::CYS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CCyclops::CYS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CCyclops::CYS_HURT:
		Hurt_Update(fTimeDelta);
		break;
	case CCyclops::CYS_ATTACK_READY:
		Attack_Ready_Update(fTimeDelta);
		break;
	case CCyclops::CYS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	}
}

void CCyclops::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CCyclops::Fitting_Scale_With_Texture(CCyclops::CYCLOPS_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CCyclops::Fitting_Scale_With_Texture(CCyclops::CYCLOPS_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CCyclops::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(CYS_IDLE);
}

void CCyclops::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(CYS_RUN);
}

void CCyclops::Hurt_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(CYS_HURT);

}

void CCyclops::Attack_Ready_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 1.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(CYS_ATTACK_READY);
}

void CCyclops::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 1.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(CYS_ATTACK);
}

_int CCyclops::Idle_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vMyPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDist = vTargetPos - vMyPos;
	_float fDist = D3DXVec3Length(&vDist);

	if (fDist < 5.f)
	{
		m_eCurState = CYS_ATTACK_READY;
		m_vAttackTarget = vTargetPos;
	}
	else if (fDist < 20.f)
		m_eCurState = CYS_RUN;

	return 0;
}

_int CCyclops::Run_Update(const _float & fTimeDelta)
{
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
		m_eCurState = CYS_ATTACK_READY;
		m_vAttackTarget = vTargetPos;
	}
	else if (fDist > 20.f)
		m_eCurState = CYS_IDLE;

	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
	{
		_vec3 vCamRight;
		Engine::Get_MainCameraRight(&vCamRight);
		vCamRight.y = 0.f;
		D3DXVec3Normalize(&vCamRight, &vCamRight);

		_float fDot = D3DXVec3Dot(&vDir, &vCamRight);

		m_eCurDir = fDot > 0.f ? CYD_RIGHT : CYD_LEFT;
	}
	break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
	{
		m_eCurDir = vDir.x > 0.f ? CYD_RIGHT : CYD_LEFT;
	}
	break;
	}

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CCyclops::Hurt_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
		m_eCurState = CYS_IDLE;
	
	m_pTransformCom->Move_Pos(m_vHurtDir * fTimeDelta);


	return 0;
}

_int CCyclops::Attack_Ready_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = CYS_ATTACK;

		//const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		//NULL_CHECK_RETURN(pTargetTransform, -1);

		//_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vDir = m_vAttackTarget - vPos;
		_float fDist = D3DXVec3Length(&vDir);

		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		switch (Engine::Get_MainCamType())
		{
		case Engine::CCameraMgr::MAIN_CAM_1ST:
		case Engine::CCameraMgr::MAIN_CAM_3RD:
		{
			_vec3 vCamRight, vCamLook;
			Engine::Get_MainCameraRight(&vCamRight);
			Engine::Get_MainCameraLook(&vCamLook);
			vCamRight.y = 0.f;
			vCamLook.y = 0.f;
			D3DXVec3Normalize(&vCamRight, &vCamRight);
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			_float fDotR = D3DXVec3Dot(&vDir, &vCamRight);
			_float fDotL = D3DXVec3Dot(&vDir, &vCamLook);

			CCyclops::CYCLOPS_DIR eLeftRight = fDotR > 0.f ? CYD_RIGHT : CYD_LEFT;
			CCyclops::CYCLOPS_DIR eUpDown = fDotL > 0.f ? CYD_UP : CYD_DOWN;

			m_eCurDir = fabs(fDotR) > fabs(fDotL) ? eLeftRight : eUpDown;
		}
		break;
		case Engine::CCameraMgr::MAIN_CAM_QUATER:
		{
			CCyclops::CYCLOPS_DIR eLeftRight = vDir.x  > 0.f ? CYD_RIGHT : CYD_LEFT;
			CCyclops::CYCLOPS_DIR eUpDown = vDir.z  > 0.f ? CYD_UP : CYD_DOWN;

			m_eCurDir = fabs(vDir.x) > fabs(vDir.z) ? eLeftRight : eUpDown;
		}
		break;
		}

		_float fAngleY = Engine::CMyMath::YAngleTransformFromVec(&vDir);
		
		if (vDir.z < 0.f)
			fAngleY += D3DXToRadian(180.f);

		_vec3 vRight;
		m_pTransformCom->GetInfo(Engine::INFO_RIGHT, &vRight);
		//D3DXVec3Nor

		switch (m_eCurDir)
		{
		case CCyclops::CYD_LEFT:
			vPos -= vRight * 0.45f;
			break;
		case CCyclops::CYD_RIGHT:
			vPos += vRight * 0.45f;
			break;
		}

		CBeamEffect* pBeam = CBeamEffect::Create(m_pGraphicDev, L"Texture_RedBeam", L"CyclopsBeam", 1.f, 0.f, &vPos, 0.2f, 20.f, fAngleY, true, 0.5f);
		Engine::Add_GameObject(L"GameLogic", L"CyclopsBeam", pBeam);

		CBasicEffect* pHitEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vPos, false, 0.f);
		NULL_CHECK_RETURN(pHitEffect, -1);
		
		CLineCollider* pLineCollider = CLineCollider::Create(m_pGraphicDev, pBeam, pHitEffect, 10.f, &_vec3(0.f, fAngleY, 0.f), L"MonsterAttack", 30);
		Engine::Add_GameObject(L"GameLogic", L"MonsterLineColl", pLineCollider);
		pLineCollider->Add_Hit_Effect(CCyclops::Beam_Hit_Function);

		Engine::PlaySound_(L"LaserBolt.wav", CSoundMgr::EFFECT);
	}

	return 0;
}

_int CCyclops::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
		m_eCurState = CYS_IDLE;

	return 0;
}

void CCyclops::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;

	m_vHurtDir = *m_pTransformCom->GetInfo(Engine::INFO_POS) - *pAtkPos;
	m_vHurtDir.y = 0.f;
	D3DXVec3Normalize(&m_vHurtDir, &m_vHurtDir);
	m_eCurState = CYS_HURT;
	Hurt_State();


	CNumberEffect* pNumber = CNumberEffect::Create(m_pGraphicDev, 0.05f, m_pTransformCom->GetInfo(Engine::INFO_POS), 1.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), iAtk);
	Engine::Add_GameObject(L"Effect", L"DamageFont", pNumber);
	if (m_iHP <= 0)
	{
		m_bIsDead = true;

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_Cyclops_Dead", L"CyclopsDieEffect", 6, 10.f, m_fScale, &vPos, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"CyclopsDieEffect", pDieEffect);

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

		Engine::PlaySound_(L"GhoulGrowl.wav", CSoundMgr::EFFECT);
	}

}

void CCyclops::Beam_Hit_Function()
{
	Engine::PlaySound_(L"ImpactPhysicalLight.wav", CSoundMgr::EFFECT);
}

CCyclops* CCyclops::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CCyclops* pInstance = new CCyclops(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(pPos);

	return pInstance;
}

void CCyclops::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

