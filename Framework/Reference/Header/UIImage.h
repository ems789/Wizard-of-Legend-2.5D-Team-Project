#ifndef UIImage_h__
#define UIImage_h__

#include "Defines.h"
#include "Base.h"

#include "Engine_Defines.h"

BEGIN(Engine)

class CTexture;
class CRcTex;

END

class CUIImage : public CBase 
{
private:
	explicit CUIImage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIImage();

public:
	HRESULT Ready_UIImage(const _vec3& vScale, const _vec3& vPos, const _tchar* pTextureTag);
	void	Render_UIImage();

public:
	void Set_Pos(const _vec3& vPos);
	void Set_Scale(const _vec3& vScale);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;

	_vec3		m_vScale = { 100.f, 100.f, 0.f };
	_vec3		m_vPos = {0.f, 0.f, 0.f};
	_matrix		m_matWorld;

public:
	static CUIImage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vScale, const _vec3& vPos, const _tchar* pTextureTag);

private:
	virtual void Free();
};

#endif // UIImage_h__
