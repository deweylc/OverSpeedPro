// StationYardView.cpp: 实现文件
//
#include<iostream>
#include "pch.h"
#include "OverSpeedPro.h"
#include "StationYardView.h"
#include"CReadData.h"
#include"CSignal.h"
#include"OverSpeedProDoc.h"
#include"StationYardDoc.h"
//#include"CScrollView.h"
// StationYardView

IMPLEMENT_DYNCREATE(StationYardView, CScrollView)
int Train = 1;//用于确定列车是否显示
int Train1 = 1;//用于确定列车是否出发
/*翻墙成功*/
// C电路图View
int TrBgn_x = 0, TrBgn_y = 75;//列车出发点的坐标
int TrEnd_x = 1000, TrEnd_y = TrBgn_y;//列车到达点的坐标
StationYardView::StationYardView()
{
	if (RD.dubiaojishu == 0)
	{
		HRESULT hr; //判断函数执行结果
		try
		{
			hr = RD.Control_Connection.CreateInstance("ADODB.Connection");			//创建对象
			if (SUCCEEDED(hr))
			{
				hr = RD.Control_Connection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Data1.mdb", "", "", adModeUnknown);
			}
		}
		catch (_com_error e)
		{
			CString errormessage = e.Description();//错误信息类型
			errormessage.Format(_T("连接数据库失败！\r\n错误信息：%s"), errormessage);
			AfxMessageBox(errormessage);
		}
		//开始读表
	//RD.m_csignal[0].fuzhi  = RD.m_csignal[0].fuzhi(1001, 0, 1, 100, 100, 2001, 1002, 0, 1);

		RD.ReadSignal();
		RD.ReadBS();
		//	
	}
}

StationYardView::~StationYardView()
{

}

BEGIN_MESSAGE_MAP(StationYardView, CScrollView)
	ON_WM_TIMER()
	//ON_WM_CREATE()
	//ON_BN_CLICKED(IDC_BUTTON1, &StationYardView::On01)
END_MESSAGE_MAP()

// StationYardView 绘图

void StationYardView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码


	CRect rc;
	CDC dcMem;
	GetClientRect(&rc);
	CBitmap bmp; //内存中承载临时图象的位图


	dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(pDC, rc.Width()*2, rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色
	dcMem.FillSolidRect(rc, RGB(0, 0, 0));

	//下面很简单的实现了一下列车移动的功能
	if ((Train == 1) && (Train1 == 0))
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));//建立个白色的画刷给内存DC
		dcMem.SelectObject(&brush);///选择这个刷子
		dcMem.Rectangle(TrBgn_x, TrBgn_y, TrBgn_x + 30, TrBgn_y + 15);//一个正方形
		dcMem.Ellipse(TrBgn_x + 2, TrBgn_y + 15, TrBgn_x + 10, TrBgn_y + 23);
		dcMem.Ellipse(TrBgn_x + 20, TrBgn_y + 15, TrBgn_x + 28, TrBgn_y + 23);/////////模拟的列车的形状

	}
	else if ((Train == 1) && (Train1 == 1))
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));//建立个白色的画刷给内存DC
		dcMem.SelectObject(&brush);///选择这个刷子
		dcMem.Rectangle(TrBgn_x, TrBgn_y, TrBgn_x + 30, TrBgn_y + 15);//一个正方形
		dcMem.Ellipse(TrBgn_x + 2, TrBgn_y + 15, TrBgn_x + 10, TrBgn_y + 23);
		dcMem.Ellipse(TrBgn_x + 20, TrBgn_y + 15, TrBgn_x + 28, TrBgn_y + 23);/////////模拟的列车的形状
		TrBgn_x = TrBgn_x + 5;
		if (TrBgn_x > TrEnd_x)
		{
			Train1 = 0;
		}
	}


	CPen pen;
	pen.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));
	dcMem.SelectObject(&pen);
	dcMem.MoveTo(1770, 125);
	dcMem.LineTo(1800, 80);
	dcMem.MoveTo(1800, 80);
	dcMem.LineTo(1885, 80);
	pen.DeleteObject();



for (int i = 0; i < 20; i++)
{
	SetSigClr(RD.m_csignal[i].ID);
	RD.m_csignal[i].DrawSignal(&dcMem);
}
for (int i = 0; i < 20; i++)
{
	SetState();
	RD.m_cblocksec[i].DrawBS(&dcMem);
}

pDC->BitBlt(0, 0, rc.Width()*2, rc.Height(), &dcMem, 0, 0, SRCCOPY);

//将内存DC上的图象拷贝到前台
	//绘图完成后的清理
dcMem.DeleteDC();     //删除DC
bmp.DeleteObject(); //删除位图
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


void StationYardView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Invalidate(false);
	CScrollView::OnTimer(nIDEvent);
}


void StationYardView::OnInitialUpdate()//
{
	CScrollView::OnInitialUpdate();
	CClientDC dc(this);
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 2500;
	SetScrollSizes(MM_TEXT, sizeTotal);
	//CView::OnInitialUpdate();
	SetTimer(1, 100, NULL);
		
	// TODO: 在此添加专用代码和/或调用基类
}

void StationYardView::SetSigClr(int SigID)
{
	for (int i = 0; i < 20; i++)
	{
		if (SigID == RD.m_csignal[i].ID)
		{
			if (RD.m_csignal[i].Attr == 1)
			{
				if (GetState(RD.m_csignal[i].ProBS) == 3)
					RD.m_csignal[i].Color = 4;
				else if (GetState(RD.m_csignal[i].ProBS) == 2)
					RD.m_csignal[i].Color = 4;
				else if (GetSigClr(RD.m_csignal[i].NextSig) == 1)
					RD.m_csignal[i].Color = 1;
				else if (GetSigClr(RD.m_csignal[i].NextSig) == 2)
					RD.m_csignal[i].Color = 1;
				else if (GetSigClr(RD.m_csignal[i].NextSig) == 3)
					RD.m_csignal[i].Color = 2;
				else if (GetSigClr(RD.m_csignal[i].NextSig) == 4)
					RD.m_csignal[i].Color = 3;
			}
			else if (RD.m_csignal[i].Attr == 3)
			{
				if ((TrBgn_x + 30) > RD.m_csignal[i].x1)
					RD.m_csignal[i].Color = 4;
			}
			
		}
	}
}

int StationYardView::GetSigClr(int SigID)
{
	for (int i = 0; i < 20; i++)
	{
		if (SigID == RD.m_csignal[i].ID)
		{
			return RD.m_csignal[i].Color;
			break;
		}
			
	}
	//return 0;
}

void StationYardView::SetState()
{
	for (int i = 0; i < 20; i++)
	{
		if ((RD.m_cblocksec[i].State == 3))
			RD.m_cblocksec[i].State = 3;
		else if ((RD.m_cblocksec[i].x1 > (TrBgn_x + 30))|| (RD.m_cblocksec[i].x2 < TrBgn_x))
			RD.m_cblocksec[i].State = 1;
		else
			RD.m_cblocksec[i].State = 2;
	}
}

int StationYardView::GetState(int BSID)
{
	for (int i = 0; i < 20; i++)
	{
		if (BSID == RD.m_cblocksec[i].ID)
		{
			return RD.m_cblocksec[i].State;
			break;
		}
	}
}


