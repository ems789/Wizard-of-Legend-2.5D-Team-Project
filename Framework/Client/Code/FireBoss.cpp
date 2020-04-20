#include "stdafx.h"
#include "FireBoss.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"

CFireBoss::CFireBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CFireBoss::~CFireBoss()
{

}

HRESULT CFireBoss::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fHeight = 1.5f;

	m_eCurDir = FBD_DOWN;
	m_eCurState = FBS_IDLE;

	m_fSpeed = 5.f;

	return S_OK;
}

_int CFireBoss::Update_GameObject(const _float& fTimeDelta)
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

void CFireBoss::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CFireBoss::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CFireBoss::Add_Component()
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

	m_vvTextureCom.resize(CFireBoss::FBS_END, vector<Engine::CTexture*>(CFireBoss::FBD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_IDLE][FBD_UP]		= pTextureCom;
	m_vvTextureCom[FBS_IDLE][FBD_DOWN]	= pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_IDLE][FBD_LEFT]	= pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_IDLE][FBD_RIGHT]	= pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Run"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_RUN][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_RUN][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_RUN][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_RUN][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_RIGHT] = pTextureCom;

	return S_OK;
}

void CFireBoss::Animation(const _float & fTimeDelta)
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

void CFireBoss::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CFireBoss::FBS_IDLE:
		Idle_State();
		break;
	case CFireBoss::FBS_RUN:
		Run_State();
		break;
	case CFireBoss::FBS_ATTACK:
		Attack_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CFireBoss::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CFireBoss::FBS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	}
}

void CFireBoss::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CFireBoss::Fitting_Scale_With_Texture(CFireBoss::FireBoss_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CFireBoss::Fitting_Scale_With_Texture(CFireBoss::FireBoss_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CFireBoss::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(FBS_IDLE);
}

void CFireBoss::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(FBS_RUN);
}

void CFireBoss::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_ATTACK);
}

_int CFireBoss::Idle_Update(const _float & fTimeDelta)
{
	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos	= *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vMyPos		= *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDist		= vTargetPos - vMyPos;
	_float fDist	= D3DXVec3Length(&vDist);
	if (fDist < 1.f)
	{
		m_eCurState = FBS_ATTACK;
	}
	if (fDist < 20.f)
	{
		m_eCurState = FBS_RUN;
	}

	return 0;
}

_int CFireBoss::Run_Update(const _float & fTimeDelta)
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
		m_eCurState = FBS_ATTACK;
	else if (fDist > 20.f)
		m_eCurState = FBS_IDLE;

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
			m_eCurDir = FBD_UP;
		else
			m_eCurDir = FBD_DOWN;
	}
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
	{
		FireBoss_DIR eUpDown		= vDir.z > 0.f ? FBD_UP : FBD_DOWN;
		FireBoss_DIR eLeftRight	= vDir.x > 0.f ? FBD_RIGHT : FBD_LEFT;

		m_eCurDir = fabs(vDir.z) > fabs(vDir.x) ? eUpDown : eLeftRight;
	}
		break;
	}

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CFireBoss::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{

	}
	return 0;
}

void CFireBoss::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_bIsDead = true;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_FireBoss_Death", L"FireBossDieEffect", 6, 10.f, m_fScale, &vPos, false, 0.f);
		Engine::Add_GameObject(L"GameLogic", L"FireBossDieEffect", pDieEffect);
	}

}

CFireBoss* CFireBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CFireBoss* pInstance = new CFireBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CFireBoss::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

