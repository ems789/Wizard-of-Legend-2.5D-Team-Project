#ifndef Wall_h__
#define Wall_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CLeftWallTex;
class CTopWallTex;
class CRightWallTex;
class CBottomWallTex;
class CCeilingWallTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CWall : public Engine::CGameObject
{
private:
	explicit CWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWall(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(void) override;

public:
	const _vec3*			Get_Pos(); 
	const bool				Get_Render(); 
	const int				Get_DrawID();
	bool					Get_HasWall(WALLPOSITION eWallPos);
	const Engine::WALLTYPE	Get_WallType();
	void					Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void					Set_Pos(const _vec3& vPos);
	void					Set_Render(const bool& bIsRender);
	void					Set_HasWall(bool bHasLeftWall, bool bHasTopWall, bool bHasRightWall, bool bHasBottomWall, bool bHasCeilingWall, Engine::WALLTYPE eWallType);
	void					Set_DrawID(const int iDrawID);
	void					Set_WallType(Engine::WALLTYPE eWallType);

private:
	HRESULT			Add_Component(void);

private:
	/// TODO; WallTex를 상속받아 배열로 관리
	Engine::CLeftWallTex*	 m_pLeftWallBufferCom = nullptr;
	Engine::CTopWallTex*	 m_pTopWallBufferCom = nullptr;
	Engine::CRightWallTex*	 m_pRightWallBufferCom = nullptr;
	Engine::CBottomWallTex*	 m_pBottomWallBufferCom = nullptr;
	Engine::CCeilingWallTex* m_pCeilingWallBufferCom = nullptr;

	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	bool					m_bIsRender = false;
	bool					m_bHasWall[WALL_END]{ false, };
	int						m_iDrawID = 0;
	Engine::WALLTYPE		m_eWallType = Engine::WALL_OUTER;

public:
	static CWall*		Create(LPDIRECT3DDEVICE9 pGraphicDev, bool bHasLeftWall, bool bHasTopWall, bool bHasRightWall, bool bHasBottomWall, bool bHasCeilingWall, Engine::WALLTYPE eWallType);

private:
	virtual void Free(void) override;
};

#endif // Wall_h__
