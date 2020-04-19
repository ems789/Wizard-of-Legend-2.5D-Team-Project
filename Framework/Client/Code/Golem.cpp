#include "stdafx.h"
#include "Golem.h"

#include "Export_Function.h"

CGolem::CGolem(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CGolem::~CGolem()
{

}

HRESULT CGolem::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tSphere.fRadius = 1.f;

	return S_OK;
}

_int CGolem::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	m_tSphere.vPos = m_pTransformCom->GetInfoRef(Engine::INFO_POS);

	Engine::Add_GameObject_To_CollisionList(L"Monster", this);

	return 0;
}

void CGolem::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	m_vvTextureCom[m_eCurState][m_eCurDir]->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

const _vec3 * CGolem::Get_Pos() const
{
	return m_pTransformCom->GetInfo(Engine::INFO_POS);
}

HRESULT CGolem::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();


	Engine::CTexture* pTextureCom = nullptr;

	m_vvTextureCom.resize(CGolem::GS_END, vector<Engine::CTexture*>(CGolem::GD_END));

	//	Texture Component Setting;
	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Golem_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GS_IDLE][GD_UP]		= pTextureCom;
	m_vvTextureCom[GS_IDLE][GD_DOWN]	= pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[GS_IDLE][GD_RIGHT]	= pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Golem_Run"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GS_RUN][GD_UP] = pTextureCom;
	m_vvTextureCom[GS_RUN][GD_DOWN] = pTextureCom;
	pTextureCom->AddRef();
	m_vvTextureCom[GS_RUN][GD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Golem_AttackUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GS_ATTACK][GD_UP] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Golem_AttackDown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GS_ATTACK][GD_DOWN] = pTextureCom;

	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Golem_AttackRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GS_ATTACK][GD_RIGHT] = pTextureCom;



	pComponent = pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Golem_AttackFollowThrough"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_vvTextureCom[GS_ATTACK_FT][GD_UP] = pTextureCom;
	m_vvTextureCom[GS_ATTACK_FT][GD_DOWN] = pTextureCom;
	pTextureCom->AddRef();

	m_vvTextureCom[GS_ATTACK_FT][GD_RIGHT] = pTextureCom;
	pTextureCom->AddRef();

	return S_OK;
}

void CGolem::Animation(const _float & fTimeDelta)
{
	m_tFrame.fCurFrame += fTimeDelta * m_tFrame.fFrameSpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
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

void CGolem::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CGolem::GS_IDLE:
		Idle_State();
		break;
	case CGolem::GS_RUN:
		Run_State();
		break;
	case CGolem::GS_ATTACK:
		Attack_State();
		break;
	case CGolem::GS_ATTACK_FT:
		Attack_FT_State();
		break;
	}

	m_ePreState = m_eCurState;
}

void CGolem::Update_State(const _float & fTimeDelta)
{
	switch (m_eCurState)
	{
	case CGolem::GS_IDLE:
		Idle_Update(fTimeDelta);
		break;
	case CGolem::GS_RUN:
		Run_Update(fTimeDelta);
		break;
	case CGolem::GS_ATTACK:
		Attack_Update(fTimeDelta);
		break;
	case CGolem::GS_ATTACK_FT:
		Attack_FT_Update(fTimeDelta);
		break;
	}
}

void CGolem::Turn_To_Camera_Look()
{
	_vec3 vAngle = { 0.f, 0.f, 0.f };
	Engine::Get_MainCameraAngle(&vAngle);
	vAngle.x = 0.f;

	m_pTransformCom->Set_Angle(&vAngle);
	m_pTransformCom->Update_Component(0.f);
}

void CGolem::Fitting_Scale_With_Texture(CGolem::GOLEM_STATE eState)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo();

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	//if (!m_bDir)
	//	vScale.x = -vScale.x;

	m_pTransformCom->Set_Scale(vScale);
}

void CGolem::Fitting_Scale_With_Texture(CGolem::GOLEM_STATE eState, _ulong dwIndex)
{
	const Engine::TEX_INFO* pTexInfo = m_vvTextureCom[eState][m_eCurDir]->Get_TexInfo(dwIndex);

	_vec3 vScale = { pTexInfo->tImgInfo.Width * m_fScale, pTexInfo->tImgInfo.Height * m_fScale, 1.f };
	//if (!m_bDir)
	//	vScale.x = -vScale.x;

	m_pTransformCom->Set_Scale(vScale);
}

void CGolem::Idle_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;
}

void CGolem::Run_State()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	m_tFrame.fFrameSpeed = 20.f;

	m_bAnimFinish = false;
	m_bAnimRepeat = true;
}

void CGolem::Attack_State()
{
}

void CGolem::Attack_FT_State()
{
}

_int CGolem::Idle_Update(const _float & fTimeDelta)
{
	return _int();
}

_int CGolem::Run_Update(const _float & fTimeDelta)
{
	return _int();
}

_int CGolem::Attack_Update(const _float & fTimeDelta)
{
	return _int();
}

_int CGolem::Attack_FT_Update(const _float & fTimeDelta)
{
	return _int();
}

void CGolem::Hit(const _int & iAtk, const _vec3 * pAtkPos)
{
	m_iHP -= iAtk;
	if (m_iHP <= 0)
		m_bIsDead = true;

}

CGolem* CGolem::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos)
{
	CGolem* pInstance = new CGolem(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(pPos);

	return pInstance;
}

void CGolem::Free()
{
	Engine::CGameObject::Free();
}

