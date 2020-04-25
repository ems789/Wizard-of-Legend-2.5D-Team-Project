#include "stdafx.h"
#include "CollisionFunctions.h"

#include "Export_Function.h"
#include "Terrain.h"
#include "RoomBlock.h"
#include "Inven.h"
#include "Coin.h"

const _uint uiFlag_Wall_Right	= 0x00000001;
const _uint uiFlag_Wall_Top		= 0x00000002;
const _uint uiFlag_Wall_Left	= 0x00000004;
const _uint uiFlag_Wall_Bottom	= 0x00000008;
const _uint uiFlag_Wall_Ceiling	= 0x00000010;

CCollisionFunctions::CCollisionFunctions()
{

}

CCollisionFunctions::~CCollisionFunctions()
{

}

HRESULT CCollisionFunctions::Ready_Functions()
{
	Engine::Add_CollisionFunction(L"Player_Bullet", L"Monster",		CCollisionFunctions::CollisionBulletToObject);
	Engine::Add_CollisionFunction(L"MonsterAttack", L"Player",		CCollisionFunctions::CollisionObjAttackToObject);
	Engine::Add_CollisionFunction(L"Player",		L"Terrain",		CCollisionFunctions::CollisionObjectToTerrain);
	Engine::Add_CollisionFunction(L"Monster",		L"Terrain",		CCollisionFunctions::CollisionObjectToTerrain);
	Engine::Add_CollisionFunction(L"Player",		L"RoomBlock",	CCollisionFunctions::CollisionObjectToRoomBlock);
	Engine::Add_CollisionFunction(L"Monster",		L"RoomBlock",	CCollisionFunctions::CollisionObjectToRoomBlock);
	Engine::Add_CollisionFunction(L"Player",		L"Coin",		CCollisionFunctions::CollisionPlayerToCoin);
	//Engine::Add_CollisionFunction(L"Player", L"Terrain", CCollisionFunctions::CollisionObjectToTerrainPoint);
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

void CCollisionFunctions::CollisionObjectToTerrain(const _tchar * pSrcTag, const _tchar * pDestTag)
{
	list<Engine::CGameObject*>* pSrcList = Engine::Get_CollisionObjectList(pSrcTag);
	list<Engine::CGameObject*>* pDestList = Engine::Get_CollisionObjectList(pDestTag);

	if (nullptr == pSrcList || nullptr == pDestList)
		return;

	for (auto& pSrc : *pSrcList)
	{
		_vec3 vSrcPos = *pSrc->Get_Pos();
		//_vec3 vSrcPrePos = *pSrc->Get_PrePos();
		//_vec3 vMoveDir = vSrcPos - vSrcPrePos;
		for (auto& pDest : *pDestList)
		{
			CTerrain* pTerrain  = dynamic_cast<CTerrain*>(pDest);
			NULL_CHECK_RETURN(pTerrain, );

			_ulong dwItv	= pTerrain->Get_Interval();
			_ulong dwTileX	= pTerrain->Get_TileX();
			_ulong dwTileZ	= pTerrain->Get_TileZ();

			_int iIndexX	= static_cast<_ulong>(vSrcPos.x / dwItv);
			_int iIndexZ	= static_cast<_ulong>(vSrcPos.z / dwItv);

			if (iIndexX < 0)
			{
				pSrc->Set_PosX(0.f);
				break;
			}
			else if (iIndexX >= dwTileX)
			{
				pSrc->Set_PosX(static_cast<_float>((dwTileX - 1) * dwItv));
				break;
			}
			if (iIndexZ < 0)
			{
				pSrc->Set_PosZ(0.f);
				break;
			}
			else if (iIndexZ >= dwTileZ)
			{
				pSrc->Set_PosZ(static_cast<_float>((dwTileZ - 1) * dwItv));
				break;
			}

			_int iStartX = iIndexX - 1;
			_int iEndX = iIndexX + 1;
			
			_int iStartZ = iIndexZ - 1;
			_int iEndZ = iIndexZ + 1;

			if (iStartX < 0)
				iStartX = 0;
			else if (iStartX >= dwTileX)
				iStartX = dwTileX - 1;

			if (iEndX <= 0)
				iEndX = 1;
			else if (iEndX >= dwTileX)
				iEndX = dwTileX - 1;

			if (iStartZ < 0)
				iStartZ = 0;
			else if (iStartZ >= dwTileZ)
				iStartZ = dwTileZ - 1;

			if (iEndZ <= 0)
				iEndZ = 1;
			else if (iEndZ >= dwTileZ)
				iEndZ = dwTileZ-1;

			for (_int z = iStartZ; z <= iEndZ; ++z)
			{
				for (_int x = iStartX; x <= iEndX; ++x)
				{
					if (pTerrain->Check_TileWall(x, z))
					{
						Engine::RECTANGLE tTileRect;
						tTileRect.vPos = { x + dwItv * 0.5f , 0.f, z + dwItv * 0.5f };
						tTileRect.fHeight = tTileRect.fWidth = dwItv;
						if (CCollisionFunctions::CollisionRectToTile(pSrc->Get_Sphere(), &tTileRect))
						{
							//_vec3 vDist = vSrcPos - tTileRect.vPos;
							//
							//if (fabs(vDist.x) > fabs(vDist.z))
							//{
							//	if (vDist.x > 0)
							//		pSrc->Set_PosX(tTileRect.vPos.x + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							//	else
							//		pSrc->Set_PosX(tTileRect.vPos.x - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							//}
							//else
							//{
							//	if (vDist.z > 0)
							//		pSrc->Set_PosZ(tTileRect.vPos.z + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							//	else
							//		pSrc->Set_PosZ(tTileRect.vPos.z - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							//}

							const bool* bHasWall = pTerrain->Get_HasWall(x, z);

							_uint	uiWallFlag = 0;

							if (bHasWall[WALL_TOP])
								uiWallFlag |= uiFlag_Wall_Top;
							if (bHasWall[WALL_RIGHT])
								uiWallFlag |= uiFlag_Wall_Right;
							if (bHasWall[WALL_BOTTOM])
								uiWallFlag |= uiFlag_Wall_Bottom;
							if (bHasWall[WALL_LEFT])
								uiWallFlag |= uiFlag_Wall_Left;
							if (bHasWall[WALL_CEILING])
								uiWallFlag |= uiFlag_Wall_Ceiling;

							switch (uiWallFlag)
							{
							case uiFlag_Wall_Left:
								pSrc->Set_PosX(tTileRect.vPos.x - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								break;
							case uiFlag_Wall_Right:
								pSrc->Set_PosX(tTileRect.vPos.x + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								break;
							case uiFlag_Wall_Top:
								pSrc->Set_PosZ(tTileRect.vPos.z + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								break;
							case uiFlag_Wall_Bottom:
								pSrc->Set_PosZ(tTileRect.vPos.z - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								break;
							case uiFlag_Wall_Left | uiFlag_Wall_Top:
							{
								_vec3	vTileLeftTop = { tTileRect.vPos.x - tTileRect.fWidth * 0.5f, 0.f, tTileRect.vPos.z + tTileRect.fHeight * 0.5f };
								_vec3	vDir = vSrcPos - vTileLeftTop;

								if (fabs(vDir.x) > fabs(vDir.z))
									pSrc->Set_PosZ(tTileRect.vPos.z + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								else
									pSrc->Set_PosX(tTileRect.vPos.x - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							}
								break;
							case uiFlag_Wall_Right | uiFlag_Wall_Top:
							{
								_vec3	vTileRightTop = { tTileRect.vPos.x + tTileRect.fWidth * 0.5f, 0.f, tTileRect.vPos.z + tTileRect.fHeight * 0.5f };
								_vec3	vDir = vSrcPos - vTileRightTop;

								if (fabs(vDir.x) > fabs(vDir.z))
									pSrc->Set_PosZ(tTileRect.vPos.z + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								else
									pSrc->Set_PosX(tTileRect.vPos.x + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							}
								break;
							case uiFlag_Wall_Left | uiFlag_Wall_Bottom:
							{
								_vec3	vTileLeftBottom = { tTileRect.vPos.x - tTileRect.fWidth * 0.5f, 0.f, tTileRect.vPos.z - tTileRect.fHeight * 0.5f };
								_vec3	vDir = vSrcPos - vTileLeftBottom;

								if (fabs(vDir.x) > fabs(vDir.z))
									pSrc->Set_PosZ(tTileRect.vPos.z - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								else
									pSrc->Set_PosX(tTileRect.vPos.x - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							}
								break;
							case uiFlag_Wall_Right | uiFlag_Wall_Bottom:
							{
								_vec3	vTileRightBottom = { tTileRect.vPos.x + tTileRect.fWidth * 0.5f, 0.f, tTileRect.vPos.z - tTileRect.fHeight * 0.5f };
								_vec3	vDir = vSrcPos - vTileRightBottom;

								if (fabs(vDir.x) > fabs(vDir.z))
									pSrc->Set_PosZ(tTileRect.vPos.z - (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
								else
									pSrc->Set_PosX(tTileRect.vPos.x + (dwItv * 0.5f + pSrc->Get_Sphere()->fRadius));
							}
								break;
							}

						}
					}
				}
			}
		}
	}

}

void CCollisionFunctions::CollisionOBjectToTerrainPoint(const _tchar * pSrcTag, const _tchar * pDestTag)
{
	list<Engine::CGameObject*>* pSrcList = Engine::Get_CollisionObjectList(pSrcTag);
	list<Engine::CGameObject*>* pDestList = Engine::Get_CollisionObjectList(pDestTag);

	if (nullptr == pSrcList || nullptr == pDestList)
		return;

	for (auto& pSrc : *pSrcList)
	{
		_vec3 vSrcPos = *pSrc->Get_Pos();

		for (auto& pDest : *pDestList)
		{
			CTerrain* pTerrain = dynamic_cast<CTerrain*>(pDest);
			NULL_CHECK_RETURN(pTerrain, );

			_ulong dwItv = pTerrain->Get_Interval();
			_ulong dwTileX = pTerrain->Get_TileX();
			_ulong dwTileZ = pTerrain->Get_TileZ();

			_int iIndexX = static_cast<_int>(vSrcPos.x / dwItv);
			_int iIndexZ = static_cast<_int>(vSrcPos.z / dwItv);

			if (iIndexX < 0)
			{
				pSrc->Set_PosX(0.f);
				break;
			}
			else if (iIndexX >= dwTileX)
			{
				pSrc->Set_PosX(static_cast<_float>((dwTileX - 1) * dwItv));
				break;
			}
			if (iIndexZ < 0)
			{
				pSrc->Set_PosZ(0.f);
				break;
			}
			else if (iIndexZ >= dwTileZ)
			{
				pSrc->Set_PosZ(static_cast<_float>((dwTileZ - 1) * dwItv));
				break;
			}

			WALLPOSITION eWall = pTerrain->Check_Wall(iIndexX, iIndexZ);
			switch (eWall)
			{
			case WALL_LEFT:
				
				break;
			case WALL_TOP:
				break;
			case WALL_RIGHT:
				break;
			case WALL_BOTTOM:
				break;
			case WALL_CEILING:
				break;
			case WALL_END:
				break;
			}

		}
	}

}

void CCollisionFunctions::CollisionObjectToRoomBlock(const _tchar * pSrcTag, const _tchar * pDestTag)
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
			CRoomBlock* pRoom = dynamic_cast<CRoomBlock*>(pDest);
			if (nullptr == pRoom)
				continue;

			CRoomBlock::BLOCK_DIR eDir = pRoom->Get_BlockDir();
			switch (eDir)
			{
			case CRoomBlock::BLOCK_UP:
				if (tSrcSph.vPos.z < pRoom->Get_Pos()->z)
					pSrc->Set_PosZ(pRoom->Get_Pos()->z + tSrcSph.fRadius);
				break;
			case CRoomBlock::BLOCK_DOWN:
				if (tSrcSph.vPos.z > pRoom->Get_Pos()->z)
					pSrc->Set_PosZ(pRoom->Get_Pos()->z - tSrcSph.fRadius);
				break;
			case CRoomBlock::BLOCK_LEFT:
				if (tSrcSph.vPos.x > pRoom->Get_Pos()->x)
					pSrc->Set_PosX(pRoom->Get_Pos()->x - tSrcSph.fRadius);
				break;
			case CRoomBlock::BLOCK_RIGHT:
				if (tSrcSph.vPos.x < pRoom->Get_Pos()->x)
					pSrc->Set_PosX(pRoom->Get_Pos()->x + tSrcSph.fRadius);
				break;
			}
			
		}
	}
}

void CCollisionFunctions::CollisionPlayerToCoin(const _tchar * pSrcTag, const _tchar * pDestTag)
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
			CCoin* pCoin = dynamic_cast<CCoin*>(pDest);

			if (Engine::CCollisionMgr::CollisionSphereToSphere(tSrcSph, tDestSph))
			{
				pDest->Die();
				pDest->Hit(0, nullptr);
				
				CInven::GetInstance()->Add_Coin(pCoin->Get_Coin());
			}
		}
	}
}

bool CCollisionFunctions::CollisionRectToTile(const Engine::SPHERE * pSphere, const Engine::RECTANGLE* pRect)
{
	Engine::RECTANGLE tRectSrc;
	tRectSrc.vPos = pSphere->vPos;
	tRectSrc.fHeight = tRectSrc.fWidth = pSphere->fRadius * 2.f;

	return CollisionRectToTile(&tRectSrc, pRect);
}

bool CCollisionFunctions::CollisionRectToTile(const Engine::RECTANGLE * pRectSrc, const Engine::RECTANGLE * pRectDest)
{
	_float fSrcLeft = pRectSrc->vPos.x - pRectSrc->fWidth * 0.5f;
	_float fSrcRight = pRectSrc->vPos.x + pRectSrc->fWidth * 0.5f;
	_float fSrcTop = pRectSrc->vPos.z + pRectSrc->fHeight * 0.5f;
	_float fSrcBot = pRectSrc->vPos.z - pRectSrc->fHeight * 0.5f;

	_float fDestLeft	= pRectDest->vPos.x - pRectDest->fWidth * 0.5f;
	_float fDestRight	= pRectDest->vPos.x + pRectDest->fWidth * 0.5f;
	_float fDestTop		= pRectDest->vPos.z + pRectDest->fHeight * 0.5f;
	_float fDestBot		= pRectDest->vPos.z - pRectDest->fHeight * 0.5f;

	if (fSrcLeft > fDestRight || fDestLeft > fSrcRight)
		return false;
	if (fSrcBot > fDestTop || fDestBot > fSrcTop)
		return false;

	return true;
}
