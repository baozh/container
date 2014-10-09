/*=====================================================================================
模块名  ：
文件名  ：stack.h
相关文件：
实现功能：栈的实现.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/7   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _STACK_H
#define _STACK_H

#include "type_def.h"
#include "darray.h"

template <typename T>
class Stack
{
public:
	typedef BOOL32 (*VisitFunc)(void *data, void *ctx);
	
	Stack() {};
	Stack(const Stack& tOther) {m_tArray = tOther.m_tArray;};
	Stack& operator=(const Stack &tOther);
	BOOL32 operator==(const Stack &tOther) {return m_tArray==tOther.m_tArray;};
	~Stack() {MakeEmpty();};
	
	BOOL32 IsEmpty();
	BOOL32 MakeEmpty();
	size_t GetSize();
	
	BOOL32 Push(const T &tData);
	BOOL32 Pop();
	T GetTop();
	BOOL32 Foreach(VisitFunc visit, void *ctx);
private:
	DArray<T> m_tArray;
};

template <typename T>
BOOL32 Stack<T>::IsEmpty()
{
	return m_tArray.IsEmpty();
};

template <typename T>
BOOL32 Stack<T>::MakeEmpty()
{
	return m_tArray.MakeEmpty();
};

template <typename T>
size_t Stack<T>::GetSize()
{
	return m_tArray.GetSize();
};

template <typename T>
BOOL32 Stack<T>::Push(const T &tData)
{
	return m_tArray.PushBack(tData);
};

template <typename T>
T Stack<T>::GetTop()
{
	return m_tArray.GetBack();
};

template <typename T>
BOOL32 Stack<T>::Pop()
{
	m_tArray.PopBack();
	return TRUE;
};

template <typename T>
BOOL32 Stack<T>::Foreach(VisitFunc visit, void *ctx)
{
	if (visit == NULL) return FALSE;
	return m_tArray.Foreach(visit, ctx);
};

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T> &tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		m_tArray = tOther.m_tArray;
	}
	return *this;
};
#endif