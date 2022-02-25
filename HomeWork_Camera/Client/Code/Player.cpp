#include "stdafx.h"
#include "Player.h"
#include "Camera.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Info(Engine::INFO_POS, &_vec3(0.f, 1.f, 5.f));

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CPlayer::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	switch (m_eCurCamState)
	{
	case CPlayer::PC_1ST:
		Key_Input_1stView(fTimeDelta);
		break;
	case CPlayer::PC_3RD:
		Key_Input_3rdView(fTimeDelta);
		break;
	case CPlayer::PC_QUATER:
		Key_Input_QuaterView(fTimeDelta);
		break;
	}
}

void CPlayer::Key_Input_1stView(const _float & fTimeDelta)
{
	Engine::CCamera* pMainCam = Engine::Get_MainCam();

	pMainCam->Get_Info(Engine::INFO_LOOK, &m_vDir);
	pMainCam->Get_Info(Engine::INFO_RIGHT, &m_vRight);

	_vec3 vAngle;
	pMainCam->Get_Angle(&vAngle);
	vAngle.x = 0.f;
	m_pTransformCom->Set_Angle(&vAngle);

	m_vDir.y = 0.f;
	m_vRight.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	if (GetAsyncKeyState('W') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));

	if (GetAsyncKeyState('S') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));

	if (GetAsyncKeyState('D') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vRight * m_fSpeed * fTimeDelta));

	if (GetAsyncKeyState('A') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vRight * -m_fSpeed * fTimeDelta));
}

void CPlayer::Key_Input_3rdView(const _float & fTimeDelta)
{
	Engine::CCamera* pMainCam = Engine::Get_MainCam();

	pMainCam->Get_Info(Engine::INFO_LOOK, &m_vDir);
	pMainCam->Get_Info(Engine::INFO_RIGHT, &m_vRight);

	_vec3 vAngle;

	m_vDir.y = 0.f;
	m_vRight.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
		pMainCam->Get_Angle(&vAngle);
		vAngle.x = 0.f;
		m_pTransformCom->Set_Angle(&vAngle);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
		pMainCam->Get_Angle(&vAngle);
		vAngle.x = 0.f;
		m_pTransformCom->Set_Angle(&vAngle);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransformCom->Move_Pos(&(m_vRight * m_fSpeed * fTimeDelta));
		pMainCam->Get_Angle(&vAngle);
		vAngle.x = 0.f;
		m_pTransformCom->Set_Angle(&vAngle);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransformCom->Move_Pos(&(m_vRight * -m_fSpeed * fTimeDelta));
		pMainCam->Get_Angle(&vAngle);
		vAngle.x = 0.f;
		m_pTransformCom->Set_Angle(&vAngle);
	}
}

void CPlayer::Key_Input_QuaterView(const _float & fTimeDelta)
{
	Engine::CCamera* pMainCam = Engine::Get_MainCam();
	pMainCam->Get_Info(Engine::INFO_LOOK, &m_vDir);
	pMainCam->Get_Info(Engine::INFO_RIGHT, &m_vRight);
	
	
	_vec3 vRadians;
	pMainCam->Get_Angle(&vRadians);
	vRadians.x = 0;
	m_pTransformCom->Set_Angle(&vRadians);

	m_vDir.y = 0;
	m_vRight.y = 0;

	D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	if (GetAsyncKeyState('W') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	if (GetAsyncKeyState('S') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	if (GetAsyncKeyState('D') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vRight * m_fSpeed * fTimeDelta));
	if (GetAsyncKeyState('A') & 0x8000)
		m_pTransformCom->Move_Pos(&(m_vRight * -m_fSpeed * fTimeDelta));
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

