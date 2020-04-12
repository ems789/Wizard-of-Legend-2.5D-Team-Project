#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
{
	_uint iContainerSize = rhs.m_vecTexInfo.size();
	m_vecTexInfo.reserve(iContainerSize);

	m_vecTexInfo = rhs.m_vecTexInfo;

	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexInfo[i]->pTexture->AddRef();
}

Engine::CTexture::~CTexture()
{

}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	m_vecTexInfo.reserve(iCnt);

	TEX_INFO* pTexInfo = nullptr;
	IDirect3DBaseTexture9*	pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		_tchar	szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);

		D3DXIMAGE_INFO tImgInfo;
		ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));
		FAILED_CHECK_RETURN(D3DXGetImageInfoFromFile(szFileName, &tImgInfo), E_FAIL);

		switch (eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		pTexInfo = new TEX_INFO;
		pTexInfo->pTexture = pTexture;
		pTexInfo->tImgInfo = tImgInfo;

		m_vecTexInfo.push_back(pTexInfo);
	}

	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexInfo.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexInfo[iIndex]->pTexture);
}

const TEX_INFO* CTexture::Get_TexInfo(const _uint& iIndex /*= 0*/, D3DXIMAGE_INFO* pOut/* = nullptr*/) const
{
	if (pOut)
		memcpy(pOut, &m_vecTexInfo[iIndex]->tImgInfo, sizeof(D3DXIMAGE_INFO));
	return m_vecTexInfo[iIndex];
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*	pInst = new CTexture(pGraphicDev);

	if (FAILED(pInst->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CTexture::Clone()
{
	return new CTexture(*this);
}

void Engine::CTexture::Free()
{
	//for_each(m_vecTexInfo.begin(), m_vecTexInfo.end(), CDeleteObj());
	for (auto& pTexInfo : m_vecTexInfo)
	{
		_ulong dwRef = Safe_Release(pTexInfo->pTexture);
		if (0 == dwRef)
		{
			delete pTexInfo;
			pTexInfo = nullptr;
		}
	}

	m_vecTexInfo.clear();

	CResources::Free();
}

