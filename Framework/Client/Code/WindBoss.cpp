#include "stdafx.h"
#include "WindBoss.h"

#include "SphereCollider.h"
#include "WindBall.h"
#include "WindSpin.h"
#include "WindTornado.h"
#include "Export_Function.h"
#include "BasicFollowingEffect.h"
#include "BasicEffect.h"
#include "UI.h"
#include "NumberEffect.h"

CWindBoss::CWindBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CWindBoss::~CWindBoss()
{
}

HRESULT CWindBoss::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 0.7f;

	m_fHeight = 1.5f;

	m_eCurDir = WBD_DOWN;
	m_eCurState = WBS_IDLE;

	m_fSpeed = 20.f;

	m_uiPattern = 0;

	m_iHP = 900;
	m_iHPMax = 900;

	return S_OK;
}

_int CWindBoss::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Turn_To_Camera_Look();
	Update_State(fTimeDelta);
	Change_State();
	Animation(fTimeDelta);
	WindEffect(fTimeDelta);

	/*if (m_bIsDead)
	return 0;*/

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	return 0;
}

void CWindBoss::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CWindBoss::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CWindBoss::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CWindBoss::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CWindBoss::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CWindBoss::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CWindBoss::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CWindBoss::Add_Component()
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

	m_vvTextureCom.resize(CWindBoss::WBS_END, vector<Engine::CTexture*>(CWindBoss::WBD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_IDLE][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_IDLE][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_IDLE][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_IDLE][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_ATTACK][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_ATTACK][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_ATTACK][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_ATTACK][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Attack2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_ATTACK2][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_ATTACK2][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_ATTACK2][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_ATTACK2][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_ATTACK3][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_ATTACK3][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_ATTACK3][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_ATTACK3][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_ReadyCharge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_READYCHARGE][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_READYCHARGE][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_READYCHARGE][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_READYCHARGE][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_DashUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_DASH][WBD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_DashDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_DASH][WBD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_DashRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_DASH][WBD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_DashLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_DASH][WBD_LEFT] = pTextureCom;

	///////////////////////
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_SpinEnd"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_SPINEND][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_SPINEND][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_SPINEND][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_SPINEND][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Spin"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_SPIN][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_SPIN][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_SPIN][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_SPIN][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Hurt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_HURT][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_HURT][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_HURT][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_HURT][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	/*pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Taunt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_TAUNT][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_TAUNT][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_TAUNT][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_TAUNT][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();*/


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_DEAD][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_DEAD][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_DEAD][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_DEAD][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	//float
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindBoss_Float"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[WBS_FlOAT][WBD_UP] = pTextureCom;
	m_vvTextureCom[WBS_FlOAT][WBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_FlOAT][WBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[WBS_FlOAT][WBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	return S_OK;
}

void CWindBoss::Animation(const _float & fTimeDelta)
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

void CWindBoss::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CWindBoss::WBS_IDLE:
		Idle_State();
		break;
	case CWindBoss::WBS_ATTACK:
		Attack_State();
		break;
	case CWindBoss::WBS_ATTACK2:
		Attack_State2();
		break;
	case CWindBoss::WBS_ATTACK3:
		Attack_State3();
		break;
	case CWindBoss::WBS_READYCHARGE:
		DashReady_State();//charge
		break;
	case CWindBoss::WBS_DASH:
		Dash_State();
		break;
	case CWindBoss::WBS_SPIN:
		Spin_State();
		break;
	case CWindBoss::WBS_HURT:
		Hurt_State();
		break;
	case CWindBoss::WBS_FlOAT:
		Float_State();
		break;
		//case CWindBoss::WBS_TAUNT:
		//	Taunt_State();
		//	break;
	case CWindBoss::WBS_SPINEND:
		SpinEnd_State();
		break;
	case CWindBoss::WBS_DEAD:
		Dead_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CWindBoss::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CWindBoss::WBS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CWindBoss::WBS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	case CWindBoss::WBS_ATTACK2:
		Attack_Update2(fTimeDelta);
		break;
	case CWindBoss::WBS_ATTACK3:
		Attack_Update3(fTimeDelta);
		break;
	case CWindBoss::WBS_READYCHARGE:
		DashReady_Update(fTimeDelta); //charge
		break;
	case CWindBoss::WBS_DASH:
		Dash_Update(fTimeDelta);
		break;
	case CWindBoss::WBS_FlOAT:
		Float_Update(fTimeDelta);
		break;
	case CWindBoss::WBS_SPIN:
		Spin_Update(fTimeDelta);
		break;
	case CWindBoss::WBS_SPINEND:
		SpinEnd_Update(fTimeDelta);
		break;
	case CWindBoss::WBS_HURT:
		Hurt_Update(fTimeDelta);
		break;
		//case CWindBoss::WBS_TAUNT:
		//	Taunt_Update(fTimeDelta);
		//	break;
	case CWindBoss::WBS_DEAD:
		Dead_Update(fTimeDelta);
		break;
	}
}

