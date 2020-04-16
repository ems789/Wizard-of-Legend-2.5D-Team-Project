
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "PropertyFormView.h"

#include "Terrain.h"
#include "TerrainGuidLine.h"
#include "DynamicCamera.h"
#include "Macro.h"
#include "Export_Function.h"

USING(Engine)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView
IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

HWND	  g_hWnd;
HINSTANCE g_hInst;

// CToolView ����/�Ҹ�

CToolView::CToolView() :
	m_pGraphicDev(Engine::CGraphicDev::GetInstance()),
	m_pTerrainGuidLine(nullptr),
	m_pDynamicCamera(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
	Engine::Safe_Release(m_pGraphicDev);

	Engine::Release_Resources();
	Engine::CInputDev::GetInstance()->DestroyInstance();
	//CGraphicDev::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	m_pGraphicDev->Render_Begin(D3DCOLOR_ARGB(255, 153, 217, 234));

	m_pTerrainGuidLine->Render_GameObject();
	m_pTerrain->Update_GameObject(1.f);
	m_pTerrain->Render_GameObject();

	m_pGraphicDev->Render_End();
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	// View ũ�⸦ �����ϱ� ���ؼ��� ������ �������� ũ�⸦ �ǵ帰��.
	// ������ �������� ������ ������ ���.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is null");

	RECT rcFrame = {};

	// �������� ũ��(�׵θ� + Ŭ���̾�Ʈ ����) ������ �Լ�.(��ũ�� ��ǥ ����)
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// �������� Ŭ���̾�Ʈ ����(�׵θ��� �������� ����) ũ�� ������ �Լ�. (Ŭ���̾�Ʈ ��ǥ ����)
	GetClientRect(&rcView);

	// ���� ������������� View ������ ������ ���Ѵ�.
	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// �������������� ũ�⸦ ���� ����.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);


	// ��ġ �ʱ�ȭ
	FAILED_CHECK_VOID(Engine::Ready_GraphicDev(
		g_hWnd,
		Engine::MODE_WIN,
		WINCX,
		WINCY,
		&m_pGraphicDev));
	m_pGraphicDev->AddRef();

	LPDIRECT3DDEVICE9 pGraphicDev = m_pGraphicDev->GetDevice();
	pGraphicDev->AddRef();
	FAILED_CHECK_VOID(pGraphicDev);

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �޸� ����
	Engine::Reserve_ContainerSize(::RESOURCE_END);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(GetParentSplitter(this, FALSE)->GetPane(0, 0));

	// ���� �ʱ�ȭ
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX,
		L"",
		pPropertyFormView->m_iTerrainX + 1, // ������ �����̹Ƿ� 1�� ����
		pPropertyFormView->m_iTerrainZ + 1,
		pPropertyFormView->m_iTerrainItv),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TileTex",
		Engine::BUFFER_TILETEX,
		L""),
		E_FAIL);

	// �ؽ��� �ε�
	/*FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(pGraphicDev,
		::RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
		E_FAIL);*/

	// Ÿ��
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(pGraphicDev,
		::RESOURCE_STAGE,
		L"Texture_Tile",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Tile/ForestTile/ForestTile%d.png", 321),
		E_FAIL);



	m_pTerrainGuidLine = CTerrainGuidLine::Create(pGraphicDev);
	NULL_CHECK_MSG(m_pTerrainGuidLine, L"TerrainGuidLine Create Failed");

	// �ͷ��� �����(���� �����Ǿ� �ִ� x * y ũ�⸸ŭ)
	m_pTerrain = CTerrain::Create(pGraphicDev, pPropertyFormView->m_iTerrainX, 
												pPropertyFormView->m_iTerrainZ,
												pPropertyFormView->m_iTerrainItv);
	NULL_CHECK_MSG(m_pTerrain, L"Terrain Create Failed");


	// dynamicCamera
	m_pDynamicCamera = CDynamicCamera::Create(pGraphicDev,
		&::_vec3(0.f, 5.f, -5.f),
		&::_vec3(0.f, 0.f, 0.f),
		&::_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		::_float(WINCX) / WINCY,
		1.f,
		1000.f);
	NULL_CHECK_RETURN_VOID(m_pDynamicCamera);

	// INPUTDEV
	FAILED_CHECK_RETURN_VOID(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	pGraphicDev->Release();
}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	Engine::Set_InputDev();

	m_pDynamicCamera->Update_KeyInput(0.25f);

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (Engine::Get_DIMouseState(Engine::DIM_LB))
	{
		const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
		NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

		const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

		::_vec3 vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		/// TODO :: �ٲ� �ؽ����� �±׸� �־����
		m_pTerrain->TileChange(vPickedTile, L"Texture_Tile", true);
	}
	else if (Engine::Get_DIMouseState(Engine::DIM_RB))
	{
		const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
		NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

		const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

		::_vec3 vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		/// TODO :: �ٲ� �ؽ����� �±׸� �־����
		m_pTerrain->TileChange(vPickedTile, L"Texture_Tile", false);
	}

	Engine::Set_InputDev();

	m_pDynamicCamera->Update_MouseMove(0.1f);

	Invalidate(FALSE);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	::_vec3 vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	/// TODO :: �ٲ� �ؽ����� �±׸� �־����
	m_pTerrain->TileChange(vPickedTile, L"Test", true);

	Invalidate(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);

	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	::_vec3 vPickedTilePos = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	/// TODO :: �ٲ� �ؽ����� �±׸� �־����
	m_pTerrain->TileChange(vPickedTilePos, L"Test", false);

	Invalidate(FALSE);
}


::_vec3 CToolView::Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom)
{
	POINT			ptMouse{};

	// ������ ���� == ����Ʈ ����
	GetCursorPos(&ptMouse);
	::ScreenToClient(hWnd, &ptMouse);

	// �� ��Ʈ���� ��������
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetDevice()->GetViewport(&ViewPort);

	::_vec3	vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// �������� �� �����̽���
	::_matrix		matProj;

	m_pGraphicDev->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj); /// TODO:: GetDevice�� �Ź� ȣ������ ���� �ѹ��� ������ ����
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	::_vec3	vRayPos, vRayDir;

	vRayPos = ::_vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// �� �����̽����� ���� �����̽���
	::_matrix		matView;

	m_pGraphicDev->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// ���忡�� ���÷�
	::_matrix		matWorld;
  	pTerrainTransformCom->GetWorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	::_ulong	dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	::_ulong	dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	const ::_vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	::_ulong			dwVtxIdx[3];
	float				fU, fV, fDist;

	for (::_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (::_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			::_ulong	dwIndex = i * dwVtxCntX + j;

			// ������ ��
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos,
				&vRayDir, &fU, &fV, &fDist))
			{
				return ::_vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}


			// ���� �Ʒ�
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[1]],
				&vRayPos,
				&vRayDir, &fU, &fV, &fDist))
			{
				return ::_vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return ::_vec3(0.f, 0.f, 0.f);
}