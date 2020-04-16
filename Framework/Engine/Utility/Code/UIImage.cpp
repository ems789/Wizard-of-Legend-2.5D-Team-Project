#include "stdafx.h"
#include "UIImage.h"

#include "Export_Function.h"

CUIImage::CUIImage(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

CUIImage::~CUIImage()
{

}

HRESULT CUIImage::Ready_UIImage(const _vec3& vScale, const _vec3& vPos, const _tchar* pTextureTag)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, pTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_vPos = vPos;
	m_vScale = vScale;

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matTrans;

	return S_OK;
}

void CUIImage::Render_UIImage()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

}

void CUIImage::Set_Pos(const _vec3 & vPos)
{
	m_vPos = vPos;
	
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matTrans;
}

void CUIImage::Set_Scale(const _vec3 & vScale)
{
	m_vScale = vScale;

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matTrans;

}

CUIImage * CUIImage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vScale, const _vec3& vPos, const _tchar * pTextureTag)
{
	CUIImage* pInstance = new CUIImage(pGraphicDev);

	if (FAILED(pInstance->Ready_UIImage(vScale, vPos, pTextureTag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CUIImage::Free()
{
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);

	Engine::Safe_Release(m_pGraphicDev);
}

