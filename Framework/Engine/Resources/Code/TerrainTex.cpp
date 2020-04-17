#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
	, m_dwCntX(rhs.m_dwCntX)
	, m_dwCntZ(rhs.m_dwCntZ)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
{

}

Engine::CTerrainTex::~CTerrainTex()
{

}

HRESULT Engine::CTerrainTex::Ready_Buffer(const _tchar* pPath, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv /*= 1*/)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong*		pPixel = nullptr;

	if (wcscmp(pPath, L"") != 0)
	{
		_ulong	dwByte = 0;

		m_hFile = CreateFile(pPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
		ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

		ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

		CloseHandle(m_hFile);
	}
	

	_ulong		dwIndex = 0;
	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong z = 0; z < dwCntZ; ++z)
	{
		for (_ulong x = 0; x < dwCntZ; ++x)
		{
			dwIndex = z * dwCntX + x;

			_float fHeight = nullptr != pPixel ? (pPixel[dwIndex] & 0x000000ff) / 20.f : 0.f;

			pVertex[dwIndex].vPos = _vec3(_float(x * dwVtxItv), fHeight, _float(z * dwVtxItv));
			pVertex[dwIndex].vTexUV = _vec2(_float(x) / (dwCntX - 1) * 20.f, _float(z) / (dwCntZ - 1) * 20.f);
		}
	}

	m_pVB->Unlock();
	Safe_Delete_Array(pPixel);

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
			++dwTriCnt;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv /*= 1*/)
{
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(pPath, dwCntX, dwCntZ, dwVtxItv)))
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

	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
}

