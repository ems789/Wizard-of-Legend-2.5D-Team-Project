#include "stdafx.h"
#include "BeamEffect.h"

#include "Export_Function.h"

CBeamEffect::CBeamEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CBeamEffect::~CBeamEffect()
{

}

_int CBeamEffect::Update_GameObject(const _float& fTimeDelta)
{
	//Turn_To_Camera_Look();

	LifeTimer(fTimeDelta);

	Animation(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CBeamEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

HRESULT CBeamEffect::Ready_GameObject(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Ready_Effect(pTextureTag), E_FAIL);

	return S_OK;
}

void CBeamEffect::LifeTimer(const _float & fTimeDelta)
{
	if (m_fLifeTime <= 0.f)
		return;

	m_fCumulativeTime += fTimeDelta;

	if (m_fCumulativeTime >= m_fLifeTime)
		m_bIsDead = true;
}

void CBeamEffect::Animation(const _float& fTimeDelta)
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

void CBeamEffect::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CBeamEffect* CBeamEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _tchar* pEffectTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _vec3* pPos, const _float& fThickness, const _float& fLength, const _float& fAngleY, _bool bAnimRepeat, _float fLifeTime)
{
	CBeamEffect* pInstance = new CBeamEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_pEffectTag = pEffectTag;
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;

	pInstance->m_pTransformCom->Set_Pos(pPos);
	
	pInstance->m_fThickness = fThickness;
	pInstance->m_fLength	= fLength;
	pInstance->m_pTransformCom->Set_ScaleX(fThickness);
	pInstance->m_pTransformCom->Set_ScaleY(fLength);
	pInstance->m_pTransformCom->Set_Angle(_vec3(D3DXToRadian(90.f), fAngleY, 0.f));
	pInstance->m_pTransformCom->Update_Component(0.f);

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

void CBeamEffect::Free()
{
	CEffect::Free();
}

