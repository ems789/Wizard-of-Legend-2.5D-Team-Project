#include "stdafx.h"
#include "Wind.h"

#include "Export_Function.h"
#include "BasicEffect.h"

CWind::CWind(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CWind::~CWind()
{

}

HRESULT CWind::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fScale = 0.05f;

	const Engine::TEX_INFO*	pTexInfo = m_pTextureCom->Get_TexInfo();
	m_pTransformCom->Set_Scale(pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CWind::Update_GameObject(const _float& fTimeDelta)
{
	Animation(fTimeDelta);

	if (m_bIsDead)
		return 0;

	m_pTransformCom->Move_Pos(Engine::INFO_UP, fTimeDelta * m_fSpeed);

	_int	iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	//Engine::Add_GameObject_To_CollisionList(L"Player_Bullet", this);
	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	return 0;
}

void CWind::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

void CWind::Add_Effect(const _vec3* pPos)
{
	_vec3 vPos = (*m_pTransformCom->GetInfo(Engine::INFO_POS) + *pPos) / 2;
	vPos.y += 0.5f;

	CBasicEffect* pEffect = CBasicEffect::Create(m_pGraphicDev, L"Texture_SlashHitSpark", L"SlashHitSpark", 7.f, 20.f, 0.05f, &vPos, false, 0.f);

	Engine::Add_GameObject(L"GameLogic", L"SlashHitSpark", pEffect);
	
}

HRESULT CWind::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_WindSlash"));
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

void CWind::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
		m_bIsDead = true;
	}
}

CWind* CWind::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vAngle, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed, _float fDir)
{
	CWind*	pInstance = new CWind(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_vInitialPos = vInitialPos;
	pInstance->m_pTransformCom->Set_Pos(vInitialPos);
	pInstance->m_pTransformCom->Set_Angle(vAngle);
	pInstance->m_pTransformCom->Set_Scale(fDir);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fSpeed = fSpeed;
	pInstance->m_tFrame.fCurFrame = 0;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;

	return pInstance;
}

void CWind::Free()
{

	CGameObject::Free();
}

