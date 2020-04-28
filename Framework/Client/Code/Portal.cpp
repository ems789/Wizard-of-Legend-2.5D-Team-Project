#include "stdafx.h"
#include "Portal.h"

#include "Export_Function.h"
#include "Dust.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CPortal::~CPortal()
{

}

_int CPortal::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	Animation(fTimeDelta);

	Generate_Dust(fTimeDelta);

	Update_Scale();

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CPortal::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

HRESULT CPortal::Ready_GameObject(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Ready_Effect(pTextureTag), E_FAIL);

	return S_OK;
}

void CPortal::Animation(const _float& fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CPortal::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CPortal::Generate_Dust(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer > 0.1f)
	{
		m_fTimer -= 0.1f;

		_vec3 vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vRight, vLook;
		Engine::Get_MainCameraRight(&vRight);
		D3DXVec3Cross(&vLook, &vRight, &vUp);
		_float	fRight = (rand() % 100 - 50.f) / 100.f;
		_float	fA = 16.f / 80.f;
		_float	fB = 40.f / 80.f;
		_float	fUp;
		if (fA > fRight * fRight)
			fUp = sqrtf((1.f - (fRight * fRight / fA)) * fB);
		else
			fUp = 0.f;
		if (rand() % 2)
			fUp *= -1.f;

		_vec3 vDustPos = vPos + fRight * vRight + fUp * vUp - vLook * 0.1f;
		_vec3 vDir = vDustPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		_float	fSpeed = 0.1f;
		_float	fLifeTime = 2.f;
		CDust* pDust = CDust::Create(m_pGraphicDev, L"Texture_Dust", 11.f, 10.f, 0.02f, &vDustPos, &vDir, fSpeed, true, fLifeTime, D3DXCOLOR(0.4f, 0.9f, 1.f, 1.f), 0.05f, 0.01f, 0.03f);
		Engine::Add_GameObject(L"Effect", L"Portal_Dust", pDust);
	}
}

void CPortal::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _float& fScale, const _vec3* pPos)
{
	CPortal* pInstance = new CPortal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale = fScale;
	pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_pTransformCom->Update_Component(0.f);


	return pInstance;
}

void CPortal::Free()
{
	CEffect::Free();
}

