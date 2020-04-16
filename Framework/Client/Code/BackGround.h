#ifndef BackGround_h__
#define BackGround_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

public:
	static CBackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // BackGround_h__
