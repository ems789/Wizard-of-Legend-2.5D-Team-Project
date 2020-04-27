#ifndef ImageObject_h__
#define ImageObject_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CImageObject : public Engine::CGameObject
{
private:
	explicit CImageObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CImageObject();

public:
	HRESULT			Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

private:
	HRESULT Add_Component(const _tchar* pTextureTag);
	void	Animation(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();
	void	Update_Scale();

private:	//	Components
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

private:
	_float			m_fScale = 0.05f;
	FRAME			m_tFrame;

public:
	static CImageObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed);

private:
	virtual void Free() override;
};


#endif // ImageObject_h__
