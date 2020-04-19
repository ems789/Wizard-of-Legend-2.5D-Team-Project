#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Defines.h"
#include "Collider.h"

BEGIN(Engine)
class CEffect;
END

class CSphereCollider : public Engine::CCollider
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphereCollider();

public:
	HRESULT			Ready_Collider(CGameObject* pParent, Engine::CEffect* pHitEffect, const _float& fRadius, const _tchar* pColliderTag, const _int& iAttack);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:	//	Get Functions
	virtual	Engine::SPHERE* Get_Sphere()	{ return &m_tSphere; }
	virtual const _vec3*	Get_Pos() const { return &m_tSphere.vPos; }
	virtual int				Get_Attack()	{ return m_iAttack; }
	virtual void			Add_Effect(const _vec3* pPos);

private:
	Engine::SPHERE		m_tSphere;
	const _tchar*		m_pColliderTag = nullptr;
	_int				m_iAttack = 20;
	Engine::CEffect*	m_pHitEffect = nullptr;
	_bool				m_bAddEffect = false;

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent, Engine::CEffect* pHitEffect, const _float& fRadius, const _tchar* pColliderTag, const _int& iAttack);

protected:
	virtual void Free() override;
};

#endif // SphereCollider_h__
