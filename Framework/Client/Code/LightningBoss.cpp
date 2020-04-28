#include "stdafx.h"
#include "LightningBoss.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "LaidFollowingEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "BeamEffect.h"
#include "BasicFollowingEffect.h"
#include "FireEffect.h"
#include "LightningJavelinThrow.h"
#include "LightningFalling.h"
#include "LightningChainDash.h"
#include "NumberEffect.h"

CLightningBoss::CLightningBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CLightningBoss::~CLightningBoss()
{

}

HRESULT CLightningBoss::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fHeight = 1.f;

	m_eCurDir = LBD_DOWN;
	m_eCurState = LBS_IDLE;

	m_fSpeed = 80.f;

	m_uiPattern = 0;

	m_iHP = 1000;
	m_iHPMax = 1000;

	return S_OK;
}

_int CLightningBoss::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Turn_To_Camera_Look();

	Update_State(fTimeDelta);
	Change_State();
	Animation(fTimeDelta);
	LightningEffect(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	return 0;
}

void CLightningBoss::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CLightningBoss::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CLightningBoss::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CLightningBoss::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CLightningBoss::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CLightningBoss::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CLightningBoss::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CLightningBoss::Add_Component()
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

	m_vvTextureCom.resize(CLightningBoss::LBS_END, vector<Engine::CTexture*>(CLightningBoss::LBD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_IDLE][LBD_UP]	= pTextureCom;
	m_vvTextureCom[LBS_IDLE][LBD_DOWN]	= pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_IDLE][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_IDLE][LBD_LEFT]	= pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Jump"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_JUMP][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_JUMP][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_JUMP][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_JUMP][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Float"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_FLOAT][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_FLOAT][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_FLOAT][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_FLOAT][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_JavelinSpawn"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_READY_JAVELIN][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_READY_JAVELIN][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_READY_JAVELIN][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_READY_JAVELIN][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_JavelinThrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_THROW_JAVELIN][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_THROW_JAVELIN][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_THROW_JAVELIN][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_THROW_JAVELIN][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_ChainDashUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_DASH][LBD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_ChainDashDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_DASH][LBD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_ChainDashRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_DASH][LBD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_ChainDashLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_DASH][LBD_LEFT] = pTextureCom;


	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningGirlSlideLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_SLIDE][LBD_LEFT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningGirlSlideRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_SLIDE][LBD_RIGHT] = pTextureCom;

	
	// AoE Ready
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningGirlPBAoEReady"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_PBAOE_Ready][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_PBAOE_Ready][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_PBAOE_Ready][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_PBAOE_Ready][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	// AoE
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningGirlPBAoE"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_PBAOE][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_PBAOE][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_PBAOE][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_PBAOE][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();

	// Dead
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[LBS_DEAD][LBD_UP] = pTextureCom;
	m_vvTextureCom[LBS_DEAD][LBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_DEAD][LBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[LBS_DEAD][LBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	
	return S_OK;
}

void CLightningBoss::Animation(const _float & fTimeDelta)
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

void CLightningBoss::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CLightningBoss::LBS_IDLE:
		Idle_State();
		break;
	case CLightningBoss::LBS_JUMP:
		Jump_State();
		break;
	case CLightningBoss:: LBS_FLOAT:
		Float_State();
		break;
	case CLightningBoss::LBS_READY_JAVELIN:
		JavelinReady_State();
		break;
	case CLightningBoss::LBS_THROW_JAVELIN:
		JavelinThrow_State();
		break;
	case CLightningBoss::LBS_DASH:
		Dash_State();
		break;
	case CLightningBoss::LBS_SLIDE:
		Slide_State();
		break;
	case CLightningBoss::LBS_PBAOE_Ready:
		PBAoEReady_State();
		break;
	case CLightningBoss::LBS_PBAOE:
		PBAoE_State();
		break;
	case CLightningBoss::LBS_DEAD:
		Dead_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CLightningBoss::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CLightningBoss::LBS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_JUMP:
		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame - 1.f) // 마지막 프레임에서 시간을 끄는 용도
			m_tFrame.fFrameSpeed = 3.f;
		Jump_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_FLOAT:
		Float_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_READY_JAVELIN:
		Ready_Javelin_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_THROW_JAVELIN:
		Throw_Javelin_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_DASH:
		Dash_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_SLIDE:
		Slide_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_PBAOE_Ready:
		PBAoEReady_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_PBAOE:
		PBAoE_Update(fTimeDelta);
		break;
	case CLightningBoss::LBS_DEAD:
		Dead_Update(fTimeDelta);
		break;
	}
}

