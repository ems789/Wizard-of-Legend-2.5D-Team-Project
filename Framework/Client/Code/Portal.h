#ifndef Portal_h__
#define Portal_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CPortal :public Engine::CGameObject
{
public:
	CPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortal();

public:
	HRESULT Ready_GameObject(const _vec3* pPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual Engine::SPHERE* Get_Sphere() override { return &m_tSphere; }

private:
	HRESULT		Add_Component();	
	void		Animation(const _float& fTimeDelta);
	void		Turn_To_Camera_Look();
	void		Update_Scale();
	void		Into_Portal();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	Engine::SPHERE	m_tSphere;

	FRAME			m_tFrame;
	float			m_fScale;
public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free();
};

#endif // Portal_h__