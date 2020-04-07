// DMIView.cpp: 实现文件
//

#include "pch.h"
#include "OverSpeedPro.h"
#include "DMIView.h"
#include<vector>
#include<math.h>

// DMIView
using namespace std;
IMPLEMENT_DYNCREATE(DMIView, CView)

DMIView::DMIView()
{

}

DMIView::~DMIView()
{
}

BEGIN_MESSAGE_MAP(DMIView, CView)
END_MESSAGE_MAP()


// DMIView 绘图

void DMIView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码

	CRect rect;
	GetClientRect(&rect);
	DrawXY(pDC, 560, 200, CPoint(50, 300), 3000, 250);
	Draw_EB_Curve(3000);
	Draw_EB_Chufa_Curve(3000);
	CPoint p;
	p.x = 1200;
	p.y = 300;
	Draw_Dashboard(CPoint(1200,300),200);
}

//坐标系绘制
void DMIView::DrawXY(CDC* pDC, int long_x, int long_y, CPoint start, double MaxX, double MaxY)
{
	//存储坐标轴信息
	this->long_x = long_x;
	this->long_y = long_y;
	this->start = start;
	this->MaxX = MaxX;
	this->MaxY = MaxY;
	CPen newPen(BS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&newPen);
	CPoint EndX, EndY;
	EndX.y = start.y;
	EndX.x = start.x + long_x + 50;
	EndY.x = start.x;
	EndY.y = start.y - long_y - 50;
	//绘制坐标轴
	//X轴
	pDC->MoveTo(start);
	pDC->LineTo(EndX);
	//Y轴
	pDC->MoveTo(start);
	pDC->LineTo(EndY);
	//绘制X轴箭头
	pDC->MoveTo(EndX);
	pDC->LineTo(EndX.x - 10, EndX.y - 10);
	pDC->MoveTo(EndX);
	pDC->LineTo(EndX.x - 10, EndX.y + 10);
	//绘制Y轴箭头
	pDC->MoveTo(EndY);
	pDC->LineTo(EndY.x - 10, EndY.y + 10);
	pDC->MoveTo(EndY);
	pDC->LineTo(EndY.x + 10, EndY.y + 10);
	//绘制X轴刻度线 绘制文字
	CFont font;
	font.CreatePointFont(70, _T("宋体"), NULL);
	CFont* oldFont = pDC->SelectObject(&font);
	double step = long_x / 10;//分成10刻度（不包括终点）
	for (int i = 0; i < 11; i++)
	{
		CPen pen1(PS_DASHDOT, 1, RGB(0, 0, 0));
		CPen* oldpen = pDC->SelectObject(&pen1);
		double step2 = MaxX / 10;
		CString str;
		str.Format(_T("%.2f"), i * step2);
		pDC->TextOutW(start.x + step * i - 1.5, start.y + 1.5, str);
		pDC->MoveTo(start.x + step * i, start.y);
		pDC->LineTo(start.x + step * i, start.y - long_y);
		pDC->SelectObject(oldpen);
	}


	//绘制Y轴刻度线 绘制文字
	step = long_y / 10;
	for (int i = 0; i < 11; i++)
	{
		CPen pen1(PS_DASHDOT, 1, RGB(0, 0, 0));
		CPen* oldpen = pDC->SelectObject(&pen1);
		double step2 = MaxY / 10;
		CString str;
		str.Format(_T("%.2f"), i * step2);
		pDC->TextOutW(start.x - 30, start.y - step * i - 5, str);
		pDC->MoveTo(start.x, start.y - step * i);
		pDC->LineTo(start.x + long_x, start.y - step * i);
		pDC->SelectObject(oldpen);
	}
	/*
	//通过函数Draw_EB_Curve实现
	
	//计算n个点的函数值
	double n = 200;
	vector<pair<double, double>> point;
	for (double i = 0; i < n; i++)
	{
		double step3 = 1;//速度步长
		point.push_back(pair<double, double>(i * step3, Function(i * step3)));
	}
	point.push_back(pair<double, double>(200, Function(200) - 300));//200km/h制动距离上预留300m的顶棚速度防护区域
	

	//vector<pair<double, double>> point = EB_Curve(3000);//目标点为3000m
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(255,0,0));
	oldPen = pDC->SelectObject(&newPen2);
	//将点集point连接起来
	//Draw_X(pDC, p[0]);图上标点
	pDC->MoveTo(p[0]);
	for (int i = 1; i < point.size(); i++)
	{
		//Draw_X(pDC, p[i]);
		pDC->LineTo(p[i]);
		pDC->MoveTo(p[i]);
	}
	pDC->LineTo(p[point.size() - 1]);
	pDC->SelectObject(oldPen);
	*/
}


