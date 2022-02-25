#ifndef Camera_h__
#define Camera_h__

#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	const _vec3*	Get_Info(INFO eType, _vec3* pInfo);
	const _vec3*	Get_Angle(_vec3* pRadians);

private:
	HRESULT			Add_Component();

protected:
	void			View_Proj_Update();

protected:
	CTransform*		m_pTransformCom = nullptr;
	_matrix			m_matView, m_matProj;
	
	_float			m_fAspect	= 1.f;
	_float			m_fFovy		= 0.f;
	_float			m_fNearZ	= 1.f;
	_float			m_fFarZ		= 1000.f;

protected:
	virtual void Free(void) override;
};

END
#endif // Camera_h__
