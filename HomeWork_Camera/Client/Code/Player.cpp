#include "stdafx.h"
#include "Player.h"

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
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &m_vDir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f * fTimeDelta));

	if (GetAsyncKeyState('A') & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(-90.f * fTimeDelta));

	if (GetAsyncKeyState('W') & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));

	if (GetAsyncKeyState('S') & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));

	if (GetAsyncKeyState('E') & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(90.f * fTimeDelta));

	if (GetAsyncKeyState('D') & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(-90.f * fTimeDelta));
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

