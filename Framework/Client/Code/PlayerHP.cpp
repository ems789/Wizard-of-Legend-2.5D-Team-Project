#include "stdafx.h"
#include "PlayerHP.h"

#include "Export_Function.h"

CPlayerHP::CPlayerHP(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayerHP::~CPlayerHP()
{

}

HRESULT CPlayerHP::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	


	return S_OK;
}

_int CPlayerHP::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return iExit;
}

void CPlayerHP::Render_GameObjcet()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CPlayerHP::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"PlayerHP : Add_Component Failed. m_pBufferCom is nullptr");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_HPBarBG"));
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"PlayerHP : Add_Component Failed. m_pTextureCom is nullptr");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);
	
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"PlayerHP : Add_Component Failed. m_pTransformCom is nullptr");
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"PlayerHP : Add_Component Failed. m_pRendererCom is nullptr");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

HRESULT CPlayerHP::SetUp_Scale_Pos(const _vec3 * pScale)
{
	D3DXIMAGE_INFO tImgInfo;
	m_pTextureCom->Get_TexInfo(0, &tImgInfo);

	_float fWidth = tImgInfo.Width * pScale->x, fHeight = tImgInfo.Height * pScale->y;
	m_pTransformCom->Set_Scale(fWidth, fHeight, 0.f);
	m_pTransformCom->Set_Pos((fWidth - WINCX) * 0.5f, (-fHeight + WINCY) * 0.5f, 0.f);

	return S_OK;
}

CPlayerHP * CPlayerHP::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pScale)
{
	CPlayerHP*	pInstance = new CPlayerHP(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->SetUp_Scale_Pos(pScale);

	return pInstance;
}

void CPlayerHP::Free()
{
	CGameObject::Free();
}

