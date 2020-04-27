#ifndef Shark_h__
#define Shark_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CShark : public Engine::CGameObject
{
private:
	explicit CShark(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShark();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual int					Get_Attack() override { return m_iAttack; }
	virtual const _vec3*		Get_Pos()	const override;
	virtual void				Add_Effect(const _vec3* pPos) override;

private:
	HRESULT		Add_Component();
	void		Animation(const _float& fTimeDelta);
	void		Update_Scale();

private:
	void Turn_To_Camera_Look();

private:
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	_vec3	m_vInitialPos = { 0.f, 0.f, 0.f };
	_vec3	m_vAngle = { 0.f, 0.f, 0.f };
	_float	m_fSpeed = 10.f;
	_int	m_iAttack = 50;
	_float	m_fScale = 1.f;
	FRAME	m_tFrame;
	Engine::SPHERE m_tSphere;
	_bool	m_bSharkSound = false;

	_bool	m_bGo = false;
	_bool	m_bSharkOn = false;
	const _tchar*	m_pCollisionTag = nullptr;

public:
	static CShark*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitialPos, const _vec3& vAngle, const _float& fSpeed, const _float& fMaxFrame, const _float& fFrameSpeed, const _float& fScale);

private:
	virtual void Free() override;
};
#endif // !Shark_h__
