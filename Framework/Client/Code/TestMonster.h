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
	virtual const _vec3*		Get_Pos()	const override;
private:
	HRESULT Add_Component();

public:
	virtual void Hit(const _int& iAtk, const _vec3* pAtkPos) override;

public:		//	About HP
	virtual _int	Get_HP()	const { return m_iHP; }
	virtual _int	Get_HPMax() const { return m_iHPMax; }

private:	//	Components
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CRenderer*	m_pRendererCom = nullptr;

private:
	Engine::SPHERE	m_tSphere;
	_int	m_iHP		= 200;
	_int	m_iHPMax	= 200;

	_float	m_fScale = 0.05f;
	_float	m_fHeight = 0.5f;

public:
	static CTestMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos);

private:
	virtual void Free() override;
};


#endif // TestMonster_h__
