#ifndef CardSpawn_h__
#define CardSpawn_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CRenderer;

END

class CCardSpawn : public Engine::CEffect
{
private:
	explicit CCardSpawn(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCardSpawn();

public:
	HRESULT		Ready_GameObject(const _tchar* pTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


private:
	void	Animation(const _float& fTimeDelta);
	void	Update_Scale();
	void	Turn_To_Camera_Look();
	void	Add_Monster();

private:
	_float		m_fCumulativeTime = 0.f;
	FRAME		m_tFrame;
	_float		m_fScale = 1.f;
	Engine::CGameObject*	m_pMonster = nullptr;

public:
	static CCardSpawn* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTextureTag, const _float& fMaxFrame, const _float& fFrameSpeed,
		const _float& fScale, const _vec3* pPos, Engine::CGameObject* pMonster);

private:
	virtual void Free() override;
};

#endif // CardSpawn_h__
