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
	virtual	void	Render_GameObject();

	virtual void		Collision(const _float& fTimeDelta) {}

public:		//	Get Functions
	virtual CCollider*		Get_Collider() { return nullptr; }
	virtual LINE*			Get_Line()  { return nullptr; }
	virtual SPHERE*			Get_Sphere() { return nullptr; }
	virtual const _vec3*	Get_Pos() const { return nullptr; }
	virtual	const _vec3*	Get_PrePos() const { return nullptr; }
	virtual const COLLIDERTYPE Get_CollType() const { return CT_SPHERE; }
	virtual	const _vec3*	Get_Move()	const { return nullptr; }

public:		//	Set Functions
	virtual void	Set_Pos(const _vec3* pPos) {}
	virtual void	Set_PosX(const _float& fx) {}
	virtual void	Set_PosY(const _float& fy) {}
	virtual void	Set_PosZ(const _float& fz) {}
	
public:	//	About Collision
	virtual void	Hit(const _int& iAtk, const _vec3* pAtkPos) {}
	virtual int		Get_Attack() { return 0; }
	virtual void	Add_Effect() {}
	virtual void	Add_Effect(const _vec3* pPos) {}
	virtual const CGameObject* Get_Parent() const { return nullptr; }

public:
	virtual _int	Get_HP() const { return 0; }
	virtual _int	Get_HPMax() const { return 0; }
	virtual _float	Get_MP() const { return 0; }
	virtual _float	Get_MPMax() const { return 0; }
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
