#ifndef QuaterViewCamera_h__
#define QuaterViewCamera_h__
#define	INITIAL_ANGLE _vec3(D3DXToRadian(45.f), 0.f, 0.f)

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

class CQuaterViewCamera : public Engine::CCamera
{
private:
	explicit CQuaterViewCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuaterViewCamera();

	virtual HRESULT Reset_Camera() override;

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int Update_Camera(const _float& fTimeDelta) override;

private:
	_int	Update_EyeAtUp(const _float& fTimeDelta);
	_int	Key_Input(const _float& fTimeDelta);
	_int	Reset_Angle();

public:
	virtual const _vec3* Get_Angle(_vec3* pAngle) { if (pAngle) memcpy(pAngle, &m_vAngle, sizeof(_vec3)); return &m_vAngle; }
	virtual void			CameraShake();
	virtual _vec2			Get_MousePos();
private:
	const Engine::CTransform*	m_pTargetInfo = nullptr;
	_float						m_fDistance = 10.f;
	_float						m_fSpeed = 10.f;
	_vec3						m_vAngle = INITIAL_ANGLE;

	_bool						m_bShaking = false;
	_float						m_fShakingTime = 0.f;
	_float						m_fShakingDir = 0.1f;

public:
	static CQuaterViewCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};

#endif // QuaterViewCamera_h__
