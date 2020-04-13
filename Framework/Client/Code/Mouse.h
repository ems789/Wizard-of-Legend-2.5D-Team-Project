#ifndef Mouse_h__
#define Mouse_h__

#include "Defines.h"
#include "Base.h"

#include "Engine_Defines.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;

END

class CMouse : public CBase 
{
	DECLARE_SINGLETON(CMouse)
private:
	explicit CMouse();
	virtual	~CMouse();

public:
	HRESULT Ready_Mouse(LPDIRECT3DDEVICE9 pGraphicDev);
	_int	Update_Mouse(const _float& fTimeDelta);
	void	Render_Mouse();

public:
	void CursorRenderOn()	{ m_bCursorRender = true; }
	void CursorRenderOff()	{ m_bCursorRender = false; }

private:
	HRESULT		Add_Component();
	void		Update_MousePos();

private:	//	Components
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	_bool				m_bCursorRender = true;

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // Mouse_h__
