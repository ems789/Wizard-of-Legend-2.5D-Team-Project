#include "stdafx.h"
#include "LightningBoss.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "FireEffect.h"

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

	m_eCurDir = FBD_DOWN;
	m_eCurState = FBS_IDLE;

	m_fSpeed = 80.f;

	m_fJumpPower = 0.f;

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
	//FireEffect(fTimeDelta);

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

	m_vvTextureCom.resize(CLightningBoss::FBS_END, vector<Engine::CTexture*>(CLightningBoss::FBD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_IDLE][FBD_UP]	= pTextureCom;
	m_vvTextureCom[FBS_IDLE][FBD_DOWN]	= pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_IDLE][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_IDLE][FBD_LEFT]	= pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Jump"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_JUMP][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_JUMP][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_JUMP][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_JUMP][FBD_LEFT] = pTextureCom;
	pTextureCom->AddRef();
	
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LightningBoss_Jump"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[FBS_FLOAT][FBD_UP] = pTextureCom;
	m_vvTextureCom[FBS_FLOAT][FBD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_FLOAT][FBD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[FBS_FLOAT][FBD_LEFT] = pTextureCom;
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
	case CLightningBoss::FBS_IDLE:
		Idle_State();
		break;
	case CLightningBoss::FBS_JUMP:
		Jump_State();
		break;
	case CLightningBoss:: FBS_FLOAT:
		Float_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CLightningBoss::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CLightningBoss::FBS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CLightningBoss::FBS_JUMP:
		Jump_Update(fTimeDelta);
		break;
	case CLightningBoss::FBS_FLOAT:
		Float_Update(fTimeDelta);
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

void CLightningBoss::FireEffect(const _float & fTimeDelta)
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

			Engine::Add_GameObject(L"Effect", L"LightningBossBackEffect", pEffect);
		}


	}
}

void CLightningBoss::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(FBS_IDLE);
}

void CLightningBoss::Jump_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_IDLE);
}

void CLightningBoss::Float_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_IDLE);
}

void CLightningBoss::Dead_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	m_tFrame.fFrameSpeed = 2.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(FBS_DEAD);
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

	LightningBoss_DIR eUpDown = fDotL < 0.f ? FBD_DOWN : FBD_UP;
	LightningBoss_DIR eLeftRight = fDotR < 0.f ? FBD_LEFT : FBD_RIGHT;

	m_eCurDir = fabs(fDotL) > fabs(fDotR) ? eUpDown : eLeftRight;

	switch (m_uiPattern)
	{
	case 0:
		m_eCurState = FBS_JUMP;
		break;
	case 1:
		m_eCurState = FBS_FLOAT;
		break;
		//case 1:
		//	m_eCurState = FBS_SPINKICK;
		//	vDist.y = 0.f;
		//	D3DXVec3Normalize(&vDist, &vDist);
		//	D3DXVec3Normalize(&m_vKickDir, &vDist);
		//	++m_uiKickCount;
		//	break;
	}
	++m_uiPattern;
	if (m_uiPattern > 2)
		m_uiPattern = 0;

	return 0;
}

void CLightningBoss::Jump_Update(const _float & fTimeDelta)
{
}

void CLightningBoss::Float_Update(const _float & fTimeDelta)
{
}

void CLightningBoss::Dead_Update(const _float & fTimeDelta)
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


void CLightningBoss::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
	{
		m_eCurState = FBS_DEAD;
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

