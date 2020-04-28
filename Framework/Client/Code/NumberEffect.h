#ifndef NumberEffect_h__
#define NumberEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRenderer;

END

class CNumberEffect : public Engine::CEffect
{
private:
	explicit CNumberEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNumberEffect();

public:
	HRESULT		Ready_GameObject(const D3DXCOLOR& d3dColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT Add_Component(const D3DXCOLOR& d3dColor);
	void	LifeTimer(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();
	void	Move_Update(const _float& fTimeDelta);

private:
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_float		m_fStartFrame = 0.f;
	_bool		m_bAnimRepeat = false;

	_float		m_fScale = 1.f;
	D3DXCOLOR	m_d3dColor;
	_int		m_iNumber = 0;
	_float		m_fUpForce = 15.f;


	_float	m_fDownSpeed = 0.f;
	_float	m_fGravity = 98.f;
	_float	m_fSpeed = 5.f;


public:
	static CNumberEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev,	const _float& fScale, const _vec3* pPos, _float fLifeTime, const D3DXCOLOR& d3dColor, const _int& iNumber);

private:
	virtual void Free() override;
};

#endif // NumberEffect_h__
