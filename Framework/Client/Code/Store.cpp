#include "stdafx.h"
#include "Store.h"

CStore::CStore(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStore::~CStore()
{
}

HRESULT CStore::Ready_GameObject()
{
	

	return S_OK;
}

_int CStore::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CStore::Render_GameObject()
{
	
}

HRESULT CStore::Add_Skill(const wstring & wstrSkillTag, Engine::CSkill * pSkill)
{


	return S_OK;
}

CStore * CStore::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos)
{
	CStore* pInstance = new CStore(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStore::Free()
{
	throw std::logic_error("The method or operation is not implemented.");
}

