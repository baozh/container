/*=====================================================================================
模块名  ：
文件名  ：list.h
相关文件：
实现功能：list双向链表的实现.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/9/21   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _List_H
#define _List_H

#include "type_def.h"

template <typename T>
class ListNode
{
public:
	ListNode<T> *m_pPrev;
	ListNode<T> *m_pNext;
	T	      m_tData;
	
	ListNode() {m_pNext = NULL; m_pPrev = NULL;}
	~ListNode() {m_pNext = NULL; m_pPrev = NULL;}
};

template <typename T>
class List
{
public:
	typedef ListNode<T>* Position;

	List();
	List(const List& tOther);
	List& operator=(const List &tOther);
	~List();

	BOOL32 IsEmpty();
	BOOL32 MakeEmpty();    //释放所有结点
	size_t GetSize();

	BOOL32 InsertBefore(Position pos, const T &tData) {return __insert_before(pos,tData);};      //将tData插入到pos结点之前
	BOOL32 InsertAfter(Position pos, const T &tData) {return __insert_after(pos,tData);};       //将tData插入到pos结点之后
	BOOL32 InsertHead(const T &tData);
	BOOL32 InsertTail(const T &tData);
	
	size_t Delete(const T &tData);   //删除所有值为tData的结点，并返回删除的结点数
	BOOL32 Delete(Position pos) {return __delete_node(pos);};
	BOOL32 DeleteHead();
	BOOL32 DeleteTail();

	Position Find(const T &tData) {return __find(tData);};
	Position GetPrev(const Position pos) {return pos->m_pPrev;};
	Position GetNext(const ListNode<T> * pos) {return pos->m_pNext;};
	Position GetHead() {return m_pHead;};
	Position GetTail() {return m_pTail;};

	T GetData(const Position pos) {return __get_data(pos);};
	BOOL32 SetData(Position pos, T tData) {return __set_data(pos, tData);};

private:
	void __copy(const List& tOther);
	ListNode<T>* __find(const T &tData);
	T __get_data(const ListNode<T> * pos);
	BOOL32 __set_data(ListNode<T> * pos, T tData);
	BOOL32 __delete_node(ListNode<T> * pos);
	BOOL32 __insert_before(ListNode<T> * pos, const T &tData);
	BOOL32 __insert_after(ListNode<T> * pos, const T &tData);
private:
	ListNode<T>* m_pHead;
	ListNode<T>* m_pTail;
};


template <class T>
List<T>::List()
{
	m_pHead = NULL;
	m_pTail = NULL;
};

template <class T>
List<T>::~List()
{
	MakeEmpty();
};

template <typename T>
BOOL32 List<T>::IsEmpty()
{
	return m_pHead == NULL && m_pTail == NULL;
};

template <typename T>
size_t List<T>::GetSize()
{
	ListNode<T> *pPos = m_pHead;
	size_t dwListSize = 0;
	while(pPos != NULL)
	{
		dwListSize ++;
		pPos = pPos->m_pNext;
	}
	return dwListSize;
};

template <typename T>
BOOL32 List<T>::__insert_before(ListNode<T> * pos, const T& tData)
{
	if (pos == NULL) return FALSE;

	ListNode<T> *pNode = new ListNode<T>;
	if(pNode == NULL)
	{
		std::cout<<"out of memory, failed to alloc space!"<<std::endl;
		return FALSE;
	}

	pNode->m_tData = tData;
	if (pos->m_pPrev == NULL)   //pos是链表第一个结点
	{
		pNode->m_pPrev = NULL;
		pNode->m_pNext = pos;
		pos->m_pPrev = pNode;
		m_pHead = pNode;    //更新链表头指针
	}
	else
	{
		ListNode<T>* pPrev = pos->m_pPrev;
		
		pPrev->m_pNext = pNode;
		pNode->m_pPrev = pPrev;
		pNode->m_pNext = pos;
		pos->m_pPrev = pNode;
	}
	return TRUE;
};

template <typename T>
BOOL32 List<T>::__insert_after(ListNode<T> * pos, const T& tData)
{
	if (pos == NULL) return FALSE;
	
	ListNode<T> *pNode = new ListNode<T>;
	if(pNode == NULL)
	{
		std::cout<<"out of memory, failed to alloc space!"<<std::endl;
		return FALSE;
	}
	pNode->m_tData = tData;

	if (pos->m_pNext == NULL)   //pos是链表最后一个结点
	{
		pNode->m_pNext = NULL;
		pNode->m_pPrev = pos;
		pos->m_pNext = pNode;
		m_pTail = pNode;    //更新链表尾指针
	}
	else
	{
		ListNode<T>* pNext = pos->m_pNext;
		
		pNext->m_pPrev = pNode;
		pNode->m_pNext = pNext;
		pNode->m_pPrev = pos;
		pos->m_pNext = pNode;
	}
	return TRUE;
};

template <typename T>
BOOL32 List<T>::InsertHead(const T &tData)
{
	if (m_pHead == NULL)  //当前链表为空
	{
		ListNode<T> *pNode = new ListNode<T>;
		if(pNode == NULL)
		{
			std::cout<<"out of memory, failed to alloc space!"<<std::endl;
			return FALSE;
		}
		pNode->m_tData = tData;

		m_pHead = pNode;
		m_pTail = pNode;
	}
	else
	{
		return InsertBefore(m_pHead, tData);
	}
};

template <typename T>
BOOL32 List<T>::InsertTail(const T& tData)
{
	if (m_pTail == NULL)  //当前链表为空
	{
		ListNode<T> *pNode = new ListNode<T>;
		if(pNode == NULL)
		{
			std::cout<<"out of memory, failed to alloc space!"<<std::endl;
			return FALSE;
		}
		pNode->m_tData = tData;
		
		m_pHead = pNode;
		m_pTail = pNode;
	}
	else
	{
		return InsertAfter(m_pTail, tData);
	}
};

template <typename T>
BOOL32 List<T>::DeleteHead()
{
	if (m_pHead != NULL)
	{
		ListNode<T>* pos = m_pHead;

		if (m_pHead == m_pTail)   //当前链表只有一个结点
		{
			m_pHead = NULL;
			m_pTail = NULL;
		}
		else
		{
			ListNode<T>* pHeadNext = m_pHead->m_pNext;
			pHeadNext->m_pPrev = NULL;
			m_pHead = pHeadNext;
		}
		delete pos;
	}
	return TRUE;
};

template <typename T>
BOOL32 List<T>::DeleteTail()
{
	if (m_pTail != NULL)
	{
		ListNode<T>* pos = m_pTail;
		
		if (m_pHead == m_pTail)   //当前链表只有一个结点
		{
			m_pHead = NULL;
			m_pTail = NULL;
		}
		else
		{
			ListNode<T>* pTailPrev = m_pTail->m_pPrev;
			pTailPrev->m_pNext = NULL;
			m_pTail = pTailPrev;
		}
		delete pos;
	}
	return TRUE;
};

template <typename T>
BOOL32 List<T>::__delete_node(ListNode<T> * pos)
{
	if (pos == NULL) return FALSE;

	if (IsEmpty() == TRUE)
	{
		return FALSE;
	}
	else if (pos == m_pHead)
	{
		return DeleteHead();
	}
	else if (pos == m_pTail)
	{
		return DeleteTail();
	}
	else
	{
		ListNode<T>* pPosPrev = pos->m_pPrev;
		ListNode<T>* pPosNext = pos->m_pNext;
		
		pPosPrev->m_pNext = pPosNext;
		pPosNext->m_pPrev = pPosPrev;
	}
	delete pos;
	return TRUE;
};

template <typename T>
size_t List<T>::Delete(const T& tData)
{
	size_t dwEraseNum = 0;
	if (m_pHead == NULL) return dwEraseNum;
	
	ListNode<T> *pPos = m_pHead;
	ListNode<T> *pNext = NULL;
	while(pPos != NULL)
	{
		pNext = pPos->m_pNext;

		if (pPos->m_tData == tData && Delete(pPos))
		{
			dwEraseNum++;
		}
		pPos =pNext;
	}
	return dwEraseNum;
};

template <typename T>
ListNode<T>* List<T>::__find(const T &tData)
{
	ListNode<T> *p = m_pHead;
	while(p != NULL && p->m_tData != tData)
	{
		p = p->m_pNext;
	}
	return p;
};

template <typename T>
T List<T>::__get_data(const ListNode<T> * pos)
{
	return pos->m_tData;
};

template <typename T>
BOOL32 List<T>::__set_data(ListNode<T> * pos, T tData)
{
	pos->m_tData = tData;
	return TRUE;
};

template <typename T>
BOOL32 List<T>::MakeEmpty()
{
	if (m_pHead != NULL)
	{
		ListNode<T> *pPos = m_pHead;
		ListNode<T> *pNext = NULL;
		while(pPos != NULL)
		{
			pNext = pPos->m_pNext;
			delete pPos;
			pPos =pNext;
		}
		m_pHead = NULL;
		m_pTail = NULL;
	}

	return TRUE;
};

template <typename T>
void List<T>::__copy(const List<T>& tOther)
{
	if (tOther->IsEmpty()) return;

	ListNode<T> *pPosOther = tOther.m_tHead;
	ListNode<T> *pPosThis = NULL;

	while(pPosOther != NULL)
	{
		if (IsEmpty())
		{
			InsertHead(pPosOther->m_tData);
			pPosThis = m_pHead;
		}
		else
		{
			InsertAfter(pPosThis, pPosOther->m_tData);
			pPosThis = pPosThis->m_pNext;
		}
	}
};

template <typename T>
List<T>::List(const List<T>& tOther)
{
	__copy(tOther);
};

template <typename T>
List<T>& List<T>::operator=(const List<T>& tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		__copy(tOther);
	}
	
	return *this;
};

#endif    /* _List_H */