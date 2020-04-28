#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "CameraMgr.h"
#include "CollisionMgr.h"
#include "MyMath.h"
#include "Effect.h"

//#include "Camera.h"

BEGIN(Engine)

//	Management Instance
//	Get
inline const	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline const	CGameObject*	Get_GameObjcet(const _tchar* pLayerTag, const _tchar* pObjTag);
inline const	CLayer*			Get_Layer(const _tchar* pLayerTag);
inline			CLayer*			Get_StaticLayer();

inline const	CComponent*		Get_Component_From_StaticLayer(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline const	CGameObject*	Get_GameObject_From_StaticLayer(const _tchar* pObjTag);

inline const	CComponent*		Get_Component_of_Player(const _tchar* pComponentTag, COMPONENTID eID);
inline const	CGameObject*	Get_Player();


//	Set
inline void						Player_Move_Pos(const _vec3* pMove);
inline void						Player_Set_Pos(const _vec3* pPos);
//	General
inline HRESULT	Create_Management(CManagement** ppManagemet);
inline HRESULT	SetUp_Scene(CScene* pScene);
inline HRESULT	Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pObj);
inline HRESULT	Add_GameObjectToStaticLayer(const _tchar* pObjTag, CGameObject* pObj);
inline HRESULT	Create_StaticLayer();
inline HRESULT	Clear_StaticLayer();

//	Renderer
inline CRenderer* Get_Renderer();

//	Camera Manager
//	Get
inline CCamera*				Get_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);
inline CCamera*				Get_MainCamera();
inline void					Get_MainCameraLook(_vec3* pLook);
inline void					Get_MainCameraRight(_vec3* pRight);
inline void					Get_MainCameraUp(_vec3* pUp);
inline const _vec3*			Get_MainCameraAngle(_vec3* pAngle);
inline CCameraMgr::MAIN_CAM Get_MainCamType();

//	Set
//	General
inline HRESULT	Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY);
inline _int		Update_MainCamera(const _float& fTimeDelta);

inline HRESULT	SetUp_MainCamera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);
inline HRESULT	Change_MainCamera(const _ubyte& byOrder);

inline HRESULT	Add_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag, CCamera* pCamera);
inline HRESULT	Add_BasicCamera(const _ubyte& byOrder, const _tchar* pCameraTag, CCamera* pCamera);
inline HRESULT	Remove_Camera(const _ulong& dwContainerIdx, const _tchar* pCameraTag);


//	CollisionMgr
//	Get
inline list<CGameObject*>*	Get_CollisionObjectList(const _tchar* pCollisionListTag);
//	Set
//	General
inline void					Add_GameObject_To_CollisionList(const _tchar* pCollisionListTag, CGameObject* pObj);
inline void					Add_CollisionFunction(const _tchar* pSrcTag, const _tchar* pDestTag, void(pFunc)(const _tchar*, const _tchar*));


//	Utility Release
inline void Release_Utility(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
