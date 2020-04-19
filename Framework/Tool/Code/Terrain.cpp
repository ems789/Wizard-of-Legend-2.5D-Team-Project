#include "stdafx.h"
#include "Terrain.h"
#include "Tile.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "TerrainGuidLine.h"

#include "Export_Function.h"

USING(Engine)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain(void)
{
	Free();
}

//HRESULT CTerrain::Ready_GameObject(void)
//{
//	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
//
//	return S_OK;
//}

HRESULT CTerrain::Ready_GameObject(::_ulong dwTileX, ::_ulong dwTileZ, ::_ulong dwItv)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	float fX = 0.f, fZ = 0.f, fY = 0.f;

	m_dwTileX = dwTileX;
	m_dwTileZ = dwTileZ;
	m_dwItv = dwItv;
	m_vecTile.reserve(m_dwTileX * m_dwTileZ);

	CTile* pTile = nullptr;
	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for(::_ulong j= 0; j < m_dwTileX; ++j)
		{			
			fX = (j * m_dwItv) + (m_dwItv * 0.5f);
			fZ = (i * m_dwItv) + (m_dwItv * 0.5f);
			fY = 0.f;

			pTile = CTile::Create(m_pGraphicDev);
			pTile->Set_Pos(fX, fY, fZ);
			pTile->Set_Render(false);

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

::_int CTerrain::Update_GameObject(const ::_float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for (::_ulong j = 0; j < m_dwTileX; ++j)
			m_vecTile[i * m_dwTileX + j]->Update_GameObject(fTimeDelta);
	}

	return 0;
}

void CTerrain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for (::_ulong j = 0; j < m_dwTileX; ++j)
			m_vecTile[i * m_dwTileX + j]->Render_GameObject();
	}

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTerrain::Release_GameObject(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

void CTerrain::TileChange(const ::_vec3& vPos, const ::_tchar* tileTag, bool bIsRender)
{
	//m_vecTile[(int)vPos.z * m_dwTileX + (int)vPos.x]->TextureChange();
	m_vecTile[(int)vPos.z * m_dwTileX + (int)vPos.x]->Set_Render(bIsRender);
}

void CTerrain::SaveTile(const ::_tchar* pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwBytes = 0;
	TILE_INFO tTempTileInfo;
	TERRAIN_INFO tTempTerrainInfo{ m_dwTileX, m_dwTileZ, m_dwItv };

	// 터레인 정보를 저장
	WriteFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);

	// 타일 정보를 저장
	for (::_ulong i = 0; i < m_vecTile.size(); ++i)
	{
		tTempTileInfo.vPos = *m_vecTile[i]->Get_Pos();
		tTempTileInfo.bRender = m_vecTile[i]->Get_Render();
		tTempTileInfo.dwItv = m_dwItv;
		WriteFile(hFile, &tTempTileInfo, sizeof(TILE_INFO), &dwBytes, nullptr);
	}

	CloseHandle(hFile);
}

void CTerrain::LoadTile(const ::_tchar* pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
		m_vecTile.clear();
	}

	DWORD dwBytes = 0;
	TILE_INFO tTempTileInfo;
	TERRAIN_INFO tTempTerrainInfo;
	
	// 터레인 정보를 불러옴
	ReadFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);
	m_vecTile.reserve(tTempTerrainInfo.dwTileX * tTempTerrainInfo.dwTileZ);
	m_dwTileX = tTempTerrainInfo.dwTileX;
	m_dwTileZ = tTempTerrainInfo.dwTileZ;
	m_dwItv = tTempTerrainInfo.dwItv;

	// 불러온 지형에 맞춰 버퍼 재구성
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	LPDIRECT3DDEVICE9 pGraphicDev = Engine::CGraphicDev::GetInstance()->GetDevice();
	pGraphicDev->AddRef();
	FAILED_CHECK_VOID(pGraphicDev);

	Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TerrainTex");
	Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TileTex");
	pToolView->m_pTerrain->Release_GameObject();
	pToolView->m_pTerrainGuidLine->Release();

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX,
		L"",
		tTempTerrainInfo.dwTileX + 1,
		tTempTerrainInfo.dwTileZ + 1,
		tTempTerrainInfo.dwItv),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TileTex",
		Engine::BUFFER_TILETEX,
		L"",
		0,
		0,
		tTempTerrainInfo.dwItv),
		E_FAIL);

	// 타일 정보를 불러옴
	CTile* pTile = nullptr;
	while (true)
	{
		ReadFile(hFile, &tTempTileInfo, sizeof(TILE_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		pTile = CTile::Create(m_pGraphicDev);
		pTile->Set_Pos(tTempTileInfo.vPos.x, tTempTileInfo.vPos.y, tTempTileInfo.vPos.z);
		pTile->Set_Render(tTempTileInfo.bRender);

		m_vecTile.push_back(pTile);
	}


	pToolView->m_pTerrainGuidLine = CTerrainGuidLine::Create(pGraphicDev);
	NULL_CHECK_MSG(pToolView->m_pTerrainGuidLine, L"Terrain Create Failed");

	pGraphicDev->Release();

	CloseHandle(hFile);
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}



CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, ::_ulong dwTileX, ::_ulong dwTileZ, ::_ulong dwItv)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(dwTileX, dwTileZ, dwItv)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