void CWindBoss::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CWindBoss::Fitting_Scale_With_Texture(CWindBoss::WindBoss_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CWindBoss::Fitting_Scale_With_Texture(CWindBoss::WindBoss_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CWindBoss::WindEffect(const _float & fTimeDelta)
{

}

void CWindBoss::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 15.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(WBS_IDLE);
}

void CWindBoss::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_ATTACK);
}

void CWindBoss::Attack_State2()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_ATTACK2);
}

void CWindBoss::Attack_State3()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_ATTACK3);
}

void CWindBoss::DashReady_State() //charge
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 11.f;
	m_tFrame.fFrameSpeed = 3.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_READYCHARGE);
}

void CWindBoss::Dash_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 4.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_DASH);
}

void CWindBoss::Spin_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 8.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(WBS_SPIN);
}

void CWindBoss::SpinEnd_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 5.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_SPINEND);
}


void CWindBoss::Hurt_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_HURT);
}

void CWindBoss::Float_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 0.1f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(WBS_FlOAT);
}

void CWindBoss::Dead_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(WBS_DEAD);
}

_int CWindBoss::Idle_Update(const _float & fTimeDelta)
{
	m_fIdleTime += fTimeDelta;

	if (m_fIdleTime > 3.f)
		return 0;

	switch (m_uiPattern)
	{
	case 0:
		m_eCurState = WBS_ATTACK;
		break;
	case 1:
		m_eCurState = WBS_ATTACK2;
		break;
	case 2:
		m_eCurState = WBS_ATTACK3;
		break;
	case 3:
		m_eCurState = WBS_READYCHARGE;
		break;
	case 4:
		m_eCurState = WBS_DASH;
		break;
	case 5:
		m_eCurState = WBS_FlOAT;
		break;
	case 6:
		m_eCurState = WBS_SPIN;
		break;
	case 7:
		m_eCurState = WBS_SPINEND;
		break;
	case 8:
		m_eCurState = WBS_HURT;
		break;
		//case 8:
		//	m_eCurState = FBS_TAUNT;
		//	break;
	}

	++m_uiPattern;
	if (m_uiPattern > 8)
		m_uiPattern = 0;

	return 0;
}

_int CWindBoss::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = WBS_ATTACK2;
		m_bAttack = false;
	}
	else if (false == m_bAttack && m_tFrame.fCurFrame >= 1.f)
	{
		m_bAttack = true;

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);

		_vec3 vDir = vTargetPos - vPos;


		//windball 持失
		CWindBall* pWindBall = CWindBall::Create(m_pGraphicDev, vPos, vTargetPos, vDir, 15.f, 6.f, 5.f);
		Engine::Add_GameObject(L"GameLogic", L"WindBall", pWindBall);
		Engine::PlaySound_(L"WindBlastDeep.wav", CSoundMgr::EFFECT);
	}



	return 0;
}

_int CWindBoss::Attack_Update2(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = WBS_ATTACK3;
		m_bAttack = false;
	}
	else if (false == m_bAttack && m_tFrame.fCurFrame >= 1.f)
	{
		m_bAttack = true;

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);

		_vec3 vDir = vTargetPos - vPos;


		//windball 持失
		CWindBall* pWindBall = CWindBall::Create(m_pGraphicDev, vPos, vTargetPos, vDir, 15.f, 6.f, 5.f);
		Engine::Add_GameObject(L"GameLogic", L"WindBall", pWindBall);
		Engine::PlaySound_(L"WindBlastSharp.wav", CSoundMgr::EFFECT);
	}

	return 0;
}

