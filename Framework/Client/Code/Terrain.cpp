#include "stdafx.h"
#include "Terrain.h"
#include "Tile.h"
#include "Wall.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain(void)
{
}

/// TODO : 경로 추가해서 로드하는 걸로
HRESULT CTerrain::Ready_GameObject(const ::_tchar* pTilePath, const ::_tchar* pWallPath)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(LoadTile(pTilePath), E_FAIL);
	FAILED_CHECK_RETURN(LoadWall(pWallPath), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	for (::_ulong i = 0; i < m_dwTileZ; ++i)
	{
		for (::_ulong j = 0; j < m_dwTileX; ++j)
			m_vecTile[i * m_dwTileX + j]->Update_GameObject(0);

		for (::_ulong j = 0; j < m_dwTileX; ++j)
		{
			for (::_ulong k = 0; k < m_vecWallList[i * m_dwTileX + j].size(); ++k)
				m_vecWallList[i * m_dwTileX + j][k]->Update_GameObject(0);
		}
	}

	return S_OK;
}

::_int CTerrain::Update_GameObject(const ::_float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Terrain", this);

	return 0;
}

void CTerrain::Render_GameObject(void)
{
	const Engine::CTransform* pTransform = dynamic_cast<const Engine::CTransform*>(Engine::Get_Component_of_Player(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vPlayerPos = *pTransform->GetInfo(Engine::INFO_POS);

	_int iX = static_cast<_int>(vPlayerPos.x);
	_int iZ = static_cast<_int>(vPlayerPos.z);

	_int iStartX = iX - 36;
	_int iEndX = iX + 36;
	_int iStartZ = iZ - 35;
	_int iEndZ = iZ + 35;

	if (iStartX < 0)
		iStartX = 0;
	else if (iStartX >= m_dwTileX)
		iStartX = m_dwTileX - 1;

	if (iEndX < 1)
		iEndX = 1;
	else if (iEndX > m_dwTileX)
		iEndX = m_dwTileX;

	if (iStartZ < 0)
		iStartZ = 0;
	else if (iStartZ >= m_dwTileZ)
		iStartZ = m_dwTileZ - 1;

	if (iEndZ < 0)
		iEndZ = 1;
	else if (iEndZ > m_dwTileZ)
		iEndZ = m_dwTileZ;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());
	
	for (::_ulong i = iStartZ; i < iEndZ; ++i)
	{
		for (::_ulong j = iStartX; j < iEndX; ++j)
		{
			m_vecTile[i * m_dwTileX + j]->Render_GameObject();

			for (::_ulong k = 0; k < m_vecWallList[i * m_dwTileX + j].size(); ++k)
				m_vecWallList[i * m_dwTileX + j][k]->Render_GameObject();
		}
	}

}

void CTerrain::Release_TileList(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Engine::CDeleteObj());
	m_vecTile.clear();
}

void CTerrain::Release_WallList(void)
{
	for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
		for_each(m_vecWallList[i].begin(), m_vecWallList[i].end(), Engine::CDeleteObj());
	m_vecWallList.clear();
}

HRESULT CTerrain::LoadTile(const ::_tchar* pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(), Engine::CDeleteObj());
		m_vecTile.clear();
	}

	DWORD dwBytes = 0;
	TILE_INFO tTempTileInfo;
	TERRAIN_INFO tTempTerrainInfo;

	// 터레인 정보를 불러옴
	ReadFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);
	m_dwTileX = tTempTerrainInfo.dwTileX;
	m_dwTileZ = tTempTerrainInfo.dwTileZ;
	m_vecTile.reserve(m_dwTileX * m_dwTileZ);
	m_vecWallList.reserve(m_dwTileX * m_dwTileZ);
	m_vecWallList.resize(m_dwTileX * m_dwTileZ);
	m_dwItv = tTempTerrainInfo.dwItv;

	LPDIRECT3DDEVICE9 pGraphicDev = Engine::CGraphicDev::GetInstance()->GetDevice();
	FAILED_CHECK_RETURN(pGraphicDev, E_FAIL);


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

	CloseHandle(hFile); 
	return S_OK;
}

