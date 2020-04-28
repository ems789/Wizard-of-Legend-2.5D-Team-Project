#include "stdafx.h"
#include "SkeletonMinion.h"


#include "Export_Function.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "LaidEffect.h"
#include "Coin.h"
CSkeletonMinion::CSkeletonMinion(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CSkeletonMinion::~CSkeletonMinion()
{
}

HRESULT CSkeletonMinion::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fHeight = 1.5f;

	m_eCurDir = SD_DOWN;
	m_eCurState = SK_IDLE;

	m_fSpeed = 5.f;

	return S_OK;
}

_int CSkeletonMinion::Update_GameObject(const _float & fTimeDelta)
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

void CSkeletonMinion::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CSkeletonMinion::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CSkeletonMinion::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CSkeletonMinion::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CSkeletonMinion::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CSkeletonMinion::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CSkeletonMinion::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CSkeletonMinion::Add_Component()
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

	m_vvTextureCom.resize(CSkeletonMinion::SK_END, vector<Engine::CTexture*>(CSkeletonMinion::SK_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_SkeletonMinion_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[SK_IDLE][SD_UP] = pTextureCom;
	m_vvTextureCom[SK_IDLE][SD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[SK_IDLE][SD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[SK_IDLE][SD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_SkeletonMinion_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[SK_RUN][SD_UP] = pTextureCom;
	m_vvTextureCom[SK_RUN][SD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[SK_RUN][SD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[SK_RUN][SD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_SkeletonMinion_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[SK_ATTACK][SD_UP] = pTextureCom;
	m_vvTextureCom[SK_ATTACK][SD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[SK_ATTACK][SD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_SkeletonMinion_AttackLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[SK_ATTACK][SD_LEFT] = pTextureCom;




	return S_OK;
}

void CSkeletonMinion::Animation(const _float & fTimeDelta)
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

void CSkeletonMinion::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CSkeletonMinion::SK_IDLE:
		Idle_State();
		break;
	case CSkeletonMinion::SK_ATTACK:
		Attack_State();
		break;
	case CSkeletonMinion::SK_RUN:
		Run_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CSkeletonMinion::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CSkeletonMinion::SK_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CSkeletonMinion::SK_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	case CSkeletonMinion::SK_RUN:
		Run_Update(fTimeDelta);
		break;
	}

}

void CSkeletonMinion::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CSkeletonMinion::Fitting_Scale_With_Texture(CSkeletonMinion::SKELETONMINION_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CSkeletonMinion::Fitting_Scale_With_Texture(CSkeletonMinion::SKELETONMINION_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CSkeletonMinion::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 1.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(SK_IDLE);
}

void CSkeletonMinion::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 1.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(SK_RUN);
}

void CSkeletonMinion::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 7.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(SK_ATTACK);
}

_int CSkeletonMinion::Idle_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vMyPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDist = vTargetPos - vMyPos;
	_float fDist = D3DXVec3Length(&vDist);
	if (fDist < 1.f)
	{
		m_eCurState = SK_ATTACK;
	}
	if (fDist < 20.f)
	{
		m_eCurState = SK_RUN;
	}

	return 0;
}

_int CSkeletonMinion::Run_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDir = vTargetPos - vPos;
	_float fDist = D3DXVec3Length(&vDir);

	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	if (fDist < 2.f)
		m_eCurState = SK_ATTACK;
	else if (fDist > 20.f)
		m_eCurState = SK_IDLE;

	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
	{
		_vec3 vCamLook;
		Engine::Get_MainCameraLook(&vCamLook);
		vCamLook.y = 0.f;
		D3DXVec3Normalize(&vCamLook, &vCamLook);

		_float fDot = D3DXVec3Dot(&vDir, &vCamLook);

		if (fDot> 0.f)
			m_eCurDir = SD_UP;
		else
			m_eCurDir = SD_DOWN;
	}
	break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
	{
		SKELETONMINION_DIR eUpDown = vDir.z > 0.f ? SD_UP : SD_DOWN;
		SKELETONMINION_DIR eLeftRight = vDir.x > 0.f ? SD_RIGHT : SD_LEFT;

		m_eCurDir = fabs(vDir.z) > fabs(vDir.x) ? eUpDown : eLeftRight;
	}
	break;
	}

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CSkeletonMinion::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = SK_IDLE;
		const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, -1);

		_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		_float fAngleY = Engine::CMyMath::YAngleTransformFromVec(&vDir);
		if (vDir.z < 0.f)
			fAngleY += D3DXToRadian(180.f);

		CLaidEffect* pEffect = CLaidEffect::Create(m_pGraphicDev, L"Texture_WindSlash", L"SkeletonMinionSlash", 7.f, 15.f, 0.1f, &vPos, fAngleY, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"SkeletonMinionSlash", pEffect);

		CBasicEffect* pHitEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vPos, false, 0.f);
		NULL_CHECK_RETURN(pHitEffect, -1);

		CSphereCollider* pCollider = CSphereCollider::Create(m_pGraphicDev, pEffect, pHitEffect, 2.f, L"MonsterAttack", 20);
		Engine::Add_GameObject(L"GameLogic", L"MonsterCollider", pCollider);
		pCollider->Add_Hit_Effect(CSkeletonMinion::SkeletonMinion_Hit_Effect);
	}
	return 0;
}

void CSkeletonMinion::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_bIsDead = true;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SkeletonMinionDead", L"SkeletonMinionDead", 9, 6.f, m_fScale, &vPos, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"SkeletonMinionDead", pDieEffect);

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

void CSkeletonMinion::SkeletonMinion_Hit_Effect()
{
	Engine::PlaySound_(L"ImpactPhysicalHeavy.wav", CSoundMgr::EFFECT);
}

CSkeletonMinion * CSkeletonMinion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos)
{
	CSkeletonMinion* pInstance = new CSkeletonMinion(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CSkeletonMinion::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

