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
	Engine::Add_CollisionFunction(L"Player_Bullet", L"Monster", CCollisionFunctions::CollisionBulletToObject);
	Engine::Add_CollisionFunction(L"MonsterAttack", L"Player", CCollisionFunctions::CollisionObjAttackToObject);


	return S_OK;
}

void CCollisionFunctions::CollisionExample(const _tchar * pSrcTag, const _tchar * pDestTag)
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

			}
		}
	}
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

		for (auto& pDest : *pDestList)
		{
			Engine::SPHERE tDestSph = *pDest->Get_Sphere();

			if (Engine::CCollisionMgr::CollisionSphereToSphere(tSrcSph, tDestSph))
			{
				pSrc->Die();
				pSrc->Add_Effect(&tDestSph.vPos);
				pDest->Hit(pSrc->Get_Attack(), pSrc->Get_Pos());
			}
		}
	}
}

void CCollisionFunctions::CollisionObjAttackToObject(const _tchar * pSrcTag, const _tchar * pDestTag)
{
	list<Engine::CGameObject*>* pSrcList = Engine::Get_CollisionObjectList(pSrcTag);
	list<Engine::CGameObject*>* pDestList = Engine::Get_CollisionObjectList(pDestTag);

	if (nullptr == pSrcList || nullptr == pDestList)
		return;

	for (auto& pSrc : *pSrcList)
	{
		Engine::COLLIDERTYPE eType = pSrc->Get_CollType();

		switch (eType)
		{
		case Engine::CT_LINE:
		{
			Engine::LINE tSrcLine = *pSrc->Get_Line();

			for (auto& pDest : *pDestList)
			{
				Engine::SPHERE tDestSph = *pDest->Get_Sphere();

				if (Engine::CCollisionMgr::CollisionLineToSphere(tSrcLine, tDestSph))
				{
					pSrc->Die();
					pSrc->Add_Effect(&tDestSph.vPos);
					pDest->Hit(pSrc->Get_Attack(), &tSrcLine.vPos1);
				}
			}
		}
			break;
		case Engine::CT_PLANE:
			break;
		case Engine::CT_SPHERE:
		{
			Engine::SPHERE tSrcSph = *pSrc->Get_Sphere();

			for (auto& pDest : *pDestList)
			{
				Engine::SPHERE tDestSph = *pDest->Get_Sphere();

				if (Engine::CCollisionMgr::CollisionSphereToSphere(tSrcSph, tDestSph))
				{
					pSrc->Die();
					pSrc->Add_Effect(&tDestSph.vPos);
					pDest->Hit(pSrc->Get_Attack(), &tSrcSph.vPos);
				}
			}
		}
			break;
		case Engine::CT_CUBE:
			break;
		}


	
	}
}
