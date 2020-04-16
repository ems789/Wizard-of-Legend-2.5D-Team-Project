#include "stdafx.h"
#include "CollisionFunctions.h"

#include "Export_Function.h"

CCollisionFunctions::CCollisionFunctions()
{

}

CCollisionFunctions::~CCollisionFunctions()
{

}

HRESULT CCollisionFunctions::Ready_Functions()
{
	Engine::CCollisionMgr::GetInstance()->Add_CollisionFunction(L"Player_Bullet", L"Monster", CCollisionFunctions::CollisionBulletToObject);
	
	return S_OK;
}

void CCollisionFunctions::CollisionBulletToObject(const _tchar * pSrcTag, const _tchar * pDestTag)
{
	list<Engine::CGameObject*>* pSrcList = Engine::Get_CollisionObjectList(pSrcTag);
	list<Engine::CGameObject*>* pDestList = Engine::Get_CollisionObjectList(pDestTag);

	if (nullptr == pSrcList || nullptr == pDestList)
		return;

	for (auto& pSrc : *pSrcList)
	{
		Engine::SPHERE tSrcSph = *pSrc->Get_Sphere();

		_bool	bSrcDie = false;
		for (auto& pDest : *pDestList)
		{
			Engine::SPHERE tDestSph = *pDest->Get_Sphere();

			if (Engine::CCollisionMgr::CollisionSphereToSphere(tSrcSph, tDestSph))
			{
				pSrc->Die();
				
			}
		}
	}
}
