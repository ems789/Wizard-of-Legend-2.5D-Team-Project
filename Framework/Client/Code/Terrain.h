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
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	HRESULT		 Ready_GameObject(const _tchar* path);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	void		 Release_GameObject(void);
	
public:
	HRESULT		LoadTile(const _tchar* pFilePath);

private:
	HRESULT		Add_Component(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	vector<CTile*>			m_vecTile;
	_ulong					m_dwTileX = 0, m_dwTileZ = 0;
	_ulong					m_dwItv;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* path);

private:
	virtual void Free(void) override;
};

#endif // Terrain_h__
