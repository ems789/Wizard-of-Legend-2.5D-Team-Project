// WallPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "WallPage.h"
#include "afxdialogex.h"

#include "Terrain.h"

#include "Export_Function.h"

USING(Engine)

// CWallPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWallPage, CPropertyPage)

CWallPage::CWallPage()
	: CPropertyPage(IDD_WALLPAGE),
	m_iDrawID(0),
	m_pGraphicDev(Engine::CGraphicDev::GetInstance())
{

}

CWallPage::~CWallPage()
{
}

void CWallPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_CHECK2, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK4, m_Check[2]);
	DDX_Control(pDX, IDC_CHECK5, m_Check[3]);
}

BEGIN_MESSAGE_MAP(CWallPage, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CWallPage::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &CWallPage::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CWallPage::OnBnClickedLoad)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK2, &CWallPage::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CWallPage::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CWallPage::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CWallPage::OnBnClickedCheck5)
END_MESSAGE_MAP()

//BEGIN_MESSAGE_MAP(CWallPage, CPropertyPage)
//END_MESSAGE_MAP()

// CWallPage 메시지 처리기입니다.

void CWallPage::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	TCHAR szFileName[MAX_STR] = L"";

	// 드래그 앤 드롭된 대상들 중 인덱스에 해당하는 파일의 이름을 얻어온다.
	//::DragQueryFile(hDropInfo, 1, szFileName, MAX_STR);
	//::AfxMessageBox(szFileName);

	// 두번째인자가 -1일 때 드래그 앤 드롭된 대상들의 전체 개수를 얻어온다.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	CString strRelative = L"";
	CString strFileName = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);

		//strRelative = CDirectoryMgr::ConvertRelativePath(szFileName);
		strFileName = ::PathFindFileName(szFileName); // 파일명 얻어내는 함수.

		lstrcpy(szFileName, strFileName);
		::PathRemoveExtension(szFileName); // 확장자 제거.

		m_ListBox.AddString(szFileName);
	}

	SettingHorizontalScrollSize();

	CDialog::OnDropFiles(hDropInfo);
}

void CWallPage::OnLbnSelchangeList()
{
	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strName = L"";
	m_ListBox.GetText(iIndex, strName);

	int i = 0;

	for (; i < strName.GetLength(); ++i)
	{
		// 현재 문자가 글자인지 숫자인지 판별. 숫자라면 true
		if (isdigit(strName[i]))
			break;
	}

	// Delete(index, count): 현재 문자열의 index자리부터 count만큼 제거.
	strName.Delete(0, i);

	// 문자 -> 정수로 변환.
	m_iDrawID = _ttoi(strName);

	// 픽처 컨트롤에 타일 미리보기 출력.
	const TEX_INFO* pTexInfo = Engine::Get_TexInfo(::RESOURCE_STAGE, L"Texture_Wall", m_iDrawID);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (float)WINCX / pTexInfo->tImgInfo.Width / 2, (float)WINCY / pTexInfo->tImgInfo.Height / 2, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale * matTrans;

	m_pGraphicDev->GetSprite()->SetTransform(&matWorld);

	m_pGraphicDev->Render_Begin(D3DCOLOR_ARGB(255, 153, 217, 234));
	m_pGraphicDev->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_pGraphicDev->GetSprite()->Draw((LPDIRECT3DTEXTURE9)(pTexInfo->pTexture), nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pGraphicDev->GetSprite()->End();
	m_pGraphicDev->Render_End(m_Picture.m_hWnd);

}

void CWallPage::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pView);

		NULL_CHECK(pView->m_pTerrain);
		pView->m_pTerrain->SaveWall(Dlg.GetPathName());
	}
}

void CWallPage::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pView);

		NULL_CHECK(pView->m_pTerrain);
		pView->m_pTerrain->LoadWall(Dlg.GetPathName());

		pView->Invalidate(FALSE);
	}
}

void CWallPage::SettingHorizontalScrollSize()
{
	// 리스트박스에 수평 스크롤 확장하기.

	CString strName = L"";

	int iSrcCX = 0, iDstCX = 0;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀 단위로 환산.
		iSrcCX = pDC->GetTextExtent(strName).cx;

		if (iDstCX < iSrcCX)
			iDstCX = iSrcCX;
	}

	m_ListBox.ReleaseDC(pDC);

	// 현재 리스트박스가 가지고 있는 수평 스크롤 범위보다 클 때
	if (iDstCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iDstCX); // 수평 스크롤 크기 갱신.
}


void CWallPage::OnBnClickedCheck2()
{	
	m_bIsCheck[WALL_LEFT] = m_Check[WALL_LEFT].GetCheck();
}


void CWallPage::OnBnClickedCheck3()
{
	m_bIsCheck[WALL_TOP] = m_Check[WALL_TOP].GetCheck();
}


void CWallPage::OnBnClickedCheck4()
{
	m_bIsCheck[WALL_RIGHT] = m_Check[WALL_RIGHT].GetCheck();
}


void CWallPage::OnBnClickedCheck5()
{
	m_bIsCheck[WALL_BOTTOM] = m_Check[WALL_BOTTOM].GetCheck();
}
