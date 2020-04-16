#ifndef ThirdViewCamera_h__
#define ThirdViewCamera_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

class CThirdViewCamera : public Engine::CCamera
{
private:
	explicit CThirdViewCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThirdViewCamera();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int Update_Camera(const _float& fTimeDelta) override;

	virtual HRESULT Reset_Camera() override;

private:
	void	Update_EyeAtUp(const _float& fTimeDelta);
	void	Mouse_Move(const _float& fTimeDelta);
	void	Key_Input();

public:
	virtual const _vec3* Get_Angle(_vec3* pAngle) { if (pAngle) memcpy(pAngle, &m_vAngle, sizeof(_vec3)); return &m_vAngle; }

private:
	const Engine::CTransform*	m_pTargetInfo	= nullptr;
	_float						m_fDistance		= 10.f;
	_float						m_fSpeed		= 10.f;
	_vec3						m_vAngle		= { 0.f, 0.f, 0.f };
	_bool						m_bFixCursor	= true;

public:
	static CThirdViewCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;
};

#endif // ThirdViewCamera_h__
