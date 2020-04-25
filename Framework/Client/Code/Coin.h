#ifndef Coin_h__
#define Coin_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CCoin : public Engine::CGameObject
{
private:
	explicit CCoin(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCoin();

public:
	HRESULT Ready_GameObject(const _vec3* pPos, const _vec3* pDir, const _float& fUpForce, const _float& fSpeed, const _int& iCoin);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual Engine::SPHERE* Get_Sphere() override { return &m_tSphere; }
	virtual void Hit(const _int& iAtk, const _vec3* pAtkPos) override;

public:
	const _int&		Get_Coin() const { return m_iCoin; }

private:
	HRESULT		Add_Component();
	void		Animation(const _float& fTimeDelta);
	void		Bouncing(const _float& fTimeDelta);
	void		Gravity(const _float& fTimeDelta);
	void		Turn_To_Camera_Look();
	void		Update_Scale();


private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;


private:
	Engine::SPHERE	m_tSphere;

	FRAME	m_tFrame;
	_float	m_fUpForce = 5.f;
	_float	m_fDownSpeed = 0.f;
	_float	m_fGravity = 98.f;

	_vec3	m_vMoveDir = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 5.f;
	_float	m_fScale = 0.05f;

	_int	m_iCoin = 1;

public:
	static CCoin* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, const _float& fUpForce, const _float& fSpeed, const _int& iCoin = 10);

private:
	virtual void Free();


};


#endif // Coin_h__
