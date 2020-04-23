#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"
#include "Skill.h"
#include "FireBall.h"
#include "WindSlash.h"
#include "FireEffect.h"
#include "MeteorStrike.h"
#include "GuidedFireBall.h"
#include "LaidEffect.h"
#include "BasicEffect.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vecEquipSkill.push_back(nullptr);
	m_vecEquipSkill.push_back(nullptr);
	m_vecEquipSkill.push_back(nullptr);
	m_vecEquipSkill.push_back(nullptr);

	ZeroMemory(&m_vPrePos, sizeof(_vec3));

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 0.f;

	m_eCurState = CPlayer::P_IDLE;
	m_eCurDir = CPlayer::PD_UP;
	m_iPreCamState = Engine::Get_MainCamType();
	m_fScale = 0.04f;

	m_tSphere.fRadius = 0.3f;
	m_pTransformCom->Move_Pos(Engine::INFO_UP, 0.5f);

	CGuidedFireBall* pGuidedFireBall = CGuidedFireBall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGuidedFireBall, E_FAIL);
	m_vecEquipSkill[3] = pGuidedFireBall;

	CMeteorStrike* pMeteorStrike = CMeteorStrike::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMeteorStrike, E_FAIL);
	m_vecEquipSkill[2] = pMeteorStrike;

	CFireBall*	pFireBall = CFireBall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pFireBall, E_FAIL);
	//Change_Normal_Skill(pFireBall);
	m_vecEquipSkill[1] = pFireBall;

	CWindSlash* pWindSlash = CWindSlash::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pWindSlash, E_FAIL);
	m_vecEquipSkill[0] = pWindSlash;

	m_fDashSpeed = 30.f;

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	//Key_Input(fTimeDelta);
	Update_State(fTimeDelta);
	Change_State();
	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_tSphere.vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Player", this);

	return iExit;
}

void CPlayer::Render_GameObject()
{
	CGameObject::Update_GameObject(0.f);
	m_vPrePos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pTextureCom->Render_Texture(static_cast<_ulong>(m_tFrame.fCurFrame));
	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture(static_cast<_ulong>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

_int CPlayer::Change_Normal_Skill(Engine::CSkill * pSkill)
{
	if (m_vecEquipSkill[1] != nullptr)
		Engine::Safe_Release(m_vecEquipSkill[1]);

	m_vecEquipSkill[1] = pSkill;
	m_vecEquipSkill[1]->AddRef();

	return 0;
}

_int CPlayer::Change_Upgrade_Skill(Engine::CSkill * pSkill)
{
	if (m_vecEquipSkill[2] != nullptr)
		Engine::Safe_Release(m_vecEquipSkill[2]);

	m_vecEquipSkill[2] = pSkill;
	m_vecEquipSkill[2]->AddRef();

	return 0;
}

HRESULT CPlayer::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_Idle"));
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

	m_vvTextureCom.resize(CPlayer::P_END, vector<Engine::CTexture*>(CPlayer::PD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_IdleUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_IDLE][PD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_IdleDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_IDLE][PD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_IdleRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_IDLE][PD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_IdleLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_IDLE][PD_LEFT] = pTextureCom;

	//	RunTexture
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_RunUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_RUN][PD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_RunDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_RUN][PD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_RunRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_RUN][PD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_RunLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_RUN][PD_LEFT] = pTextureCom;



	//	Dash Texture
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_DashUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_DASH][PD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_DashDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_DASH][PD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_DashRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_DASH][PD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_DashLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_DASH][PD_LEFT] = pTextureCom;


	//	Attack Texture
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_AttackUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK][PD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_AttackDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK][PD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_AttackRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK][PD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_AttackLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK][PD_LEFT] = pTextureCom;



	//	Attack2 Texture
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_Attack2Up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK2][PD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_Attack2Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK2][PD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_Attack2Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK2][PD_RIGHT] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_Attack2Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[P_ATTACK2][PD_LEFT] = pTextureCom;

	//	Skill1 Texture
	m_vvTextureCom[P_SKILL1][PD_UP] = m_vvTextureCom[P_ATTACK2][PD_UP];
	m_vvTextureCom[P_SKILL1][PD_UP]->AddRef();
	m_vvTextureCom[P_SKILL1][PD_DOWN] = m_vvTextureCom[P_ATTACK2][PD_DOWN];
	m_vvTextureCom[P_SKILL1][PD_DOWN]->AddRef();
	m_vvTextureCom[P_SKILL1][PD_RIGHT] = m_vvTextureCom[P_ATTACK2][PD_RIGHT];
	m_vvTextureCom[P_SKILL1][PD_RIGHT]->AddRef();
	m_vvTextureCom[P_SKILL1][PD_LEFT] = m_vvTextureCom[P_ATTACK2][PD_LEFT];
	m_vvTextureCom[P_SKILL1][PD_LEFT]->AddRef();

	return S_OK;
}