_int CWindBoss::Attack_Update3(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = WBS_READYCHARGE;
		Engine::PlaySound_(L"WindHowl.wav", CSoundMgr::EFFECT);
		m_bAttack = false;
	}
	else if (false == m_bAttack && m_tFrame.fCurFrame >= 1.f)
	{
		m_bAttack = true;

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);

		_vec3 vDir = vTargetPos - vPos;


		//windball 持失
		CWindBall* pWindBall = CWindBall::Create(m_pGraphicDev, vPos, vTargetPos, vDir, 15.f, 6.f, 5.f);
		Engine::Add_GameObject(L"GameLogic", L"WindBall", pWindBall);
		Engine::PlaySound_(L"WindBlastDeep.wav", CSoundMgr::EFFECT);
	}

	return 0;
}

void CWindBoss::DashReady_Update(const _float & fTimeDelta)
{
	//charge
	if (m_bAnimFinish)
	{
		m_eCurState = WBS_DASH;
		Engine::PlaySound_(L"WindBlastDeep.wav", CSoundMgr::EFFECT);
		Engine::PlaySound_(L"WindBlastDeep.wav", CSoundMgr::EFFECT);
		Engine::PlaySound_(L"WindBlastDeep.wav", CSoundMgr::EFFECT);
		Engine::PlaySound_(L"WindBlastDeep.wav", CSoundMgr::EFFECT);

		return;
	}

	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
	_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);

	_vec3 vDir = vTargetPos - vPos;
	vDir.y = 0.f;

	D3DXVec3Normalize(&m_vDashDir, &vDir);

	_float fDotR = D3DXVec3Dot(&vRight, &m_vDashDir);
	_float fDotL = D3DXVec3Dot(&vLook, &m_vDashDir);

	WindBoss_DIR eUpDown = fDotL < 0.f ? WBD_DOWN : WBD_UP;
	WindBoss_DIR eLeftRight = fDotR < 0.f ? WBD_LEFT : WBD_RIGHT;

	m_eCurDir = fabs(fDotL) > fabs(fDotR) ? eUpDown : eLeftRight;

	CBasicFollowingEffect* pEffect = CBasicFollowingEffect::Create(m_pGraphicDev, L"Texture_WindReadyEffect", L"WindReadyEffect", 11.f, 8.f, 0.05f, &_vec3(0.f, 1.f, 0.f), m_pTransformCom, false, 1.f);
	Add_GameObject(L"Effect", L"WindReadyEffect", pEffect);


	CSphereCollider* pColl = CSphereCollider::Create(m_pGraphicDev, pEffect, nullptr, 1.5f, L"MonsterAttack", 10);
	Add_GameObject(L"GameLogic", L"MonsterAttack", pColl);
	m_bFillar = false;


}

_int CWindBoss::Dash_Update(const _float & fTimeDelta)
{
	/*m_fDashTime += fTimeDelta;

	if (m_fDashTime > 1.f)
	{
	m_eCurState = WBS_FlOAT;
	m_fDashTime = 0.f;
	return 0;
	}*/
	CBasicFollowingEffect* pEffect = CBasicFollowingEffect::Create(m_pGraphicDev, L"Texture_Tornado", L"Tornado", 12.f, 8.f, 0.05f, &_vec3(0.f, 1.f, 0.f), m_pTransformCom, false, 1.f);
	Add_GameObject(L"Effect", L"Tornado", pEffect);

	CSphereCollider* pColl = CSphereCollider::Create(m_pGraphicDev, pEffect, nullptr, 1.5f, L"MonsterAttack", 10);
	Add_GameObject(L"GameLogic", L"MonsterAttack", pColl);
	if (m_bAnimFinish)
	{
		m_eCurState = WBS_FlOAT;
		return 0;
	}
	m_pTransformCom->Move_Pos(m_vDashDir * 40.f * fTimeDelta);

	return 0;
}

void CWindBoss::Float_Update(const _float & fTimeDelta)
{
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDir = { 0.f, 1.f, 0.f };
	D3DXVec3Normalize(&m_vFloatDir, &vDir);
	m_pTransformCom->Move_Pos(m_vFloatDir * 2.f * fTimeDelta);

	if (vPos.y > 7.5f)
	{
		m_eCurState = WBS_SPIN;
		return;
	}
}

