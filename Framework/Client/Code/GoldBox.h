#ifndef GoldBox_h__
#define GoldBox_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcAlphaTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CGoldBox : public Engine::CGameObject
{
private:
	explicit CGoldBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGoldBox();

public:
	HRESULT			Ready_GameObject(const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:
	virtual void Hit(const _int & iAtk, const _vec3 * pAtkPos);

public:
	virtual Engine::SPHERE*		Get_Sphere() override { return &m_tSphere; }
	virtual const _vec3*		Get_Pos()	const override;

public:		//	Set_Functions
	void	Set_Pos(const _vec3* pPos);
	void	Set_Pos(const _vec3& vPos);
	virtual void Set_PosX(const _float& fx);
	virtual void Set_PosY(const _float& fy);
	virtual void Set_PosZ(const _float& fz);


private:
	HRESULT Add_Component(const _tchar* pTextureTag);
	void	Animation(const _float& fTimeDelta);
	void	Turn_To_Camera_Look();
	void	Update_Scale();

private:	//	Components
	Engine::CRcAlphaTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

private:
	_float			m_fScale = 0.05f;
	FRAME			m_tFrame;
	D3DXCOLOR		m_d3dColor;

	Engine::SPHERE m_tSphere;

	_int	m_iHP = 100;
	_int	m_iHPMax = 100;


public:
	static CGoldBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _vec3* pPos, const _float& fScale, const _float& fMaxFrame, const _float& fFrameSpeed);

private:
	virtual void Free() override;
};


#endif // GoldBox_h__
