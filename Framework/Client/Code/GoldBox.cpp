#include "stdafx.h"
#include "GoldBox.h"

#include "Export_Function.h"
#include "SphereCollider.h"
#include "FireKick.h"
#include "BasicEffect.h"
#include "Coin.h"

CGoldBox::CGoldBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGoldBox::~CGoldBox()
{
}

HRESULT CGoldBox::Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(pTextureTag), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	
	m_fScale = fScale;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = fMaxFrame;
	m_tFrame.fFrameSpeed = fFrameSpeed;

	m_d3dColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	Update_Scale();

	return S_OK;
}

_int CGoldBox::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Turn_To_Camera_Look();

	Animation(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	m_tSphere.vPos = *m_pTransformCom->GetInfo(Engine::INFO_POS);

	return 0;
}

void CGoldBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Set_Color(m_d3dColor);
	m_pBufferCom->Render_Buffer();
}

void CGoldBox::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_bIsDead = true;

		_uint iRepeat = rand() % 10 + 10;
		for (_uint i = 0; i < iRepeat; ++i)
		{
			_vec3 vDir = { rand() % 100 / 50.f - 1.f, 0.f ,rand() % 100 / 50.f - 1.f };
			D3DXVec3Normalize(&vDir, &vDir);
			_float fUpForce = rand() % 100 / 10.f + 20.f;
			_float	fSpeed = rand() % 100 / 50.f;
			_int	iCoin = rand() % 20 + 1;
			CCoin* pCoin = CCoin::Create(m_pGraphicDev, m_pTransformCom->GetInfo(Engine::INFO_POS), &vDir, fUpForce, fSpeed, iCoin);
			Engine::Add_GameObject(L"GameLogic", L"Coin", pCoin);
		}


	}
}

const _vec3 * CGoldBox::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CGoldBox::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CGoldBox::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CGoldBox::Set_PosX(const _float & fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CGoldBox::Set_PosY(const _float & fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CGoldBox::Set_PosZ(const _float & fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CGoldBox::Add_Component(const _tchar * pTextureTag)
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcAlphaTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcColTex"));
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

void CGoldBox::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CGoldBox::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CGoldBox::Update_Scale()
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	m_pTransformCom->Set_Scale(vScale);
}

CGoldBox * CGoldBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3 * pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed)
{
	CGoldBox* pInstance = new CGoldBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTextureTag, pPos, fScale, fMaxFrame, fFrameSpeed)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CGoldBox::Free()
{
	CGameObject::Free();
}

