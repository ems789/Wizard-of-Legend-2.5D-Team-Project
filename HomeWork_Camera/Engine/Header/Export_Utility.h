#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "CameraMgr.h"

BEGIN(Engine)

// Management Instance
// Get
inline 	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag,	COMPONENTID eID);
// Set
// General
inline HRESULT		Create_Management(CManagement** ppManagement);


// Renderer
inline CRenderer*		Get_Renderer(void);

// Camera
// Get
inline CCamera*	Get_MainCam();
//General
inline HRESULT	Add_Camera(const _tchar* pCamTag, CCamera* pCamera);
inline HRESULT	Remove_Camera(const _tchar* pCamTag);

inline HRESULT	SetUp_Camera(const _tchar* pCamTag);
inline _int		Update_Camera(const _float& fTimeDelta);


// Utility Release
inline void			Release_Utility(void);


#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
