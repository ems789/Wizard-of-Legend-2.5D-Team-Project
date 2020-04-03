#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"

BEGIN(Engine)

// Management Instance
// Get
inline 	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag,	COMPONENTID eID);
// Set
// General
inline HRESULT		Create_Management(CManagement** ppManagement);


// Renderer
inline CRenderer*		Get_Renderer(void);


// Utility Release
inline void			Release_Utility(void);


#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
