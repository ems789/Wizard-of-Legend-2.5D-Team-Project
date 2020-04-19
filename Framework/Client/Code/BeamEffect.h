#ifndef BeamEffect_h__
#define BeamEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CBeamEffect : public Engine::CEffect
{
private:
	explicit CBeamEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBeamEffect();

public:
	HRESULT		Ready_GameObject(const _tchar* pTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	void	LifeTimer(const _float& fTimeDelta);
	void	Animation(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();

private:
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fThickness = 0.5f;
	_float		m_fLength = 10.f;

public:
	static CBeamEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _tchar* pEffectTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _vec3* pPos, const _float& fThickness, const _float& fLength, const _float& fAngleY, _bool bAnimRepeat, _float fLifeTime);

private:
	virtual void Free() override;
};

#endif // BeamEffect_h__
