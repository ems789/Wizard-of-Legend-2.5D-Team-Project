#include "CeilingWallTex.h"

USING(Engine)

Engine::CCeilingWallTex::CCeilingWallTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_bClone(false)
{

}

Engine::CCeilingWallTex::CCeilingWallTex(const CCeilingWallTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
{

}

Engine::CCeilingWallTex::~CCeilingWallTex(void)
{

}

HRESULT Engine::CCeilingWallTex::Ready_Buffer(const _ulong dwVtxItv)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);		// 1. �޸� ��״� ����, 2. 3���ڸ� ���ؼ� ����� ���ؽ� ���� ������ ����
												// ������ �� 
	pVertex[0].vPos = _vec3(dwVtxItv * -0.5f, 0.f, dwVtxItv * 0.5f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(dwVtxItv * 0.5f, 0.f, dwVtxItv * 0.5f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(dwVtxItv * 0.5f, 0.f, dwVtxItv * -0.5f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(dwVtxItv * -0.5f, 0.f, dwVtxItv * -0.5f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 0
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 1
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CResources* Engine::CCeilingWallTex::Clone(void)
{
	return new CCeilingWallTex(*this);
}

Engine::CCeilingWallTex* Engine::CCeilingWallTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
												const _ulong& dwVtxItv)
{
	CCeilingWallTex*	pInstance = new CCeilingWallTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCeilingWallTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
}
