// WallPage.cpp : ���� �����Դϴ�.
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

// CWallPage ��ȭ �����Դϴ�.

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

// CWallPage �޽��� ó�����Դϴ�.

void CWallPage::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	TCHAR szFileName[MAX_STR] = L"";

	// �巡�� �� ��ӵ� ���� �� �ε����� �ش��ϴ� ������ �̸��� ���´�.
	//::DragQueryFile(hDropInfo, 1, szFileName, MAX_STR);
	//::AfxMessageBox(szFileName);

	// �ι�°���ڰ� -1�� �� �巡�� �� ��ӵ� ������ ��ü ������ ���´�.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	CString strRelative = L"";
	CString strFileName = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);

		//strRelative = CDirectoryMgr::ConvertRelativePath(szFileName);
		strFileName = ::PathFindFileName(szFileName); // ���ϸ� ���� �Լ�.

		lstrcpy(szFileName, strFileName);
		::PathRemoveExtension(szFileName); // Ȯ���� ����.

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
		// ���� ���ڰ� �������� �������� �Ǻ�. ���ڶ�� true
		if (isdigit(strName[i]))
			break;
	}

	// Delete(index, count): ���� ���ڿ��� index�ڸ����� count��ŭ ����.
	strName.Delete(0, i);

	// ���� -> ������ ��ȯ.
	m_iDrawID = _ttoi(strName);

	// ��ó ��Ʈ�ѿ� Ÿ�� �̸����� ���.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L".dat", L"���� ����.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L".dat", L"���� ����.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
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
	// ����Ʈ�ڽ��� ���� ��ũ�� Ȯ���ϱ�.

	CString strName = L"";

	int iSrcCX = 0, iDstCX = 0;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// ���� ���ڿ��� ���̸� �ȼ� ������ ȯ��.
		iSrcCX = pDC->GetTextExtent(strName).cx;

		if (iDstCX < iSrcCX)
			iDstCX = iSrcCX;
	}

	m_ListBox.ReleaseDC(pDC);

	// ���� ����Ʈ�ڽ��� ������ �ִ� ���� ��ũ�� �������� Ŭ ��
	if (iDstCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iDstCX); // ���� ��ũ�� ũ�� ����.
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
