#pragma once
#include"CReadData.h"
#include "CSignal.h"

// StationYardView 视图
//将原来的基类CView修改为CScrollView为了便于加滚动条
class StationYardView : public CScrollView
{
	DECLARE_DYNCREATE(StationYardView)

protected:
	StationYardView();           // 动态创建所使用的受保护的构造函数
	virtual ~StationYardView();

public:
	//CButton m_button1, m_button2;
	CReadData RD;
	CButton m_button1;//正线发车
	CButton m_button2;//正线接车
	CButton m_button3;//列车复位


	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On01();//正线发车
	afx_msg void On02();//正线接车
	afx_msg void On03();//列车复位

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	int FindBS(int x);//根据列车当前位置确定列车所处闭塞分区，并返回该闭塞分区的防护信号机的ID
	void DisCount(int x);//根据列车当前所处闭塞分区的防护信号机的ID,计算行车目标点的距离

	//StationYardDoc* GetDocument();
public:
	void SetSigClr(int SigID);//设置信号机的颜色
	int GetSigClr(int SigID);//获得信号机的颜色
	void SetState();//设置闭塞分区的状态
	int GetState(int BSID);//获得闭塞分区的状态
	int GetSigX(int SigID);
	afx_msg void On132772();
	afx_msg void On132773();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void On132775();
	afx_msg void On132776();
};


