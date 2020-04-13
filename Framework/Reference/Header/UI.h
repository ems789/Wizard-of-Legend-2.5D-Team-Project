#ifndef UI_h__
#define UI_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUI : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObjcet() override;

protected:
	virtual void Free() override;

};


END


#endif // UI_h__
