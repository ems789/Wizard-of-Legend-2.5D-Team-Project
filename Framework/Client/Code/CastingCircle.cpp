#include "stdafx.h"
#include "CastingCircle.h"

#include "Export_Function.h"

CCastingCircle::CCastingCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CEffect(pGraphicDev)
{

}

CCastingCircle::~CCastingCircle()
{

}

HRESULT CCastingCircle::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 27.f;
	m_tFrame.fFrameSpeed = 20.f;

	return S_OK;
}

_int CCastingCircle::Update_GameObject(const _float& fTimeDelta)
{
	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return iExit;
}

void CCastingCircle::Render_GameObjcet()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
}

HRESULT CCastingCircle::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"CastingCircle failed to add transform component");
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_CastingCircle"));
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"CastingCircle failed to add Texture component");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"CastingCircle failed to add Buffer_RcTex");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"CastingCircle failed to add RendererCom");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CCastingCircle::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

CCastingCircle * CCastingCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fScale, const _vec3* pPos)
{
	CCastingCircle* pInstance = new CCastingCircle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Scale(fScale);
	pInstance->m_pTransformCom->Set_Pos(pPos);

	return pInstance;
}

void CCastingCircle::Free()
{
	Engine::CEffect::Free();
}

