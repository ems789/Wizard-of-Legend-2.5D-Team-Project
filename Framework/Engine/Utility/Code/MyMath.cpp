#include "MyMath.h"

USING(Engine)
void CMyMath::ViewPortToOrth(_vec3 * pOut, const _vec2 * pPos, _long dwWinCX, _long dwWinCY)
{
	pOut->x = - dwWinCX * 0.5f + pPos->x;
	pOut->y =	dwWinCY * 0.5f - pPos->y;
	pOut->z = 0.f;
}

void CMyMath::ViewPortToProj(_vec3 * pOut, const _vec2 * pPos, const _long & dwWinCX, const _long & dwWinCY)
{
	pOut->x = pPos->x / (dwWinCX * 0.5f) - 1.f;
	pOut->y = -pPos->y / (dwWinCY * 0.5f) + 1.f;
	pOut->z = 0.f;
}

void CMyMath::ViewPortToProj(_vec3 * pOut, const POINT * pPos, const _long & dwWinCX, const _long & dwWinCY)
{
	pOut->x = pPos->x / (dwWinCX * 0.5f) - 1.f;
	pOut->y = -pPos->y / (dwWinCY * 0.5f) + 1.f;
	pOut->z = 0.f;
}

void CMyMath::ViewPortToProj(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut)
{
	POINT			ptMouse{};

	// 윈도우 영역 == 뷰포트 영역
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰 포트에서 투영으로

	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	pGraphicDev->GetViewport(&ViewPort);

	pOut->x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	pOut->y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	pOut->z = 0.f;
}

void CMyMath::ViewPortToProj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const _vec2 * pPos)
{
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	pGraphicDev->GetViewport(&ViewPort);

	pOut->x = pPos->x / (ViewPort.Width * 0.5f) - 1.f;
	pOut->y = -pPos->y / (ViewPort.Height * 0.5f) + 1.f;
	pOut->z = 0.f;
}

void CMyMath::ViewPortToProj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const POINT * pPos)
{
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	pGraphicDev->GetViewport(&ViewPort);

	pOut->x = pPos->x / (ViewPort.Width * 0.5f) - 1.f;
	pOut->y = -pPos->y / (ViewPort.Height * 0.5f) + 1.f;
	pOut->z = 0.f;
}

void CMyMath::ProjToView(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const _vec3 * pvProj)
{
	_matrix		matProj;

	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(pOut, pvProj, &matProj);
}

void CMyMath::ProjToView(_vec3 * pOut, const _matrix * pmatProj, const _vec3 * pvProj)
{
	_matrix		matProj;

	D3DXMatrixInverse(&matProj, 0, pmatProj);
	D3DXVec3TransformCoord(pOut, pvProj, &matProj);
}

void CMyMath::ViewToWorld(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const _vec3 * pvView)
{
	_matrix		matView;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(pOut, pvView, &matView);
}

void CMyMath::ViewToWorld(_vec3 * pOut, const _matrix * pmatView, const _vec3 * pvView)
{
	_matrix		matView;

	D3DXMatrixInverse(&matView, 0, pmatView);
	D3DXVec3TransformCoord(pOut, pvView, &matView);
}

void CMyMath::CrossPointForLineToPlane(_vec3* pOut, const _vec3 * pPoint1, const _vec3 * pPoint2, const D3DXPLANE * pPlane)
{
	LINE3D tLine3D;
	LinearEquationFromPoints(&tLine3D, pPoint2, pPoint1);

	CrossPointForLineToPlane(pOut, &tLine3D, pPlane);
}

void CMyMath::CrossPointForLineToPlane(_vec3 * pOut, const LINE3D * pLine, const D3DXPLANE * pPlane)
{
	_vec3 N = { pPlane->a, pPlane->b, pPlane->c };
	
	_vec3 PlaneP = { 1.f, 1.f, 1.f };

	if (0 != pPlane->a)
		PlaneP.x = -(pPlane->b + pPlane->c + pPlane->d) / pPlane->a;

	else if (0 != pPlane->b)
		PlaneP.y = -(pPlane->a + pPlane->c + pPlane->d) / pPlane->b;

	else if (0 != pPlane->c)
		PlaneP.z = -(pPlane->a + pPlane->b + pPlane->d) / pPlane->c;

	else
	{
		ZeroMemory(pOut, sizeof(_vec3));
		return;
	}
	
	if (0.f == D3DXVec3Dot(&pLine->vDir, &N))
	{
		ZeroMemory(pOut, sizeof(_vec3));
		return;
	}

	_float parameter = (D3DXVec3Dot(&(PlaneP - pLine->vPos), &N) / D3DXVec3Dot(&pLine->vDir, &N));

	*pOut = pLine->vPos + parameter * pLine->vDir;
}