void CPlayer::Animation(const _float& fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
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

void CPlayer::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CPlayer::P_IDLE:
		Idle_State();
		break;
	case CPlayer::P_RUN:
		Run_State();
		break;
	case CPlayer::P_ATTACK:
		Attack_State();
		break;
	case CPlayer::P_ATTACK2:
		Attack2_State();
		break;
	case CPlayer::P_DASH:
		Dash_State();
		break;
	case CPlayer::P_SKILL1:
		Skill1_State();
		break;
	case CPlayer::P_SKILL2:
		Skill2_State();
		break;
	}

	m_ePreState = m_eCurState;
}

_int CPlayer::Update_State(const _float& fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CPlayer::P_IDLE:
		iExit = Idle_Update(fTimeDelta);
		break;
	case CPlayer::P_RUN:
		iExit = Run_Update(fTimeDelta);
		break;
	case CPlayer::P_ATTACK:
	case CPlayer::P_ATTACK2:
		iExit = Attack_Update(fTimeDelta);
		break;
	case CPlayer::P_DASH:
		iExit = Dash_Update(fTimeDelta);
		break;
	case CPlayer::P_SKILL1:
		iExit = Skill1_Update(fTimeDelta);
		break;
	case CPlayer::P_SKILL2:
		iExit = Skill2_Update(fTimeDelta);
		break;
	}

	return iExit;
}

void CPlayer::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CPlayer::Fitting_Scale_With_Texture(CPlayer::PLAYER_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	//if (!m_bDir)
	//	vScale.x = -vScale.x;

	m_pTransformCom->Set_Scale(vScale);
}

void CPlayer::Fitting_Scale_With_Texture(CPlayer::PLAYER_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	//if (!m_bDir)
	//	vScale.x = -vScale.x;

	m_pTransformCom->Set_Scale(vScale);
}

void CPlayer::PickingPlane(_vec3 * pOut)
{
	D3DXPLANE Plane = { 0.f, 1.f, 0.f, 0.f };
	_vec2	vMouse;

	_matrix	matProj, matView;
	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);
	Engine::CMyMath::ClientMousePos(g_hWnd, &vMouse);

	Engine::CMyMath::PickingOnPlane(pOut, &vMouse, WINCX, WINCY, &matProj, &matView, &Plane);
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CPlayer::P_IDLE:
	case CPlayer::P_RUN:
		Key_Input_Move(fTimeDelta);
		Key_Input_Attack(fTimeDelta);
		Key_Input_Dash(fTimeDelta);
		Key_Input_Skill1(fTimeDelta);
		break;
	case CPlayer::P_ATTACK:
	case CPlayer::P_ATTACK2:
		Key_Input_Attack(fTimeDelta);
		break;
	case CPlayer::P_DASH:
		break;
	case CPlayer::P_SKILL1:
		break;
	case CPlayer::P_SKILL2:
		break;
	}
}

void CPlayer::Key_Input_Attack(const _float & fTimeDelta)
{
	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
		Key_Input_Attack_For_1stAnd3rdView(fTimeDelta);
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		Key_Input_Attack_For_QuaterView(fTimeDelta);
		break;
	}
}

