#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

protected:
	virtual void Free() override;

};

#endif // Stage_h__
