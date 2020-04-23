#include "stdafx.h"
#include "FireBoss.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "FireEffect.h"
#include "FireThrow.h"
#include "FireRoad.h"
#include "FireKick.h"
#include "FireHeel.h"
#include "FireStomp.h"
#include "FirePoint.h"

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

	m_tSphere.fRadius = 0.3f;

	m_fHeight = 1.f;

	m_eCurDir = FBD_DOWN;
	m_eCurState = FBS_IDLE;

	m_fSpeed = 30.f;

	m_fJumpPower = 0.f;

	m_uiPattern = 0;

	m_iHP = 1000;
	m_iHPMax = 1000;

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
	FireEffect(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	return 0;
}

void CFireBoss::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CFireBoss::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CFireBoss::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CFireBoss::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CFireBoss::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CFireBoss::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CFireBoss::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
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
	m_vvTextureCom[FBS_IDLE][FBD_UP]	= pTextureCom;
	m_vvTextureCom[FBS_IDLE][FBD_DOWN]	= pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_IDLE][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_IDLE][FBD_LEFT]	= pTextureCom;
	pTextureCom->AddRef();


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_AttackLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ATTACK][FBD_LEFT] = pTextureCom;


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH][FBD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH][FBD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH][FBD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH][FBD_LEFT] = pTextureCom;


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashReadyRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH_READY][FBD_RIGHT] = pTextureCom;
	m_vvTextureCom[FBS_DASH_READY][FBD_UP] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashReadyLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH_READY][FBD_LEFT] = pTextureCom;
	m_vvTextureCom[FBS_DASH_READY][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashFinishRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH_FINISH][FBD_RIGHT] = pTextureCom;
	m_vvTextureCom[FBS_DASH_FINISH][FBD_UP] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_DashFinishRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DASH_FINISH][FBD_LEFT] = pTextureCom;
	m_vvTextureCom[FBS_DASH_FINISH][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Heel"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_HEEL][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_HEEL][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_HEEL][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_HEEL][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Hurt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_HURT][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_HURT][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_HURT][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_HURT][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_PointUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_POINT][FBD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_PointDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_POINT][FBD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_PointRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_POINT][FBD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_PointLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_POINT][FBD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Stomp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_STOMP][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_STOMP][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_STOMP][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_STOMP][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Squat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_SQUAT][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_SQUAT][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_SQUAT][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_SQUAT][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Taunt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_TAUNT][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_TAUNT][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_TAUNT][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_TAUNT][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_SpinKickRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_SPINKICK][FBD_RIGHT] = pTextureCom;
	m_vvTextureCom[FBS_SPINKICK][FBD_UP] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_SpinKickLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_SPINKICK][FBD_LEFT] = pTextureCom;
	m_vvTextureCom[FBS_SPINKICK][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_RoundhouseKickRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ROUNDHOUSEKICK][FBD_RIGHT] = pTextureCom;
	m_vvTextureCom[FBS_ROUNDHOUSEKICK][FBD_UP] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_RoundhouseKickRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_ROUNDHOUSEKICK][FBD_LEFT] = pTextureCom;
	m_vvTextureCom[FBS_ROUNDHOUSEKICK][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_FireBoss_Dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_DEAD][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_DEAD][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_DEAD][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_DEAD][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

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
	case CFireBoss::FBS_ATTACK:
		Attack_State();
		break;
	case CFireBoss::FBS_DASH_READY:
		DashReady_State();
		break;
	case CFireBoss::FBS_DASH:
		Dash_State();
		break;
	case CFireBoss::FBS_DASH_FINISH:
		DashFinish_State();
		break;
	case CFireBoss::FBS_SPINKICK:
		SpinKick_State();
		break;
	case CFireBoss::FBS_ROUNDHOUSEKICK:
		RoundKick_State();
		break;
	case CFireBoss::FBS_HEEL:
		Heel_State();
		break;
	case CFireBoss::FBS_STOMP:
		Stomp_State();
		break;
	case CFireBoss::FBS_POINT:
		Point_State();
		break;
	case CFireBoss::FBS_HURT:
		Hurt_State();
		break;
	case CFireBoss::FBS_SQUAT:
		Squat_State();
		break;
	case CFireBoss::FBS_TAUNT:
		Taunt_State();
		break;
	case CFireBoss::FBS_DEAD:
		Dead_State();
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
	case CFireBoss::FBS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_DASH_READY:
		DashReady_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_DASH:
		Dash_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_DASH_FINISH:
		DashFinish_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_SPINKICK:
		SpinKick_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_ROUNDHOUSEKICK:
		RoundKick_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_HEEL:
		Heel_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_STOMP:
		Stomp_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_POINT:
		Point_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_HURT:
		Hurt_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_SQUAT:
		Squat_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_TAUNT:
		Taunt_Update(fTimeDelta);
		break;
	case CFireBoss::FBS_DEAD:
		Dead_Update(fTimeDelta);
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

void CFireBoss::FireEffect(const _float & fTimeDelta)
{
	m_fFireEffectTime += fTimeDelta;

	if (m_fFireEffectTime >= 0.1f)
	{
		m_fFireEffectTime -= 0.1f;
		
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);

		const _tchar* pTextureTag = nullptr;

		for (_uint i = 0; i < 3; ++i)
		{
			switch (rand() % 4)
			{
			case 0:
				pTextureTag = L"Texture_FireParticle1";
				break;
			case 1:
				pTextureTag = L"Texture_FireParticle2";
				break;
			case 2:
				pTextureTag = L"Texture_FireParticle3";
				break;
			case 3:
				pTextureTag = L"Texture_FireParticle4";
				break;
			default:
				break;
			}

			_float fRight = (rand() % 100 - 50.f) / 800.f;
			_float fUp = (rand() % 100 - 150.f) / 200.f;

			_vec3 vDir = fRight * vRight + fUp * vUp;
			_vec3 vCreatePos = vPos + vLook * 0.1f + vUp * 0.6f + vDir;
			D3DXVec3Normalize(&vDir, &vDir);

			CFireEffect* pEffect = CFireEffect::Create(m_pGraphicDev, pTextureTag, L"FireParticle", 6.f, 15.f, 0.05f, &vCreatePos, &vDir, 3.f,
				false, 0.f, D3DXCOLOR(1.f, 0.7f, 0.5f, 1.f), D3DXCOLOR(0.f, 1.f, 2.0f, 0.f));

			Engine::Add_GameObject(L"Effect", L"FireBossBackEffect", pEffect);
		}


	}
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


void CFireBoss::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_ATTACK);
}

