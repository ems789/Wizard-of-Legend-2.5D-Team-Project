#ifndef TerrainGuidLine_h__
#define TerrainGuidLine_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CTerrainGuidLine : public Engine::CGameObject
{
private:
	explicit CTerrainGuidLine(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrainGuidLine(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CTerrainTex*	m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

public:
	static CTerrainGuidLine*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // TerrainGuidLine_h__
