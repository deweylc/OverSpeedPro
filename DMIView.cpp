// DMIView.cpp: 实现文件
//


#include "pch.h"
#include "OverSpeedPro.h"
#include "DMIView.h"
#include<vector>
#include"OverSpeedProDoc.h"
#include<math.h>
#include "framework.h"
#include<string>
#include"MainFrm.h"
#include<algorithm>

// DMIView
using namespace std;
IMPLEMENT_DYNCREATE(DMIView, CView)

DMIView::DMIView()
{
	long_x = 0;
	long_y = 0;
	start.x = 0;
	start.y = 0;
	MaxX = 0;
	MaxY = 0;
}

DMIView::~DMIView()
{
}

BEGIN_MESSAGE_MAP(DMIView, CView)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL DMIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}
// DMIView 绘图

void DMIView::OnDraw(CDC* pDC)
{
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO:  在此添加绘制代码
	
	//双缓冲绘图
	CDC MemDC;//新建内存DC
	CBitmap MemBitmap;//新建位图
	CRect memRc;

	GetClientRect(&memRc);//获得客户区矩形大小

	MemDC.CreateCompatibleDC(pDC);//创建与目标DC兼容的内存DC
	MemBitmap.CreateCompatibleBitmap(pDC, memRc.Width(), memRc.Height());//根据目标DC创建相应的位图
	MemDC.SelectObject(&MemBitmap);//把位图选入内存DC

	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));//为内存DC创建画刷
	MemDC.SelectObject(&brush);//将画刷选入内存DC
	CBitmap* pOldBit = MemDC.SelectObject(&MemBitmap);

	//先用背景色将位图清理干净，这里用黑色
	MemDC.FillSolidRect(memRc.left, memRc.top, memRc.Width(), memRc.Height(), RGB(0,0,0));
	////

	/*以下是程序的相关操作*/

	DrawXY(&MemDC, 660, 300, CPoint(50, 400), 15000, 250);
	//Draw_OverSpeedCurve(1600, 3800);
	//Draw_EB_Curve(&MemDC,4000, 0);//紧急制动
	//Draw_EB_Chufa_Curve(&MemDC,10000);//紧急制动触发
	//Draw_SB_Curve(&MemDC,4000);//常用制动
	//Draw_EB_Curve(&MemDC,3200, 4000, 0, 200);

	Draw_EB_Curve_new(&MemDC, pDoc->target, pDoc->position);
	Draw_EB_Chufa_Curve_new(&MemDC, pDoc->target, pDoc->position);
	Draw_SB_Curve_new(&MemDC, pDoc->target, pDoc->position);
