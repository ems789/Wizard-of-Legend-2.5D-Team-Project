#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CPlayer : public Engine::CGameObject
{
public:
	enum P_CAM_STATE { PC_1ST, PC_3RD, PC_QUATER, PC_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

public:
	void		Set_Cam_State(CPlayer::P_CAM_STATE eCamState) { m_eCurCamState = eCamState; }

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		Key_Input_1stView(const _float& fTimeDelta);
	void		Key_Input_3rdView(const _float& fTimeDelta);
	void		Key_Input_QuaterView(const _float& fTimeDelta);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	
	_vec3					m_vDir, m_vRight;
	_float					m_fSpeed = 10.f;
	CPlayer::P_CAM_STATE	m_eCurCamState = PC_END;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // Player_h__

