
// OverSpeedProView.cpp: COverSpeedProView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OverSpeedPro.h"
#endif

#include "OverSpeedProDoc.h"
#include "OverSpeedProView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COverSpeedProView

IMPLEMENT_DYNCREATE(COverSpeedProView, CView)

BEGIN_MESSAGE_MAP(COverSpeedProView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// COverSpeedProView 构造/析构

COverSpeedProView::COverSpeedProView() noexcept
{
	// TODO: 在此处添加构造代码

}

COverSpeedProView::~COverSpeedProView()
{
}

BOOL COverSpeedProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COverSpeedProView 绘图

void COverSpeedProView::OnDraw(CDC* /*pDC*/)
{
	COverSpeedProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: 在此处为本机数据添加绘制代码
}


// COverSpeedProView 打印

BOOL COverSpeedProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COverSpeedProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COverSpeedProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// COverSpeedProView 诊断

#ifdef _DEBUG
void COverSpeedProView::AssertValid() const
{
	CView::AssertValid();
}

void COverSpeedProView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COverSpeedProDoc* COverSpeedProView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COverSpeedProDoc)));
	return (COverSpeedProDoc*)m_pDocument;
}
#endif //_DEBUG


// COverSpeedProView 消息处理程序
