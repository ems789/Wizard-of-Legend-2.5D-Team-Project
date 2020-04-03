#ifndef Camera_h__
#define Camera_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CCamera : public Engine::CGameObject
{
private:
	enum STATE { CAM_1ST, CAM_3RD, CAM_TOP, CAM_END };

private:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component();
	_int		Cam_First_View_Update(const _float& fTimeDelta);
	_int		Cam_Third_View_Update(const _float& fTimeDelta);
	_int		Cam_Top_View_Update(const _float& fTimeDelta);

	void		Cam_Update();
	void		Key_Input();
	void		ChangeCameraView();

private:
	Engine::CTransform*		m_pTransformCom = nullptr;

	CCamera::STATE			m_eCurState		= CCamera::CAM_END;
	CCamera::STATE			m_ePreState		= CCamera::CAM_END;

	_float					m_fCamToTarget_Dist = 10.f;
	_float					m_fField_Of_View_Y	= D3DXToRadian(60.f);
	_float					m_fNearZ			= 1.f;
	_float					m_fFarZ				= 1000.f;

	_vec3					m_vEye, m_vAt, m_vUp;
	_matrix					m_matView, m_matProj;

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};






#endif // Camera_h__
