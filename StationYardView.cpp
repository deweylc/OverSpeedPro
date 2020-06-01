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
#define safe_dis 100
//#include"CScrollView.h"
// StationYardView


IMPLEMENT_DYNCREATE(StationYardView, CScrollView)
int Train = 0;//用于确定列车是否显示
int Train1 = 1;//用于确定列车是否出发
/*翻墙成功*/
// C电路图View
int TrBgn_x, TrBgn_y = 107;//列车出发点的坐标
int TrNow_x, TrNow_y = TrBgn_y;//列车当前位置的坐标
int TrEnd_x, TrEnd_y = TrNow_y;//列车停车点的坐标
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
	ON_COMMAND(ID_1_32772, &StationYardView::On132772)
	ON_COMMAND(ID_1_32773, &StationYardView::On132773)
	ON_BN_CLICKED(IDC_BUTTON1, On01)//正线发车
	ON_BN_CLICKED(IDC_BUTTON2, On02)//正线接车
	ON_BN_CLICKED(IDC_BUTTON3, On03)//列车复位

	ON_WM_RBUTTONDOWN()
	//	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_COMMAND(ID_1_32775, &StationYardView::On132775)
	ON_COMMAND(ID_1_32776, &StationYardView::On132776)
END_MESSAGE_MAP()

// StationYardView 绘图

void StationYardView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rc;
	CDC dcMem;
	GetClientRect(&rc);
	CBitmap bmp; //内存中承载临时图象的位图

	dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(pDC, rc.Width() * 2, rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色
	dcMem.FillSolidRect(rc, RGB(0, 0, 0));

	//下面很简单的实现了一下列车移动的功能
	if ((Train == 1) && (Train1 == 0))
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));//建立个白色的画刷给内存DC
		dcMem.SelectObject(&brush);///选择这个刷子
		dcMem.Rectangle(TrNow_x - 30, TrNow_y, TrNow_x, TrNow_y + 15);//一个正方形
	}
	else if ((Train == 1) && (Train1 == 1))
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));//建立个白色的画刷给内存DC
		dcMem.SelectObject(&brush);///选择这个刷子
		dcMem.Rectangle(TrNow_x - 30, TrNow_y, TrNow_x, TrNow_y + 15);//一个正方形
		int X = FindBS(TrNow_x);
		DisCount(X);
		pDoc->target = (TrEnd_x - TrBgn_x) * 20 / 3.0 - safe_dis;
		TrNow_x = (pDoc->position) * 3 / 20.0 + TrBgn_x;
		//(pDoc->position)*3/20.0 + TrBgn_x;

		/*if (TrNow_x >= TrEnd_x)
		{
			Train1 = 0;
		}*/
	}
	//车站侧线区段
	CPen pen;
	pen.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));
	dcMem.SelectObject(&pen);
	dcMem.MoveTo(1770, 125);
	dcMem.LineTo(1800, 80);
	dcMem.MoveTo(1800, 80);
	dcMem.LineTo(1885, 80);

	dcMem.MoveTo(150, 125);
	dcMem.LineTo(105, 80);
	dcMem.MoveTo(105, 80);
	dcMem.LineTo(15, 80);

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

	pDC->BitBlt(0, 0, rc.Width() * 2, rc.Height(), &dcMem, 0, 0, SRCCOPY);

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


void StationYardView::On01()//正线发车
{
	Train = 1;
	TrBgn_x = 50;
	TrNow_x = TrBgn_x;
	for (int i = 0; i < 20; i++)
	{
		if (RD.m_csignal[i].ID == 1013)
		{
			RD.m_csignal[i].Color = 1;
			break;
		}
	}
	for (int i = 0; i < 20; i++)
	{
		if (RD.m_cblocksec[i].ID == 2014)
		{
			RD.m_cblocksec[i].State = 3;
			break;
		}
	}

}

void StationYardView::On02()//正线接车
{
	for (int i = 0; i < 20; i++)//开放进站信号
	{
		if (RD.m_csignal[i].ID == 1015)
		{
			RD.m_csignal[i].Color = 3;
			break;
		}
	}
	for (int i = 0; i < 20; i++)//办理进路
	{
		if ((RD.m_cblocksec[i].ID == 2015) || (RD.m_cblocksec[i].ID == 2017))
		{
			RD.m_cblocksec[i].State = 3;
		}
	}
}

