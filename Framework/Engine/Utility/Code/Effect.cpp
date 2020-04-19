#include "Effect.h"

#include "Export_Function.h"

USING(Engine)

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CEffect::~CEffect()
{

}

HRESULT CEffect::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

HRESULT CEffect::Ready_Effect(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	return S_OK;
}

_int CEffect::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;
	
	return iExit;
}

void CEffect::Render_GameObject()
{
	
}

void CEffect::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

HRESULT CEffect::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, pTextureTag));
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

void CEffect::Free()
{
	CGameObject::Free();
}