//	Draw_Dashboard(CPoint(1100,300),160);
	DrawInfoTable(&MemDC);


	////
	pDC->BitBlt(memRc.left, memRc.top, memRc.Width(), memRc.Height(), &MemDC, 0, 0, SRCCOPY);//将内存DC上的全部内容放入pDC,这样才会在显示窗口上显示出来
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	SetTimer(1, 50, NULL);
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
	CPen newPen(BS_SOLID, 2, RGB(255, 255, 255));
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
	pDC->SetTextColor(RGB(255,255,255));
	CFont* oldFont = pDC->SelectObject(&font);
	double step = long_x / 10;//分成10刻度（不包括终点）
	for (int i = 0; i < 11; i++)
	{
		CPen pen1(PS_DASHDOT, 1, RGB(255, 255, 255));
		CPen* oldpen = pDC->SelectObject(&pen1);
		double step2 = MaxX / 10;
		CString str;
		str.Format(_T("%.2f"), i * step2);
		
		pDC->TextOutA(start.x + step * i - 1.5, start.y + 1.5, str);
		pDC->MoveTo(start.x + step * i, start.y);
		pDC->LineTo(start.x + step * i, start.y - long_y);
		pDC->SelectObject(oldpen);
	}


	//绘制Y轴刻度线 绘制文字
	step = long_y / 10;
	for (int i = 0; i < 11; i++)
	{
		CPen pen1(PS_DASHDOT, 1, RGB(255, 255, 255));
		CPen* oldpen = pDC->SelectObject(&pen1);
		double step2 = MaxY / 10;
		CString str;
		str.Format(_T("%.2f"), i * step2);
		pDC->TextOutA(start.x - 30, start.y - step * i - 5, str);
		pDC->MoveTo(start.x, start.y - step * i);
		pDC->LineTo(start.x + long_x, start.y - step * i);
		pDC->SelectObject(oldpen);
	}
	pDC->SelectObject(oldFont);
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
	else if (v <= 250)
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
double DMIView::SB_Breaking_Acc(double v)
{
	double a;
	if (v < 0)
		a = 0;
	else if (v <= 70)
		a = 0.7472;
	else if (v <= 250)
	{
		double a5 = 0.000000000029217 * pow(v, 5);
		double a4 = -0.000000020548613 * pow(v, 4);
		double a3 = 0.000005555126797 * pow(v, 3);
		double a2 = -0.000710742552015 * pow(v, 2);
		double a1 = 0.040413892468555 * pow(v, 1);
		double a0 = -0.060695538869822 * pow(v, 0);
		a = a5 + a4 + a3 + a2 + a1 + a0;
	}
	else
		a = 0;
	return a;
}
//基本阻力
double DMIView::Basic_Drag_Acc(double v)
{
	double a = (0.00112 * pow(v, 2) + 0.07295 * pow(v, 1) + 8.63) / 1000.0;
	return	a;
}

//附加阻力
double DMIView::Add_Drag_Acc(double v)
{
	return 0;//暂留
}

//总减速度
double DMIView::GetAcc_EB(double v)
{
	double a = EB_Breaking_Acc(v) + Basic_Drag_Acc(v) + Add_Drag_Acc(v);

	return a;
}
double DMIView::GetAcc_SB(double v)
{
	double a = SB_Breaking_Acc(v) + Basic_Drag_Acc(v) + Add_Drag_Acc(v);
	return a;
}
double DMIView::EB_Distance(double v1, double v2)
{
	int k = (int)(v1 - v2);
	double S = 0;
	for (int i = 0; i < k; i++)
	{
		double a_new = (GetAcc_EB(v1) + GetAcc_EB(v2)) / 2;
		double s_new = ((v1 - i) * (v1 - i) - (v1 - i - 1) * (v1 - i - 1)) / ((2.0 * (12.96 / 1.08)) * a_new); // 回转系数考虑0.08
		S = S + s_new;
	}
	
	//保留6位小数
	//return floor(S * 100000000.000f + 0.5) / 100000000.000f;
	return S;
}
double DMIView::SB_Distance(double v1, double v2)
{
	int k = (int)(v1 - v2);
	double S = 0;
	S = (v1 * 3.05) / 3.6;
	for (int i = 0; i < k; i++)
	{
		double a_new = (GetAcc_SB(v1) + GetAcc_SB(v2)) / 2;
		double s_new = ((v1 - i) * (v1 - i) - (v1 - i - 1) * (v1 - i - 1)) / ((2.0 * (12.96 / 1.08)) * a_new); // 回转系数考虑0.08
		S = S + s_new;
	}
	

	double S_c;
	S_c = (v1 * 2.3) / 3.6 + EB_Distance_chufa(v1, v2);
	return S;
}
CPoint* DMIView::Draw_EB_Curve(CDC *pDC,double target, double target_v)
{
	
	//计算n个点的函数值
	double n = 200 - target_v;
	vector<pair<double, double>> point;
	for (double i = target_v; i < 200; i++)
	{
		double step3 = 1;//速度步长
		point.push_back(pair<double, double>(i * step3, target - EB_Distance(i, target_v)));
	}
	point.push_back(pair<double, double>(200, target - EB_Distance(200, target_v) - 0.0));//200km/h制动距离上预留#m的顶棚速度防护区域


	//vector<pair<double, double>> point = EB_Curve(3000);//目标点为3000m

	static CPoint res[2];
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
		if (i == point.size() - 1)
			res[0] = p[i];
		if (i == 0)
			res[1] = p[i];
	}
	CPen newPen2(BS_SOLID, 2, RGB(255, 0, 0));
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
	return res;
}
CPoint* DMIView::Draw_EB_Curve(CDC* pDC,double target1, double target2, double target_v, double start_v)
{
	

	vector<pair<double, double>> point;
	double end_v = target_v;
	if (target_v > start_v)
		end_v = start_v;
	for (double i = end_v; i < 200; i++)
	{
		double step3 = 1;//速度步长
		double s = target2 - EB_Distance(i, end_v);
		if (s < target1)
		{
			double v = i;
			if (v > start_v)
				v = start_v;
			else v--;
			point.push_back(pair<double, double>(v, target1));
			break;
		}

		double v = i;
		if (v > start_v)
			v = start_v;
		point.push_back(pair<double, double>(v, s));
	}
	if (point.back().second > target1)
		point.push_back(pair<double, double>(200, target1));



	static CPoint res[2];
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
		if (i == point.size() - 1)
			res[0] = p[i];
		if (i == 0)
			res[1] = p[i];
	}

	CPen newPen2(BS_SOLID, 2, RGB(255, 0, 0));
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
	return res;
}
void DMIView::Draw_SB_Curve(CDC* pDC,double target)
{
	
	//计算n个点的函数值
	double n = 200;
	vector<pair<double, double>> point;
	for (double i = 0; i < n; i++)
	{
		double step3 = 1;//速度步长
		point.push_back(pair<double, double>(i * step3, target - SB_Distance(i, 0)));
	}
	point.push_back(pair<double, double>(200, target - SB_Distance(200, 0) - 300.0));//200km/h制动距离上预留300m的顶棚速度防护区域


	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(0, 255, 0));
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

