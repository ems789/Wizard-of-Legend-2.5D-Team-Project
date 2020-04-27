#ifndef BasicFollowingEffect_h__
#define BasicFollowingEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CBasicFollowingEffect : public Engine::CEffect
{
private:
	explicit CBasicFollowingEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBasicFollowingEffect();

public:
	HRESULT		Ready_GameObject(const _tchar* pTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


private:
	void	LifeTimer(const _float& fTimeDelta);
	void	Animation(const _float& fTimeDelta);
	void	Update_Scale();
	void	Turn_To_Camera_Look();

private:
	_float		 m_fCumulativeTime = 0.f;
	_float		 m_fLifeTime = 0.f;
	FRAME		 m_tFrame;
	_bool		 m_bAnimRepeat = false;
	Engine::CTransform* m_pTarget;
	_vec3		 m_vHandPos;

	_float		m_fScale = 1.f;

public:
	static CBasicFollowingEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _tchar* pEffectTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, Engine::CTransform* pTarget, _bool bAnimRepeat, _float fLifeTime);

private:
	virtual void Free() override;
};

#endif // BasicFollowingEffect_h__