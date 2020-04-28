#ifndef EndingScene_h__
#define EndingScene_h__

#include "Defines.h"
#include "Scene.h"

#include "BackGround.h"
class CUIButton;
class CUIObject;
class CUIBlink;
class CEndingScene : public Engine::CScene
{
private:
	enum ENDING_SCENE_STATE { ENDING_MOVING, THANKS_FOR_WATCHING, ENDING_END };

private:
	explicit CEndingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEndingScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	_int	Update_Moving(const _float& fTimeDelta);
	_int	Update_Thanks(const _float& fTimeDelta);

private:
	static _bool		m_bResourceLoading;
	ENDING_SCENE_STATE	m_eCurState;
	CUIObject*			m_pCredits = nullptr;
	_float				m_fSpeed = 100.f;
	


public:
	static CEndingScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // EndingScene_h__
