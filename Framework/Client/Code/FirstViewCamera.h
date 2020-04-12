#ifndef FirstViewCamera_h__
#define FirstViewCamera_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

class CFirstViewCamera : public Engine::CCamera
{
private:
	explicit CFirstViewCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirstViewCamera();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int Update_Camera(const _float& fTimeDelta) override;

private:
	void	Update_EyeAtUp(const _float& fTimeDelta);
	void	Mouse_Input(const _float& fTimeDelta);

private:
	const Engine::CTransform*		m_pTargetInfo = nullptr;

public:
	static CFirstViewCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

#endif // FirstViewCamera_h__
