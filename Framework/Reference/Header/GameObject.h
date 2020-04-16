#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class CCollider;
class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject();

public:
	const CComponent*	Get_Component(const _tchar* pComponentTag, COMPONENTID eID) const;

public:
	virtual HRESULT Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObjcet();

	virtual void		Collision(const _float& fTimeDelta) {}
	virtual CCollider*	Get_Collider() { return nullptr; }
	virtual SPHERE*		Get_Sphere() { return nullptr; }


public:
	const _bool& IsDead() const { return m_bIsDead; }
	void	Set_IsDead(const _bool& bIsDead) { m_bIsDead = bIsDead; }
	void	Die() { m_bIsDead = true; }

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	map<const _tchar*, CComponent*> m_mapComponent[ID_END];
	_bool				m_bIsDead = false;

public:
	virtual void Free();
};

END

#endif // GameObject_h__
