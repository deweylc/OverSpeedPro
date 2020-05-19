#pragma once
#include"CReadData.h"
#include "CSignal.h"

// StationYardView 视图

class StationYardView : public CView
{
	DECLARE_DYNCREATE(StationYardView)

protected:
	StationYardView();           // 动态创建所使用的受保护的构造函数
	virtual ~StationYardView();

public:
	//CButton m_button1, m_button2;
	CReadData RD;
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	/*afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void On01();*/
public:
	void SetSigClr(int SigID);//设置信号机的颜色
	int GetSigClr(int SigID);//获得信号机的颜色
	void SetState();//设置闭塞分区的状态
	int GetState(int BSID);//获得闭塞分区的状态
	
};


