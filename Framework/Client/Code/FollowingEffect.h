#ifndef FollowingEffect_h__
#define FollowingEffect_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CFollowingEffect : public Engine::CEffect
{
private:
	explicit CFollowingEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFollowingEffect();

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
	const _vec3* m_pTargetPos;
	_vec3		 m_vHandPos;

	_float		m_fScale = 1.f;

public:
	static CFollowingEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _tchar* pEffectTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, const _vec3* pTargetPos, _bool bAnimRepeat, _float fLifeTime);

private:
	virtual void Free() override;
};

#endif // FollowingEffect_h__
