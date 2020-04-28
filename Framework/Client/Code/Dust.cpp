#include "stdafx.h"
#include "Dust.h"

#include "Export_Function.h"

_bool	CDust::m_bAddResource = false;

CDust::CDust(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CDust::~CDust()
{

}

_int CDust::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	Animation(fTimeDelta);

	ScaleChange_And_Move(fTimeDelta);

	Update_Scale();

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CDust::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferColCom->Set_Color(m_d3dColor);
	m_pBufferColCom->Render_Buffer();
}

HRESULT CDust::Ready_GameObject(const _tchar * pTextureTag, const D3DXCOLOR & d3dColor, const _float& fScaleDelta)
{
	FAILED_CHECK_RETURN(Add_Resource(pTextureTag), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(pTextureTag, d3dColor), E_FAIL);

	m_fScaleDelta = fScaleDelta;

	return S_OK;
}

HRESULT CDust::Add_Resource(const _tchar* pTextureTag)
{
	if (m_bAddResource)
		return S_OK;

	m_bAddResource = true;

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, pTextureTag, Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Dust/Dust%d.png", 11), E_FAIL);


	return S_OK;
}

HRESULT CDust::Add_Component(const _tchar * pTextureTag, const D3DXCOLOR & d3dColor)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferColCom = dynamic_cast<Engine::CRcAlphaTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcColTex"));
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

	m_pBufferColCom->Set_Color(d3dColor);
	m_d3dColor = d3dColor;

	return S_OK;
}

void CDust::LifeTimer(const _float & fTimeDelta)
{
	if (m_fLifeTime <= 0.f)
		return;

	m_fCumulativeTime += fTimeDelta;

	if (m_fCumulativeTime >= m_fLifeTime)
		m_bIsDead = true;
}

void CDust::Animation(const _float& fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	{
		if (m_bAnimRepeat)
			m_tFrame.fCurFrame = 0.f;
		else
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
			if (m_fLifeTime <= 0.f)
				m_bIsDead = true;
		}
	}

}

void CDust::ScaleChange_And_Move(const _float & fTimeDelta)
{
	m_fScale += m_fScaleDelta * fTimeDelta;

	if (m_fScaleDelta >= 0.f)
	{
		if (m_fScaleMax < m_fScale)
			m_fScaleDelta = -m_fScaleDelta;
	}
	else
	{
		if (m_fScaleMin > m_fScale)
			m_fScaleDelta = -m_fScaleDelta;
	}

	m_pTransformCom->Move_Pos(m_vMoveDir * fTimeDelta * m_fSpeed);
}

void CDust::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CDust::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CDust* CDust::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
	const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _float& fScale, const _vec3* pPos, const _vec3* pMoveDir, const _float fSpeed,
	_bool bAnimRepeat, _float fLifeTime, const D3DXCOLOR& d3dColor, 
	const _float& fScaleDelta, const _float& fScaleMin, const _float& fScaleMax)
{
	CDust* pInstance = new CDust(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, d3dColor, fScaleDelta)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale = fScale;
	pInstance->Update_Scale();
	pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_vMoveDir = *pMoveDir;
	pInstance->m_fSpeed = fSpeed;

	pInstance->m_bAnimRepeat = bAnimRepeat;
	pInstance->m_fLifeTime = fLifeTime;

	pInstance->m_fScaleMin = fScaleMin;
	pInstance->m_fScaleMax = fScaleMax;


	return pInstance;
}

void CDust::Free()
{
	CEffect::Free();
}

