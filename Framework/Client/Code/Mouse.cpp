#include "stdafx.h"
#include "Mouse.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse()
{

}

CMouse::~CMouse()
{
	Free();
}

HRESULT CMouse::Ready_Mouse(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_MouseCursor0", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Mouse/MouseCursor0.png"), E_FAIL);
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXIMAGE_INFO tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));
	m_pTextureCom->Get_TexInfo(0, &tImgInfo);
	
	m_pTransformCom->Set_ScaleX((_float)tImgInfo.Width * 2.f);
	m_pTransformCom->Set_ScaleY((_float)tImgInfo.Height * 2.f);

	return S_OK;
}

_int CMouse::Update_Mouse(const _float& fTimeDelta)
{
	Update_MousePos();

	m_pTransformCom->Update_Component(fTimeDelta);

	return 0;
}

void CMouse::Render_Mouse()
{
	if (false == m_bCursorRender)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CMouse::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_MouseCursor0"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

void CMouse::Update_MousePos()
{
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_vec3 vMousePos;
	vMousePos.x = pt.x - WINCX * 0.5f;
	vMousePos.y = -pt.y + WINCY * 0.5f;
	vMousePos.z = 0.f;

	m_pTransformCom->Set_Pos(vMousePos);
}

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouse* pInstance = new CMouse;

	if (FAILED(pInstance->Ready_Mouse(pGraphicDev)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMouse::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pGraphicDev);
}