_int CWindBoss::Spin_Update(const _float & fTimeDelta)
{
	m_fSpinTime += fTimeDelta;
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	_vec3 vDir = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

	_vec3 mvDir = vPos - vTargetPos;
	D3DXVec3Normalize(&mvDir, &mvDir);
	mvDir.y = 0;
	if (m_fSpinTime > 2.5f)
	{
		m_eCurState = WBS_SPINEND;
		m_fSpinTime = 0.f;
		m_bSpin = true;
		return 0;
	}
	if (m_bSpin == true)
	{

		CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindSpin", L"WindSpin", 6.f, 12.f, 0.065f, &vPos, true, 2.5f);
		Add_GameObject(L"Effect", L"WindSpin", pEffect);
		pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindSpin", L"WindSpin", 6.f, 8.f, 0.1f, &vPos, true, 2.5f);
		Add_GameObject(L"Effect", L"WindSpin", pEffect);
		pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindSpin", L"WindSpin", 6.f, 6.f, 0.14f, &vPos, true, 2.5f);
		Add_GameObject(L"Effect", L"WindSpin", pEffect);
		pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindSpin", L"WindSpin", 6.f, 15.f, 0.19f, &vPos, true, 2.5f);
		Add_GameObject(L"Effect", L"WindSpin", pEffect);
		Engine::PlaySound_(L"WindVortexLoop.wav", CSoundMgr::EFFECT);
		//	Engine::PlaySound_(L"TornadoLoop.wav", CSoundMgr::EFFECT);
		m_bSpin = false;
	}
	Engine::Player_Move_Pos(&(mvDir * 10.f * fTimeDelta));

	return 0;
}

void CWindBoss::SpinEnd_Update(const _float & fTimeDelta)
{
	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	//Engine::PlaySound_(L"AquaWhoosh.wav", CSoundMgr::EFFECT);
	_vec3 vDir = { 0.f, -1.f, 0.f };
	D3DXVec3Normalize(&m_vFloatDir, &vDir);
	m_pTransformCom->Move_Pos(m_vFloatDir * 10.f * fTimeDelta);
	if (m_bAnimFinish)
	{
		if (m_bFillar == true)
		{
			CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_WindPillar", L"WindPillar", 10.f, 4.f, 0.05f, &(vPos + _vec3(0.f, 1.f, 0.f)), false, 1.5f);
			Add_GameObject(L"Effect", L"WindPillar", pEffect);
			Engine::PlaySound_(L"WindWindup.wav", CSoundMgr::EFFECT);
			Engine::PlaySound_(L"WindWindup.wav", CSoundMgr::EFFECT);
			Engine::PlaySound_(L"WindWindup.wav", CSoundMgr::EFFECT);


			CSphereCollider* pColl = CSphereCollider::Create(m_pGraphicDev, pEffect, nullptr, 1.5f, L"MonsterAttack", 15);
			Add_GameObject(L"GameLogic", L"MonsterAttack", pColl);
			m_bFillar = false;
		}

		if (vPos.y < 1.5)
		{
			vPos.y = 1.5;
			m_eCurState = WBS_ATTACK;

			return;
		}
		m_bFillar = true;
	}
}

void CWindBoss::Hurt_Update(const _float & fTimeDelta)
{
	m_fHurtTime += fTimeDelta;
	if (m_fHurtTime >= 3.f)
	{
		m_fHurtTime = 0.f;
		m_eCurState = WBS_IDLE;
	}
	Engine::PlaySound_(L"AirBossHurt0.wav", CSoundMgr::EFFECT);
}

void CWindBoss::Dead_Update(const _float & fTimeDelta)
{
	CUI::GetInstance()->ShowOffBossUI();

	m_fDeadTime += fTimeDelta;

	if (m_fDeadTime >= 3.f)
	{
		m_bIsDead = true;

		const _tchar* pTextureTag = nullptr;
		switch (rand() % 4)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}

	}
}

void CWindBoss::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;


	CNumberEffect* pNumber = CNumberEffect::Create(m_pGraphicDev, 0.05f, m_pTransformCom->GetInfo(Engine::INFO_POS), 1.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), iAtk);
	Engine::Add_GameObject(L"Effect", L"DamageFont", pNumber);

	if (m_iHP <= 0)
	{
		m_eCurState = WBS_DEAD;
		m_iHP = 0;
	}
}

CWindBoss * CWindBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos)
{
	CWindBoss* pInstance = new CWindBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CWindBoss::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}
