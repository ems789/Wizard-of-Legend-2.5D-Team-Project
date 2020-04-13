#ifndef Mouse_h__
#define Mouse_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

class ENGINE_DLL CMouse : public CBase
{
	DECLARE_SINGLETON(CMouse)

private:
	explicit CMouse();
	virtual ~CMouse();

public:
	HRESULT		Ready_Mouse(LPDIRECT3DDEVICE9 pGraphicDev);
	_int		Update_Mouse();
	void		Render_Mouse();

public:
	HRESULT		Set_Texture(const _ushort& wContainerIdx, const _tchar* pTextureTag);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	_bool				m_bCursorRender = true;

private:
	virtual void Free() override;
};

END

#endif // Mouse_h__
