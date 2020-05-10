#include "pch.h"
#include "CSignal.h"
CSignal::CSignal()
{
}
CSignal::~CSignal()
{
}
void CSignal::DrawSignal(CDC* pDC)//画不同显示的信号机；
{
	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));//hei色画笔
	pDC->SelectObject(&pen1);
	CPen pen2;
	pen2.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));//红色画笔
	pDC->SelectObject(&pen2);
	CPen pen3;
	pen3.CreatePen(PS_SOLID, 4, RGB(255, 0, 0));//红色画笔
	pDC->SelectObject(&pen3);


	CBrush brushred;//红色笔刷
	brushred.CreateSolidBrush(RGB(255, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject(&brushred);
	CBrush brushyellow;//黄色笔刷
	brushyellow.CreateSolidBrush(RGB(255, 255, 0));
	CBrush brushblue;//蓝色笔刷
	brushblue.CreateSolidBrush(RGB(0, 0, 255));
	CBrush brushgreen;//绿色笔刷
	brushgreen.CreateSolidBrush(RGB(0, 255, 0));
	CBrush brushwhite;//白色笔刷
	brushwhite.CreateSolidBrush(RGB(255, 255, 255));

	/*CFont font120;
	font120.CreatePointFont(90, _T("微软雅黑"));
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(0, 0, 0));*/
	switch (Attr)
	{
	case 1://通过信号机
		//信号机柱
		pDC->SelectObject(&pen2);
		pDC->MoveTo(x1, y1);
		pDC->LineTo(x1, y1 + 15);
		if(Color==1)//信号机点绿灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushgreen);
			pDC->Ellipse(x1, y1, x1 + 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 + 15, y1, x1 + 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 2)//信号机点绿黄灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushgreen);
			pDC->Ellipse(x1, y1, x1 + 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushyellow);
			pDC->Ellipse(x1 + 15, y1, x1 + 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 3)//信号机点黄灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushyellow);
			pDC->Ellipse(x1, y1, x1 + 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 + 15, y1, x1 + 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 4)//信号机点红灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushred);
			pDC->Ellipse(x1, y1, x1 + 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 + 15, y1, x1 + 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 5)//信号机平时状态不点灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1, y1, x1 + 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 + 15, y1, x1 + 30, y1 + 15);//第二个灯位无色
		}
		break;
	case 2://甲站上行进站信号机
		//信号机柱
		pDC->SelectObject(&pen2);
		pDC->MoveTo(x1, y1);
		pDC->LineTo(x1, y1 + 15);
		if (Color == 1)//信号机点绿灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushgreen);
			pDC->Ellipse(x1, y1, x1 - 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 - 15, y1, x1 - 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 2)//信号机点绿黄灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushgreen);
			pDC->Ellipse(x1, y1, x1 - 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 - 15, y1, x1 - 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 3)//信号机点黄灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushyellow);
			pDC->Ellipse(x1, y1, x1 - 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 - 15, y1, x1 - 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 4)//信号机点红灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushred);
			pDC->Ellipse(x1, y1, x1 - 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 - 15, y1, x1 - 30, y1 + 15);//第二个灯位无色
		}
		else if (Color == 5)//信号机平时状态不点灯
		{
			//信号机灯位
			pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1, y1, x1 - 15, y1 + 15);//第一个灯位绿色	
			pDC->SelectObject(&brushwhite);
			pDC->Ellipse(x1 - 15, y1, x1 - 30, y1 + 15);//第二个灯位无色
		}
		break;

	}
}
void CSignal::fuzhi(int ID, CString  Name, int Attr, int x1, int y1, int ProBS, int NextSig, int FormerSig, int Color)
{
	this->ID = ID;
	this->Name = Name;
	this->Attr = Attr;
	this->x1 = x1;
	this->y1 = y1;
	this->ProBS = ProBS;
	this->NextSig = NextSig;
	this->FormerSig = FormerSig;
	this->Color = Color;
}