HRESULT	CTerrain::LoadWall(const ::_tchar* pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_vecWallList.empty())
	{
		for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
			for_each(m_vecWallList[i].begin(), m_vecWallList[i].end(), Engine::CDeleteObj());
		m_vecWallList.clear();
	}

	DWORD dwBytes = 0;
	WALL_INFO tTempWallInfo;
	TERRAIN_INFO tTempTerrainInfo;


	LPDIRECT3DDEVICE9 pGraphicDev = Engine::CGraphicDev::GetInstance()->GetDevice();
	FAILED_CHECK_RETURN(pGraphicDev, E_FAIL);

	Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TerrainTex");
	//Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TileTex");

	// 터레인 정보를 불러옴
	ReadFile(hFile, &tTempTerrainInfo, sizeof(TERRAIN_INFO), &dwBytes, nullptr);
	m_vecWallList.reserve(tTempTerrainInfo.dwTileX * tTempTerrainInfo.dwTileZ);
	m_vecWallList.resize(tTempTerrainInfo.dwTileX * tTempTerrainInfo.dwTileZ);
	m_dwTileX = tTempTerrainInfo.dwTileX;
	m_dwTileZ = tTempTerrainInfo.dwTileZ; 
	m_dwItv = tTempTerrainInfo.dwItv;

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX,
		Engine::WALL_OUTER,
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
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

		pWall = CWall::Create(m_pGraphicDev, tTempWallInfo.bHasWall[WALL_LEFT], tTempWallInfo.bHasWall[WALL_TOP], tTempWallInfo.bHasWall[WALL_RIGHT], tTempWallInfo.bHasWall[WALL_BOTTOM], tTempWallInfo.bHasWall[WALL_CEILING], tTempWallInfo.eWallType);
		pWall->Set_Pos(tTempWallInfo.vPos.x, tTempWallInfo.vPos.y, tTempWallInfo.vPos.z);
		pWall->Set_Render(tTempWallInfo.bRender);
		pWall->Set_DrawID(tTempWallInfo.dwDrawID);

		m_vecWallList[tTempWallInfo.dwIndex].push_back(pWall);
	}

	CloseHandle(hFile);
	return S_OK;
}


_bool CTerrain::Check_TileWall(const _ulong & dwIndexX, const _ulong & dwIndexZ)
{
	_int	iIndex = dwIndexZ * m_dwTileX + dwIndexX;
	if (iIndex < 0)
		return false;
	if (m_vecTile.size() <= iIndex)
		return false;

	return !m_vecWallList[iIndex].empty();
}

WALLPOSITION CTerrain::Check_Wall(const _ulong & dwIndexX, const _ulong & dwIndexZ)
{
	_int	iIndex = dwIndexZ * m_dwTileX + dwIndexX;
	if (iIndex < 0)
		return WALL_END;
	if (m_vecTile.size() <= iIndex)
		return WALL_END;
	if (m_vecWallList[iIndex].empty())
		return WALL_END;

	for (_int i = 0; i < WALL_END; ++i)
		if (m_vecWallList[iIndex].front()->Get_HasWall((WALLPOSITION)i))
			return (WALLPOSITION)i;

	return WALL_END;
}

const _bool * CTerrain::Get_HasWall(const _ulong & dwIndexX, const _ulong & dwIndexZ)
{
	_int	iIndex = dwIndexZ * m_dwTileX + dwIndexX;
	if (iIndex < 0)
		return nullptr;
	if (m_vecTile.size() <= iIndex)
		return nullptr;
	if (m_vecWallList[iIndex].empty())
		return nullptr;

	return m_vecWallList[iIndex].front()->Get_HasWall();
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"Terrain Add_Component failed. m_pRenderer is nullptr");
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}



CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, const ::_tchar* pTilePath, const ::_tchar* pWallPath)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTilePath, pWallPath)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	for_each(m_vecTile.begin(), m_vecTile.end(), Engine::CDeleteObj());
	m_vecTile.clear();

	for (::_ulong i = 0; i < m_vecWallList.size(); ++i)
		for_each(m_vecWallList[i].begin(), m_vecWallList[i].end(), Engine::CDeleteObj());
	m_vecWallList.clear();
}