void CMyMath::PickingOnPlane(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const _vec2 * pPos, const D3DXPLANE* pPlane)
{
	CMyMath::ViewPortToProj(pGraphicDev, pOut, pPos);
	CMyMath::ProjToView(pGraphicDev, pOut, pOut);
	CMyMath::ViewToWorld(pGraphicDev, pOut, pOut);

	_vec3 vRayStart = { 0.f, 0.f, 0.f };
	CMyMath::ViewToWorld(pGraphicDev, &vRayStart, &vRayStart);

	CrossPointForLineToPlane(pOut, &vRayStart, pOut, pPlane);
}

void CMyMath::PickingOnPlane(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const POINT * pPos, const D3DXPLANE* pPlane)
{
	CMyMath::ViewPortToProj(pGraphicDev, pOut, pPos);
	CMyMath::ProjToView(pGraphicDev, pOut, pOut);
	CMyMath::ViewToWorld(pGraphicDev, pOut, pOut);

	_vec3 vRayStart = { 0.f, 0.f, 0.f };
	CMyMath::ViewToWorld(pGraphicDev, &vRayStart, &vRayStart);

	CrossPointForLineToPlane(pOut, &vRayStart, pOut, pPlane);
}

void CMyMath::PickingOnPlane(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut, const D3DXPLANE* pPlane)
{
	CMyMath::ViewPortToProj(hWnd, pGraphicDev, pOut);
	CMyMath::ProjToView(pGraphicDev, pOut, pOut);
	CMyMath::ViewToWorld(pGraphicDev, pOut, pOut);

	_vec3 vRayStart = { 0.f, 0.f, 0.f };
	CMyMath::ViewToWorld(pGraphicDev, &vRayStart, &vRayStart);

	CrossPointForLineToPlane(pOut, &vRayStart, pOut, pPlane);
}

void CMyMath::PickingOnPlane(_vec3 * pOut, _vec2* pPos, const _long & dwWinCX, const _long & dwWinCY, const _matrix * pmatProj, const _matrix * pmatView, const D3DXPLANE* pPlane)
{
	CMyMath::ViewPortToProj(pOut, pPos, dwWinCX, dwWinCY);
	CMyMath::ProjToView(pOut, pmatProj, pOut);
	CMyMath::ViewToWorld(pOut, pmatView, pOut);

	_vec3 vRayStart = { 0.f, 0.f, 0.f };
	CMyMath::ViewToWorld(&vRayStart, pmatView, &vRayStart);

	CrossPointForLineToPlane(pOut, &vRayStart, pOut, pPlane);
}

void CMyMath::LinearEquationFromPoints(LINE3D * pOut, const _vec3 * pPos1, const _vec3 * pPos2)
{
	_vec3 vDir = *pPos1 - *pPos2;

	pOut->vDir = vDir;
	pOut->vPos = *pPos1;
}

void CMyMath::DirectionalVectorToAngle(_vec3 * pOut, const _vec3 * pDir)
{
	_vec3 vTemp = *pDir;
	vTemp.x = 0;
	D3DXVec3Normalize(&vTemp, &vTemp);

	pOut->x = acosf(D3DXVec3Dot(&AXIS_Z, &vTemp));
	if (pDir->y > 0.f) 
		pOut->x = -pOut->x;

	vTemp = *pDir;
	vTemp.y = 0;
	D3DXVec3Normalize(&vTemp, &vTemp);

	pOut->y = acosf(D3DXVec3Dot(&AXIS_Z, &vTemp));

	if (pDir->z < 0.f)
		pOut->y += D3DXToRadian(180.f);

	if (pDir->x < 0.f)
		pOut->y = -pOut->y;


	pOut->z = 0;
}

_float CMyMath::XAngleTransformFromVec(const _vec3 * pDir)
{
	_vec3 vTemp = *pDir;

	vTemp.x = 0;
	D3DXVec3Normalize(&vTemp, &vTemp);

	_float fx = acosf(D3DXVec3Dot(&AXIS_Z, &vTemp));

	if (pDir->z > 0.f)
		fx += D3DXToRadian(180.f);

	if (pDir->y < 0.f)
		fx = -fx;

	return fx;
}

_float CMyMath::YAngleTransformFromVec(const _vec3 * pDir)
{
	_vec3 vTemp = *pDir;

	vTemp.y = 0;
	D3DXVec3Normalize(&vTemp, &vTemp);

	_float fy = acosf(D3DXVec3Dot(&AXIS_Z, &vTemp));

	if (pDir->z < 0.f)
		fy += D3DXToRadian(180.f);

	if (pDir->x < 0.f)
		fy = -fy;

	return fy;
}

_float CMyMath::ZAngleTransformFromVec(const _vec3 * pDir)
{
	_vec3 vTemp = *pDir;

	vTemp.z = 0;
	D3DXVec3Normalize(&vTemp, &vTemp);

	_float fz = acosf(D3DXVec3Dot(&AXIS_Z, &vTemp));

	if (pDir->y < 0.f)
		fz += D3DXToRadian(180.f);

	if (pDir->x < 0.f)
		fz = -fz;

	return fz;
}

void CMyMath::ClientMousePos(HWND hWnd, _vec2 * pOut)
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	pOut->x = ptMouse.x;
	pOut->y = ptMouse.y;
}