double DMIView::EB_Distance_chufa(double v1, double v2)
{
	double a_max = 0.42;//失控加速时考虑的牵引加速度 
	double tc = 0.75;
	double td = 0.4;
	double tefg = 1.9;//安全制动模型各阶段时间
	double Sc = (v1 * tc) / 3.6;
	double Sd = (v1 / 3.6) * td + 0.5 * a_max * td * td;
	double Vend = v1 + a_max * td * 3.6;
	double Sefg = (Vend * tefg) / 3.6;
	double S = Sc + Sd + Sefg + EB_Distance(Vend, v2);
	return S;
}
void DMIView::Draw_EB_Chufa_Curve(CDC* pDC,double target)
{
	
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
	CPen newPen2(BS_SOLID, 2, RGB(0, 0, 255));
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

void DMIView::Draw_Dashboard(CDC* pDC,CPoint center, double r)
{
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	double v;
	v = pDoc->speed;
	
	CPen newpen(PS_SOLID, 2, RGB(255, 255, 255));
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
	CPen newpen2(PS_DASHDOTDOT, 0.5, RGB(255, 255, 255));
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
				str.Format(_T("%d"), 250 - i);
				pDC->TextOutA(x, y, str);
			}
		}
	}
	//指针
	CPoint p_1;
	CPoint p_2;
	CPoint p_3;
	p_1.x = center.x + (r - 40) * cos(((215.0 - v) / 180.0) * PI);
	p_1.y = center.y - (r - 40) * sin(((215.0 - v) / 180.0) * PI);
	p_2.x = center.x + 30 * cos(((215.0 - v - 10) / 180.0) * PI);
	p_2.y = center.y - 30 * sin(((215.0 - v - 10) / 180.0) * PI);
	p_3.x = center.x + 30 * cos(((215.0 - v + 10) / 180.0) * PI);
	p_3.y = center.y - 30 * sin(((215.0 - v + 10) / 180.0) * PI);
	pDC->Ellipse(center.x - 30, center.y - 30, center.x + 30, center.y + 30);
	CString str;
	str.Format(_T("%.0f"), v);
	pDC->TextOutA(center.x - 12, center.y - 7, str);
	pDC->MoveTo(p_2);
	pDC->LineTo(p_1);
	pDC->MoveTo(p_3);
	pDC->LineTo(p_1);
	pDC->SelectObject(oldpen);

}