void CLightningBoss::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CLightningBoss::Fitting_Scale_With_Texture(CLightningBoss::LightningBoss_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CLightningBoss::Fitting_Scale_With_Texture(CLightningBoss::LightningBoss_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CLightningBoss::LightningEffect(const _float & fTimeDelta)
{
	m_fLightningEffectTime += fTimeDelta;

	if (m_fLightningEffectTime >= 0.1f)
	{
		m_fLightningEffectTime -= 0.1f;
		
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);

		const _tchar* pTextureTag = nullptr;

		pTextureTag = L"Texture_MiniLightning";

		_float fRight = (rand() % 100 - 50.f) / 800.f;
		_float fUp = (rand() % 100 - 150.f) / 200.f;

		_vec3 vDir = fRight * vRight + fUp * vUp;
		_vec3 vCreatePos = vPos + vLook * 0.1f + vUp * 0.6f + vDir;
		D3DXVec3Normalize(&vDir, &vDir);

		CFireEffect* pEffect = CFireEffect::Create(m_pGraphicDev, pTextureTag, L"MiniLightning", 8.f, 15.f, 0.05f, &vCreatePos, &vDir, 3.f,
			false, 0.f, D3DXCOLOR(1.f, 1.f, 0.f, 1.f), D3DXCOLOR(0.f, 0.f, 0.0f, 0.f));

		Engine::Add_GameObject(L"Effect", L"LightningBossBackEffect", pEffect);

	}
}

void CLightningBoss::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(LBS_IDLE);
}

void CLightningBoss::Jump_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 1.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_JUMP);
}

void CLightningBoss::Float_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_FLOAT);
}

void CLightningBoss::JavelinReady_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_READY_JAVELIN);
}

void CLightningBoss::JavelinThrow_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_THROW_JAVELIN);
}

void CLightningBoss::Dash_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 3.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_DASH);
}

void CLightningBoss::Slide_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 15.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_SLIDE);
}

void CLightningBoss::PBAoEReady_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_PBAOE_Ready);
}

void CLightningBoss::PBAoE_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(LBS_PBAOE);
}

void CLightningBoss::Dead_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(LBS_DEAD);
}

_int CLightningBoss::Idle_Update(const _float & fTimeDelta)
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

	LightningBoss_DIR eUpDown = fDotL < 0.f ? LBD_DOWN : LBD_UP;
	LightningBoss_DIR eLeftRight = fDotR < 0.f ? LBD_LEFT : LBD_RIGHT;

	m_eCurDir = fabs(fDotL) > fabs(fDotR) ? eUpDown : eLeftRight;

	switch (m_uiPattern)
	{
	case 0:
		m_eCurState = LBS_JUMP;
		break;
	case 1:
		m_eCurState = LBS_FLOAT;
		break;
	case 2:
		m_eCurState = LBS_READY_JAVELIN;
		break;
	case 3:
		m_eCurState = LBS_THROW_JAVELIN;
		break;
	case 4:
		m_eCurState = LBS_DASH;
		break;
	case 5:
		m_eCurState = LBS_PBAOE_Ready;
		break;
	case 6:
		m_eCurState = LBS_PBAOE;
		break;
	}
	++m_uiPattern;
	if (m_uiPattern > 6)
		m_uiPattern = 0;

	return 0;
}

void CLightningBoss::Jump_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = LBS_FLOAT;
		m_bLightningLand = false;		
	}

	if (m_tFrame.fCurFrame > 1.f)
	{
		if (!m_bLightningLand)
		{
			CBasicEffect* pLightningBossLand = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningBoss_Land", L"", 7.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);			
			Engine::Add_GameObject(L"Effect", L"LightningBoss_Land", pLightningBossLand);
			m_bLightningLand = true;
		}		
		m_pTransformCom->Move_Pos(AXIS_Y * fTimeDelta * m_fJumpSpeed);
	}
}

