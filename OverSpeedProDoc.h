
// OverSpeedProDoc.h: COverSpeedProDoc 类的接口
//


#pragma once


class COverSpeedProDoc : public CDocument
{
protected: // 仅从序列化创建
	COverSpeedProDoc() noexcept;
	DECLARE_DYNCREATE(COverSpeedProDoc)
//成员
public:
	int speed = 0;//列车速度
	int position = 0;//列车位置
	int Train_Length=200;//列车长度
	int target=0;//目标位置
// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~COverSpeedProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
