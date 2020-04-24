#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
protected:
	explicit	CTransform();
	virtual		~CTransform();

public:	//	Set Functions
	void			Set_Scale(const _float& fx, const _float& fy, const _float& fz) { m_vScale = { fx, fy, fz }; }
	void			Set_Scale(const _vec3& vScale)									{ m_vScale = vScale; }
	void			Set_Scale(const _vec3* pScale)									{ memcpy(&m_vScale, pScale, sizeof(_vec3)); }
	void			Set_Scale(const _float& fScale)									{ m_vScale.x = fScale, m_vScale.y = fScale, m_vScale.z = fScale; }

	void			Set_ScaleX(const _float& fx)									{ m_vScale.x = fx; }
	void			Set_ScaleY(const _float& fy)									{ m_vScale.y = fy; }
	void			Set_ScaleZ(const _float& fz)									{ m_vScale.z = fz; }

	void			Set_Pos(const _vec3& vPos)										{ m_vInfo[INFO_POS] = vPos; }
	void			Set_Pos(const _vec3* pPos)										{ memcpy(&m_vInfo[INFO_POS], pPos, sizeof(_vec3)); }
	void			Set_Pos(const _float& fx, const _float& fy, const _float& fz)	{ m_vInfo[INFO_POS] = _vec3(fx, fy, fz); }
	void			Set_PosX(const _float& fx)										{ m_vInfo[INFO_POS].x = fx; }
	void			Set_PosY(const _float& fy)										{ m_vInfo[INFO_POS].y = fy; }
	void			Set_PosZ(const _float& fz)										{ m_vInfo[INFO_POS].z = fz; }

	void			Set_Angle(const _vec3& vAngle)									{ m_vAngle = vAngle; }
	void			Set_Angle(const _vec3* pAngle)									{ memcpy(&m_vAngle, pAngle, sizeof(_vec3)); }
	void			Set_Angle(const _float& fx, const _float& fy, const _float& fz) { m_vAngle = { fx, fy, fz }; }
	void			Set_AngleX(const _float& fx)									{ m_vAngle.x = fx; }
	void			Set_AngleY(const _float& fy)									{ m_vAngle.x = fy; }
	void			Set_AngleZ(const _float& fz)									{ m_vAngle.x = fz; }

	void			Set_WorldMatrix(const _matrix mat)								{ m_matWorld = mat; }

public:	//	Get Functions
	const _vec3*	GetInfo(INFO eType, _vec3* pInfo = nullptr)		const { if (pInfo)		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));	return &m_vInfo[eType]; }
	const _matrix*	GetWorldMatrix(_matrix* pMatrix = nullptr)		const { if (pMatrix)	memcpy(pMatrix, &m_matWorld, sizeof(_matrix));			return &m_matWorld; }
	const _vec3*	GetAngle(_vec3* pAngle = nullptr)				const {	if (pAngle)		memcpy(pAngle, &m_vAngle, sizeof(_vec3));				return &m_vAngle; }

public:	//	return Reference
	const _vec3&	GetInfoRef(INFO eType)							const { return m_vInfo[eType]; }
	const _vec3&	GetScaleRef()									const { return m_vScale; }
	const _vec3&	GetAngleRef()									const { return m_vAngle; }
	const _matrix&	GetWorldMatrixRef()								const { return m_matWorld; }

public:	//	Move or Rotationd etc
	void			Move_Pos(const _vec3* const pDir)				{ m_vInfo[INFO_POS] += *pDir; }
	void			Move_Pos(const _vec3& vDir)						{ m_vInfo[INFO_POS] += vDir; }
	void			Move_Pos(INFO eType, const _float& fDist)		{ m_vInfo[INFO_POS] += m_vInfo[eType] * fDist; }
	void			Rotation(ROTATION eType, const _float& fAngle)	{ *(((_float*)&m_vAngle) + eType) += fAngle; }
	void			Multiply_Scale(const _vec3* const pScale)		{ m_vScale.x *= pScale->x, m_vScale.y *= pScale->y, m_vScale.z *= pScale->z; }

public:
	HRESULT			Ready_Component();
	virtual _int	Update_Component(const _float& fTimeDelta);

private:
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;
	_matrix		m_matWorld;

public:
	static CTransform* Create();

protected:
	virtual void Free() override;

};

END

#endif // Transform_h__
