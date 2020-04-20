#include "stdafx.h"
#include "Terrain.h"
#include "Tile.h"
#include "Wall.h"

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

	m_vecWallList.reserve(m_dwTileX * m_dwTileZ);
	m_vecWallList.resize(m_dwTileX * m_dwTileZ);
	//CWall* pWall1 = nullptr;
	//for (::_ulong i = 0; i < m_dwTileZ; ++i)
	//{
	//	for (::_ulong j = 0; j < m_dwTileX; ++j)
	//	{
	//		fX = (j * m_dwItv) + (m_dwItv * 0.5f);
	//		fZ = (i * m_dwItv) + (m_dwItv * 0.5f);
	//		fY = 0.f;

	//		pWall1 = CWall::Create(m_pGraphicDev);
	//		pWall1->Set_Pos(fX, fY, fZ);
	//		pWall1->Set_Render(false);

	//		//m_vecWallList[i * m_dwTileX + j].push_back(pWall1);
	//	}
	//}

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

		for (::_ulong j = 0; j < m_dwTileX; ++j)
		{
			for (::_ulong k = 0; k < m_vecWallList[i * m_dwTileX + j].size(); ++k)
				m_vecWallList[i * m_dwTileX + j][k]->Update_GameObject(fTimeDelta);
		}
	}

	return 0;
}

void CTerrain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());

	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for (::_ulong j = 0; j < m_dwTileX; ++j)
			m_vecTile[i * m_dwTileX + j]->Render_GameObject();

		for (::_ulong j = 0; j < m_dwTileX; ++j)
		{
			for (::_ulong k = 0; k < m_vecWallList[i * m_dwTileX + j].size(); ++k)
				m_vecWallList[i * m_dwTileX + j][k]->Render_GameObject();
		}
	}
}

void CTerrain::Release_TileList(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

void CTerrain::Release_WallList(void)
{
	for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
		for_each(m_vecWallList[i].begin(), m_vecWallList[i].end(), CDeleteObj());
	m_vecWallList.clear();
}

void CTerrain::TileChange(const ::_vec3& vPos, const ::_tchar* tileTag, const int iDrawID, bool bIsRender)
{
	m_vecTile[(int)vPos.z * m_dwTileX + (int)vPos.x]->Set_DrawID(iDrawID);
	m_vecTile[(int)vPos.z * m_dwTileX + (int)vPos.x]->Set_Render(bIsRender);
}

void CTerrain::WallChange(const ::_vec3& vPos, const ::_tchar* wallTag, const int iDrawID, bool bIsRender, bool bHasLeftWall, bool bHasTopWall, bool bHasRightWall, bool bHasBottomWall)
{
	// 왼클릭 - 벽 추가
	if (bIsRender)
	{
		CWall* pWall = CWall::Create(m_pGraphicDev, bHasLeftWall, bHasTopWall, bHasRightWall, bHasBottomWall);
		pWall->Set_DrawID(iDrawID);
		pWall->Set_Render(bIsRender);
		pWall->Set_Pos((int)vPos.x + m_dwItv * 0.5f,
			m_vecWallList[(int)vPos.z * m_dwTileX + (int)vPos.x].size(),
			(int)vPos.z + m_dwItv * 0.5f);

		m_vecWallList[(int)vPos.z * m_dwTileX + (int)vPos.x].push_back(pWall);
	}
	// 우클릭 - 벽 제거
	else
	{
		if (m_vecWallList[(int)vPos.z * m_dwTileX + (int)vPos.x].size() > 0)
		{
			CWall* pWall = m_vecWallList[(int)vPos.z * m_dwTileX + (int)vPos.x].back();
			Engine::Safe_Release(pWall);
			m_vecWallList[(int)vPos.z * m_dwTileX + (int)vPos.x].pop_back();
		}
	}
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
		tTempTileInfo.dwDrawID = m_vecTile[i]->Get_DrawID();
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
	
	ReadFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);

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
	pToolView->m_pTerrain->Release_TileList();
	pToolView->m_pTerrain->Release_WallList(); 
	pToolView->m_pTerrainGuidLine->Release();

	// 터레인 정보를 불러옴
	m_dwTileX = tTempTerrainInfo.dwTileX;
	m_dwTileZ = tTempTerrainInfo.dwTileZ;
	m_vecTile.reserve(m_dwTileX * m_dwTileZ);
	m_vecWallList.reserve(m_dwTileX * m_dwTileZ);
	m_vecWallList.resize(m_dwTileX * m_dwTileZ);
	m_dwItv = tTempTerrainInfo.dwItv;

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
		pTile->Set_DrawID(tTempTileInfo.dwDrawID);

		m_vecTile.push_back(pTile);
	}


	pToolView->m_pTerrainGuidLine = CTerrainGuidLine::Create(pGraphicDev);
	NULL_CHECK_MSG(pToolView->m_pTerrainGuidLine, L"Terrain Create Failed");

	pGraphicDev->Release();

	CloseHandle(hFile);
}

