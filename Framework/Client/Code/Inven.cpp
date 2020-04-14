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

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CInven::Update_Inven(const _float & fTImeDelta)
{
	//	키 입력 : 상하좌우, Enter, Backspace
	

	return 0;
}

void CInven::Render_Inven()
{

}

HRESULT CInven::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L""));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

void CInven::Free()
{
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);

	for (_uint i = 0; i < SKILL_END; ++i)
	{
		for_each(m_vecSkill[i].begin(), m_vecSkill[i].end(), Engine::CDeleteObj());
		m_vecSkill[i].clear();
	}

	Engine::Safe_Release(m_pGraphicDev);
}
