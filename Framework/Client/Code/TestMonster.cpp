#include "stdafx.h"
#include "TestMonster.h"

#include "Export_Function.h"

CTestMonster::CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CTestMonster::~CTestMonster()
{

}

HRESULT CTestMonster::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CTestMonster::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	return 0;
}

void CTestMonster::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CTestMonster::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CTestMonster::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_FireBoss_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();



	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();
	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Update_Component(0.f);

	return S_OK;
}

void CTestMonster::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
		m_bIsDead = true;

}

CTestMonster* CTestMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CTestMonster* pInstance = new CTestMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(pPos);

	return pInstance;
}

void CTestMonster::Free()
{
	Engine::CGameObject::Free();
}

