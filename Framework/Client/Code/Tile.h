#ifndef Tile_h__
#define Tile_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTileTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CTile : public Engine::CGameObject
{
private:
	explicit CTile(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTile(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(void) override;

public:
	const _vec3*	Get_Pos(); 
	const bool		Get_Render(); 
	void			Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void			Set_Pos(const _vec3& vPos);
	void			Set_Render(const bool& bIsRender);

public:
	void			TextureChange();

private:
	HRESULT			Add_Component(void);

private:
	Engine::CTileTex*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	bool					m_bIsRender = false;

public:
	static CTile*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // Tile_h__
