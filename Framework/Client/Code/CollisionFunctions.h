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
	static void		CollisionBulletToObject(const _tchar* pSrcDest, const _tchar* pDest);
};

#endif // CollisionFunctions_h__
