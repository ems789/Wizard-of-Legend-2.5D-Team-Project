#include "stdafx.h"
#include "RotatedObject.h"

#include "Export_Function.h"

CRotatedObject::CRotatedObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CRotatedObject::~CRotatedObject()
{

}

_int CRotatedObject::Update_GameObject(const _float& fTimeDelta)
{
	//Turn_To_Camera_Look();

	Animation(fTimeDelta);

	if (m_tFrame.fMaxFrame > 1.f)
		Update_Scale();

	if (m_bIsDead)
		return 0;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CRotatedObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();
}

HRESULT CRotatedObject::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
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

HRESULT CRotatedObject::Ready_GameObject(const _tchar * pTextureTag)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	return S_OK;
}

void CRotatedObject::Animation(const _float& fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	{
		if (m_bAnimRepeat)
			m_tFrame.fCurFrame = 0.f;
		else
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
		}
	}

}

void CRotatedObject::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(static_cast<_uint>(m_tFrame.fCurFrame));

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CRotatedObject* CRotatedObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
	const _float& fScale, const _vec3* pPos, const _vec3* pAngle, _bool bAnimRepeat)
{
	CRotatedObject* pInstance = new CRotatedObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag)))
		Engine::Safe_Release(pInstance);
	
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = fMaxFrame;
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_fScale = fScale;
	pInstance->Update_Scale();
	pInstance->m_pTransformCom->Set_Pos(pPos);
	pInstance->m_pTransformCom->Set_Angle(pAngle);
	pInstance->m_pTransformCom->Update_Component(0.f);

	pInstance->m_bAnimRepeat = bAnimRepeat;

	return pInstance;
}

void CRotatedObject::Free()
{
	CGameObject::Free();
}

