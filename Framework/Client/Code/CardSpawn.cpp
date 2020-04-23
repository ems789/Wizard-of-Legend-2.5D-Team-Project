#include "stdafx.h"
#include "CardSpawn.h"

#include "Export_Function.h"

CCardSpawn::CCardSpawn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{

}

CCardSpawn::~CCardSpawn()
{

}

_int CCardSpawn::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	Animation(fTimeDelta);

	Update_Scale();

	if (m_bIsDead)
		return 0;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CCardSpawn::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

HRESULT CCardSpawn::Ready_GameObject(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Ready_Effect(pTextureTag), E_FAIL);

	return S_OK;
}

void CCardSpawn::Animation(const _float& fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
		m_bIsDead = true;
		Add_Monster();
	}

}

void CCardSpawn::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

void CCardSpawn::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CCardSpawn::Add_Monster()
{
	Engine::Add_GameObject(L"Monster", L"Monster", m_pMonster);
	m_pMonster->AddRef();
}

CCardSpawn* CCardSpawn::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _float& fScale, const _vec3* pPos, Engine::CGameObject* pMonster)
{
	CCardSpawn* pInstance = new CCardSpawn(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale	= fScale;
	pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_pTransformCom->Update_Component(0.f);
	pInstance->m_pMonster = pMonster;

	return pInstance;
}

void CCardSpawn::Free()
{
	Engine::Safe_Release(m_pMonster);
	CEffect::Free();
}

