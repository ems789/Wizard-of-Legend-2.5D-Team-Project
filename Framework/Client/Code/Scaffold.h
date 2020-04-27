#ifndef Scaffold_h__
#define Scaffold_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CScaffold : public Engine::CGameObject
{
private:
	explicit CScaffold(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScaffold();

public:
	HRESULT			Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual const _vec3*		Get_Pos()	const override;

	_bool	Get_Down() { return m_bDown; }

public:		//	Set_Functions
	void	Set_Pos(const _vec3* pPos);
	void	Set_Pos(const _vec3& vPos);
	virtual void Set_PosX(const _float& fx);
	virtual void Set_PosY(const _float& fy);
	virtual void Set_PosZ(const _float& fz);


private:
	HRESULT Add_Component(const _tchar* pTextureTag);
	void	Update_Scale();

private:	//	Components
	Engine::CCubeTex*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

private:
	_float			m_fScale = 0.05f;
	_bool			m_bDown = false;
	_bool			m_bPreIn = false;


	Engine::SPHERE m_tSphere;

public:
	static CScaffold*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale);

private:
	virtual void Free() override;
};


#endif // Scaffold_h__
