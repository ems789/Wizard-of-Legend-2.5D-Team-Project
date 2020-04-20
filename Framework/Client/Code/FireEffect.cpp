#include "stdafx.h"
#include "FireEffect.h"

#include "Export_Function.h"

CFireEffect::CFireEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CFireEffect::~CFireEffect()
{

}

_int CFireEffect::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	Animation(fTimeDelta);

	//Update_Scale();

	ColorChange_And_Move(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CFireEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferColCom->Render_Buffer();
}

HRESULT CFireEffect::Ready_GameObject(const _tchar * pTextureTag, const D3DXCOLOR & d3dColor, const D3DXCOLOR& d3dColorDifference)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag, d3dColor), E_FAIL);

	m_fdRed = d3dColorDifference.r;
	m_fdGreen = d3dColorDifference.g;
	m_fdBlue = d3dColorDifference.b;
	m_fdAlpha = d3dColorDifference.a;

	return S_OK;
}

HRESULT CFireEffect::Add_Component(const _tchar * pTextureTag, const D3DXCOLOR & d3dColor)
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

void CFireEffect::LifeTimer(const _float & fTimeDelta)
{
	if (m_fLifeTime <= 0.f)
		return;

	m_fCumulativeTime += fTimeDelta;

	if (m_fCumulativeTime >= m_fLifeTime)
		m_bIsDead = true;
}

void CFireEffect::Animation(const _float& fTimeDelta)
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

void CFireEffect::ColorChange_And_Move(const _float & fTimeDelta)
{
	m_d3dColor.a -= m_fdAlpha * fTimeDelta;
	m_d3dColor.r -= m_fdRed * fTimeDelta;
	m_d3dColor.g -= m_fdGreen * fTimeDelta;
	m_d3dColor.b -= m_fdBlue * fTimeDelta;

	m_pBufferColCom->Set_Color(m_d3dColor);

	m_pTransformCom->Move_Pos(m_vMoveDir * fTimeDelta * m_fSpeed);
}

void CFireEffect::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CFireEffect::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CFireEffect* CFireEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
	const _tchar* pTextureTag, const _tchar* pEffectTag, 
	const _float& fMaxFrame, const _float& fFrameSpeed,
	const _float& fScale, const _vec3* pPos, const _vec3* pMoveDir, const _float fSpeed,
	_bool bAnimRepeat, _float fLifeTime, 
	const D3DXCOLOR& d3dColor, const D3DXCOLOR& d3dColorDifference)
{
	CFireEffect* pInstance = new CFireEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, d3dColor, d3dColorDifference)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_pEffectTag = pEffectTag;
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale = fScale;
	pInstance->Update_Scale();
	pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_vMoveDir = *pMoveDir;
	pInstance->m_fSpeed = fSpeed;

	if (bAnimRepeat && 0.f <= fLifeTime)
	{
		pInstance->m_bAnimRepeat = false;
		pInstance->m_fLifeTime = 1.f;
	}
	else
	{
		pInstance->m_bAnimRepeat = bAnimRepeat;
		pInstance->m_fLifeTime = fLifeTime;
	}

	return pInstance;
}

void CFireEffect::Free()
{
	CEffect::Free();
}

