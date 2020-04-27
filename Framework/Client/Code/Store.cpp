#include "stdafx.h"
#include "Store.h"

#include "SkillCard.h"

CStore::CStore(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStore::~CStore()
{
}

HRESULT CStore::Ready_GameObject(const _vec3* pPos, CSkillCard* pSkillCard1, CSkillCard* pSkillCard2, CSkillCard* pSkillCard3)
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

CStore * CStore::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, CSkillCard* pSkillCard1, CSkillCard* pSkillCard2, CSkillCard* pSkillCard3)
{
	CStore* pInstance = new CStore(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos, pSkillCard1, pSkillCard2, pSkillCard3)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStore::Free()
{
	CGameObject::Free();
}

