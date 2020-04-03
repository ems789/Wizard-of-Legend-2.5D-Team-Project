#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
{
	_uint	iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();

}

Engine::CTexture::~CTexture(void)
{

}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, 
										TEXTURETYPE eType, 
										const _uint& iCnt /*= 1*/)
{
	m_vecTexture.reserve(iCnt);
	
	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CTexture::Clone(void)
{
	return new CTexture(*this);
}

void Engine::CTexture::Free(void)
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();

	CResources::Free();
}