void StationYardView::On03()
{
	TrBgn_x = 0;
	Train1 = 1;
	TrNow_x = TrBgn_x;
	Train = 0;/*
	RD.dubiaojishu == 0;
	StationYardView();*/
}

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
int StationYardView::FindBS(int x)
{
	if (x <= 80)
	{
		return 1013;
	}
	else
	{
		for (int i = 0; i < 20; i++)
		{
			if ((x > RD.m_cblocksec[i].x1) && (x < RD.m_cblocksec[i].x2))
				return RD.m_cblocksec[i].ProSignal;
		}
	}
}
int Num = 0;
void StationYardView::DisCount(int x)//x是列车所处区段的防护信号机的ID
{
	for (int i = 0; i < 20; i++)
	{
		if (x == RD.m_csignal[i].ID)
		{
			if (TrNow_x < 80)
			{
				if (GetSigClr(RD.m_csignal[i].NextSig) == 4)
				{
					if (Num > 7)
					{
						TrEnd_x = GetSigX(RD.m_csignal[i].ID);
					}
					else
					{
						TrEnd_x = GetSigX(RD.m_csignal[i].NextSig);
					}
					Num = 0;
					//return 0;
				}
				else if (Num >= 7)
				{
					TrEnd_x = GetSigX(RD.m_csignal[i].ID);
					Num = 0;
					//return 0;
				}
				else
				{
					Num++;
					x = RD.m_csignal[i].NextSig;
					DisCount(x);
				}
			}
			else
			{
				if (GetSigClr(RD.m_csignal[i].NextSig) == 4)
				{
					if (Num >= 8)
					{
						TrEnd_x = GetSigX(RD.m_csignal[i].ID);
					}
					else
					{
						TrEnd_x = GetSigX(RD.m_csignal[i].NextSig);
					}
					Num = 0;
					//return 0;
				}
				else if (Num >= 8)
				{
					TrEnd_x = GetSigX(RD.m_csignal[i].ID);
					Num = 0;
					//return 0;
				}
				else
				{
					Num++;
					x = RD.m_csignal[i].NextSig;
					DisCount(x);
				}
			}

			break;
		}
	}
	/*TrEnd_x = GetSigX(x);
	return (TrEnd_x - TrBgn_x);*/

}



