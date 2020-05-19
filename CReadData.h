#pragma once
#include"stdafx.h"
#include "CSignal.h"
#include "CBlockSec.h"
#include<iostream>
class CReadData
{
public:
	//数据库变量
	_ConnectionPtr m_pConnection;			//指向Connection的指针(打开数据库连接的变量)
	_RecordsetPtr m_pRecordset;

	_ConnectionPtr Write_Connection;			//指向Connection的指针(打开数据库连接的变量)
	_ConnectionPtr Control_Connection;			//指向Connection的指针(打开数据库连接的变量)

	int dubiaojishu = 0;

	CSignal m_csignal[15];
	CBlockSec m_cblocksec[15];
	void ReadSignal();
	void ReadBS();
	
	//void CReadData::operator=(const CReadData& b);
public:
	CReadData();
	~CReadData();
};

