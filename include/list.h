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
	
	ListNode():m_pNext(NULL), m_pPrev(NULL) {};
	~ListNode() {m_pNext = NULL; m_pPrev = NULL;};
};

template <typename T>
class List
{
public:
	typedef ListNode<T>* Position;
	typedef BOOL32 (*VisitFunc)(void *data, void *ctx);

	List():m_pHead(NULL), m_pTail(NULL) {};
	List(const List& tOther);
	List& operator=(const List &tOther);
	BOOL32 operator==(const List &tOther);
	~List();

	BOOL32 IsEmpty();
	BOOL32 MakeEmpty();    //释放所有结点
	size_t GetSize();

	BOOL32 InsertBefore(Position pos, const T &tData);      //将tData插入到pos结点之前
	BOOL32 InsertAfter(Position pos, const T &tData);       //将tData插入到pos结点之后
	BOOL32 InsertHead(const T &tData);
	BOOL32 InsertTail(const T &tData);
	
	size_t Delete(const T &tData);   //删除所有值为tData的结点，并返回删除的结点数
	BOOL32 Delete(Position pos);
	BOOL32 DeleteHead();
	BOOL32 DeleteTail();

	Position Find(const T &tData);
	Position GetPrev(const Position pos) {return pos->m_pPrev;};
	Position GetNext(const Position pos) {return pos->m_pNext;};
	Position GetHead() {return m_pHead;};
	Position GetTail() {return m_pTail;};

	T GetData(const Position pos);
	BOOL32 SetData(Position pos, T tData);
	BOOL32 Sort() {__sort_list(m_pHead, m_pTail); return TRUE;};
	BOOL32 Foreach(VisitFunc visit, void *ctx);
	BOOL32 Swap(Position pos1, Position pos2);
private:
	void __copy(const List& tOther);
	void __sort_list(Position begin, Position end);
	void __swap_ptr(ListNode<T> **pos1, ListNode<T> **pos2);
private:
	ListNode<T>* m_pHead;
	ListNode<T>* m_pTail;
};

template <class T>
void List<T>::__swap_ptr(ListNode<T> **pos1, ListNode<T> **pos2)
{
	if (*pos1 == *pos2)
	{
		return;
	}
	ListNode<T> *tmp = *pos1;
	*pos1 = *pos2;
	*pos2 = tmp;
};

template <class T>
void List<T>::__sort_list(Position begin, Position end)
{
	if(begin == NULL || end == NULL)  
        return;  
    if(begin == end)  
        return;  
    ListNode<T> *pslow = begin;  
    ListNode<T> *pfast = begin->m_pNext;  
    ListNode<T> *ptemp = NULL, *ptemp1 = NULL, *pEndFlag = NULL;
	pEndFlag = end;
    while(true)  
    {  
		ptemp = pfast;
		ptemp1 = pfast->m_pNext;
        if(pfast->m_tData < begin->m_tData)       //每次都选择待排序链表的头结点作为划分的基准  
        {   
            pslow = pslow->m_pNext;
            Swap(pslow, pfast);                   //pslow指针指向比基准小的结点组成的链表的最后一个结点
			__swap_ptr(&pslow, &pfast);
        }  
		if (ptemp == pEndFlag || ptemp == NULL)
		{
			break;
		}
        pfast = ptemp1;
    }  
	
	BOOL32 bIsPivotLeft = FALSE, bIsPivotRight = FALSE;
	if (pslow == begin)
	{
		bIsPivotLeft = TRUE;   //轴值的位置在最左边，则此时不需要对轴值左边的部分排序
	}
	if (pslow == end)
	{
		bIsPivotRight = TRUE;
		end = begin;
	}
    Swap(pslow, begin);   //此时pslow就是基准的位置，所以要与基准（begin结点）交换
	__swap_ptr(&pslow, &begin);
	
	ListNode<T> *pLeftEnd = pslow->m_pPrev;
	ListNode<T> *pRightBegin = pslow->m_pNext;
	if (bIsPivotLeft == FALSE)
	{
		__sort_list(begin, pLeftEnd);     //ptemp为左右两部分分割点（基准）的前一个结点 
	}
 
	if (bIsPivotRight == FALSE)
	{
		__sort_list(pRightBegin, end);     //右部分是比基准大的结点组成的链表
	}
};