void CFireBoss::DashReady_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_DASH_READY);
}

void CFireBoss::Dash_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 4.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_DASH);
}

void CFireBoss::DashFinish_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_DASH_FINISH);
}

void CFireBoss::RoundKick_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_ROUNDHOUSEKICK);
}

void CFireBoss::SpinKick_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 5.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_SPINKICK);
}

void CFireBoss::Heel_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_HEEL);
}

void CFireBoss::Stomp_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_STOMP);
}

void CFireBoss::Point_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_POINT);
}

void CFireBoss::Hurt_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_HURT);
}

void CFireBoss::Squat_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_SQUAT);
}

void CFireBoss::Taunt_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_TAUNT);
}

void CFireBoss::Dead_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_DEAD);
}

_int CFireBoss::Idle_Update(const _float & fTimeDelta)
{
	m_fIdleTime += fTimeDelta;

	if (m_fIdleTime < 1.f)
		return 0;

	m_fIdleTime = 0.f;

	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos	= *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vMyPos		= *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
	_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);

	_vec3 vDist		= vTargetPos - vMyPos;
	_float fDist	= D3DXVec3Length(&vDist);

	_float fDotR = D3DXVec3Dot(&vRight, &vDist);
	_float fDotL = D3DXVec3Dot(&vLook, &vDist);

	FireBoss_DIR eUpDown = fDotL < 0.f ? FBD_DOWN : FBD_UP;
	FireBoss_DIR eLeftRight = fDotR < 0.f ? FBD_LEFT : FBD_RIGHT;

	m_eCurDir = fabs(fDotL) > fabs(fDotR) ? eUpDown : eLeftRight;

	switch (m_uiPattern)
	{
	case 0:
		m_eCurState = FBS_ATTACK;
		break;
	case 1:
		m_eCurState = FBS_DASH_READY;
		break;
	case 2:
		m_eCurState = FBS_SPINKICK;
		vDist.y = 0.f;
		D3DXVec3Normalize(&vDist, &vDist);
		D3DXVec3Normalize(&m_vKickDir, &vDist);
		++m_uiKickCount;
		Engine::PlaySound_(L"FlameLight.wav", CSoundMgr::EFFECT);
		break;
	case 3:
		m_eCurState = FBS_HEEL;
		break;
	case 4:
		m_eCurState = FBS_STOMP;
		break;
	case 5:
		m_eCurState = FBS_POINT;
		break;
	case 6: 
		m_eCurState = FBS_HURT;
		break;
	case 7:
		m_eCurState = FBS_SQUAT;
		break;
	case 8:
		m_eCurState = FBS_TAUNT;
		break;
	}
	
	++m_uiPattern;
	if (m_uiPattern > 8)
		m_uiPattern = 0;

	return 0;
}


