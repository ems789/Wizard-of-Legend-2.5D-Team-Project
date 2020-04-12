#ifndef Camera_h__
#define Camera_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CBase
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_Camera();
	virtual	_int	Update_Camera(const _float& fTimeDelta);


public:	//	Get Functions
	const _matrix*	Get_View(_matrix* pmatView = nullptr)		const { if (pmatView) memcpy(pmatView, &m_matView, sizeof(_matrix)); return &m_matView; }
	const _matrix*	Get_Projection(_matrix* pmatProj = nullptr) const { if (pmatProj) memcpy(pmatProj, &m_matProj, sizeof(_matrix)); return &m_matProj; }

	const _vec3*	Get_Eye(_vec3* pEye = nullptr)	const { if (pEye) memcpy(pEye, &m_vEye, sizeof(_vec3)); return &m_vEye; }
	const _vec3*	Get_At(_vec3* pAt = nullptr)	const { if (pAt) memcpy(pAt, &m_vAt, sizeof(_vec3)); return &m_vAt; }
	const _vec3*	Get_Up(_vec3* pUp = nullptr)	const { if (pUp) memcpy(pUp, &m_vUp, sizeof(_vec3)); return &m_vUp; }

	const _float Get_Fovy()		const { return m_fFovY; }
	const _float Get_Aspect()	const { return m_fAspect; }
	const _float Get_Near()		const { return m_fNear; }
	const _float Get_Far()		const { return m_fFar; }

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	_matrix				m_matView, m_matProj;
	_vec3				m_vEye, m_vAt, m_vUp;
	_float				m_fFovY		= D3DXToRadian(60.f);
	_float				m_fAspect	= 1.f;
	_float				m_fNear		= 1.f;
	_float				m_fFar		= 1000.f;
	
protected:
	virtual void Free(void) override;
};

END

#endif // Camera_h__
