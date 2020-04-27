#include "stdafx.h"
#include "RoomBlock.h"

#include "Export_Function.h"

CRoomBlock::CRoomBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CRoomBlock::~CRoomBlock()
{

}

HRESULT CRoomBlock::Ready_GameObject(const _vec3 * pScale, const _vec3 * pPos, BLOCK_DIR eBlockDir)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Pos(pPos);
	m_eCurDir = eBlockDir;
	m_eCurState = BLOCK_CLOSING;
	m_ePreState = BLOCK_END;

	switch (eBlockDir)
	{
	case CRoomBlock::BLOCK_UP:
		m_pTransformCom->Set_Angle(0.f, D3DXToRadian(180.f), 0.f);
		break;
	case CRoomBlock::BLOCK_DOWN:
		
		break;
	case CRoomBlock::BLOCK_LEFT:
		m_pTransformCom->Set_Angle(0.f, D3DXToRadian(90.f), 0.f);
		break;
	case CRoomBlock::BLOCK_RIGHT:
		m_pTransformCom->Set_Angle(0.f, D3DXToRadian(270.f), 0.f);
		break;
	}
	m_pTransformCom->Update_Component(0.f);

	Engine::PlaySound_(L"RoomLock.wav", CSoundMgr::EFFECT);


	return S_OK;
}

_int CRoomBlock::Update_GameObject(const _float& fTimeDelta)
{
	Change_State();

	switch (m_eCurState)
	{
	case CRoomBlock::BLOCK_CLOSING:
	{
		if (m_bAnimFinish)
			m_eCurState = BLOCK_CLOSED;
	
	}
		break;
	case CRoomBlock::BLOCK_CLOSED:
		//MonsterCheck();
		break;
	case CRoomBlock::BLOCK_OPENING:
		if (m_bAnimFinish)
			m_bIsDead = true;
		break;
	}


	if (m_bIsDead)
		return 0;

	Animation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"RoomBlock", this);

	return 0;
}

void CRoomBlock::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_pTextureCom->Render_Texture(static_cast<_uint>(m_tFrame.fCurFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

const _vec3* CRoomBlock::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

void CRoomBlock::Set_Pos(const _vec3* pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CRoomBlock::Set_PosX(const _float& fx)
{
	m_pTransformCom->Set_PosX(fx);
}

void CRoomBlock::Set_PosY(const _float& fy)
{
	m_pTransformCom->Set_PosY(fy);
}

void CRoomBlock::Set_PosZ(const _float& fz)
{
	m_pTransformCom->Set_PosZ(fz);
}

HRESULT CRoomBlock::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_RoomBlock"));
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

void CRoomBlock::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		if (m_bAnimRepeat)
			m_tFrame.fCurFrame = 0.f;
		else
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
			m_bAnimFinish = true;
		}
	}

}

void CRoomBlock::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CRoomBlock::BLOCK_CLOSING:
		Closing_State();
		break;
	case CRoomBlock::BLOCK_CLOSED:
		Closed_State();
		break;
	case CRoomBlock::BLOCK_OPENING:
		Opening_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CRoomBlock::Closing_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 9.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;

}

void CRoomBlock::Closed_State()
{
	m_tFrame.fCurFrame = 8.f;
	m_tFrame.fMaxFrame = 9.f;
	m_tFrame.fFrameSpeed = 0.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;
}

void CRoomBlock::Opening_State()
{
	m_tFrame.fCurFrame = 9.f;
	m_tFrame.fMaxFrame = 18.f;
	m_tFrame.fFrameSpeed = 10.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = false;
}

void CRoomBlock::MonsterCheck()
{
	const Engine::CLayer* pLayer = Engine::Get_Layer(L"Monster");
	
	if (false == m_bMonsterGen && 0 != pLayer->Get_ObjectMapSize())
		m_bMonsterGen = true;

	if (true == m_bMonsterGen && 0 == pLayer->Get_ObjectMapSize())
	{
		m_eCurState = BLOCK_OPENING;
		Engine::PlaySound_(L"RoomVictoryUnlock.wav", CSoundMgr::EFFECT);
		if (Engine::Get_MainCamType() != Engine::CCameraMgr::MAIN_CAM_QUATER)
		{
			Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_QUATER);
		}
	}

}

CRoomBlock * CRoomBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pScale, const _vec3 * pPos, BLOCK_DIR eBlockDir)
{
	CRoomBlock*	pInstance = new CRoomBlock(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pScale, pPos, eBlockDir)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRoomBlock::Free()
{
	CGameObject::Free();
}

