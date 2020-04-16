#ifndef MyMath_h__
#define MyMath_h__

#include "Engine_Defines.h"

#ifndef AXIS_X
#define AXIS_X _vec3(1.f, 0.f, 0.f)
#endif

#ifndef AXIS_Y
#define AXIS_Y _vec3(0.f, 1.f, 0.f)
#endif

#ifndef	AXIS_Z
#define AXIS_Z _vec3(0.f, 0.f, 1.f)
#endif
BEGIN(Engine)

class ENGINE_DLL CMyMath
{
public:
	static void ViewPortToOrth(_vec3* pOut, const _vec2 * pPos, _long dwWinCX, _long dwWinCY);
	
	static void ViewPortToProj(_vec3* pOut, const _vec2* pPos, const _long& dwWinCX, const _long& dwWinCY);
	static void ViewPortToProj(_vec3* pOut, const POINT* pPos, const _long& dwWinCX, const _long& dwWinCY);
	static void ViewPortToProj(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut);
	static void ViewPortToProj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const _vec2* pPos);
	static void ViewPortToProj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const POINT* pPos);
	
	static void ProjToView(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const _vec3* pvProj);
	static void ProjToView(_vec3* pOut, const _matrix* pmatProj, const _vec3* pvProj);

	static void ViewToWorld(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const _vec3* pvView);
	static void ViewToWorld(_vec3* pOut, const _matrix* pmatView, const _vec3* pvView);

	static void CrossPointForLineToPlane(_vec3* pOut, const _vec3* pPoint1, const _vec3* pPoint2, const D3DXPLANE* pPlane);
	static void CrossPointForLineToPlane(_vec3* pOut, const LINE3D* pLine, const D3DXPLANE* pPlane);

	static void PickingOnPlane(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const _vec2* pPos, const D3DXPLANE* pPlane);
	static void PickingOnPlane(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const POINT* pPos, const D3DXPLANE* pPlane);
	static void PickingOnPlane(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut, const D3DXPLANE* pPlane);
	static void PickingOnPlane(_vec3* pOut, _vec2* pPos,const _long& dwWinCX, const _long& dwWinCY, const _matrix* pmatProj, const _matrix* pmatView, const D3DXPLANE* pPlane);

	static void LinearEquationFromPoints(LINE3D* pOut, const _vec3* pPos1, const _vec3* pPos2);

	static void	DirectionalVectorToAngle(_vec3* pOut, const _vec3* pDir);
};

END

#endif // MyMath_h__
