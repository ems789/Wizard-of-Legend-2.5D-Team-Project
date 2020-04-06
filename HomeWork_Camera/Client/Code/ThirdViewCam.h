#ifndef ThirdViewCam_h__
#define ThirdViewCam_h__

#include "Defines.h"
#include "Camera.h"

class CThirdViewCam : public Engine::CCamera
{
private:
	explicit CThirdViewCam(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThirdViewCam();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void	Rotation(const _float& fTimeDelta);

private:
	_float		m_fTargetDistance = 10.f;
	_float		m_fSpeed = 10.f;

public:
	static CThirdViewCam* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fFovy = D3DXToRadian(60.f),
		const _float& fAspect = _float(WINCX) / WINCY, const _float& fNearZ = 1.f, const _float& fFarZ = 1000.f, const _float& fTargetDistance = 10.f);

private:
	virtual void Free(void) override;

};

#endif // ThirdViewCam_h__
