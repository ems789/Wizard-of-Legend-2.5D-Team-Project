#include "stdafx.h"
#include "FollowingEffect.h"

#include "Export_Function.h"

CFollowingEffect::CFollowingEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CFollowingEffect::~CFollowingEffect()
{

}

_int CFollowingEffect::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();
	 
	LifeTimer(fTimeDelta);

	Animation(fTimeDelta);

	Update_Scale();

	if (m_bIsDead)
		return 0;

	m_pTransformCom->Set_Pos(*m_pTargetPos + m_vHandPos);
									
	_int iExit = CEffect::Update_GameObject(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CFollowingEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

HRESULT CFollowingEffect::Ready_GameObject(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Ready_Effect(pTextureTag), E_FAIL);

	return S_OK;
}

void CFollowingEffect::LifeTimer(const _float & fTimeDelta)
{
	if (m_fLifeTime <= 0.f)
		return;

	m_fCumulativeTime += fTimeDelta;

	if (m_fCumulativeTime >= m_fLifeTime)
		m_bIsDead = true;
}

void CFollowingEffect::Animation(const _float& fTimeDelta)
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

void CFollowingEffect::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CFollowingEffect::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CFollowingEffect* CFollowingEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _tchar* pEffectTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _float& fScale, const _vec3* pPos, const _vec3* pTargetPos,_bool bAnimRepeat, _float fLifeTime)
{
	CFollowingEffect* pInstance = new CFollowingEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_pEffectTag = pEffectTag;
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale = fScale;
	//pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_vHandPos = *pPos;
	pInstance->m_pTargetPos = pTargetPos;
	pInstance->m_pTransformCom->Update_Component(0.f);

	
		pInstance->m_bAnimRepeat = bAnimRepeat;
		pInstance->m_fLifeTime = fLifeTime;
	

	return pInstance;
}

void CFollowingEffect::Free()
{
	CEffect::Free();
}

