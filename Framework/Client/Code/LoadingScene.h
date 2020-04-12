#ifndef LoadingScene_h__
#define LoadingScene_h__

#include "Defines.h"
#include "Scene.h"

#include "Loading.h"

class CLoadingScene : public Engine::CScene
{
private:
	explicit CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingScene();

public:
	virtual HRESULT Ready_Scene()	override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene()	override;

private:
	HRESULT		Ready_Effect_Layer(const _tchar* pLayerTag);

private:
	CLoading*		m_pLoading;

public:
	static CLoadingScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};
#endif // LoadingScene_h__
