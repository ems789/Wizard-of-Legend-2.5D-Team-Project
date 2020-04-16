#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

END

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera(void);

public:
	HRESULT		Ready_Camera(const _vec3* pEye,
							const _vec3* pAt,
							const _vec3* pUp,
							const _float& fFovY,
							const _float& fAspect,
							const _float& fNear,
							const _float& fFar);

	_int		 Update_KeyInput(const _float& fTimeDelta);
	_int		 Update_MouseMove(const _float& fTimeDelta);
	virtual _int Update_Camera(const _float& fTimeDelta) override;

private:
	void	Key_Input(const _float& fTimeDelta);
	void	Mouse_Move(const _float& fTimeDelta);
	void	Mouse_Fix(void);

private :
	_bool			m_bClick;
	_bool			m_bMouseFix;
	_float			m_fSpeed = 10.f;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fFovY,
										const _float& fAspect,
										const _float& fNear,
										const _float& fFar);

private:
	virtual void Free(void) override;

};

#endif // DynamicCamera_h__
