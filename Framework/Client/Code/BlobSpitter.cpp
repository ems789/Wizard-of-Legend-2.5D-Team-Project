#include "stdafx.h"
#include "BlobSpitter.h"

#include "Export_Function.h"
#include "LaidEffect.h"
#include "SphereCollider.h"
#include "BasicEffect.h"
#include "Coin.h"
#include "BlobBullet.h"
#include "NumberEffect.h"

CBlobSpitter::CBlobSpitter(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CBlobSpitter::~CBlobSpitter()
{

}

HRESULT CBlobSpitter::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	m_fHeight = 1.5f;

	m_eCurDir = BSD_RIGHT;
	m_eCurState = BSS_IDLE;

	m_fSpeed = 5.f;

	return S_OK;
}

_int CBlobSpitter::Update_GameObject(const _float& fTimeDelta)
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

void CBlobSpitter::Render_GameObject()
{
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CBlobSpitter::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CBlobSpitter::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CBlobSpitter::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CBlobSpitter::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CBlobSpitter::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CBlobSpitter::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CBlobSpitter::Add_Component()
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

	m_vvTextureCom.resize(CBlobSpitter::BSS_END, vector<Engine::CTexture*>(CBlobSpitter::BSD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BlobSpitter_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[BSS_IDLE][BSD_LEFT]	= pTextureCom;
	m_vvTextureCom[BSS_IDLE][BSD_RIGHT]	= pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BlobSpitter_Run_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[BSS_RUN][BSD_LEFT] = pTextureCom;
	
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BlobSpitter_Run_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[BSS_RUN][BSD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BlobSpitter_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[BSS_ATTACK][BSD_LEFT] = pTextureCom;
	m_vvTextureCom[BSS_ATTACK][BSD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BlobSpitter_Hurt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[BSS_HURT][BSD_LEFT] = pTextureCom;
	m_vvTextureCom[BSS_HURT][BSD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();


	return S_OK;
}

void CBlobSpitter::Animation(const _float & fTimeDelta)
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

void CBlobSpitter::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CBlobSpitter::BSS_IDLE:
		Idle_State();
		break;
	case CBlobSpitter::BSS_RUN:
		Run_State();
		break;
	case CBlobSpitter::BSS_ATTACK:
		Attack_State();
		break;
	case CBlobSpitter::BSS_HURT:
		Hurt_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CBlobSpitter::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CBlobSpitter::BSS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CBlobSpitter::BSS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CBlobSpitter::BSS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	case CBlobSpitter::BSS_HURT:
		Hurt_Update(fTimeDelta);
		break;
	}
}

void CBlobSpitter::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CBlobSpitter::Fitting_Scale_With_Texture(CBlobSpitter::BLOB_SPITTER_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CBlobSpitter::Fitting_Scale_With_Texture(CBlobSpitter::BLOB_SPITTER_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };

	m_pTransformCom->Set_Scale(vScale);
}

void CBlobSpitter::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(BSS_IDLE);
}

void CBlobSpitter::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(BSS_RUN);
}

void CBlobSpitter::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(BSS_ATTACK);
}

void CBlobSpitter::Hurt_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 0.5f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(BSS_HURT);
}

_int CBlobSpitter::Idle_Update(const _float & fTimeDelta)
{
	if (m_bFireCool)
	{
		m_fFireCool += fTimeDelta;
		if (m_fFireCool < 1.f)
			return 0;
		else
		{
			m_fFireCool = 0.f;
			m_bFireCool = false;
		}
	}

	const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTargetTransform, -1);

	_vec3 vTargetPos	= *pTargetTransform->GetInfo(Engine::INFO_POS);
	_vec3 vMyPos		= *m_pTransformCom->GetInfo(Engine::INFO_POS);

	_vec3 vDist		= vTargetPos - vMyPos;
	_float fDist	= D3DXVec3Length(&vDist);
	if (fDist < 5.f)
	{
		m_eCurState = BSS_ATTACK;
	}
	if (fDist < 20.f)
	{
		m_eCurState = BSS_RUN;
	}

	return 0;
}

_int CBlobSpitter::Run_Update(const _float & fTimeDelta)
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
		m_eCurState = BSS_ATTACK;
	else if (fDist > 20.f)
		m_eCurState = BSS_IDLE;

	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
	{
		_vec3 vCamRight;
		Engine::Get_MainCameraRight(&vCamRight);
		vCamRight.y = 0.f;
		D3DXVec3Normalize(&vCamRight, &vCamRight);

		_float fDotR = D3DXVec3Dot(&vDir, &vCamRight);

		m_eCurDir = fDotR > 0 ? BSD_RIGHT : BSD_LEFT;

	}
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		m_eCurDir = vDir.x > 0.f ? BSD_RIGHT : BSD_LEFT;
		break;
	}

	m_pTransformCom->Move_Pos(vDir * fTimeDelta * m_fSpeed);

	return 0;
}

_int CBlobSpitter::Attack_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_bFire = false;
		if (m_uiFireCnt >= m_uiFireMax)
		{
			m_eCurState = BSS_IDLE;
			m_uiFireCnt = 0;
			m_bFireCool = true;
		}
		else
		{
			Attack_State();
		}
	}

	if (false == m_bFire)
	{
		m_bFire = true;
		++m_uiFireCnt;

		const Engine::CTransform* pTargetTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, -1);

		_vec3 vTargetPos = *pTargetTransform->GetInfo(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vRight = *m_pTransformCom->GetInfo(Engine::INFO_RIGHT);
		D3DXVec3Normalize(&vRight, &vRight);
		vPos += vRight * 0.5f;

		CBlobBullet* pBlobBullet = CBlobBullet::Create(m_pGraphicDev, vPos, vTargetPos, _vec3(0.f, 1.f, 0.f), 5.f, 5.f);
		Engine::Add_GameObject(L"GameLogic", L"BlobBullet", pBlobBullet);
	}


	return 0;
}

_int CBlobSpitter::Hurt_Update(const _float & fTimeDelta)
{
	if (m_bAnimFinish)
		m_eCurState = BSS_IDLE;
	m_pTransformCom->Move_Pos(m_vHurtDir * fTimeDelta);

	return 0;
}

void CBlobSpitter::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_vHurtDir = *m_pTransformCom->GetInfo(Engine::INFO_POS) - *pAtkPos;
	m_vHurtDir.y = 0.f;
	D3DXVec3Normalize(&m_vHurtDir, &m_vHurtDir);
	m_eCurState = BSS_HURT;

	Change_State();
	m_iHP -= iAtk;



	CNumberEffect* pNumber = CNumberEffect::Create(m_pGraphicDev, 0.05f, m_pTransformCom->GetInfo(Engine::INFO_POS), 1.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), iAtk);
	Engine::Add_GameObject(L"Effect", L"DamageFont", pNumber);
	if (m_iHP <= 0)
	{
		m_bIsDead = true;
		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		CBasicEffect* pDieEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_BlobSpitter_Dead", L"GelemDieEffect", 7, 10.f, m_fScale, &vPos, false, 0.f);
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
	}

}

void CBlobSpitter::BlobSpitter_Hit_Effect()
{
	Engine::PlaySound_(L"ImpactPhysicalHeavy.wav", CSoundMgr::EFFECT);
}

CBlobSpitter* CBlobSpitter::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CBlobSpitter* pInstance = new CBlobSpitter(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_vec3(pPos->x, pInstance->m_fHeight, pPos->z));

	return pInstance;
}

void CBlobSpitter::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	Engine::CGameObject::Free();
}

