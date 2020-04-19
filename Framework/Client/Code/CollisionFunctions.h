#ifndef CollisionFunctions_h__
#define CollisionFunctions_h__

#include "Defines.h"

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
};

#endif // CollisionFunctions_h__
