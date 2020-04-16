#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"
#include "Skill.h"
#include "FireBall.h"
#include "WindSlash.h"

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

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 0.f;

	m_eCurState = CPlayer::P_IDLE;

	m_pTransformCom->Move_Pos(Engine::INFO_UP, 0.5f);

	CFireBall*	pFireBall = CFireBall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pFireBall, E_FAIL);
	//Change_Normal_Skill(pFireBall);
	m_vecEquipSkill[1] = pFireBall;

	CWindSlash* pWindSlash = CWindSlash::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pWindSlash, E_FAIL);

	m_vecEquipSkill[0] = pWindSlash;

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	Change_State();
	Update_State(fTimeDelta);
	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);


	Engine::Add_GameObject_To_CollisionList(L"Player", this);

	return iExit;
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Render_Texture(static_cast<_ulong>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Player_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CPlayer::Animation(const _float& fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
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
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	//switch (Engine::Get_MainCamType())
	//{
	//case Engine::CCameraMgr::MAIN_CAM_1ST:
	//case Engine::CCameraMgr::MAIN_CAM_3RD:
	//	Key_Input_For_1stAnd3rdView(fTimeDelta);
	//	break;
	//case Engine::CCameraMgr::MAIN_CAM_QUATER:
	//	Key_Input_For_QuaterView(fTimeDelta);
	//	break;
	//}

	switch(Engine::Get_MainCamType())
	{
	case Engine::CCameraMgr::MAIN_CAM_1ST:
	case Engine::CCameraMgr::MAIN_CAM_3RD:
		Key_Input_For_Move(fTimeDelta);
		Key_Input_For_Attack(fTimeDelta);
		break;
	case Engine::CCameraMgr::MAIN_CAM_QUATER:
		Key_Input_Move_For_QuaterView(fTimeDelta);
		Key_Input_Attack_For_QuaterView(fTimeDelta);
		break;
	}

}

void CPlayer::Key_Input_For_Attack(const _float & fTimeDelta)
{
	if (Engine::MouseDown(Engine::DIM_LB))
	{
		Turn_To_Camera_Look();
		m_vecEquipSkill[0]->Use_Skill(fTimeDelta);
	}

	if (Engine::MousePress(Engine::DIM_RB))
	{
		Turn_To_Camera_Look();
		m_vecEquipSkill[1]->Use_Skill(fTimeDelta);
	}

	if (Engine::KeyDown(DIK_Q))
	{
		Turn_To_Camera_Look();
		m_vecEquipSkill[2]->Use_Skill(fTimeDelta);
	}

}

void CPlayer::Key_Input_Attack_For_QuaterView(const _float & fTimeDelta)
{
	if (Engine::MouseDown(Engine::DIM_LB))
	{
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
	}

	if (Engine::MousePress(Engine::DIM_RB))
	{
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

		m_vecEquipSkill[1]->Use_Skill(fTimeDelta, &vCurPos, &vDir);
	}

	if (Engine::KeyDown(DIK_Q))
	{
		D3DXPLANE Plane = { 0.f, 1.f, 0.f, 0.f };
		_vec3	vPicking;
		Engine::CMyMath::PickingOnPlane(g_hWnd, m_pGraphicDev, &vPicking, &Plane);

		m_vecEquipSkill[2]->Use_Skill(fTimeDelta);
	}
}

void CPlayer::Key_Input_For_Move(const _float & fTimeDelta)
{
	_vec3 vCamLook, vCamRight;
	Engine::Get_MainCameraLook(&vCamLook);
	Engine::Get_MainCameraRight(&vCamRight);

	vCamRight.y = 0;
	vCamLook.y = 0;

	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vCamRight, &vCamRight);

	if (Engine::KeyPress(DIK_W))
	{
		m_pTransformCom->Move_Pos(&(vCamLook * m_fSpeed * fTimeDelta));
		Turn_To_Camera_Look();
	}
	if (Engine::KeyPress(DIK_S))
	{
		m_pTransformCom->Move_Pos(&(vCamLook * -m_fSpeed * fTimeDelta));
		Turn_To_Camera_Look();
	}
	if (Engine::KeyPress(DIK_A))
	{
		m_pTransformCom->Move_Pos(&(vCamRight * -m_fSpeed * fTimeDelta));
		Turn_To_Camera_Look();
	}
	if (Engine::KeyPress(DIK_D))
	{
		m_pTransformCom->Move_Pos(&(vCamRight * m_fSpeed * fTimeDelta));
		Turn_To_Camera_Look();
	}
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

	_vec3 vMove = { 0.f, 0.f, 0.f };

	if (Engine::KeyPress(DIK_W))
		vMove += vCamLook * m_fSpeed * fTimeDelta;
	if (Engine::KeyPress(DIK_S))
		vMove -= vCamLook * m_fSpeed * fTimeDelta;
	if (Engine::KeyPress(DIK_A))
		vMove -= vCamRight * m_fSpeed * fTimeDelta;
	if (Engine::KeyPress(DIK_D))
		vMove += vCamRight * m_fSpeed * fTimeDelta;
	
	m_pTransformCom->Move_Pos(vMove);
}

void CPlayer::Key_Input_For_1stAnd3rdView(const _float & fTimeDelta)
{
	_vec3 vCamLook, vCamRight;
	Engine::Get_MainCameraLook(&vCamLook);
	Engine::Get_MainCameraRight(&vCamRight);

	vCamRight.y = 0;
	vCamLook.y = 0;

	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vCamRight, &vCamRight);

	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;
	if (Engine::KeyPress(DIK_W))
	{
		m_pTransformCom->Move_Pos(&(vCamLook * m_fSpeed * fTimeDelta));
		m_pTransformCom->Set_Angle(&vAngle);
	}
	if (Engine::KeyPress(DIK_S))
	{
		m_pTransformCom->Move_Pos(&(vCamLook * -m_fSpeed * fTimeDelta));
		m_pTransformCom->Set_Angle(&vAngle);
	}
	if (Engine::KeyPress(DIK_A))
	{
		m_pTransformCom->Move_Pos(&(vCamRight * -m_fSpeed * fTimeDelta));
		m_pTransformCom->Set_Angle(&vAngle);
	}
	if (Engine::KeyPress(DIK_D))
	{
		m_pTransformCom->Move_Pos(&(vCamRight * m_fSpeed * fTimeDelta));
		m_pTransformCom->Set_Angle(&vAngle);
	}
}

void CPlayer::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
}

void CPlayer::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
}

void CPlayer::Attack_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
}

void CPlayer::Dash_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
}

void CPlayer::Skill1_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
}

void CPlayer::Skill2_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_tFrame.fFrameSpeed = 10.f;
}

_int CPlayer::Idle_Update(const _float& fTimeDelta)
{
	return 0;
}

_int CPlayer::Run_Update(const _float& fTimeDelta)
{
	return 0;
}

_int CPlayer::Attack_Update(const _float& fTimeDelta)
{
	return 0;
}

_int CPlayer::Dash_Update(const _float& fTimeDelta)
{
	return 0;
}

_int CPlayer::Skill1_Update(const _float& fTimeDelta)
{
	m_vecEquipSkill[0]->Use_Skill(fTimeDelta);
	

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

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	for (_uint i = 0; i < m_vecEquipSkill.size(); ++i)
	{
		if (m_vecEquipSkill[i])
			Engine::Safe_Release(m_vecEquipSkill[i]);
	}

	m_vecEquipSkill.clear();
	CGameObject::Free();
}