_int CFireBoss::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_IDLE;
		m_bAttack = false;
	}

	else if (false == m_bAttack && m_tFrame.fCurFrame >= 1.f)
	{
		m_bAttack = true;

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos		= *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vRight	= *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		_vec3 vUp		= *m_pTransformCom->GetInfo(Engine::INFO_UP);
		
		_float fBentDecal = 1.5f;

		CFireThrow* pFireThrow = CFireThrow::Create(m_pGraphicDev, vPos, vTargetPos, vRight, fBentDecal, 20.f);
		Engine::Add_GameObject(L"GameLogic", L"FireThrow", pFireThrow);

		pFireThrow = CFireThrow::Create(m_pGraphicDev, vPos, vTargetPos, vRight + vUp, fBentDecal, 20.f);
		Engine::Add_GameObject(L"GameLogic", L"FireThrow", pFireThrow);

		pFireThrow = CFireThrow::Create(m_pGraphicDev, vPos, vTargetPos, vUp, fBentDecal, 20.f);
		Engine::Add_GameObject(L"GameLogic", L"FireThrow", pFireThrow);

		pFireThrow = CFireThrow::Create(m_pGraphicDev, vPos, vTargetPos, -vRight + vUp, fBentDecal, 20.f);
		Engine::Add_GameObject(L"GameLogic", L"FireThrow", pFireThrow);

		pFireThrow = CFireThrow::Create(m_pGraphicDev, vPos, vTargetPos, -vRight, fBentDecal, 20.f);
		Engine::Add_GameObject(L"GameLogic", L"FireThrow", pFireThrow);

		Engine::PlaySound_(L"FireSpinLoop.wav", CSoundMgr::EFFECT);
	}

	return 0;
}

void CFireBoss::DashReady_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_DASH;
		++m_uiDashCount;

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

		FireBoss_DIR eUpDown	= fDotL < 0.f ? FBD_DOWN : FBD_UP;
		FireBoss_DIR eLeftRight = fDotR < 0.f ? FBD_LEFT : FBD_RIGHT;

		m_eCurDir = fabs(fDotL) > fabs(fDotR) ? eUpDown : eLeftRight;
		Engine::PlaySound_(L"FireBurn.wav", CSoundMgr::EFFECT);
	}
}

void CFireBoss::Dash_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = m_uiDashCount < 3 ? FBS_DASH_READY : FBS_DASH_FINISH;
		return;
	}

	m_pTransformCom->Move_Pos(m_vDashDir * m_fSpeed * fTimeDelta);

	m_fFireRoadItv += fTimeDelta;
	if (m_fFireRoadItv >= 0.025f)
	{
		m_fFireRoadItv -= 0.025f;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CFireRoad* pFireRoad = CFireRoad::Create(m_pGraphicDev, vPos, 1.f);
		Engine::Add_GameObject(L"GameLogic", L"FireRoad", pFireRoad);
	}
}

void CFireBoss::DashFinish_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_IDLE;
		m_uiDashCount = 0;
		return;
	}
}