void StationYardView::SetSigClr(int SigID)
{
	for (int i = 0; i < 20; i++)
	{
		if (SigID == RD.m_csignal[i].ID)
		{
			if (RD.m_csignal[i].Attr == 1)
			{
				if ((GetState(RD.m_csignal[i].ProBS) == 3) || (GetState(RD.m_csignal[i].ProBS) == 4))
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
			else
			{
				if ((TrNow_x) > RD.m_csignal[i].x1)
					RD.m_csignal[i].Color = 4;
			}

		}
	}
}

int StationYardView::GetSigClr(int SigID)
{
	if (SigID > 1000)
	{
		for (int i = 0; i < 20; i++)
		{
			if (SigID == RD.m_csignal[i].ID)
			{
				return RD.m_csignal[i].Color;
				break;
			}

		}
	}
	else
		return 0;
}

void StationYardView::SetState()
{
	for (int i = 0; i < 20; i++)
	{
		if ((RD.m_cblocksec[i].State == 3))
		{
			if (RD.m_cblocksec[i].Attr == 1)
				RD.m_cblocksec[i].State = 3;
			else if (TrNow_x < 80)
			{
				RD.m_cblocksec[i].State = 3;
			}
			else if (TrNow_x < 1740)
			{
				if ((RD.m_cblocksec[i].ID == 2015) || (RD.m_cblocksec[i].ID == 2017))
				{
					RD.m_cblocksec[i].State = 3;
				}
				else if ((RD.m_cblocksec[i].x1 >= (TrNow_x)) || (RD.m_cblocksec[i].x2 <= (TrNow_x - 30)))
					RD.m_cblocksec[i].State = 1;
				else
					RD.m_cblocksec[i].State = 2;
			}
			else if (TrNow_x < 1840)
			{
				if (RD.m_cblocksec[i].ID == 2017)
				{
					RD.m_cblocksec[i].State = 3;
				}
				else if ((RD.m_cblocksec[i].x1 >= (TrNow_x)) || (RD.m_cblocksec[i].x2 <= (TrNow_x - 30)))
					RD.m_cblocksec[i].State = 1;
				else
					RD.m_cblocksec[i].State = 2;
			}
			else if ((RD.m_cblocksec[i].x1 >= (TrNow_x)) || (RD.m_cblocksec[i].x2 <= (TrNow_x - 30)))
				RD.m_cblocksec[i].State = 1;
			else
				RD.m_cblocksec[i].State = 2;

		}
			else if (RD.m_cblocksec[i].State == 4)//异物侵限
				RD.m_cblocksec[i].State = 4;
			else if ((RD.m_cblocksec[i].x1 >= (TrNow_x)) || (RD.m_cblocksec[i].x2 <= (TrNow_x - 30)))
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

	int StationYardView::GetSigX(int SigID)
	{
		for (int i = 0; i < 20; i++)
		{
			if (SigID == RD.m_csignal[i].ID)
				return RD.m_csignal[i].x1;
		}
	}

	int ID;
	void StationYardView::On132772()//设置轨道区段故障
	{
		// TODO: 在此添加命令处理程序代码
		for (int i = 0; i < 20; i++)
		{
			if (RD.m_cblocksec[i].ID == ID)
			{
				RD.m_cblocksec[i].State = 3;
				break;
			}
		}
	}


	void StationYardView::On132773()//解除轨道区段故障
	{
		// TODO: 在此添加命令处理程序代码
		/*CPoint point;
		GetCursorPos(&point);*/
		for (int i = 0; i < 20; i++)
		{
			if (RD.m_cblocksec[i].ID == ID)
			{
				RD.m_cblocksec[i].State = 1;
				break;
			}

		}
	}


	void StationYardView::OnRButtonDown(UINT nFlags, CPoint p1)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

		for (int i = 0; i < 20; i++)
		{
			if ((p1.x > RD.m_cblocksec[i].x1) && (p1.x < RD.m_cblocksec[i].x2) && (p1.y > (RD.m_cblocksec[i].y1 - 3)) && (p1.y < (RD.m_cblocksec[i].y1 + 3)))
			{
				CMenu menu;
				CMenu* pSubMenu;
				menu.LoadMenu(IDR_MENU4);
				pSubMenu = menu.GetSubMenu(0);
				GetCursorPos(&p1);
				pSubMenu->TrackPopupMenu(TPM_CENTERALIGN, p1.x, p1.y, this);
				//RD.m_cblocksec[i].State = 3;
				ID = RD.m_cblocksec[i].ID;
				break;
			}

		}

		CScrollView::OnRButtonDown(nFlags, p1);
	}



	int StationYardView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CScrollView::OnCreate(lpCreateStruct) == -1)
			return -1;
		m_button1.Create(_T("正线发车"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, 10, 110, 30), this, IDC_BUTTON1);
		m_button2.Create(_T("正线接车"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1800, 10, 1910, 30), this, IDC_BUTTON2);
		m_button3.Create(_T("列车复位"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1800, 45, 1910, 65), this, IDC_BUTTON3);
		// TODO:  在此添加您专用的创建代码

		return 0;
	}



	void StationYardView::On132775()//设置异物侵限
	{
		// TODO: 在此添加命令处理程序代码
		for (int i = 0; i < 20; i++)
		{
			if (RD.m_cblocksec[i].ID == ID)
			{
				RD.m_cblocksec[i].State = 4;
				break;
			}

		}
	}


	void StationYardView::On132776()//解除异物侵限
	{
		// TODO: 在此添加命令处理程序代码
		for (int i = 0; i < 20; i++)
		{
			if (RD.m_cblocksec[i].ID == ID)
			{
				RD.m_cblocksec[i].State = 1;
				break;
			}

		}
	}
