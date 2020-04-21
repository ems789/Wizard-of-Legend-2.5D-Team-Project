#ifndef AlphaLaidEffect_h__
#define AlphaLaidEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcAlphaTex;
class CRenderer;

END

class CAlphaLaidEffect : public Engine::CEffect
{
private:
	explicit CAlphaLaidEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAlphaLaidEffect();

public:
	HRESULT		Ready_GameObject(const _tchar* pTextureTag, const D3DXCOLOR& d3dColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT Add_Component(const _tchar* pTextureTag, const D3DXCOLOR& d3dColor);
	void	LifeTimer(const _float& fTimeDelta);
	void	Animation(const _float& fTimeDelta);
	void	Update_Scale();
	void	Turn_To_Camera_Look();

private:
	Engine::CRcAlphaTex* m_pBufferColCom = nullptr;
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fScale = 1.f;
	D3DXCOLOR	m_d3dColor;

public:
	static CAlphaLaidEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _tchar* pEffectTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, const _float& fAngleY, _bool bAnimRepeat, _float fLifeTime, const D3DXCOLOR& d3dColor);

private:
	virtual void Free() override;
};

#endif // AlphaLaidEffect_h__