void CLightningBoss::Float_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = m_uiJavelinCount < 4 ? LBS_READY_JAVELIN : LBS_IDLE;
		if (m_eCurState == LBS_IDLE)
		{
			CBasicEffect* pLightningBossTeleportAir = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningGirlTeleportAir", L"", 3.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
			Engine::Add_GameObject(L"Effect", L"Texture_LightningGirlTeleportAir", pLightningBossTeleportAir);

			m_pTransformCom->Set_PosY(1.f);
			CBasicEffect* pLightningBossLand = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningBoss_Land", L"", 7.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
			Engine::Add_GameObject(L"Effect", L"LightningBoss_Land", pLightningBossLand);			

			CBasicEffect* pLightningBossTeleportFloor = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningGirlTeleportFloor", L"", 6.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
			Engine::Add_GameObject(L"Effect", L"Texture_LightningGirlTeleportFloor", pLightningBossTeleportFloor);

			m_uiPattern = LBS_DASH - 1; // 바로 대쉬로 넘어감
			m_uiJavelinCount = 0;
			return;
		}
		else
			Engine::PlaySound_(L"ThunderBounce.wav", CSoundMgr::EFFECT);

		// 날아갈 방향 설정
		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);

		_vec3 vDir = vTargetPos - vPos;
		vDir.y = 0.f;

		D3DXVec3Normalize(&m_vTargetDir, &vDir);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pHandPos = vRight * -0.5f + vUp * 0.5f;
		_vec3 vHandPos = vRight * -0.5f + vUp * 0.5f;

		CBasicFollowingEffect* pThunderJavelinSpawn = CBasicFollowingEffect::Create(m_pGraphicDev, L"Texture_ThunderJavelin_Spawn", L"", 4.f, 10.f, 0.035f, &vHandPos, m_pTransformCom, false, 0.f);
		Engine::Add_GameObject(L"Effect", L"Texture_ThunderJavelin_Spawn", pThunderJavelinSpawn);

		CBasicFollowingEffect* pLightningStageEffect = CBasicFollowingEffect::Create(m_pGraphicDev, L"Texture_LightningStageEffect", L"", 16.f, 40.f, 0.035f, &vHandPos, m_pTransformCom, false, 0.f);
		Engine::Add_GameObject(L"Effect", L"Texture_LightningStageEffect", pLightningStageEffect);
	}
}

void CLightningBoss::Ready_Javelin_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		_vec3 vUp = *m_pTransformCom->GetInfo(Engine::INFO_UP);

		_vec3 vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		CLightningJavelinThrow* pLightningJavelin = CLightningJavelinThrow::Create(m_pGraphicDev, vPos, vDir, 20.f);
		Engine::Add_GameObject(L"Effect", L"Texture_ThunderJavelin", pLightningJavelin);

		m_eCurState = LBS_THROW_JAVELIN;
		++m_uiJavelinCount;		
	}
	
	m_pTransformCom->Move_Pos(m_vTargetDir * fTimeDelta * 20.f);
}

void CLightningBoss::Throw_Javelin_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = LBS_FLOAT;
	}
}

void CLightningBoss::Dash_Update(const _float & fTimeDelta)
{
	if (!m_bIsDash)
	{				
		// 날아갈 방향 설정 (자벨린을 다 던지고 대쉬 방향을 지정)
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

		// 대쉬 이펙트
		CLaidFollowingEffect* pLightningChainDash = CLaidFollowingEffect::Create(m_pGraphicDev, L"Texture_LightningChainDash", L"", 1.f, 10.f, 0.05f, &_vec3(0.f, 0.f, 0.f), m_pTransformCom->GetInfo(Engine::INFO_POS), fAngleY, true, 0.3f);
		Engine::Add_GameObject(L"Effect", L"Texture_LightningChainDash", pLightningChainDash);
		Engine::PlaySound_(L"ThunderKnock.wav", CSoundMgr::EFFECT);
		/*
				fAngleY = Engine::CMyMath::YAngleTransformFromVec(&vDir) + D3DXToRadian(90.f);
				CLaidEffect* pLightningFalling = CLaidEffect::Create(m_pGraphicDev, L"Texture_LightningParticles", L"", 1.f, 10.f, 0.05f, m_pTransformCom->GetInfo(Engine::INFO_POS), fAngleY, true, 0.5f);
				Engine::Add_GameObject(L"Effect", L"Texture_LightningParticles", pLightningFalling);*/
		++m_uiDashCount;
		m_bIsDash = true;
	}

	if (m_bAnimFinish)
	{		
		const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vTargetPos = *pTransform->GetInfo(Engine::INFO_POS);

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vLook = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);


		_vec3 vDir = vTargetPos - vPos;
		vDir.y = 0.f;

		D3DXVec3Normalize(&m_vTargetDir, &vDir);

		_float fDotR = D3DXVec3Dot(&vRight, &m_vTargetDir);
		_float fDotL = D3DXVec3Dot(&vLook, &m_vTargetDir);

		LightningBoss_DIR eUpDown = fDotL < 0.f ? LBD_DOWN : LBD_UP;
		LightningBoss_DIR eLeftRight = fDotR < 0.f ? LBD_LEFT : LBD_RIGHT;

		// 슬라이딩 방향은 2개
		m_eCurDir = (vTargetPos.x > vPos.x) ? LBD_RIGHT : LBD_LEFT;

		m_eCurState = LBS_SLIDE;
		m_bIsDash = false;
	}

	m_fLightningTipTime += fTimeDelta;
	if (m_fLightningTipTime > 0.02f)
	{				

		m_fLightningTipTime = 0.f;
		_float fAngleY = Engine::CMyMath::YAngleTransformFromVec(&m_vTargetDir) + D3DXToRadian(90.f);

		CLightningChainDash* pLightningJavelin = CLightningChainDash::Create(m_pGraphicDev, *m_pTransformCom->GetInfo(Engine::INFO_POS), fAngleY, 1.f);
		Engine::Add_GameObject(L"Effect", L"Texture_LightningChainDash2", pLightningJavelin);
	}

	m_pTransformCom->Move_Pos(m_vTargetDir * fTimeDelta * 20.f);
}

