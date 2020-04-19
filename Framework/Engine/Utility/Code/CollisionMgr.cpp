#include "CollisionMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

Engine::CCollisionMgr::CCollisionMgr()
{

}

Engine::CCollisionMgr::~CCollisionMgr()
{

}

_int CCollisionMgr::Collision(const _float fTimeDelta)
{
	for (_uint i = 0; i < m_vecFunc.size(); ++i)
	{
		m_vecFunc[i](m_vecTag[i].first, m_vecTag[i].second);
	}
	
	Clear_AllObjectList();

	return 0;
}

void CCollisionMgr::Add_GameObject(const _tchar * pCollisionListTag, CGameObject * pObj)
{
	auto iter = find_if(m_mapObjList.begin(), m_mapObjList.end(), CTag_Finder(pCollisionListTag));

	if (m_mapObjList.end() == iter)
	{
		list<CGameObject*> ObjList;
		ObjList.push_back(pObj);

		m_mapObjList.emplace(pCollisionListTag, ObjList);
	}
	else
		iter->second.push_back(pObj);
}

list<CGameObject*>* CCollisionMgr::Get_CollisionObjectList(const _tchar * pCollisionListTag)
{
	auto iter = find_if(m_mapObjList.begin(), m_mapObjList.end(), CTag_Finder(pCollisionListTag));

	if (m_mapObjList.end() == iter)
		return nullptr;

	return &iter->second;
}

void CCollisionMgr::Add_CollisionFunction(const _tchar* pSrcTag, const _tchar* pDestTag, void(pFunc)(const _tchar*, const _tchar*))
{
	m_vecTag.push_back(make_pair(pSrcTag, pDestTag));
	m_vecFunc.push_back(pFunc);
}

void CCollisionMgr::Clear_AllObjectList()
{
	for (auto& iter : m_mapObjList)
	{
		iter.second.clear();
	}
	m_mapObjList.clear();
}


_bool CCollisionMgr::CollisionSphereToSphere(const SPHERE & SrcSph, const SPHERE & DestSph)
{
	_vec3 vDist = SrcSph.vPos - DestSph.vPos;
	_float fDist = D3DXVec3Length(&vDist);

	if (SrcSph.fRadius + DestSph.fRadius > fDist)
		return true;
	else
		return false;
}

_bool CCollisionMgr::CollisionLineToSphere(const LINE & SrcLine, const SPHERE & DestSph)
{
	_vec3 SphCenter_To_LinePos1= SrcLine.vPos1 - DestSph.vPos;

	_float fC = D3DXVec3Dot(&SphCenter_To_LinePos1,	&SphCenter_To_LinePos1) - DestSph.fRadius * DestSph.fRadius;

	if (fC <= 0.0f)
		return true;

	_vec3 vLineDir = SrcLine.vPos2 - SrcLine.vPos1;
	_float fLineLength = D3DXVec3Length(&vLineDir);
	if (0.f == fLineLength)
		return false;

	_vec3 vLineDirNorm = vLineDir / fLineLength;

	_float fB_Prime = D3DXVec3Dot(&SphCenter_To_LinePos1, &vLineDirNorm);

	if (fB_Prime > 0.0f)
		return false;

	_float fSqrtDiscriminant = sqrt(fB_Prime * fB_Prime - fC);

	_float fT1 = -fB_Prime + fSqrtDiscriminant;
	if (fT1 >= 0.0f && fT1 <= fLineLength)
		return true;

	_float fT2 = -fB_Prime - fSqrtDiscriminant;
	if (fT2 >= 0.0f && fT2 <= fLineLength)
		return true;

	return false;
}

void Engine::CCollisionMgr::Free()
{
	for (auto& iter : m_mapObjList)
	{
		for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
		iter.second.clear();
	}
	m_mapObjList.clear();
}
