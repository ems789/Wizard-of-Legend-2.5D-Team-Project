// PathTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "afxdialogex.h"


// PathTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHTOOL, pParent)
{

}

CPathTool::~CPathTool()
{
}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathTool::OnBnClickedLoad)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// PathTool �޽��� ó�����Դϴ�.


void CPathTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L".txt", L"���� ����.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		wofstream fout;

		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szCount[MIN_STR] = L"";

		for (auto pPathInfo : m_PathInfoList)
		{
			// ���� -> ����
			_itow_s(pPathInfo->iImgCount, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szCount + L"|" + pPathInfo->wstrRelative;

			fout << wstrCombined << endl;
		}

		fout.close();

		// ������ �⺻ ���� ���α׷� �����ϴ� �ý��� �Լ�.
		WinExec("notepad.exe ../Bin/Data/PathInfo.txt", SW_SHOW);
	}
}


void CPathTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L".txt", L"���� ����.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_PathInfoList.empty())
		{
			for_each(m_PathInfoList.begin(), m_PathInfoList.end(), Safe_Delete<PATH_INFO*>);
			m_PathInfoList.clear();

			m_ListBox.ResetContent();
		}

		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szCount[MAX_STR] = L"";
		TCHAR szRelative[MAX_STR] = L"";

		PATH_INFO* pPathInfo = nullptr;

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szRelative, MAX_STR);

			if (fin.eof())
				break;

			pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iImgCount = _ttoi(szCount);
			pPathInfo->wstrRelative = szRelative;

			m_PathInfoList.push_back(pPathInfo);
		}

		fin.close();

		wstring wstrCombined = L"";

		for (auto pPathInfo : m_PathInfoList)
		{
			// ���� -> ����
			_itow_s(pPathInfo->iImgCount, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szCount + L"|" + pPathInfo->wstrRelative;

			m_ListBox.AddString(wstrCombined.c_str());
		}

		SettingHorizontalScrollSize();
	}
}


void CPathTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);
		CDirectoryMgr::ExtractPathInfo(szFileName, m_PathInfoList);
	}

	wstring wstrCombined = L"";
	TCHAR szCount[MIN_STR] = L"";

	for (auto pPathInfo : m_PathInfoList)
	{
		// ���� -> ����
		_itow_s(pPathInfo->iImgCount, szCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
			+ szCount + L"|" + pPathInfo->wstrRelative;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SettingHorizontalScrollSize();

	CDialog::OnDropFiles(hDropInfo);
}

void CPathTool::SettingHorizontalScrollSize()
{
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