void CPlayer::Key_Input_Attack_For_1stAnd3rdView(const _float & fTimeDelta)
{
	if (Engine::MouseDown(Engine::DIM_LB))
	{
		switch (m_dwAttackCnt)
		{
		case 0:
			++m_dwAttackCnt;
			m_eCurState = P_ATTACK;
			break;
		case 1:
			if (m_tFrame.fCurFrame > 5.f)
			{
				++m_dwAttackCnt;
				m_eCurState = P_ATTACK2;
			}
			else
				return;
			break;
		case 2:
			if (m_tFrame.fCurFrame > 5.f)
			{
				+m_dwAttackCnt;
				m_eCurState = P_ATTACK;
			}
			else
				return;
			break;
		default:
			return;
		}

		m_eCurDir = PD_UP;
		//m_bDir = true;
		m_vecEquipSkill[0]->Use_Skill(fTimeDelta);
		m_vAttackDir = *m_pTransformCom->GetInfo(Engine::INFO_LOOK);
	}



}

void CPlayer::Key_Input_Attack_For_QuaterView(const _float & fTimeDelta)
{
	if (Engine::MouseDown(Engine::DIM_LB))
	{
		switch (m_dwAttackCnt)
		{
		case 0:
			++m_dwAttackCnt;
			m_eCurState = P_ATTACK;
			break;
		case 1:
			if (m_tFrame.fCurFrame > 5.f)
			{
				++m_dwAttackCnt;
				m_eCurState = P_ATTACK2;
			}
			else
				return;
			break;
		case 2:
			if (m_tFrame.fCurFrame > 5.f)
			{
				+m_dwAttackCnt;
				m_eCurState = P_ATTACK;
			}
			else
				return;
			break;
		default:
			return;
		}

		D3DXPLANE Plane = { 0.f, 1.f, 0.f, 0.f };
		_vec3	vPicking;
		_vec2	vMouse;

		_matrix	matProj, matView;
		Engine::Get_MainCamera()->Get_View(&matView);
		Engine::Get_MainCamera()->Get_Projection(&matProj);
		Engine::CMyMath::ClientMousePos(g_hWnd, &vMouse);

		Engine::CMyMath::PickingOnPlane(&vPicking, &vMouse, WINCX, WINCY, &matProj, &matView, &Plane);

		_vec3 vCurPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vDir = vPicking - vCurPos;
		vDir.y = 0;
		D3DXVec3Normalize(&vDir, &vDir);

		m_vecEquipSkill[0]->Use_Skill(fTimeDelta, &vCurPos, &vDir);

		CPlayer::PLAYER_DIR eUpDown = vDir.z > 0.f ? PD_UP : PD_DOWN;
		CPlayer::PLAYER_DIR eLeftRight = vDir.x > 0.f ? PD_RIGHT : PD_LEFT;

		m_eCurDir = fabs(vDir.x) > fabs(vDir.z) ? eLeftRight : eUpDown;
		//m_bDir = vDir.x < 0.f ? false : true;

		m_vAttackDir = vDir;
	}


}

void CPlayer::Key_Input_Move(const _float & fTimeDelta)
{
	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
		Key_Input_Move_For_1stAnd3rdView(fTimeDelta);
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		Key_Input_Move_For_QuaterView(fTimeDelta);
		break;
	}
}

void CPlayer::Key_Input_Move_For_1stAnd3rdView(const _float & fTimeDelta)
{
	_vec3 vCamLook, vCamRight;
	Engine::Get_MainCameraLook(&vCamLook);
	Engine::Get_MainCameraRight(&vCamRight);

	vCamRight.y = 0;
	vCamLook.y = 0;

	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vCamRight, &vCamRight);

	m_vMove = { 0.f, 0.f, 0.f };

	//_bool bDir = m_bDir;
	if (Engine::KeyPress(DIK_W))
	{
		m_vMove += vCamLook;
		m_eCurDir = PD_UP;
	}
	if (Engine::KeyPress(DIK_S))
	{
		m_vMove -= vCamLook;
		m_eCurDir = PD_DOWN;
	}
	if (Engine::KeyPress(DIK_A))
	{
		m_vMove -= vCamRight;
		m_eCurDir = PD_LEFT;
	}
	if (Engine::KeyPress(DIK_D))
	{
		m_vMove += vCamRight;
		m_eCurDir = PD_RIGHT;
	}

	//if (m_bDir != bDir)
	//{
	//	_vec3 vScale = m_pTransformCom->GetScaleRef();
	//	m_pTransformCom->Set_ScaleX(-vScale.x);
	//	m_bDir = bDir;
	//}

	if (0.f != m_vMove.x || 0.f != m_vMove.y || 0.f != m_vMove.z)
	{
		D3DXVec3Normalize(&m_vMove, &m_vMove);
		m_vDashDir = m_vMove;
		m_pTransformCom->Move_Pos(m_vMove * m_fSpeed * fTimeDelta);
		m_eCurState = P_RUN;
	}
	else
		m_eCurState = P_IDLE;
}

