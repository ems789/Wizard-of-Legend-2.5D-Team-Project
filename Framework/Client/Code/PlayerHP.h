#ifndef PlayerHP_h__
#define PlayerHP_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CPlayerHP : public Engine::CGameObject
{
private:
	explicit CPlayerHP(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerHP();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObjcet() override;

private:
	HRESULT Add_Component();
	HRESULT SetUp_Scale_Pos(const _vec3* pScale);

private:	//	Componenets
	Engine::CRcTex*		m_pBufferCom	= nullptr;
	Engine::CTexture*	m_pTextureCom	= nullptr;
	Engine::CTransform* m_pTransformCom	= nullptr;
	Engine::CRenderer*	m_pRendererCom	= nullptr;

public:
	static CPlayerHP*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pScale);

private:
	virtual void Free() override;

};

#endif // PlayerHP_h__
