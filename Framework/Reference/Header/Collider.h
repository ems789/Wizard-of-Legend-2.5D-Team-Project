#ifndef Collider_h__
#define Collider_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CCollider : public CGameObject
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	virtual HRESULT Ready_Collider(CGameObject* pParent);
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual const CGameObject* Get_Parent() const { return m_pParent; }
	virtual const COLLIDERTYPE Get_CollType() const { return m_eType; }

	void Add_Hit_Effect(void (pHitFunc)()) { m_pHitFunc = pHitFunc; }

protected:
	CGameObject*		m_pParent = nullptr;
	COLLIDERTYPE		m_eType = CT_END;

	void(*m_pHitFunc)() = nullptr;

	



protected:
	virtual void Free() override;
};

END

#endif // Collider_h__
