#ifndef MoveObject_h__
#define MoveObject_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CMoveObject : public CGameObject
{
protected:
	explicit CMoveObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

	virtual SPHERE* Get_Sphere() override;
	

public:
	virtual void Hit(const _float& fDamage, const _vec3* pAtkPos) {};

protected:
	virtual void Free() override;

};

END
#endif // MoveObject_h__
