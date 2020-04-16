#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Defines.h"
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr : public CBase
{
DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	_int	Collision(const _float fTimeDelta);

public:
	void					Add_GameObject(const _tchar* pCollisionListTag, CGameObject* pObj);
	list<CGameObject*>*		Get_CollisionObjectList(const _tchar* pCollisionListTag);
	
	void Add_CollisionFunction(const _tchar* pSrcTag, const _tchar* pDestTag, void(pFunc)(const _tchar*, const _tchar*));

private:
	void Clear_AllObjectList();

private:
	map<const _tchar*, list<CGameObject*>>	m_mapObjList;
	vector<pair<const _tchar*, const _tchar*>>	m_vecTag;

	vector<void(*)(const _tchar*, const _tchar*)>	m_vecFunc;

public:
	static _bool CollisionSphereToSphere(const SPHERE& SrcSph, const SPHERE& DestSph);

private:
	virtual void Free()	override;
};
END

#endif // CollisionMgr_h__