void CFireBoss::RoundKick_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		if (m_uiKickCount > 3)
		{
			m_eCurState = FBS_IDLE;
			m_fKickAngle = 0.f;
			m_uiKickCount = 0;
		}
		else
		{
			m_eCurState = FBS_SPINKICK;
			m_fKickAngle = 0.f;
			++m_uiKickCount;
			Engine::PlaySound_(L"FlameLight.wav", CSoundMgr::EFFECT);
		}
		return;
	}

	if (m_tFrame.fCurFrame < 1.f)
		 m_pTransformCom->Move_Pos(m_vKickDir * fTimeDelta * m_fSpeed);

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &m_vKickDir, &AXIS_Y);

	_vec3 vFirePos;
	_matrix matRotAxix;

	_float fFrameEndTime = (m_tFrame.fMaxFrame / m_tFrame.fFrameSpeed);	//	MaxFrame 까지 가는데 걸리는 시간.
	_float fAngleSpeed = D3DXToRadian(180.f) / fFrameEndTime;
	m_fKickAngle += fTimeDelta * fAngleSpeed;

	m_fKickFireTime += fTimeDelta;
	if (m_fKickFireTime > 0.05f)
	{
		m_fKickFireTime -= 0.05f;
		D3DXMatrixRotationAxis(&matRotAxix, &(-AXIS_Y), m_fKickAngle);
		D3DXVec3TransformNormal(&vFirePos, &(-vRight), &matRotAxix);
		vFirePos *= 4.f;
		vFirePos += vPos;

		CFireKick* pFireKick = CFireKick::Create(m_pGraphicDev, vFirePos, 0.1f);

		Engine::Add_GameObject(L"GameLogic", L"FireKick", pFireKick);

	}
}

void CFireBoss::SpinKick_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{

		if (m_uiKickCount > 3)
		{
			m_eCurState = FBS_IDLE;
			m_fKickAngle = 0.f;
			m_uiKickCount = 0;
		}
		else
		{
			m_eCurState = FBS_ROUNDHOUSEKICK;
			m_fKickAngle = 0.f;
			++m_uiKickCount;
			Engine::PlaySound_(L"FlameLight.wav", CSoundMgr::EFFECT);
		}
		return;
	}
	if (m_tFrame.fCurFrame < 1.f)
		m_pTransformCom->Move_Pos(m_vKickDir * fTimeDelta * m_fSpeed);

	_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &m_vKickDir, &AXIS_Y);
	
	_vec3 vFirePos;
	_matrix matRotAxix;

	_float fFrameEndTime = (m_tFrame.fMaxFrame / m_tFrame.fFrameSpeed);	//	MaxFrame 까지 가는데 걸리는 시간.
	_float fAngleSpeed = D3DXToRadian(180.f) / fFrameEndTime;
	m_fKickAngle += fTimeDelta * fAngleSpeed;
	
	m_fKickFireTime += fTimeDelta;
	if (m_fKickFireTime > 0.05f)
	{
		m_fKickFireTime -= 0.05f;
		D3DXMatrixRotationAxis(&matRotAxix, &AXIS_Y, m_fKickAngle);
		D3DXVec3TransformNormal(&vFirePos, &vRight, &matRotAxix);
		vFirePos *= 4.f;
		vFirePos += vPos;

		CFireKick* pFireKick = CFireKick::Create(m_pGraphicDev, vFirePos, 0.1f);

		Engine::Add_GameObject(L"GameLogic", L"FireKick", pFireKick);

	}

}

