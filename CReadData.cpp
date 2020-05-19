#include "pch.h"
#include "CReadData.h"
#include<string.h>
#include<stdlib.h>
#include"stdafx.h"
#include "afxtempl.h"
#include <string>
#include<iostream>
CReadData::CReadData()
{

}
CReadData::~CReadData()
{

}

void CReadData::ReadSignal()
{
	//开始写函数,按照自己数据库格式参数修改一下
	_variant_t RecordsAffected;	//操作完成后所影响的行数 
	int signal_count = 0;

	//读进路表格，以行为单位	
	m_pRecordset = Control_Connection->Execute("SELECT *FROM 信号机", &RecordsAffected, adCmdText);
	m_pRecordset->MoveFirst();//第一行

	while (!m_pRecordset->adoEOF)
	{
		_variant_t var;//定义一个容器
					   //表格参数
		CString id;
		int ID;
		CString Name;
		CString attr;
		int Attr;
		CString x1;
		int X1;
		CString y1;
		int Y1;
		CString proBS;
		int ProBS;
		CString nextSig;
		int NextSig;
		CString formerSig;
		int FormerSig;
		CString color;
		int Color;
		//开始读表
		var = m_pRecordset->GetCollect("ID");
		id = var.bstrVal;
		ID = _ttoi(id);
		//ID = var.intVal; ;
		var = m_pRecordset->GetCollect("名称");
		Name = var.bstrVal;
		/*name = atoi(Name);*/
		var = m_pRecordset->GetCollect("Attr");
		attr = var.bstrVal;
		Attr = atoi(attr);
		//Attr=  var.intVal;
		var = m_pRecordset->GetCollect("x1");
		x1 = var.bstrVal;
		X1 = atoi(x1);
		//X1 = var.intVal;
		var = m_pRecordset->GetCollect("y1");
		y1 = var.bstrVal;
		Y1 = atoi(y1);
		//Y1 = var.intVal;
		var = m_pRecordset->GetCollect("防护的闭塞分区ID");
		proBS = var.bstrVal;
		ProBS = atoi(proBS);
		var = m_pRecordset->GetCollect("下一架信号机");
		nextSig = var.bstrVal;
		NextSig = atoi(nextSig);
		var = m_pRecordset->GetCollect("上一架信号机");
		formerSig = var.bstrVal;
		FormerSig = atoi(formerSig);
		var = m_pRecordset->GetCollect("颜色");
		color = var.bstrVal;
		Color = atoi(color);

		m_csignal[signal_count++].fuzhi(ID, Name, Attr, X1, Y1, ProBS, NextSig, FormerSig, Color);
		m_pRecordset->MoveNext();
	}
	m_pRecordset->MoveFirst();
	signal_count = 0;

}

void CReadData::ReadBS()
{
	//开始写函数,按照自己数据库格式参数修改一下
	_variant_t RecordsAffected;	//操作完成后所影响的行数 
	int bs_count = 0;

	//读进路表格，以行为单位	
	m_pRecordset = Control_Connection->Execute("SELECT *FROM 闭塞分区", &RecordsAffected, adCmdText);
	m_pRecordset->MoveFirst();//第一行

	while (!m_pRecordset->adoEOF)
	{
		_variant_t var;//定义一个容器
					   //表格参数
		CString id;
		int ID;
		CString attr;
		int Attr;
		CString x1;
		int X1;
		CString y1;
		int Y1;
		CString x2;
		int X2;
		CString y2;
		int Y2;
		CString proSignal;
		int ProSignal;
		CString state;
		int State;
		CString length;
		int Length;
		CString speed;
		int Speed;
		//开始读表
		var = m_pRecordset->GetCollect("ID");
		id = var.bstrVal;
		ID = atoi(id);
		var = m_pRecordset->GetCollect("Attr");
		attr = var.bstrVal;
		Attr = atoi(attr);
		var = m_pRecordset->GetCollect("X1");
		x1 = var.bstrVal;
		X1 = atoi(x1);
		var = m_pRecordset->GetCollect("Y1");
		y1 = var.bstrVal;
		Y1 = atoi(y1);
		var = m_pRecordset->GetCollect("X2");
		x2 = var.bstrVal;
		X2 = atoi(x2);
		var = m_pRecordset->GetCollect("Y2");
		y2 = var.bstrVal;
		Y2 = atoi(y2);
		var = m_pRecordset->GetCollect("防护信号机");
		proSignal = var.bstrVal;
		ProSignal = atoi(proSignal);
		var = m_pRecordset->GetCollect("状态");
		state = var.bstrVal;
		State = atoi(state);
		var = m_pRecordset->GetCollect("长度");
		length = var.bstrVal;
		Length = atoi(length);
		var = m_pRecordset->GetCollect("速度");
		speed = var.bstrVal;
		Speed = atoi(speed);

		m_cblocksec[bs_count++].fuzhi(ID, Attr, X1, Y1, X2, Y2, ProSignal, State, Length, Speed);
		m_pRecordset->MoveNext();
	}
	m_pRecordset->MoveFirst();
	bs_count = 0;
}