void DMIView::Draw_X(CDC* pDC, CPoint p)
{
	CPen newpen(BS_SOLID, 2, RGB(255, 0, 0));
	CPen* oldpen = pDC->SelectObject(&newpen);
	pDC->MoveTo(p.x - 2, p.y - 2);
	pDC->LineTo(p.x + 2, p.y + 2);
	pDC->MoveTo(p.x + 2, p.y - 2);
	pDC->LineTo(p.x - 2, p.y + 2);
}

//制动
double DMIView::EB_Breaking_Acc(double v)
{
	double a;
	if (v < 0)
		a = 0;
	else if (v <= 70)
		a = 1.1222;
	else if (v <= 210)
	{
		double a5 = 0.000000000043466 * pow(v, 5);
		double a4 = -0.000000030563084 * pow(v, 4);
		double a3 = 0.000008259831998 * pow(v, 3);
		double a2 = -0.001056295336669 * pow(v, 2);
		double a1 = 0.059985451595838 * pow(v, 1);
		double a0 = -0.073919969590845 * pow(v, 0);
		a = a5 + a4 + a3 + a2 + a1 + a0;
	}
	else
		a = 0;
	return a;
}

//基本阻力
double DMIView::Basic_Drag_Acc(double v)
{
	double a= (0.00112 * pow(v, 2) + 0.07295 * pow(v, 1) + 8.63) / 1000.0;
	return	a;
}

//附加阻力
double DMIView::Add_Drag_Acc(double v)
{
	return 0;//暂留
}

//总减速度
double DMIView::GetAcc(double v)
{
	double a = EB_Breaking_Acc(v) + Basic_Drag_Acc(v) + Add_Drag_Acc(v);
	//保留4位小数;
	return a;
}
double DMIView::EB_Distance(double v1, double v2)
{
	int k = (int)(v1 - v2);
	double S = 0;
	for (int i = 0; i < k; i++)
	{
		double a_new = (GetAcc(v1) + GetAcc(v2)) / 2;
		double s_new = ((v1 - i) * (v1 - i) - (v1 - i - 1) * (v1 - i - 1)) / ((2.0 * (12.96 / 1.08)) * a_new); // 回转系数考虑0.08
		S = S + s_new;
	}
	//保留6位小数
	//return floor(S * 100000000.000f + 0.5) / 100000000.000f;
	return S+100;
}
void DMIView::Draw_EB_Curve(double target)
{	
	CDC* pDC = CWnd::GetDC();
	//计算n个点的函数值
	double n = 200;
	vector<pair<double, double>> point;
	for (double i = 0; i < n; i++)
	{
		double step3 = 1;//速度步长
		point.push_back(pair<double, double>(i * step3, target-EB_Distance(i,0)));
	}
	point.push_back(pair<double, double>(200, target-EB_Distance(200,0)-300.0));//200km/h制动距离上预留300m的顶棚速度防护区域


	//vector<pair<double, double>> point = EB_Curve(3000);//目标点为3000m
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(255, 0, 0));
	CPen *oldPen = pDC->SelectObject(&newPen2);
	//将点集point连接起来
	//Draw_X(pDC, p[0]);图上标点
	pDC->MoveTo(p[0]);
	for (int i = 1; i < point.size(); i++)
	{
		//Draw_X(pDC, p[i]);
		pDC->LineTo(p[i]);
		pDC->MoveTo(p[i]);
	}
	pDC->LineTo(p[point.size() - 1]);
	pDC->SelectObject(oldPen);
}
double DMIView::EB_Distance_chufa(double v1, double v2)
{
	double a_max = 0.42;//失控加速时考虑的牵引加速度 
	double tc = 0.8;
	double td = 1.5;
	double tefg = 2.8;//安全制动模型各阶段时间
	double Sc = (v1*tc) / 3.6;
	double Sd = (v1 / 3.6) * td + 0.5 * a_max * td * td;
	double Vend = v1 + a_max * td * 3.6;
	double Sefg = (Vend * tefg) / 3.6;
	double S = Sc + Sd + Sefg + EB_Distance(Vend, v2);
	return S;
}
void DMIView::Draw_EB_Chufa_Curve(double target)
{
	CDC* pDC = CWnd::GetDC();
	//计算n个点的函数值
	double n = 200;
	vector<pair<double, double>> point;
	for (double i = 0; i < n; i++)
	{
		double step3 = 1;//速度步长
		point.push_back(pair<double, double>(i * step3, target - EB_Distance_chufa(i, 0)));
	}
	point.push_back(pair<double, double>(200, target - EB_Distance_chufa(200, 0) - 300.0));//200km/h制动距离上预留300m的顶棚速度防护区域


	//vector<pair<double, double>> point = EB_Curve(3000);//目标点为3000m
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(0,0,255));
	CPen* oldPen = pDC->SelectObject(&newPen2);
	//将点集point连接起来
	//Draw_X(pDC, p[0]);图上标点
	pDC->MoveTo(p[0]);
	for (int i = 1; i < point.size(); i++)
	{
		//Draw_X(pDC, p[i]);
		pDC->LineTo(p[i]);
		pDC->MoveTo(p[i]);
	}
	pDC->LineTo(p[point.size() - 1]);
	pDC->SelectObject(oldPen);
}