template <class T>
BOOL32 List<T>::Swap(Position pos1, Position pos2)/*任意交换两个结点*/ 
{
	if (pos1 == NULL || pos2 == NULL)
	{
		return FALSE;
	}
	if(pos1 == pos2)
	{
		return TRUE;
	}

	ListNode<T> *temp, *temp1; 

	//对pos1,pos2调整一下位置：pos1放前面的结点, pos2放后面的结点
	if (pos2 == m_pHead && pos1 == m_pTail)
	{
		pos1 = m_pHead;
		pos2 = m_pTail;
	}
	else if (pos2 == m_pHead)
	{
		pos2 = pos1;
		pos1 = m_pHead;
	}
	else if (pos1 == m_pTail)
	{
		pos1 = pos2;
		pos2 = m_pTail;
	}

	if (pos2->m_pNext == pos1)
	{
		ListNode<T> *temp1 = pos1;
		pos1 = pos2;
		pos2 = temp1;
	}

	if(pos1 == m_pHead && pos2 == m_pTail)/*首和尾巴的交换*/ 
	{ 
		if(pos1->m_pNext==pos2)/*只有两个结点的情况下*/ 
		{ 
			pos2->m_pNext=pos1; 
			pos2->m_pPrev=NULL; 
			pos1->m_pPrev=pos2; 
			pos1->m_pNext=NULL; 
			m_pHead = pos2; 
			m_pTail = pos1;
		} 
		else/*有间隔的首尾交换*/ 
		{ 
			pos1->m_pNext->m_pPrev=pos2; 
			pos2->m_pPrev->m_pNext=pos1; 
			pos2->m_pNext=pos1->m_pNext; 
			pos1->m_pPrev=pos2->m_pPrev; 
			pos2->m_pPrev = pos1->m_pNext = NULL; 
			m_pHead = pos2;   /*尾结点成为头结点*/ 
			m_pTail = pos1;
		}
	}
	else if(pos2 == m_pTail)/*尾和任意一个交换*/ 
	{ 
		if(pos1->m_pNext==pos2)/*交换最后两个结点*/ 
		{ 
			pos1->m_pPrev->m_pNext=pos2;
			pos2->m_pPrev=pos1->m_pPrev;
			pos2->m_pNext=pos1; 
			pos1->m_pPrev=pos2; 
			pos1->m_pNext=NULL;
			m_pTail = pos1;
		} 
		else/*和前面其他结点交换*/ 
		{ 
			temp=pos2->m_pPrev; 
			temp->m_pNext=pos1; 
			pos1->m_pPrev->m_pNext=pos2; 
			pos1->m_pNext->m_pPrev=pos2; 
			pos2->m_pPrev=pos1->m_pPrev; 
			pos2->m_pNext=pos1->m_pNext; 
			pos1->m_pPrev=temp; 
			pos1->m_pNext=NULL; 
			m_pTail = pos1;
		} 
	} 
	else if(pos1 == m_pHead)/*头和任意一个交换*/ 
	{ 
		if(pos1->m_pNext==pos2)/*交换头两个结点*/ 
		{ 
			pos2->m_pNext->m_pPrev=pos1; 
			pos1->m_pNext=pos2->m_pNext; 
			pos1->m_pPrev=pos2; 
			pos2->m_pNext=pos1; 
			pos2->m_pPrev=NULL; 
			m_pHead=pos2; 
		} 
		else/*头结点和后面其他结点交换*/ 
		{ 
			temp=pos1->m_pNext; 
			temp->m_pPrev=pos2; 
			pos1->m_pPrev=pos2->m_pPrev; 
			pos1->m_pNext=pos2->m_pNext; 
			pos2->m_pPrev->m_pNext=pos1; 
			pos2->m_pNext->m_pPrev=pos1; 
			pos2->m_pNext=temp; 
			pos2->m_pPrev=NULL; 
			m_pHead=pos2;/*交换的结点成为头结点*/ 
		} 
	} 
	else/*当中的任意两个交换*/ 
	{ 
		if(pos1->m_pNext==pos2)/*交换连在一起的两个结点*/ 
		{ 
			temp=pos1->m_pPrev; 
			pos1->m_pPrev->m_pNext=pos2; 
			pos1->m_pNext->m_pPrev=pos2; 
			pos1->m_pPrev=pos2; 
			pos1->m_pNext=pos2->m_pNext; 
			pos2->m_pNext->m_pPrev=pos1; 
			pos2->m_pNext=pos1; 
			pos2->m_pPrev=temp; 
		} 
		else/*交换隔开的两个结点*/ 
		{ 
			temp1 = pos1->m_pPrev;
			pos1->m_pPrev->m_pNext=pos2; 
			pos1->m_pNext->m_pPrev=pos2; 
			pos1->m_pPrev=pos2->m_pPrev; 

			temp=pos1->m_pNext;
			pos1->m_pNext=pos2->m_pNext; 

			pos2->m_pPrev->m_pNext=pos1; 
			pos2->m_pNext->m_pPrev=pos1; 
			pos2->m_pNext=temp; 

			pos2->m_pPrev=temp1; 
		} 
	} 
	return TRUE;
}; 

// template <class T>
// List<T>::List()
// {
// 	m_pHead = NULL;
// 	m_pTail = NULL;
// };

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
BOOL32 List<T>::InsertBefore(Position pos, const T& tData)
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
BOOL32 List<T>::InsertAfter(Position pos, const T& tData)
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
		return TRUE;
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
BOOL32 List<T>::Delete(Position pos)
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
ListNode<T>* List<T>::Find(const T &tData)
{
	ListNode<T> *p = m_pHead;
	while(p != NULL && p->m_tData != tData)
	{
		p = p->m_pNext;
	}
	return (Position)p;
};

template <typename T>
T List<T>::GetData(const Position pos)
{
	return pos->m_tData;
};

template <typename T>
BOOL32 List<T>::SetData(Position pos, T tData)
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

template <typename T>
BOOL32 List<T>::Foreach(VisitFunc visit, void *ctx)
{
	if (visit == NULL) return FALSE;

	ListNode<T>* pos = m_pHead;
	BOOL32 bResult = FALSE;
	while(pos != NULL)
	{
		bResult = visit((void *)&(pos->m_tData), ctx);
		pos = pos->m_pNext;
	}
	return bResult;
};

template <typename T>
BOOL32 List<T>::operator==(const List<T> &tOther)
{
	if (GetSize() != tOther.GetSize())
	{
		return FALSE;
	}

	ListNode<T>* pos1 = m_pHead;
	ListNode<T>* pos2 = tOther.m_pHead;
	
	while(pos1 != NULL && pos2 != NULL)
	{
		if (pos1->m_tData != pos2->m_tData)
		{
			return FALSE;
		}
		pos1 = pos1->m_pNext;
		pos2 = pos2->m_pNext;
	}
	return TRUE;
};
#endif    /* _List_H */