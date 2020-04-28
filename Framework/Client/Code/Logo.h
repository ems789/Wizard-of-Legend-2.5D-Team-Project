#ifndef Logo_h__
#define Logo_h__

#include "Defines.h"
#include "Scene.h"

#include "BackGround.h"
class CUIButton;
class CUIObject;
class CUIBlink;
class CLogo : public Engine::CScene
{
private:
	enum LOGO_STATE { PRESS_ENTER, LOGO_MOVING, LOGO_BUTTON, LOGO_STATE_END };

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);
	HRESULT		Ready_StaticLayer();

private:
	_int		Update_PressEnter(const _float& fTimeDelta);
	_int		Update_Moving(const _float& fTimeDelta);
	_int		Update_Button(const _float& fTimeDelta);

private:
	LOGO_STATE m_eCurState;
	CUIObject*	m_pTitleLogo = nullptr;
	CUIBlink*	m_pPressEnterKey = nullptr;
	_float		m_fSpeed = 300.f;
	
	vector<CUIButton*>	m_vecButtons;
	_int				m_iOnButton = 0;

	static _bool m_bResourceLoading;
	

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // Logo_h__