void CFireBoss::Heel_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_IDLE;
		m_pTransformCom->Set_PosY(m_fHeight);
		m_bHeelInit = false;
		m_bHeelFire = false;
		return;
	}

	if (false == m_bHeelInit)
	{
		m_bHeelInit = true;
		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);
		
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		m_vHeelDir = vTargetPos - vPos;
		_float fDist = D3DXVec3Length(&m_vHeelDir);
		_float fFrameEndTime = (m_tFrame.fMaxFrame / m_tFrame.fFrameSpeed) / 2;	//	MaxFrame 까지 가는데 걸리는 시간.
		m_fHeelSpeed = fDist / fFrameEndTime;

		m_vHeelDir.y = 0.f;
		D3DXVec3Normalize(&m_vHeelDir, &m_vHeelDir);
		m_fJumpPower = 1.f;
		m_fFallSpeed = 2.f * m_fJumpPower / fFrameEndTime;
	}

	if (m_tFrame.fCurFrame < 1.f)
	{
		m_fJumpPower -= m_fFallSpeed * fTimeDelta;

		m_pTransformCom->Move_Pos(m_vHeelDir * m_fHeelSpeed * fTimeDelta + AXIS_Y * m_fJumpPower);
	}
	else
	{
		m_pTransformCom->Set_PosY(m_fHeight);
		if (false == m_bHeelFire)
		{
			m_bHeelFire = true;
			_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
			_vec3 vLook = m_vHeelDir;
			_vec3 vRight;
			D3DXVec3Cross(&vRight, &vLook, &AXIS_Y);

			_vec3 vDir1 = vLook;
			D3DXVec3Normalize(&vDir1, &vDir1);
			CFireHeel* pFireHeel = CFireHeel::Create(m_pGraphicDev, vPos, vDir1, 20.f, 1.f);
			Engine::Add_GameObject(L"GameLogic", L"FireHeel", pFireHeel);

			_vec3 vDir2 = vLook * 2.f + vRight;
			D3DXVec3Normalize(&vDir2, &vDir2);
			pFireHeel = CFireHeel::Create(m_pGraphicDev, vPos, vDir2, 20.f, 1.f);
			Engine::Add_GameObject(L"GameLogic", L"FireHeel", pFireHeel);

			_vec3 vDir3 = vLook * 2.f - vRight;
			D3DXVec3Normalize(&vDir3, &vDir3);
			pFireHeel = CFireHeel::Create(m_pGraphicDev, vPos, vDir3, 20.f, 1.f);
			Engine::Add_GameObject(L"GameLogic", L"FireHeel", pFireHeel);

			Engine::PlaySound_(L"ImpactFire.wav", CSoundMgr::EFFECT);
		}
	}

}

void CFireBoss::Stomp_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_IDLE;
		m_bFireStomp = false;
		return;
	}

	if (false == m_bFireStomp && m_tFrame.fCurFrame > 1.f)
	{
		m_bFireStomp = true;

		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		CFireStomp* pFireStomp = CFireStomp::Create(m_pGraphicDev, vTargetPos, 50);
		Engine::Add_GameObject(L"GameLogic", L"FireStomp", pFireStomp);

		pFireStomp = CFireStomp::Create(m_pGraphicDev, vTargetPos + 5.f * vLook, 50);
		Engine::Add_GameObject(L"GameLogic", L"FireStomp", pFireStomp);

		pFireStomp = CFireStomp::Create(m_pGraphicDev, vTargetPos - 5.f * vLook, 50);
		Engine::Add_GameObject(L"GameLogic", L"FireStomp", pFireStomp);

		pFireStomp = CFireStomp::Create(m_pGraphicDev, vTargetPos + 5.f * vRight, 50);
		Engine::Add_GameObject(L"GameLogic", L"FireStomp", pFireStomp);

		pFireStomp = CFireStomp::Create(m_pGraphicDev, vTargetPos - 5.f * vRight, 50);
		Engine::Add_GameObject(L"GameLogic", L"FireStomp", pFireStomp);
	}
}

void CFireBoss::Point_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_IDLE;
		m_bFirePoint = false;
		return;
	}

	if (false == m_bFirePoint)
	{
		m_bFirePoint = true;

		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);
		_vec3 vDir = vTargetPos - vPos;
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);
		_vec3 vRight;
		D3DXVec3Cross(&vRight, &vUp, &vDir);

		CFirePoint* pFirePoint = CFirePoint::Create(m_pGraphicDev, vPos, vDir, vRight, 20.f);
		Engine::Add_GameObject(L"GameLogic", L"FirePoint", pFirePoint);
	}
}

void CFireBoss::Hurt_Update(const _float & fTimeDelta)
{
	m_fHurtTime += fTimeDelta;
	if (m_fHurtTime >= 3.f)
	{
		m_fHurtTime = 0.f;
		m_eCurState = FBS_IDLE;
	}
}

void CFireBoss::Squat_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish) 
	{
		m_eCurState = FBS_IDLE;
		return;
	}
}

void CFireBoss::Taunt_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = FBS_IDLE;
		return;
	}
}

void CFireBoss::Dead_Update(const _float & fTimeDelta)
{
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

void CFireBoss::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_eCurState = FBS_DEAD;
		m_iHP = 0;
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

