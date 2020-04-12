#ifndef QuaterViewCamera_h__
#define QuaterViewCamera_h__

#include "Defines.h"
#include "Camera.h"

class CQuaterViewCamera : public Engine::CCamera
{
private:
	explicit CQuaterViewCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuaterViewCamera();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int Update_Camera(const _float& fTimeDelta) override;

public:
	static CQuaterViewCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};

#endif // QuaterViewCamera_h__
