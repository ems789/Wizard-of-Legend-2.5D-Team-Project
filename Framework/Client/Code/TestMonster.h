#ifndef TestMonster_h__
#define TestMonster_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CTestMonster : public Engine::CGameObject
{
private:
	explicit CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestMonster();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }

private:
	HRESULT Add_Component();

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	Engine::SPHERE	m_tSphere;


public:
	static CTestMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};


#endif // TestMonster_h__
