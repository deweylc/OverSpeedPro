// StationYardView.cpp: 实现文件
//

#include "pch.h"
#include "OverSpeedPro.h"
#include "StationYardView.h"


// StationYardView

IMPLEMENT_DYNCREATE(StationYardView, CView)

StationYardView::StationYardView()
{

}

StationYardView::~StationYardView()
{
}

BEGIN_MESSAGE_MAP(StationYardView, CView)
END_MESSAGE_MAP()


// StationYardView 绘图

void StationYardView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
	CFont font;
	font.CreatePointFont(500, _T("宋体"));
	CFont* oldfont = pDC->SelectObject(&font);
	pDC->TextOutW(1, 1, _T("站场图"));
	pDC->SelectObject(oldfont);
}


// StationYardView 诊断

#ifdef _DEBUG
void StationYardView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void StationYardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// StationYardView 消息处理程序
