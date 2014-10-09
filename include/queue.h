/*=====================================================================================
模块名  ：
文件名  ：queue.h
相关文件：
实现功能：队列的实现.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/7   V1.0    包增辉                    创建
========================================================================================*/


#ifndef _QUEUE_H
#define _QUEUE_H

#include "type_def.h"
#include "list.h"


template <typename T>
class Queue
{
public:
	typedef BOOL32 (*VisitFunc)(void *data, void *ctx);

	Queue() {};
	Queue(const Queue& tOther) {m_tList = tOther.m_tList;};
	Queue& operator=(const Queue &tOther);
	BOOL32 operator==(const Queue &tOther) {return m_tList==tOther.m_tList;};
	~Queue() {MakeEmpty();};
	
	BOOL32 IsEmpty();
	BOOL32 MakeEmpty();    //释放所有结点
	size_t GetSize();
	
	BOOL32 PushBack(const T &tData);
	BOOL32 PopFront();
	T GetFront();
	BOOL32 Foreach(VisitFunc visit, void *ctx);
private:
	List<T> m_tList;
};

template <typename T>
BOOL32 Queue<T>::IsEmpty()
{
	return m_tList.IsEmpty();
};

template <typename T>
BOOL32 Queue<T>::MakeEmpty()
{
	return m_tList.MakeEmpty();
};

template <typename T>
size_t Queue<T>::GetSize()
{
	return m_tList.GetSize();
};

template <typename T>
BOOL32 Queue<T>::PushBack(const T &tData)
{
	return m_tList.InsertTail(tData);
};

template <typename T>
BOOL32 Queue<T>::PopFront()
{
	return m_tList.DeleteHead();
};

template <typename T>
T Queue<T>::GetFront()
{
	return m_tList.GetData(m_tList.GetHead());
};

template <typename T>
BOOL32 Queue<T>::Foreach(VisitFunc visit, void *ctx)
{
	if (visit == NULL) return FALSE;
	return m_tList.Foreach(visit, ctx);
};

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T> &tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		m_tList = tOther.m_tList;
	}
	
	return *this;
};
#endif