void CLightningBoss::Slide_Update(const _float & fTimeDelta)
{	
	if (m_bAnimFinish)
	{
		m_eCurState = m_uiDashCount < 3 ? LBS_DASH : LBS_PBAOE_Ready;

		if (m_eCurState == LBS_PBAOE_Ready)
		{
			m_uiDashCount = 0;
		}
	}
}

void CLightningBoss::PBAoEReady_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
		m_eCurState = LBS_PBAOE;
}

void CLightningBoss::PBAoE_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = LBS_IDLE;
		m_uiPattern = 0.f; // IDLE로 넘어가므로 패턴 초기화
	}
	else
	{
		m_fLightningBaseEffectTime += fTimeDelta;
		if (m_fLightningBaseEffectTime >= 0.2f)
		{			
			CLightningFalling* pLightningFalling = CLightningFalling::Create(m_pGraphicDev, *m_pTransformCom->GetInfo(Engine::INFO_POS), 0.5f);
			Engine::Add_GameObject(L"Effect", L"Texture_LightningFalling", pLightningFalling);

			++m_uiLightningCount;
			m_fLightningBaseEffectTime = 0.f;
		}

		if (m_uiLightningCount >= 20)
		{
			m_bAnimFinish = true;
			m_uiLightningCount = 0;
		}
	}
}

void CLightningBoss::Dead_Update(const _float & fTimeDelta)
{
	m_fDeadTime += fTimeDelta;

	if (m_fDeadTime >= 3.f)
	{
		m_bIsDead = true;

		CBasicEffect* pLightningBossTeleportAir = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningGirlTeleportAir", L"", 3.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
		Engine::Add_GameObject(L"Effect", L"Texture_LightningGirlTeleportAir", pLightningBossTeleportAir);

		m_pTransformCom->Set_PosY(1.f);
		CBasicEffect* pLightningBossLand = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningBoss_Land", L"", 7.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
		Engine::Add_GameObject(L"Effect", L"LightningBoss_Land", pLightningBossLand);

		CBasicEffect* pLightningBossTeleportFloor = CBasicEffect::Create(m_pGraphicDev, L"Texture_LightningGirlTeleportFloor", L"", 6.f, 10.f, 0.06f, m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
		Engine::Add_GameObject(L"Effect", L"Texture_LightningGirlTeleportFloor", pLightningBossTeleportFloor);

		Engine::PlaySound_(L"LightningBurst.wav", CSoundMgr::EFFECT);
	}
}

void CLightningBoss::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;


	CNumberEffect* pNumber = CNumberEffect::Create(m_pGraphicDev, 0.05f, m_pTransformCom->GetInfo(Engine::INFO_POS), 1.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), iAtk);
	Engine::Add_GameObject(L"Effect", L"DamageFont", pNumber);
	if (m_iHP <= 0)
	{
		m_eCurState = LBS_DEAD;
		m_iHP = 0;
	}

}

CLightningBoss* CLightningBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CLightningBoss* pInstance = new CLightningBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CLightningBoss::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

