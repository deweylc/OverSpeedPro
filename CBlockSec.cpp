#include "pch.h"
#include "CBlockSec.h"
CBlockSec::CBlockSec()
{

}
CBlockSec::~CBlockSec()
{

}

void CBlockSec::DrawBS(CDC* pDC)
{
	CPen pen1;
	pen1.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));
	CPen pen2;
	pen2.CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
	CPen pen3;
	pen3.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen pen4;
	pen4.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	//pDC->SelectObject(&pen1);
	switch (Attr)
	{
	case 1:
		if (State == 1)
		{
			pDC->SelectObject(&pen1);
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
			pDC->SelectObject(&pen4);
			pDC->MoveTo(x1, y1 - 5);
			pDC->LineTo(x1, y1 + 5);
			pDC->MoveTo(x2, y2 - 5);
			pDC->LineTo(x2, y2 + 5);


		}
		else if (State == 2)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
			pDC->SelectObject(&pen4);
			pDC->MoveTo(x1, y1 - 5);
			pDC->LineTo(x1, y1 + 5);
			pDC->MoveTo(x2, y2 - 5);
			pDC->LineTo(x2, y2 + 5);
		}
		else if (State == 3)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
			pDC->SelectObject(&pen3);
			pDC->MoveTo((x1+x2)/2-10, (y1+y2)/2-10);
			pDC->LineTo((x1 + x2) / 2 + 10, (y1 + y2) / 2 + 10);
			pDC->MoveTo((x1 + x2) / 2 + 10, (y1 + y2) / 2 - 10);
			pDC->LineTo((x1 + x2) / 2 - 10, (y1 + y2) / 2 + 10);
			pDC->SelectObject(&pen4);
			pDC->MoveTo(x1, y1 - 5);
			pDC->LineTo(x1, y1 + 5);
			pDC->MoveTo(x2, y2 - 5);
			pDC->LineTo(x2, y2 + 5);
		}
		break;	
	case 2:
		if (State == 1)
		{
			pDC->SelectObject(&pen1);
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
			pDC->MoveTo(x2 - 60, y2 - 45);
			pDC->LineTo(x2 - 30, y2);
			pDC->MoveTo(x1, y1 - 45);
			pDC->LineTo(x2 - 60, y2 - 45);
		}
		else if (State == 2)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
			pDC->MoveTo(x2 - 60, y2 - 45);
			pDC->LineTo(x2 - 30, y2);
			pDC->MoveTo(x1, y1 - 45);
			pDC->LineTo(x2 - 60, y2 - 45);
		}
		break;
	}
}
void CBlockSec::fuzhi(int ID, int Attr, int x1, int y1, int x2, int y2, int ProSignal, int State, int Length, int Speed)
{
	this->ID = ID;
	this->Attr = Attr;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->ProSignal = ProSignal;
	this->State = State;
	this->Length = Length;
	this->Speed = Speed;
}
