#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit	CTransform(void);
	virtual		~CTransform(void);

public:
	void			Get_Info(INFO eType, _vec3* pInfo);
	const _vec3*	Get_Info(INFO eType);
	void			Move_Pos(const _vec3* const pDir);
	void			Rotation(ROTATION eType, const _float& fAngle);

	const	_matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);
	
	void				Chase_Target(const _vec3* pTargetPos,
										const _float& fSpeed);

public:
	HRESULT				Ready_Component(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

public:
	static CTransform*		Create(void);
private:
	virtual void	Free(void);

};
END
#endif // Transform_h__
