#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

Engine::CCamera::~CCamera()
{

}

HRESULT CCamera::Ready_GameObject(void)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	FAILED_CHECK_RETURN_MSG(Add_Component(), E_FAIL, L"Camera Failed Add_Component");

	return S_OK;
}

_int CCamera::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	View_Proj_Update();

	return 0;
}

const _vec3 * CCamera::Get_Info(INFO eType, _vec3 * pInfo)
{
	if (pInfo)
		m_pTransformCom->Get_Info(eType, pInfo);

	return m_pTransformCom->Get_Info(eType);
}

const _vec3 * CCamera::Get_Angle(_vec3 * pRadians)
{
	return m_pTransformCom->Get_Angle(pRadians);
}

HRESULT CCamera::Add_Component()
{
	CComponent* pComponent = m_pTransformCom = CTransform::Create();
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent->emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CCamera::View_Proj_Update()
{
	_vec3 vEye, vAt, vUp;

	m_pTransformCom->Get_Info(INFO_POS,		&vEye);
	m_pTransformCom->Get_Info(INFO_LOOK,	&vAt);
	m_pTransformCom->Get_Info(INFO_UP,		&vUp);

	vAt += vEye;

	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovy, m_fAspect, m_fNearZ, m_fFarZ);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}


void CCamera::Free(void)
{
	CGameObject::Free();
}

