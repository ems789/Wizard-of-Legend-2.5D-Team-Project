#ifndef Town_h__
#define Town_h__

#include "Defines.h"
#include "Scene.h"

class CTown : public Engine::CScene
{
private:
	explicit CTown(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTown();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_StaticLayer();
	HRESULT		Ready_Camera();

public:
	static CTown* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // Town_h__
