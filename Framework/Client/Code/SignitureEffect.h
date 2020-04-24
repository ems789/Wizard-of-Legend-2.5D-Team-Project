#ifndef SignitureEffect_h__
#define SignitureEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CSignitureEffect : public Engine::CEffect
{
private:
	explicit CSignitureEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSignitureEffect();

public:
	HRESULT		Ready_GameObject(const _tchar* pTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	void	Turn_To_Camera_Look();
	void	DecreaseLength(const _float& fTimeDelta);

private:
	_float		m_fCumulativeTime = 0.f;
	_float		m_fLifeTime = 0.f;
	FRAME		m_tFrame;
	_bool		m_bAnimRepeat = false;

	_float		m_fThickness = 0.5f;
	_float		m_fLength = 10.f;
	_float		m_fDieLength = 10.f;
	_float		m_fDLengthSpeed = 5.f;
	_float		m_fDelayCount = 0.f;
	_float		m_fDelayTime = 0.3f;
	_bool		m_bIncrease = false;
	

public:
	static CSignitureEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _vec3* pPos, const _float& fThickness, const _float& fLength, const _float& fDieLength, const _float& fDLengthSpeed, const _vec3* pAngle);

private:
	virtual void Free() override;
};

#endif // SignitureEffect_h__
