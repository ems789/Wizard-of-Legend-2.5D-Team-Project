#include "RcAlphaTex.h"

USING(Engine)

Engine::CRcAlphaTex::CRcAlphaTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcAlphaTex::CRcAlphaTex(const CRcAlphaTex& rhs)
	: CVIBuffer(rhs)
{
	
}

Engine::CRcAlphaTex::~CRcAlphaTex()
{

}

HRESULT Engine::CRcAlphaTex::Ready_Buffer(const D3DXCOLOR& d3dColor)
{
	m_d3dColor = d3dColor;

	m_dwVtxSize = sizeof(VTXCOLTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_COLTEX;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOLTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].dwColor = d3dColor;
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].dwColor = d3dColor;
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = d3dColor;
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].dwColor = d3dColor;
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//	0
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	//	1
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRcAlphaTex::Set_Color(const D3DXCOLOR & d3dColor)
{
	VTXCOLTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].dwColor = d3dColor;

	pVertex[1].dwColor = d3dColor;

	pVertex[2].dwColor = d3dColor;

	pVertex[3].dwColor = d3dColor;

	m_pVB->Unlock();
}

Engine::CRcAlphaTex* Engine::CRcAlphaTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXCOLOR& d3dColor)
{
	CRcAlphaTex* pInst = new CRcAlphaTex(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(d3dColor)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CRcAlphaTex::Clone()
{
	return new CRcAlphaTex(*this);
}

void Engine::CRcAlphaTex::Free()
{
	CVIBuffer::Free();
}

