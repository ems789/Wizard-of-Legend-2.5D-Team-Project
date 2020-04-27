#include "stdafx.h"
#include "ImageObject.h"

#include "Export_Function.h"

CImageObject::CImageObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CImageObject::~CImageObject()
{
}

HRESULT CImageObject::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	
	m_fScale = fScale;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = fMaxFrame;
	m_tFrame.fFrameSpeed = fFrameSpeed;

	Update_Scale();

	return S_OK;
}

_int CImageObject::Update_GameObject(const _float& fTimeDelta)
{
	Turn_To_Camera_Look();

	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CImageObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CImageObject::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

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

void CImageObject::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CImageObject::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CImageObject::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CImageObject * CImageObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed)
{
	CImageObject* pInstance = new CImageObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pPos, fScale, fMaxFrame, fFrameSpeed)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CImageObject::Free()
{
	CGameObject::Free();
}