void CPlayer::Key_Input_Move_For_QuaterView(const _float & fTimeDelta)
{
	_vec3 vCamLook, vCamRight;
	Engine::Get_MainCameraLook(&vCamLook);
	Engine::Get_MainCameraRight(&vCamRight);

	vCamRight.y = 0;
	vCamLook.y = 0;

	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vCamRight, &vCamRight);

	m_vMove = { 0.f, 0.f, 0.f };

	//_bool bDir = m_bDir;
	if (Engine::KeyPress(DIK_W))
	{
		m_vMove += vCamLook;
		m_eCurDir = PD_UP;
	}
	if (Engine::KeyPress(DIK_S))
	{
		m_vMove -= vCamLook;
		m_eCurDir = PD_DOWN;
	}
	if (Engine::KeyPress(DIK_A))
	{
		m_vMove -= vCamRight;
		m_eCurDir = PD_LEFT;
		//bDir = false;
	}
	if (Engine::KeyPress(DIK_D))
	{
		m_vMove += vCamRight;
		m_eCurDir = PD_RIGHT;
		//bDir = true;
	}

	//if (m_bDir != bDir)
	//{
	//	_vec3 vScale = m_pTransformCom->GetScaleRef();
	//	m_pTransformCom->Set_ScaleX(-vScale.x);
	//	m_bDir = bDir;
	//}

	if (0.f != m_vMove.x || 0.f != m_vMove.y || 0.f != m_vMove.z)
	{
		D3DXVec3Normalize(&m_vMove, &m_vMove);
		m_vDashDir = m_vMove;
		m_pTransformCom->Move_Pos(m_vMove * m_fSpeed * fTimeDelta);
		m_eCurState = P_RUN;
	}
	else
		m_eCurState = P_IDLE;

}

void CPlayer::Key_Input_Dash(const _float & fTimeDelta)
{
	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
		Key_Input_Dash_For_1stAnd3rdView(fTimeDelta);
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		Key_Input_Dash_For_QuaterView(fTimeDelta);
		break;
	}
}

void CPlayer::Key_Input_Dash_For_1stAnd3rdView(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_SPACE))
	{
		const _tchar* pTextureTag = nullptr;
		switch (m_eCurDir)
		{
		case CPlayer::PD_UP:
			pTextureTag = L"Texture_AirRingUp";
			break;
		case CPlayer::PD_DOWN:
			pTextureTag = L"Texture_AirRingDown";
			break;
		case CPlayer::PD_RIGHT:
			pTextureTag = L"Texture_AirRingRight";
			break;
		case CPlayer::PD_LEFT:
			pTextureTag = L"Texture_AirRingLeft";
			break;
		}
		CBasicEffect* pAirRing = CBasicEffect::Create(m_pGraphicDev, pTextureTag, L"PlayerDash", 4, 10.f, 0.1f,
			m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
		Engine::Add_GameObject(L"Effect", L"PlayerDash", pAirRing);
		m_eCurState = CPlayer::P_DASH;
	}
}

