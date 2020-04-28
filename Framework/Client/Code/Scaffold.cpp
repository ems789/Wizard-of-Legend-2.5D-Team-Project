#include "stdafx.h"
#include "Scaffold.h"

#include "Export_Function.h"
#include "SphereCollider.h"
#include "FireKick.h"
#include "BasicEffect.h"

CScaffold::CScaffold(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CScaffold::~CScaffold()
{
}

HRESULT CScaffold::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	
	m_fScale = fScale;

	Update_Scale();

	return S_OK;
}

_int CScaffold::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	const Engine::CComponent* pCom = Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC);

	const _vec3* pPos = dynamic_cast<const Engine::CTransform*>(pCom)->GetInfo(Engine::INFO_POS);

	const _vec3* pMyPos = m_pTransformCom->GetInfo(Engine::INFO_POS);

	_float fMinX = pMyPos->x - m_fScale / 2;
	_float fMaxX = pMyPos->x + m_fScale / 2;

	_float fMinZ = pMyPos->z - m_fScale / 2;
	_float fMaxZ = pMyPos->z + m_fScale / 2;

	if (fMinX <= pPos->x && pPos->x <= fMaxX && fMinZ <= pPos->z && pPos->z <= fMaxZ)
	{
		if (!m_bPreIn)
		{
			m_bDown = !m_bDown;
			m_bPreIn = true;
			Engine::PlaySound_(L"LancerAttackWindup.wav", CSoundMgr::EFFECT);
		}
	}
	else
	{
		if (m_bPreIn)
			m_bPreIn = false;
	}

	if (m_bDown)
		m_pTransformCom->Set_PosY(-m_fScale * 0.4);
	else
		m_pTransformCom->Set_PosY(0.f);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	return 0;
}

void CScaffold::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CScaffold::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CScaffold::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CScaffold::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CScaffold::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CScaffold::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CScaffold::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CScaffold::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_CubeTex"));
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

void CScaffold::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { m_fScale, m_fScale, m_fScale };
	m_pTransformCom->Set_Scale(vScale);
}

CScaffold * CScaffold::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pPos, const _float& fScale)
{
	CScaffold* pInstance = new CScaffold(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pPos, fScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScaffold::Free()
{
	CGameObject::Free();
}

