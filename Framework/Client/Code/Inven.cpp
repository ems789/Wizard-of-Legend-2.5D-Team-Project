#include "stdafx.h"
#include "Inven.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CInven)

CInven::CInven()
{

}

CInven::~CInven()
{
	Free();
}

HRESULT CInven::Ready_Inven(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_Inven", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Inven/SpellbookUIArcana.png"), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPos = { 0.f, 0.f, 0.f };
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	
	m_matWorld = matScale * matTrans;

	return S_OK;
}

_int CInven::Update_Inven(const _float & fTImeDelta)
{
	if (Engine::KeyDown(DIK_T))
		m_bInvenOn = !m_bInvenOn;

	if (false == m_bInvenOn)
		return 0;
	//	키 입력 : 상하좌우, Enter, Backspace
	

	return 0;
}

void CInven::Render_Inven()
{
	if (false == m_bInvenOn)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

}

HRESULT CInven::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Inven"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

void CInven::Free()
{
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	for (_uint i = 0; i < SKILL_END; ++i)
	{
		for_each(m_vecSkill[i].begin(), m_vecSkill[i].end(), Engine::CDeleteObj());
		m_vecSkill[i].clear();
	}

	Engine::Safe_Release(m_pGraphicDev);
}
