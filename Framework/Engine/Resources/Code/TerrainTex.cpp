#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTerrainTex::~CTerrainTex()
{

}

HRESULT Engine::CTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv /*= 1*/)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong		dwIndex = 0;
	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong z = 0; z < dwCntZ; ++z)
	{
		for (_ulong x = 0; x < dwCntZ; ++x)
		{
			dwIndex = z * dwCntX + x;

			pVertex[dwIndex].vPos = _vec3(_float(x * dwVtxItv), 0.f, _float(z * dwVtxItv));
			pVertex[dwIndex].vTexUV = _vec2(_float(x) / (dwCntX - 1), _float(z) / (dwCntZ - 1));
		}
	}

	m_pVB->Unlock();

	_ulong	dwTriCnt = 0;

	INDEX32*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong z = 0; z < dwCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < dwCntX - 1; ++x)
		{
			dwIndex = z * dwCntX + x;

			//	Upper right triangle
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2= dwIndex + 1;
			++dwTriCnt;

			//	Lower left triangle
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv /*= 1*/)
{
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CTerrainTex::Clone()
{
	return new CTerrainTex(*this);
}

void Engine::CTerrainTex::Free()
{
	CVIBuffer::Free();
}

