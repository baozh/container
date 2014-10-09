/*=====================================================================================
模块名  ：
文件名  ：deque.h
相关文件：
实现功能：双端队列的实现.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/7   V1.0    包增辉                    创建
========================================================================================*/


#ifndef _DEQUE_H
#define _DEQUE_H

#include "type_def.h"
#include "list.h"

template <typename T>
class Deque
{
public:
	typedef BOOL32 (*VisitFunc)(void *data, void *ctx);
	
	Deque() {};
	Deque(const Deque& tOther) {m_tList = tOther.m_tList;};
	Deque& operator=(const Deque &tOther);
	BOOL32 operator==(const Deque &tOther) {return m_tList==tOther.m_tList;};
	~Deque() {MakeEmpty();};
	
	BOOL32 IsEmpty();
	BOOL32 MakeEmpty();    //释放所有结点
	size_t GetSize();
	
	BOOL32 PushBack(const T &tData);
	BOOL32 PushFront(const T &tData);
	BOOL32 PopBack();
	BOOL32 PopFront();
	T GetFront();
	T GetBack();
	BOOL32 Foreach(VisitFunc visit, void *ctx);
private:
	List<T> m_tList;
};

template <typename T>
BOOL32 Deque<T>::IsEmpty()
{
	return m_tList.IsEmpty();
};

template <typename T>
BOOL32 Deque<T>::MakeEmpty()
{
	return m_tList.MakeEmpty();
};

template <typename T>
size_t Deque<T>::GetSize()
{
	return m_tList.GetSize();
};

template <typename T>
BOOL32 Deque<T>::PushBack(const T &tData)
{
	return m_tList.InsertTail(tData);
};

template <typename T>
BOOL32 Deque<T>::PushFront(const T &tData)
{
	return m_tList.InsertHead(tData);
};

template <typename T>
BOOL32 Deque<T>::PopFront()
{
	return m_tList.DeleteHead();
};

template <typename T>
BOOL32 Deque<T>::PopBack()
{
	return m_tList.DeleteTail();
};

template <typename T>
T Deque<T>::GetFront()
{
	return m_tList.GetData(m_tList.GetHead());
};

template <typename T>
T Deque<T>::GetBack()
{
	return m_tList.GetData(m_tList.GetTail());
};

template <typename T>
BOOL32 Deque<T>::Foreach(VisitFunc visit, void *ctx)
{
	if (visit == NULL) return FALSE;
	return m_tList.Foreach(visit, ctx);
};

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque<T> &tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		m_tList = tOther.m_tList;
	}
	
	return *this;
};

#endif

