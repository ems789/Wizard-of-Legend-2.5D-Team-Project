#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

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

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 0.f;

	m_eCurState = CPlayer::P_IDLE;

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Change_State();
	Update_State(fTimeDelta);
	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return iExit;
}

void CPlayer::Render_GameObjcet()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Render_Texture(static_cast<_ulong>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
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
	CGameObject::Free();
}