void DMIView::Draw_OverSpeedCurve(CDC* pDC,double position, double target)
{
	
	//获取临时限速信息
	vector<pair<pair<double, double>, pair<double, double>>> ssp;//限速值,入口速度，出口速度，限速区间
	pair<double, double> p1(100, 160);
	pair<double, double> p2(1000, 1500);
	ssp.push_back(pair < pair<double, double>, pair<double, double>>(p1, p2));
	pair<double, double> p3(160, 200);
	pair<double, double> p4(1500, 2000);
	ssp.push_back(pair < pair<double, double>, pair<double, double>>(p3, p4));

	if (ssp.size() == 0)
	{
		Draw_EB_Curve(pDC,target, 0);
		return;
	}

	//画出限速区域
	CPoint point1;
	CPoint point2;
	point1.x = (ssp[0].second.first / MaxX) * long_x + start.x;
	point1.y = start.y - (ssp[0].first.first / MaxY) * long_y;
	point2.x = (ssp[0].second.second / MaxX) * long_x + start.x;
	point2.y = start.y - (ssp[0].first.first / MaxY) * long_y;
	CPoint point3;
	CPoint point4;
	point3.x = (ssp[1].second.first / MaxX) * long_x + start.x;
	point3.y = start.y - (ssp[1].first.first / MaxY) * long_y;
	point4.x = (ssp[1].second.second / MaxX) * long_x + start.x;
	point4.y = start.y - (ssp[1].first.first / MaxY) * long_y;


	
	CPen pen1(PS_SOLID, 2, RGB(0, 255, 0));
	CPen* oldpen1 = pDC->SelectObject(&pen1);
	pDC->MoveTo(point1);
	pDC->LineTo(point2);
	pDC->MoveTo(point3);
	pDC->LineTo(point4);
	pDC->SelectObject(oldpen1);

	pair<pair<double, double>, pair<double, double>> ssp_1;
	CPoint* s_e;


	vector<pair<CPoint, CPoint>> vec;
	//限速区1
	ssp_1 = ssp[0];
	//限速区的超速防护速度小于限速值？
	if (EB_Distance(ssp_1.first.first, 0) > target - ssp_1.second.first)
	{
		Draw_EB_Curve(pDC,target, 0);

	}
	else if (EB_Distance(ssp_1.first.first, ssp_1.first.second) > ssp_1.second.second - ssp_1.second.first)
	{
		double a = EB_Distance(ssp_1.first.first, ssp_1.first.second);
		double b = ssp_1.second.second - ssp_1.second.first;
		Draw_EB_Curve(pDC,ssp_1.second.second, ssp_1.first.second);
	}
	else
	{
		Draw_EB_Curve(pDC,position, ssp_1.second.first, ssp_1.first.first, 200);
		s_e = Draw_EB_Curve(pDC,ssp_1.second.first, ssp_1.second.second, ssp_1.first.second, ssp_1.first.first);
		vec.push_back(pair<CPoint, CPoint>(s_e[0], s_e[1]));
	}

	//限速区2	
	ssp_1 = ssp[1];
	//限速区的超速防护速度小于限速值？
	if (EB_Distance(ssp_1.first.first, 0) > target - ssp_1.second.first)
	{
		s_e = Draw_EB_Curve(pDC,ssp_1.second.first, target, 0.0, ssp_1.first.first);
		vec.push_back(pair<CPoint, CPoint>(s_e[0], s_e[1]));
	}
	else
	{

		if (EB_Distance(ssp_1.first.first, 0) > target - ssp_1.second.second)
		{
			s_e = Draw_EB_Curve(pDC,ssp_1.second.first, target, 0, ssp_1.first.first);
			vec.push_back(pair<CPoint, CPoint>(s_e[0], s_e[1]));
		}
		else
		{
			s_e = Draw_EB_Curve(pDC,ssp_1.second.first, ssp_1.second.second, ssp_1.first.second, ssp_1.first.first);
			vec.push_back(pair<CPoint, CPoint>(s_e[0], s_e[1]));
			s_e = Draw_EB_Curve(pDC,ssp_1.second.second, target, 0, ssp_1.first.second);
			vec.push_back(pair<CPoint, CPoint>(s_e[0], s_e[1]));
		}

	}


	//分段曲线连接
	CPen newPen2(BS_SOLID, 2, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&newPen2);
	for (int i = 1; i < vec.size(); i++)
	{
		pDC->MoveTo(vec[i - 1].second);
		pDC->LineTo(vec[i].first);
	}
	pDC->SelectObject(oldPen);
	//Draw_EB_Curve(target, 0);
	return;


}
void DMIView::DrawInfoTable(CDC *dc)
{
	COverSpeedProDoc* pDoc =(COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPoint p;
	p.x = 850;
	p.y = 100;
	int length = 900;
	int width = length/3.0;
	CPoint aPoint[5];
	aPoint[0] = p;
	aPoint[4] = p;
	aPoint[1].x = p.x;
	aPoint[1].y = p.y + width;
	aPoint[2].x = p.x + length;
	aPoint[2].y = p.y + width;
	aPoint[3].x = p.x + length;
	aPoint[3].y = p.y;

	CPen newpen(PS_SOLID, 2, RGB(255, 255, 255));
	CPen* oldpen = dc->SelectObject(&newpen);
	//POINT aPoint[5] = { 10,500,10,820,910,820,910,500,10,500 };
	dc->Polyline(aPoint, 5);
	
	CPoint cow[8];
	cow[0].x = p.x + length / 3.0;
	cow[0].y = p.y;
	cow[1].x = p.x + length / 3.0;
	cow[1].y = p.y + width;

	cow[2].x = p.x + length / 2.0;
	cow[2].y = p.y;
	cow[3].x = p.x + length / 2.0;
	cow[3].y = p.y + width;

	cow[4].x = p.x + ((double)length * 2) / 3.0;
	cow[4].y = p.y;
	cow[5].x = p.x + ((double)length * 2) / 3.0;
	cow[5].y = p.y + width;

	cow[6].x = p.x + ((double)length * 5) / 6.0;
	cow[6].y = p.y;
	cow[7].x = p.x + ((double)length * 5) / 6.0;
	cow[7].y = p.y + width;

	for (int i = 0; i < 8; )
	{
		dc->MoveTo(cow[i]);
		dc->LineTo(cow[i + 1]);
		i = i + 2;
	}

	
	/*
	dc->MoveTo(310, 500);
	dc->LineTo(310, 820);
	dc->MoveTo(460, 500);
	dc->LineTo(460, 820);
	dc->MoveTo(610, 500);
	dc->LineTo(610, 820);//列	
	*/


	CPoint row[6];

	row[0].x = cow[0].x;
	row[0].y = p.y + width / 4.0;
	row[1].x = p.x + length;
	row[1].y = p.y + width / 4.0;

	row[2].x = cow[0].x;
	row[2].y = p.y + (2 * (double)width) / 4.0;
	row[3].x = p.x + length;
	row[3].y = p.y + (2 * (double)width) / 4.0;

	row[4].x = p.x;
	row[4].y = p.y + ((double)width * 3) / 4.0;
	row[5].x = p.x + length;
	row[5].y = p.y + ((double)width * 3) / 4.0;

	for (int i = 0; i < 6; )
	{
		dc->MoveTo(row[i]);
		dc->LineTo(row[i + 1]);
		i = i + 2;
	}

	dc->MoveTo(p.x, p.y + ((double)width * 7) / 8.0);
	dc->LineTo(cow[0].x, p.y + ((double)width * 7) / 8.0);
	
	dc->MoveTo(p.x + length / 6, p.y + ((double)width * 6) / 8.0);
	dc->LineTo(p.x + length / 6, p.y + width);
	
	CPoint center;
	center.x = p.x + length / 6;
	center.y = p.y + (7 * width) / 16;
	Draw_Dashboard(dc,center, length / 7);
	
	/*
	dc->MoveTo(310, 580);
	dc->LineTo(910, 580);
	dc->MoveTo(310, 660);
	dc->LineTo(910, 660);
	dc->MoveTo(10, 740);
	dc->LineTo(910, 740);//行


	dc->MoveTo(160, 740);
	dc->LineTo(160, 820);
	dc->MoveTo(10, 780);
	dc->LineTo(310, 780);//小格
	*/
	CFont font;
	font.CreatePointFont(150, TEXT("宋体"));
	dc->SelectObject(&font);

	dc->TextOut(cow[0].x+30, p.y+((double)width*3)/32.0, TEXT("当前速度"));
	CString strSpeed;
	strSpeed.Format("%d", pDoc->speed);
	dc->TextOut(cow[0].x + 30+length/6.0, p.y + ((double)width * 3) / 32.0, strSpeed);


	dc->TextOut(cow[0].x + 30+length/3.0, p.y + ((double)width * 3) / 32.0, TEXT("当前位置"));
	CString strPosition ;
	strPosition.Format("%.0f", pDoc->position);
	dc->TextOut(cow[0].x + 30 + length / 3.0+length/6.0, p.y + ((double)width * 3) / 32.0, strPosition);


	dc->TextOut(cow[0].x + 30, p.y + ((double)width * 3) / 32.0+width/4.0, TEXT("目标速度"));
	CString strTargetSpeed = "0";
	dc->TextOut(cow[0].x + 30+length/6.0, p.y + ((double)width * 3) / 32.0 + width / 4.0,strTargetSpeed);

	dc->TextOut(cow[0].x + 30 + length / 3.0, p.y + ((double)width * 3) / 32.0+width/4.0, TEXT("目标位置"));
	CString strTargetPosition;
	strTargetPosition.Format("%.0f", pDoc->target);
	dc->TextOut(cow[0].x + 30 + length / 3.0+length/6.0, p.y + ((double)width * 3) / 32.0 + width / 4.0, strTargetPosition);

	//dc->TextOut(cow[0].x + 30, p.y + ((double)width * 3) / 32.0 + width / 2.0, TEXT("目标速度"));


	//dc->TextOut(cow[0].x + 30 + length / 3.0, p.y + ((double)width * 3) / 32.0 + width / 2.0, TEXT("目标位置"));


	//dc->TextOut(cow[0].x + 30, p.y + ((double)width * 3) / 32.0 + ((double)width * 3) / 4.0, TEXT("运行时间"));


	//dc->TextOut(cow[0].x + 30 + length / 3.0, p.y + ((double)width * 3) / 32.0 + ((double)width * 3) / 4.0, TEXT("运行距离"));
	


	dc->TextOut(p.x+50, p.y+((double)width*25)/32, TEXT("列车号"));//列车号
	CString strTrainNumber = "NULL";
	dc->TextOut(p.x + 50+length/6.0, p.y + ((double)width * 25) / 32, strTrainNumber);
	dc->TextOut(p.x + 50, p.y + ((double)width * 25) / 32+width/8.0, TEXT("车次号"));//车次号

	CString strDriverNumber = "NULL";
	dc->TextOut(p.x + 50+length/6.0, p.y + ((double)width * 25) / 32 + width / 8.0, strDriverNumber);

	dc->SelectObject(oldpen);
}
double DMIView::GetLimitSpeed(double position)
{
	double train_speed, solid_speed, rood_speed, limit_speed;
	double limitv = 205;
	//获得临时限速信息

	if (position >= (3000) && position <= (4000))
		return 100;
	if (position >= (4000) && position <= (5000))
		return 150;
	//考虑是以闭塞分区为单位？
	
	return limitv;//min(train_speed,solid_speed,rood_speed,limit_speed);
}

void DMIView::Draw_EB_Curve_new(CDC* pDC, double target, double position)
{
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//计算n个点的函数值
	double n = 205;
	double s = 0;//长度
	vector<pair<double, double>> point;
	int i = 0;
	while (true)
	{
		if (target - s <= position)
			break;
		
		if (point.size() == 0)
		{
			s += EB_Distance(1, 0);
			point.push_back(pair<double,double>(1, target - EB_Distance(1, 0)));
		}
		else
		{
			double v = point.back().first+1;
			double pos = point.back().second;
			double step = EB_Distance(v, v - 1);
			s += EB_Distance(v, v - 1);
			double limte_v;
			limte_v = min(GetLimitSpeed(pos - step), GetLimitSpeed(pos - step - pDoc->Train_Length));
			point.push_back(pair<double, double>(min(limte_v,v), pos - step));
		}
	}
	/*
	if (point.size() == 0)
		pDoc->speed = 0;
	else
		pDoc->speed = point.back().first;
	//将该曲线的速度赋予列车速度【假设列车贴线运行】
	*/
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(0, 0, 255));
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


void DMIView::Draw_EB_Chufa_Curve_new(CDC* pDC, double target, double position)
{
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//计算n个点的函数值
	double n = 205;
	double s = 0;//长度
	vector<pair<double, double>> point;
	int i = 0;
	while (true)
	{
		if (target - s <= position)
			break;

		if (point.size() == 0)
		{
			s += EB_Distance_chufa(1, 0);
			point.push_back(pair<double, double>(1, target - EB_Distance_chufa(1, 0)));
		}
		else
		{
			double v = point.back().first + 1;
			double pos = point.back().second;
			double step = EB_Distance_chufa(v, 0)-EB_Distance_chufa(v-1,0);
			s += step;
		
			double limte_v;
			limte_v = min(GetLimitSpeed(pos - step), GetLimitSpeed(pos - step - pDoc->Train_Length));
			limte_v = min(limte_v, 200);
			point.push_back(pair<double, double>(min(limte_v, v), pos - step));
		}
	}
	/*
	if (point.size() == 0)
		pDoc->speed = 0;
	else
		pDoc->speed = point.back().first;
		//速度赋予
		*/
	//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(255, 0, 255));
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

void DMIView::Draw_SB_Curve_new(CDC* pDC, double target, double position)
{
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//计算n个点的函数值
	double n = 205;
	double s = 0;//长度
	vector<pair<double, double>> point;
	int i = 0;
	while (true)
	{
		if (target - s <= position)
			break;

		if (point.size() == 0)
		{
			s += SB_Distance(1, 0);
			point.push_back(pair<double, double>(1, target - SB_Distance(1, 0)));
		}
		else
		{
			double v = point.back().first + 1;
			double pos = point.back().second;
			double step = SB_Distance(v, 0) - SB_Distance(v - 1, 0);
			s += step;

			double limte_v;
			limte_v = min(GetLimitSpeed(pos - step), GetLimitSpeed(pos - step - pDoc->Train_Length));
			limte_v = min(limte_v, 195);
			point.push_back(pair<double, double>(min(limte_v, v), pos - step));
		}
	}

	if (point.size() == 0)
		pDoc->speed = 0;
	else
		pDoc->speed = point.back().first;
	//速度赋予

//函数值与坐标转换
	CPoint* p = new CPoint[point.size()];
	for (int i = 0; i < point.size(); i++)
	{
		double x = (point[i].second / MaxX) * long_x;
		double y = (point[i].first / MaxY) * long_y;//速度-距离转距离-速度关系
		p[i].x = x + start.x;
		p[i].y = start.y - y;
	}
	CPen newPen2(BS_SOLID, 2, RGB(255, 215, 0));
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


void DMIView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	COverSpeedProDoc* pDoc = (COverSpeedProDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	


	if (pDoc->position < pDoc->target)
		pDoc->position+=((double)pDoc->speed/3.6);



	//实现DMI和站场图的计时器同步
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	StationYardView* pStation = (StationYardView*)pFrame->pStation;
	pStation->Invalidate(false);
	Invalidate(false);
	CView::OnTimer(nIDEvent);
}
