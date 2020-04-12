#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "CameraMgr.h"

//#include "Camera.h"

BEGIN(Engine)

//	Management Instance
//	Get
inline const	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline const	CGameObject*	Get_GameObjcet(const _tchar* pLayerTag, const _tchar* pObjTag);
inline			CLayer*			Get_StaticLayer();

inline const CComponent*	Get_Component_From_StaticLayer(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline const CGameObject*	Get_GameObject_From_StaticLayer(const _tchar* pObjTag);

inline const CComponent*	Get_Component_of_Player(const _tchar* pComponentTag, COMPONENTID eID);
inline const CGameObject*	Get_Player();

//	Set
//	General
inline HRESULT Create_Management(CManagement** ppManagemet);
inline HRESULT SetUp_Scene(CScene* pScene);
inline HRESULT Add_GameObjectToStaticLayer(const _tchar* pObjTag, CGameObject* pObj);

//	Renderer
inline CRenderer* Get_Renderer();

//	Camera Manager
//	Get
inline CCamera*	Get_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);
//	Set
//	General
inline HRESULT	Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY);
inline _int		Update_MainCamera(const _float& fTimeDelta);

inline HRESULT	SetUp_MainCamera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);

inline HRESULT	Add_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag, CCamera* pCamera);
inline HRESULT	Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);


//	Utility Release
inline void Release_Utility(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