void CTerrain::SaveWall(const ::_tchar * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwBytes = 0;
	WALL_INFO tTempWallInfo;
	TERRAIN_INFO tTempTerrainInfo{ m_dwTileX, m_dwTileZ, m_dwItv };

	// 터레인 정보를 저장
	WriteFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);

	// 벽 정보를 저장
	for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
	{
		for (::_ulong j = 0; j < m_vecWallList[i].size(); ++j)
		{
			tTempWallInfo.vPos = *m_vecWallList[i][j]->Get_Pos();
			tTempWallInfo.bRender = m_vecWallList[i][j]->Get_Render();
			tTempWallInfo.dwItv = m_dwItv;
			tTempWallInfo.dwIndex = i;
			tTempWallInfo.dwDrawID = m_vecWallList[i][j]->Get_DrawID();
			for (::_ulong k = 0; k < WALL_END; ++k)
				tTempWallInfo.bHasWall[k] = m_vecWallList[i][j]->Get_HasWall((WALLPOSITION)k);
			WriteFile(hFile, &tTempWallInfo, sizeof(WALL_INFO), &dwBytes, nullptr);
		}
	}

	CloseHandle(hFile);
}

void CTerrain::LoadWall(const ::_tchar * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	if (!m_vecWallList.empty())
	{
		for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
			for_each(m_vecWallList[i].begin(), m_vecWallList[i].end(), CDeleteObj());
		m_vecWallList.clear();
	}

	DWORD dwBytes = 0;
	WALL_INFO tTempWallInfo;
	TERRAIN_INFO tTempTerrainInfo;

	// 불러온 지형에 맞춰 버퍼 재구성
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	LPDIRECT3DDEVICE9 pGraphicDev = Engine::CGraphicDev::GetInstance()->GetDevice();
	pGraphicDev->AddRef();
	FAILED_CHECK_VOID(pGraphicDev);

	Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TerrainTex");
	//Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TileTex");
	pToolView->m_pTerrain->Release_WallList();
	pToolView->m_pTerrainGuidLine->Release();

	// 터레인 정보를 불러옴
	ReadFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);
	m_vecWallList.reserve(tTempTerrainInfo.dwTileX * tTempTerrainInfo.dwTileZ);
	m_vecWallList.resize(tTempTerrainInfo.dwTileX * tTempTerrainInfo.dwTileZ);
	m_dwTileX = tTempTerrainInfo.dwTileX;
	m_dwTileZ = tTempTerrainInfo.dwTileZ; 
	m_dwItv = tTempTerrainInfo.dwItv;

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX,
		L"",
		tTempTerrainInfo.dwTileX + 1,
		tTempTerrainInfo.dwTileZ + 1,
		tTempTerrainInfo.dwItv),
		E_FAIL);

	// 타일 정보를 불러옴
	CWall* pWall = nullptr;
	while (true)
	{
		ReadFile(hFile, &tTempWallInfo, sizeof(WALL_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		pWall = CWall::Create(m_pGraphicDev, tTempWallInfo.bHasWall[WALL_LEFT], tTempWallInfo.bHasWall[WALL_TOP], tTempWallInfo.bHasWall[WALL_RIGHT], tTempWallInfo.bHasWall[WALL_BOTTOM]);
		pWall->Set_Pos(tTempWallInfo.vPos.x, tTempWallInfo.vPos.y, tTempWallInfo.vPos.z);
		pWall->Set_Render(tTempWallInfo.bRender);
		pWall->Set_DrawID(tTempWallInfo.dwDrawID);

		m_vecWallList[tTempWallInfo.dwIndex].push_back(pWall);
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

	for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
		for_each(m_vecWallList[i].begin(), m_vecWallList[i].end(), CDeleteObj());
	m_vecWallList.clear();
}

