/*=====================================================================================
ģ����  ��
�ļ���  ��ordered_set.h
����ļ���
ʵ�ֹ��ܣ�ʹ��red-black treeʵ��set.
����    ��������(bao.z.h.2008@gmail.com)
��Ȩ    ��
---------------------------------------------------------------------------------------
�޸ļ�¼��
����        �汾    �޸���      �߶���      �޸ļ�¼
2014/10/21   V1.0    ������                    ����
========================================================================================*/

#ifndef _ORDERED_SET_H
#define _ORDERED_SET_H

#include "type_def.h"
#include "rbtree.h"

class CSetVoid{};

template<class T>
class Set
{
public:
	typedef RBNode<T,CSetVoid>* Position;
	
	Set():m_rbTree(RBTree<T,CSetVoid>()) {};
	~Set(){MakeEmpty();};
	
public:	
	Set(const Set<T>& tOther) { m_rbTree = tOther.m_rbTree;};
	
	Set<T>& operator=(const Set<T> &tOther)
	{
		if (this != &tOther)
		{
			m_rbTree = tOther.m_rbTree;
		}
		return *this;
	};
	BOOL32 Swap(Set<T>& tOther)
	{
		RBTree<T, CSetVoid> tTreeTmp = tOther.m_rbTree;
		tOther.m_rbTree = this->m_rbTree;
		this->m_rbTree = tTreeTmp;
		return TRUE;
	};

	BOOL32 operator==(Set<T> &tOther)
	{
		return m_rbTree == tOther.m_rbTree;
	};
	
public:
	size_t GetSize() const {return m_rbTree.GetSize();};
	
	void Insert(const T& key) {m_rbTree.Insert(key, CSetVoid());};
	BOOL32 Exist(const T& key);
	BOOL32 Delete(const T& key) {return m_rbTree.Delete(key);};
	
	void MakeEmpty() {m_rbTree.MakeEmpty();};
	BOOL32 IsEmpty() const { return m_rbTree.IsEmpty();};
	
	Position GetBeginPos() {return m_rbTree.GetInOrderFirstNode();};
	Position GetEndPos() {return m_rbTree.GetNullNode();}; 
	Position GetNextPos(Position pos) {return m_rbTree.InOrderSuccessor(pos);};
	T GetItem(Position pos) {return pos->m_tKey;};
	
	BOOL32 ForeachPrint() {m_rbTree.InOrderPrintNode(); return TRUE;};
private:
	RBTree<T, CSetVoid> m_rbTree;
};

template<class T>
BOOL32 Set<T>::Exist(const T& key)
{
	RBNode<T, CSetVoid>* tmp = m_rbTree.Find(key);
	RBNode<T, CSetVoid>* pNullNode = m_rbTree.GetNullNode();
	
	if (tmp != pNullNode)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

#endif