void CPlayer::Key_Input_Dash_For_QuaterView(const _float & fTimeDleta)
{
	if (Engine::KeyDown(DIK_SPACE))
	{
		const _tchar* pTextureTag = nullptr;
		switch (m_eCurDir)
		{
		case CPlayer::PD_UP:
			pTextureTag = L"Texture_AirRingUp";
			break;
		case CPlayer::PD_DOWN:
			pTextureTag = L"Texture_AirRingDown";
			break;
		case CPlayer::PD_RIGHT:
			pTextureTag = L"Texture_AirRingRight";
			break;
		case CPlayer::PD_LEFT:
			pTextureTag = L"Texture_AirRingLeft";
			break;
		}
		CBasicEffect* pAirRing = CBasicEffect::Create(m_pGraphicDev, pTextureTag, L"PlayerDash", 4, 10.f, 0.1f,
			m_pTransformCom->GetInfo(Engine::INFO_POS), false, 0.f);
		Engine::Add_GameObject(L"Effect", L"PlayerDash", pAirRing);
		m_eCurState = CPlayer::P_DASH;
	}
}

void CPlayer::Key_Input_Skill1(const _float & fTimeDelta)
{
	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
		Key_Input_Skill1_For_1stAnd3rdView(fTimeDelta);
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		Key_Input_Skill1_For_QuaterView(fTimeDelta);
		break;
	}
}

void CPlayer::Key_Input_Skill1_For_1stAnd3rdView(const _float & fTimeDelta)
{
	if (Engine::MousePress(Engine::DIM_RB))
	{
		if (m_vecEquipSkill[1])
		{
			_int iMotion = m_vecEquipSkill[1]->Use_Skill(fTimeDelta);
			if (1 == iMotion)
			{
				m_eCurState = P_SKILL1;
				m_eCurDir = PD_UP;
			}
		}
	}

	if (Engine::KeyDown(DIK_Q))
	{
		if (m_vecEquipSkill[2])
		{
			_int iMotion = m_vecEquipSkill[2]->Use_Skill(fTimeDelta);
			if (1 == iMotion)
			{
				m_eCurState = P_SKILL1;
				m_eCurDir = PD_UP;
			}
		}
	}

	if (Engine::KeyDown(DIK_E))
	{
		if (m_vecEquipSkill[3])
		{
			_int iMotion = m_vecEquipSkill[3]->Use_Skill(fTimeDelta);
			if (1 == iMotion)
			{
				m_eCurState = P_SKILL1;
				m_eCurDir = PD_UP;
			}
		}
	}

}

void CPlayer::Key_Input_Skill1_For_QuaterView(const _float & fTimeDelta)
{
	if (Engine::MousePress(Engine::DIM_RB))
	{
		_vec3	vPicking;
		PickingPlane(&vPicking);

		_vec3 vCurPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vDir = vPicking - vCurPos;
		vDir.y = 0;
		D3DXVec3Normalize(&vDir, &vDir);

		if (m_vecEquipSkill[1])
		{
			_int iMotion = m_vecEquipSkill[1]->Use_Skill(fTimeDelta, &vCurPos, &vDir);
			if (1 == iMotion)
			{
				m_eCurState = P_SKILL1;

				CPlayer::PLAYER_DIR eUpDown		= vDir.z > 0.f ? PD_UP : PD_DOWN;
				CPlayer::PLAYER_DIR eLeftRight	= vDir.x > 0.f ? PD_RIGHT : PD_LEFT;

				m_eCurDir = fabs(vDir.x) > fabs(vDir.z) ? eLeftRight : eUpDown;

			}
		}
		//else if (1 == iMotion)
		//{

		//}

	}

	if (Engine::KeyDown(DIK_Q))
	{
		_vec3	vPicking;
		PickingPlane(&vPicking);

		_vec3 vCurPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vDir = vPicking - vCurPos;
		vDir.y = 0;
		D3DXVec3Normalize(&vDir, &vDir);

		if (m_vecEquipSkill[2])
		{
			_int iMotion = m_vecEquipSkill[2]->Use_Skill(fTimeDelta, &vCurPos, &vDir);

			if (1 == iMotion)
			{
				m_eCurState = P_SKILL1;

				CPlayer::PLAYER_DIR eUpDown = vDir.z > 0.f ? PD_UP : PD_DOWN;
				CPlayer::PLAYER_DIR eLeftRight = vDir.x > 0.f ? PD_RIGHT : PD_LEFT;

				m_eCurDir = fabs(vDir.x) > fabs(vDir.z) ? eLeftRight : eUpDown;

			}
		}
	}

	if (Engine::KeyDown(DIK_E))
	{
		_vec3	vPicking;
		PickingPlane(&vPicking);

		_vec3 vCurPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);
		_vec3 vDir = vPicking - vCurPos;
		vDir.y = 0;
		D3DXVec3Normalize(&vDir, &vDir);

		if (m_vecEquipSkill[3])
		{
			_int iMotion = m_vecEquipSkill[3]->Use_Skill(fTimeDelta, &vCurPos, &vDir);

			if (1 == iMotion)
			{
				m_eCurState = P_SKILL1;

				CPlayer::PLAYER_DIR eUpDown = vDir.z > 0.f ? PD_UP : PD_DOWN;
				CPlayer::PLAYER_DIR eLeftRight = vDir.x > 0.f ? PD_RIGHT : PD_LEFT;

				m_eCurDir = fabs(vDir.x) > fabs(vDir.z) ? eLeftRight : eUpDown;

			}
		}
	}
}

