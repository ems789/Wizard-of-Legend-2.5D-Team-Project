#ifndef Dust_h__
#define Dust_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcAlphaTex;
class CRenderer;

END

class CDust : public Engine::CEffect
{
private:
	explicit CDust(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDust();

public:
	HRESULT		Ready_GameObject(const _tchar* pTextureTag, const D3DXCOLOR & d3dColor, const _float& fScaleDelta);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


private:
	HRESULT	Add_Resource(const _tchar* pTextureTag);
	HRESULT	Add_Component(const _tchar* pTextureTag, const D3DXCOLOR& d3dColor);
	void	LifeTimer(const _float& fTimeDelta);
	void	Animation(const _float& fTimeDelta);
	void	ScaleChange_And_Move(const _float& fTimeDelta);
	void	Update_Scale();
	void	Turn_To_Camera_Look();

private:
	Engine::CRcAlphaTex* m_pBufferColCom = nullptr;
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fScale = 1.f;
	D3DXCOLOR	m_d3dColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	_vec3		m_vMoveDir;
	_float		m_fSpeed = 1.f;

	_float		m_fScaleDelta = 0.f;

	_float		m_fScaleMin = 0.f;
	_float		m_fScaleMax = 0.f;

	static _bool m_bAddResource;


public:
	static CDust* Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, const _vec3* pMoveDir, const _float fSpeed,
		_bool bAnimRepeat, _float fLifeTime, const D3DXCOLOR& d3dColor, 
		const _float& fScaleDelta, const _float& fScaleMin, const _float& fScaleMax);

private:
	virtual void Free() override;
};

#endif // Dust_h__
