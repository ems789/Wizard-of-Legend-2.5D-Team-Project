#ifndef LineCollider_h__
#define LineCollider_h__

#include "Defines.h"
#include "Collider.h"

BEGIN(Engine)
class CEffect;
END

class CLineCollider : public Engine::CCollider
{
private:
	explicit CLineCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLineCollider();

public:
	HRESULT			Ready_Collider(CGameObject* pParent, Engine::CEffect* pHitEffect, const _float& fLength, const _vec3* pAngle, const _tchar* pColliderTag, const _int& iAttack);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:	//	Get Functions
	virtual int				Get_Attack()	{ return m_iAttack; }
	virtual void			Add_Effect(const _vec3* pPos);
	virtual Engine::LINE*	Get_Line() { return &m_tLine; }

private:
	Engine::LINE		m_tLine;
	_vec3				m_vAngle;
	_vec3				m_vPos2;

	const _tchar*		m_pColliderTag = nullptr;
	_int				m_iAttack = 20;
	Engine::CEffect*	m_pHitEffect = nullptr;
	_bool				m_bAddEffect = false;

public:
	static CLineCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent, Engine::CEffect* pHitEffect, const _float& fLength, const _vec3* pAngle, const _tchar* pColliderTag, const _int& iAttack);

protected:
	virtual void Free() override;
};

#endif // LineCollider_h__
