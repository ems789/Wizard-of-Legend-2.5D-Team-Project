#include "stdafx.h"
#include "NumberFont.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CNumberFont)

CNumberFont::CNumberFont()
{
}

CNumberFont::~CNumberFont()
{
	Free();
}

HRESULT CNumberFont::Ready_NumberFont(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Number", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Number/%d.png", 10), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	

	return S_OK;
}

void CNumberFont::DrawNumber(const _int & iNum, const _vec3 * pPos, const _float & fScale, D3DXCOLOR d3dColor)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	_int iTemp = iNum;
	stack<_int> NumStack;
	while (iTemp)
	{
		NumStack.push(iTemp % 10);
		iTemp /= 10;
	}

	_int	iSize = NumStack.size();
	_float	fWidth = iSize * 6 * fScale;

	_vec3 vPos = *pPos;
	vPos.x -= fWidth * 0.5f;

	//_vec3 vCamAngle;
	//Engine::Get_MainCameraAngle(&vCamAngle);
	//vCamAngle.x = 0.f;

	_matrix matWorld, matRotY, matRotZ;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationY(&matRotY, vCamAngle.y);
	//D3DXMatrixRotationZ(&matRotZ, vCamAngle.z);
	matWorld._11 = 5.f * fScale;
	matWorld._22 = 6.f * fScale;

	//matWorld *= matRotY * matRotZ;

	memcpy(&matWorld.m[3][0], &vPos, sizeof(_vec3));
	

	for (_int i = 0; i < iSize; ++i)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pTextureCom->Render_Texture(NumStack.top());
		m_pBufferCom->Set_Color(d3dColor);
		m_pBufferCom->Render_Buffer();
		NumStack.pop();
		matWorld._41 += 6 * fScale;
	}

	if (0 == iSize)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Set_Color(d3dColor);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CNumberFont::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcAlphaTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcColTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Number"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

void CNumberFont::Free()
{
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);

	Engine::Safe_Release(m_pGraphicDev);
}
