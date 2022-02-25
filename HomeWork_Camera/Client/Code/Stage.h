#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"

#include "Terrain.h"
#include "Player.h"
#include "FirstViewCam.h"
#include "ThirdViewCam.h"
#include "QuaterViewCam.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);


public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);

private:
	_matrix						m_matView, m_matProj;
	//CCamera*					m_pCamera = nullptr;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // Stage_h__