void CPlayer::Key_Input_Skill2(const _float & fTimeDelta)
{
	switch (Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
		Key_Input_Skill2_For_1stAnd3rdView(fTimeDelta);
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		Key_Input_Skill2_For_QuaterView(fTimeDelta);
		break;
	}
}

void CPlayer::Key_Input_Skill2_For_1stAnd3rdView(const _float & fTimeDelta)
{
}

void CPlayer::Key_Input_Skill2_For_QuaterView(const _float & fTimeDelta)
{
}

void CPlayer::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(P_IDLE);
}

void CPlayer::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;

	Fitting_Scale_With_Texture(P_RUN);
}

void CPlayer::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 8.f;
	m_tFrame.fFrameSpeed = 20.f;
	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(P_ATTACK);
}

void CPlayer::Attack2_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 9.f;
	m_tFrame.fFrameSpeed = 20.f;
	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(P_ATTACK2);
}

void CPlayer::Dash_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 9.f;
	m_tFrame.fFrameSpeed = 30.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(P_DASH);
}

void CPlayer::Skill1_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 9.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

	Fitting_Scale_With_Texture(P_SKILL1);
}

void CPlayer::Skill2_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
	m_bAnimFinish = false;
	m_bAnimRepeat = false;
}

_int CPlayer::Idle_Update(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);


	return 0;
}

_int CPlayer::Run_Update(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);



	return 0;
}

_int CPlayer::Attack_Update(const _float& fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = CPlayer::P_IDLE;
		m_dwAttackCnt = 0;
		return 0;
	}

	if (m_tFrame.fCurFrame < 5.f)
	{
		m_pTransformCom->Move_Pos(m_vAttackDir * fTimeDelta);
	}

	Key_Input(fTimeDelta);


	return 0;
}

_int CPlayer::Dash_Update(const _float& fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = CPlayer::P_IDLE;
		return 0;
	}

	if (m_tFrame.fCurFrame <= 6.f)
	{
		m_pTransformCom->Move_Pos(m_vDashDir * fTimeDelta * m_fDashSpeed);
	}

	return 0;
}

_int CPlayer::Skill1_Update(const _float& fTimeDelta)
{
	if (m_bAnimFinish)
	{
		m_eCurState = CPlayer::P_IDLE;
		m_dwAttackCnt = 0;
		return 0;
	}

	Key_Input(fTimeDelta);

	return 0;
}

_int CPlayer::Skill2_Update(const _float& fTimeDelta)
{
	return 0;
}

void CPlayer::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CPlayer::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CPlayer::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CPlayer::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CPlayer::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

const _vec3 * CPlayer::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CPlayer::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP < 0)
		m_iHP = 0;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	for (_uint i = 0; i < m_vvTextureCom.size(); ++i)
	{
		for (_uint j = 0; j < m_vvTextureCom[i].size(); ++j)
			Engine::Safe_Release(m_vvTextureCom[i][j]);
	}

	for (_uint i = 0; i < m_vecEquipSkill.size(); ++i)
	{
		if (m_vecEquipSkill[i])
			Engine::Safe_Release(m_vecEquipSkill[i]);
	}

	m_vecEquipSkill.clear();
	CGameObject::Free();
}
