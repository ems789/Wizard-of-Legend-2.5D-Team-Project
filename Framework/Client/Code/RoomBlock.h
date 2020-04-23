#ifndef RoomBlock_h__
#define RoomBlock_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CRenderer;

END

class CRoomBlock : public Engine::CGameObject
{
public:
	enum BLOCK_DIR { BLOCK_UP, BLOCK_DOWN, BLOCK_LEFT, BLOCK_RIGHT, BLOCKDIR_END };
	
private:
	enum BLOCK_STATE { BLOCK_CLOSING, BLOCK_CLOSED, BLOCK_OPENING, BLOCK_END };

private:
	explicit CRoomBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoomBlock();

public:
	HRESULT Ready_GameObject(const _vec3* pScale, const _vec3* pPos, BLOCK_DIR eBlockDir);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual const _vec3* Get_Pos() const override;
	
	virtual void Set_Pos(const _vec3* pPos) override;
	virtual void Set_PosX(const _float& fx) override;
	virtual void Set_PosY(const _float& fy) override;
	virtual void Set_PosZ(const _float& fz) override;
	
public:
	void Open() { m_eCurState = BLOCK_OPENING; }
	const BLOCK_DIR	Get_BlockDir() const { return m_eCurDir; }

private:
	HRESULT		Add_Component();
	void		Animation(const _float& fTimeDelta);
	void		Change_State();
	void		Closing_State();
	void		Closed_State();
	void		Opening_State();

	void		MonsterCheck();

private:	//	Components
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	//	Animation
	FRAME		m_tFrame;
	_bool		m_bAnimFinish;
	_bool		m_bAnimRepeat;

	//	Dir
	BLOCK_DIR	m_eCurDir;
	
	//	State
	BLOCK_STATE	m_eCurState;
	BLOCK_STATE	m_ePreState;

	_bool		m_bMonsterGen = false;

public:
	static CRoomBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pScale, const _vec3* pPos, BLOCK_DIR eBlockDir);

private:
	virtual void Free() override;

};


#endif // RoomBlock_h__
