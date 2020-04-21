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
	//virtual HRESULT Ready_GameObject(void) override;
	HRESULT		 Ready_GameObject(_ulong dwTileX, _ulong dwTileZ, _ulong dwItv);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	void		 Release_TileList(void);
	void		 Release_WallList(void);
	

public:
	void		TileChange(const _vec3& vPos, const _tchar* tileTag, const int iDrawID ,bool bIsRender);
	void		WallChange(const _vec3& vPos, const _tchar* wallTag, const int iDrawID, bool bIsRender, bool bHasLeftWall, bool bHasTopWall, bool bHasRightWall, bool bHasBottomWall, bool bHasCeilingWall, Engine::WALLTYPE eWallType);
	void		SaveTile(const _tchar* pFilePath);
	void		LoadTile(const _tchar* pFilePath);
	void		SaveWall(const _tchar* pFilePath);
	void		LoadWall(const _tchar* pFilePath);

private:
	HRESULT		Add_Component(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	vector<CTile*>			m_vecTile;
	vector<vector<CWall*>>	m_vecWallList;
	_ulong					m_dwTileX = 0, m_dwTileZ = 0;
	_ulong					m_dwItv;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwTileX, _ulong dwTileZ, _ulong dwItv);

private:
	virtual void Free(void) override;
};

#endif // Terrain_h__
