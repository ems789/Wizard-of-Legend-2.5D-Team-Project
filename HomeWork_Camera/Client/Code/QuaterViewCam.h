#ifndef QuaterViewCam_h__
#define QuaterViewCam_h__


#include "Defines.h"
#include "Camera.h"

class CQuaterViewCam : public Engine::CCamera
{
private:
	explicit CQuaterViewCam(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuaterViewCam();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	_float	m_fDistance = 10.f;
	_float	m_fAngle_Degree = 45.f;

public:
	static CQuaterViewCam* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fFovy = D3DXToRadian(60.f),
		const _float& fAspect = _float(WINCX) / WINCY, const _float& fNearZ = 1.f, const _float& fFarZ = 1000.f);

private:
	virtual void Free(void) override;

};



#endif // QuaterViewCam_h__
