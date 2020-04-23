#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CTile;
class CWall;
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	HRESULT		 Ready_GameObject(const _tchar* pTilePath, const _tchar* pWallPath);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	void		 Release_TileList(void);
	void		 Release_WallList(void);
	
public:
	HRESULT		LoadTile(const _tchar* pFilePath);
	HRESULT		LoadWall(const _tchar* pFilePath);

public:		//	Get Functions
	const _ulong		Get_TileX()	const { return m_dwTileX; }
	const _ulong		Get_TileZ()	const { return m_dwTileZ; }
	const _ulong		Get_Interval() const { return m_dwItv; }

public:
	_bool			Check_TileWall(const _ulong& dwIndexX, const _ulong& dwIndexZ);
	WALLPOSITION	Check_Wall(const _ulong& dwIndexX, const _ulong& dwIndexZ);
	const _bool*	Get_HasWall(const _ulong& dwIndexX, const _ulong& dwIndexZ);

private:
	HRESULT		Add_Component(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	vector<CTile*>			m_vecTile;
	vector<vector<CWall*>>	m_vecWallList;
	_ulong					m_dwTileX = 0, m_dwTileZ = 0;
	_ulong					m_dwItv;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTilePath, const _tchar* pWallPath);

private:
	virtual void Free(void) override;
};

#endif // Terrain_h__
