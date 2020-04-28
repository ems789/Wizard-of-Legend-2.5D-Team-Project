#ifndef Portal_h__
#define Portal_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CPortal : public Engine::CEffect
{
private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortal();

public:
	HRESULT			Ready_GameObject(const _tchar* pTextureTag);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;


private:
	void	Animation(const _float& fTimeDelta);
	void	Update_Scale();
	void	Generate_Dust(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();

private:
	_float		m_fCumulativeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fScale = 1.f;
	
	_float		m_fTimer = 0.f;

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos);

private:
	virtual void Free() override;
};

#endif // Portal_h__
