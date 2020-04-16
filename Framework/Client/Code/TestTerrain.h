#ifndef TestTerrain_h__
#define TestTerrain_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTerrainTex;
class CTransform;
class CRenderer;

END

class CTestTerrain : public Engine::CGameObject
{
private:
	explicit CTestTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestTerrain();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CTerrainTex*	m_pBufferCom	= nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

public:
	static CTestTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // TestTerrain_h__