void DMIView::Draw_Dashboard(CPoint center,double r)
{
	double v;
	v = 120;
	CDC* pDC = CWnd::GetDC();
	CPen newpen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen = pDC->SelectObject(&newpen);
	CRect rec;
	rec.left = center.x - r;
	rec.right = center.x + r;
	rec.top = center.y - r;
	rec.bottom = center.y + r;
	//pDC->Rectangle(rec);

	double PI = acos(-1);
	CPoint p1;
	CPoint p2;
	double arc;
	p1.x = center.x + r * cos((-35.0 / 180.0) * PI);
	p1.y = center.y - r * sin((-35.0 / 180.0) * PI);
	p2.x = center.x + r * cos((215.0 / 180.0) * PI);
	p2.y = center.y - r * sin((215.0 / 180.0) * PI);
	CPen newpen2(PS_DASHDOTDOT, 0.5, RGB(0, 0, 0));
	oldpen = pDC->SelectObject(&newpen2);
	//刻度盘外沿
	pDC->Arc(rec, p1, p2);
	
	pDC->SelectObject(oldpen);
	//分刻度
	for (int i = 0; i < 251; i++)
	{
		if (i % 10 == 0)
		{
			int a = 10;
			if (i % 50 == 0)
				a = 30;
			arc = -35.0 + i;
			CPoint p1;
			CPoint p2;
			p1.x = center.x + (r - a) * cos((arc / 180.0) * PI);
			p1.y = center.y - (r - a) * sin((arc / 180.0) * PI);
			p2.x = center.x + r * cos((arc / 180.0) * PI);
			p2.y = center.y - r * sin((arc / 180.0) * PI);
			pDC->MoveTo(p1);
			pDC->LineTo(p2);
			if (i % 50 == 0)
			{
				double x = center.x + (r - a - 15) * cos((arc / 180.0) * PI);
				double y = center.y - (r - a - 15) * sin((arc / 180.0) * PI);

				CString str;
				str.Format(_T("%d"),250-i);
				pDC->TextOutW(x, y, str);
			}
		}
	}
	//指针
	CPoint p_1;
	CPoint p_2;
	CPoint p_3;
	p_1.x = center.x + (r-40) * cos(((215.0-v) / 180.0) * PI);
	p_1.y = center.y - (r-40) * sin(((215.0 - v) / 180.0) * PI);
	p_2.x = center.x + 30 * cos(((215.0 - v-10) / 180.0) * PI);
	p_2.y = center.y - 30 * sin(((215.0 - v-10) / 180.0) * PI);
	p_3.x = center.x + 30 * cos(((215.0 - v+10) / 180.0) * PI);
	p_3.y = center.y - 30 * sin(((215.0 - v+10) / 180.0) * PI);
	pDC->Ellipse(center.x - 30, center.y - 30, center.x + 30, center.y + 30);
	CString str;
	str.Format(_T("%.0f"), v);
	pDC->TextOutW(center.x-12, center.y-7, str);
	pDC->MoveTo(p_2);
	pDC->LineTo(p_1);
	pDC->MoveTo(p_3);
	pDC->LineTo(p_1);
	pDC->SelectObject(oldpen);
	
}

double DMIView::US_Distance(double v1, double v2)
{
	return 0;
}
// DMIView 诊断

#ifdef _DEBUG
void DMIView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void DMIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// DMIView 消息处理程序
