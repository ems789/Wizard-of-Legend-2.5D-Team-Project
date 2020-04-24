#include "stdafx.h"
#include "SignitureEffect.h"

#include "Export_Function.h"

CSignitureEffect::CSignitureEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CSignitureEffect::~CSignitureEffect()
{

}

_int CSignitureEffect::Update_GameObject(const _float& fTimeDelta)
{
	//	Turn_To_Camera_Look();

	DecreaseLength(fTimeDelta);

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CSignitureEffect::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSignitureEffect::Ready_GameObject(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Ready_Effect(pTextureTag), E_FAIL);

	return S_OK;
}

void CSignitureEffect::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CSignitureEffect::DecreaseLength(const _float & fTimeDelta)
{
	if (m_bIncrease)
	{
		m_fLength += m_fDLengthSpeed * fTimeDelta;
		m_pTransformCom->Set_ScaleY(m_fLength);

		if (m_fLength > m_fDieLength)
		{
			m_bIsDead = true;
		}
	}
	else
	{
		m_fLength -= m_fDLengthSpeed * fTimeDelta;
		m_pTransformCom->Set_ScaleY(m_fLength);

		if (m_fLength < m_fDieLength)
		{
			m_bIsDead = true;
		}
	}



}

CSignitureEffect* CSignitureEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _vec3* pPos, const _float& fThickness, const _float& fLength, const _float& fDieLength, const _float& fDLengthSpeed, const _vec3* pAngle)
{
	CSignitureEffect* pInstance = new CSignitureEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;

	pInstance->m_pTransformCom->Set_Pos(pPos);
	
	pInstance->m_fThickness = fThickness;
	pInstance->m_fLength	= fLength;
	pInstance->m_fDieLength = fDieLength;
	pInstance->m_bIncrease = fLength < fDieLength;
	pInstance->m_pTransformCom->Set_ScaleX(fThickness);
	pInstance->m_pTransformCom->Set_ScaleY(fLength);
	pInstance->m_pTransformCom->Set_Angle(pAngle);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_fDLengthSpeed = fDLengthSpeed;

	return pInstance;
}

void CSignitureEffect::Free()
{
	CEffect::Free();
}

