
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;
class CTerrainTex;
class CTransform;

END

class CTerrainGuidLine;
class CDynamicCamera;
class CToolDoc;
class CTerrain;
class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

// Message Funtions
public:	
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

private:
	void		 CreateObject(bool bIsRender);
	_vec3		 Picking_OnTerrain(HWND hWnd,
		const Engine::CTerrainTex* pTerrainBufferCom,
		const Engine::CTransform*	pTerrainTransformCom);

public:
	Engine::CGraphicDev* m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9	 m_pGraphicDev = nullptr;;
	CTerrain*			 m_pTerrain;
	CTerrainGuidLine*	 m_pTerrainGuidLine;
	CDynamicCamera*		 m_pDynamicCamera;
	_matrix				 m_matView, m_matProj;
};
