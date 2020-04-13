#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"


BEGIN(Engine)
class ENGINE_DLL CEffect : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();
	
public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObjcet() override;

protected:
	virtual void Free() override;

};

END

#endif // Effect_h__
