#ifndef CollisionFunctions_h__
#define CollisionFunctions_h__

#include "Defines.h"
#include "Engine_Defines.h"

class CCollisionFunctions
{
private:
	explicit CCollisionFunctions();
	~CCollisionFunctions();

public:
	static HRESULT	Ready_Functions();
	static void		CollisionExample(const _tchar* pSrcTag, const _tchar* pDestTag);
	static void		CollisionBulletToObject(const _tchar* pSrcTag, const _tchar* pDestTag);
	static void		CollisionObjAttackToObject(const _tchar* pSrcTag, const _tchar* pDestTag);
	static void		CollisionObjectToTerrain(const _tchar* pSrcTag, const _tchar* pDestTag);
	static void		CollisionOBjectToTerrainPoint(const _tchar* pSrcTag, const _tchar* pDestTag);
	static void		CollisionObjectToRoomBlock(const _tchar* pSrcTag, const _tchar* pDestTag);

public:
	static bool		CollisionRectToTile(const Engine::SPHERE* pSphere, const Engine::RECTANGLE* pRect);
	static bool		CollisionRectToTile(const Engine::RECTANGLE* pRectSrc, const Engine::RECTANGLE* pRectDest);
};

#endif // CollisionFunctions_h__
