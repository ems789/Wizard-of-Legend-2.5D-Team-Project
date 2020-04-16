#ifndef Collider_h__
#define Collider_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();


protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	CGameObject*		m_pParent = nullptr;
	COLLIDERTYPE		m_eType = CT_END;


public:
	virtual CComponent* Clone(void) override;
protected:
	virtual void Free() override;
};

END

#endif // Collider_h__
