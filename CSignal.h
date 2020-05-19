#pragma once
#include <cstring>
using namespace std;

class CSignal
{
public:
	int ID;//信号机的ID，唯一标识码
	CString Name;//信号机的命名
	int Dir;//信号机的方向，1表示下行，2表示上行
	int Attr;//信号机的属性。为1表示为区间通过信号机，为2表示进站信号机，3表示出站信号机
	int x1, y1;//绘制信号机的基点坐标
	int ProBS;//该信号机防护的闭塞分区的ID
	int NextSig;//相邻下一架信号机的ID
	int FormerSig;//相邻前一架信号机的ID
	int Color;//信号机的颜色。为1表示绿色，2表示绿黄，3表示黄色，4表示红色,5表示无色
public:
	CSignal();
	~CSignal();
	void CSignal::DrawSignal(CDC* pDC);	//画不同显示的信号机；
	void CSignal::fuzhi(int ID, CString  Name,int Dir, int Attr, int x1, int y1,int ProBS, int NextSig, int FormerSig, int Color);

};

