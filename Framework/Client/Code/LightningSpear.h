#ifndef LightningSpear_h__
#define LightningSpear_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CLightningSpear : public Engine::CGameObject
{
private:
	explicit CLightningSpear(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningSpear();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;
	virtual void				Add_Effect(const _vec3* pPos) override;

public:
	void	Go();
	void	Update_Scale();

private:
	HRESULT		Add_Component();
	void		Animation(const _float& fTimeDelta);

private:
	void Turn_To_Camera_Look();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_float	m_fLimitDistance = 50.f;
	_float	m_fScale = 0.05f;
	_int	m_iAttack = 18;

	FRAME	m_tFrame;
	Engine::SPHERE m_tSphere;

	_bool	m_bGo = false;
	const _tchar*	m_pCollisionTag = nullptr;

public:
	static CLightningSpear* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vDir, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed, const _float& fAngle, const _tchar* pCollisionTag = L"Player_Bullet" , _bool bGo = true);

private:
	virtual void Free() override;

};

#endif // LightningSpear_h__
