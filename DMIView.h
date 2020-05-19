#pragma once
#include<vector>
// DMIView 视图

class DMIView : public CView
{
	DECLARE_DYNCREATE(DMIView)

protected:
	DMIView();           // 动态创建所使用的受保护的构造函数
	virtual ~DMIView();
public:
	//成员 保存坐标系信息
	int long_x;
	int long_y;
	CPoint start;
	double MaxX;
	double MaxY;
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void DrawXY(CDC* pDC, int long_x, int long_y, CPoint start, double maxX, double maxY);//函数图绘制
	virtual void Draw_X(CDC* pDC, CPoint p);//离散点标注
	virtual double EB_Breaking_Acc(double v);//速度v对应的制动加速度a
	virtual double Basic_Drag_Acc(double v);//基本阻力导致的加速度
	virtual double Add_Drag_Acc(double v);//附加阻力导致的加速度
	virtual double GetAcc_EB(double v);//速度v对应的紧急制动总加速度
	virtual double SB_Breaking_Acc(double v);//常用制动加速度
	virtual double GetAcc_SB(double v);//速度v对应的常用制动总加速度
	virtual double EB_Distance(double v1, double v2);//从速度v1减速至速度v2的制动距离 考虑安全余量100m
	virtual double EB_Distance_chufa(double v1, double v2);//触发曲线，考虑安全制动模型
	virtual CPoint* Draw_EB_Curve(CDC *pDC,double target, double target_v);
	CPoint* Draw_EB_Curve(CDC* pDC,double target1, double target2, double target_v, double start_v);
	//根据目标点绘制紧急制动曲线 target1 列车位置 target2 目标位置 target_v 出口速度 start_v 入口速度 

	virtual void Draw_EB_Chufa_Curve(CDC* pDC,double target);//绘制紧急制动触发曲线
	virtual void Draw_Dashboard(CDC* pDC,CPoint center, double r);//根据当前速度绘制仪表盘
	virtual void Draw_SB_Curve(CDC* pDC,double target);//根据目标点绘制7N常用制动曲线
	virtual double SB_Distance(double v1, double v2);//常用制动 制动距离 安全余量100m
	virtual void Draw_OverSpeedCurve(CDC* pDC,double position, double target);
	virtual void DrawInfoTable(CDC* pDC);//绘制列车信息表

	virtual double GetLimitSpeed(double position);//获得某个位置的固定限速
	virtual void Draw_EB_Curve_new(CDC* pDC, double target,  double position);//设计了新的紧急制动曲线的计算方法
	
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


