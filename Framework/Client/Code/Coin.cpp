#include "stdafx.h"
#include "Coin.h"

#include "Export_Function.h"

CCoin::CCoin(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CGameObject(pGraphicDev)
{

}

CCoin::~CCoin()
{

}

HRESULT CCoin::Ready_GameObject(const _vec3* pPos, const _vec3* pDir, const _float& fUpForce, const _float& fSpeed, const _int& iCoin)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	m_vMoveDir = *pDir;
	m_fUpForce = fUpForce;
	m_fSpeed = fSpeed;


	m_fGravity = 98.f;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_tSphere.fRadius = 0.5f;

	m_fScale = 0.05f;

	m_iCoin = iCoin;

	return S_OK;
}

_int CCoin::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	Animation(fTimeDelta);
	Bouncing(fTimeDelta);
	Gravity(fTimeDelta);

	Update_Scale();

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_tSphere.vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Coin", this);

	return 0;
}

void CCoin::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

void CCoin::Hit(const _int& iAtk, const _vec3* pAtkPos)
{
	m_bIsDead = true;

	Engine::PlaySound_(L"Coin.wav", CSoundMgr::EFFECT);

	return;
}

HRESULT CCoin::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Coin"));
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

void CCoin::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
	
}

void CCoin::Bouncing(const _float & fTimeDelta)
{
	if (m_pTransformCom->GetInfo(Engine::INFO_POS)->y - m_tSphere.fRadius < 0.f)
	{
		m_fDownSpeed = 0.f;
		m_fUpForce *= 0.6f;
		m_fSpeed *= 0.6f;
		m_pTransformCom->Set_PosY(m_tSphere.fRadius);
		
	}
}

void CCoin::Gravity(const _float & fTimeDelta)
{
	_vec3 vMove = m_vMoveDir * fTimeDelta * m_fSpeed;

	m_fDownSpeed += m_fGravity * fTimeDelta;
	vMove.y += (m_fUpForce - m_fDownSpeed) * fTimeDelta;

	m_pTransformCom->Move_Pos(vMove);
}

void CCoin::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CCoin::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CCoin* CCoin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, const _float& fUpForce, const _float& fSpeed, const _int& iCoin)
{
	CCoin*	pInstance = new CCoin(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos, pDir, fUpForce, fSpeed, iCoin)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCoin::Free()
{
	Engine::CGameObject::Free();
}

