// TilePage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "TilePage.h"
#include "afxdialogex.h"

#include "Terrain.h"

#include "Export_Function.h"

USING(Engine)

// CTilePage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTilePage, CPropertyPage)

CTilePage::CTilePage()
	: CPropertyPage(IDD_TILEPAGE),
	m_iDrawID(0),
	m_pGraphicDev(Engine::CGraphicDev::GetInstance())
{

}

CTilePage::~CTilePage()
{
}

void CTilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BEGIN_MESSAGE_MAP(CTilePage, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CTilePage::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &CTilePage::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CTilePage::OnBnClickedLoad)
END_MESSAGE_MAP()

//BEGIN_MESSAGE_MAP(CTilePage, CPropertyPage)
//END_MESSAGE_MAP()

// CTilePage �޽��� ó�����Դϴ�.

void CTilePage::OnDropFiles(HDROP hDropInfo)
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

void CTilePage::OnLbnSelchangeList()
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
	const TEX_INFO* pTexInfo = Engine::Get_TexInfo(::RESOURCE_STAGE, L"Texture_Tile", m_iDrawID);
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

void CTilePage::OnBnClickedSave()
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
		pView->m_pTerrain->SaveTile(Dlg.GetPathName());
	}
}

void CTilePage::OnBnClickedLoad()
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
		pView->m_pTerrain->LoadTile(Dlg.GetPathName());

		pView->Invalidate(FALSE);
	}
}

void CTilePage::SettingHorizontalScrollSize()
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




BOOL CTilePage::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CPropertyPage::OnSetActive();
}
