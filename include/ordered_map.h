/*=====================================================================================
模块名  ：
文件名  ：ordered_map.h
相关文件：
实现功能：使用red-black tree实现map.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/21   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _ORDERED_MAP_H
#define _ORDERED_MAP_H

#include "rbtree.h"
#include "type_def.h"

template<class KEY, class VALUE>
class Map  
{
public:
	typedef RBNode<KEY,VALUE>* Position;
	typedef BOOL32 (*VisitFunc)(void *key, void *value, void *ctx);
	
	Map():m_rbTree(RBTree<KEY,VALUE>()) {};
	Map(const Map<KEY, VALUE>& tOther) {m_rbTree = tOther.m_rbTree;};
	Map<KEY, VALUE>& operator=(const Map<KEY, VALUE> &tOther)
	{
		if (this != &tOther)
		{
			MakeEmpty();
			m_rbTree = tOther.m_rbTree;
		}
		return *this;
	};

	BOOL32 Swap(Map<KEY, VALUE>& tOther);
	BOOL32 operator==(Map<KEY, VALUE> &tOther) {return m_rbTree == tOther.m_rbTree;};
	~Map() {MakeEmpty();};
	
	BOOL32 IsEmpty() {return m_rbTree.GetSize() == 0;};
	BOOL32 MakeEmpty() {m_rbTree.MakeEmpty(); return TRUE;};

	size_t GetSize() {return m_rbTree.GetSize();};
	
	VALUE& operator[](const KEY& key);
	BOOL32 Insert(const KEY& key, const VALUE& value) {m_rbTree.Insert(key,value);return TRUE;};
	
	Position Find(const KEY& key) {return m_rbTree.Find(key);};
	BOOL32 Find(const KEY& key, VALUE& value);
	BOOL32 Exist(const KEY& key);
	
	Position GetBeginPos() {return m_rbTree.GetInOrderFirstNode();};
	Position GetEndPos() {return m_rbTree.GetNullNode();}; 
	Position GetNextPos(Position pos) {return m_rbTree.InOrderSuccessor(pos);};
	KEY GetKey(Position pos) {return pos->m_tKey;};
	VALUE GetValue(Position pos) {return pos->m_tData;};
	
	BOOL32 Delete(const KEY& key) {m_rbTree.Delete(key);return TRUE;};
	
	BOOL32 Foreach(VisitFunc visit, void *ctx);
	BOOL32 ForeachPrint() {m_rbTree.InOrderPrintNode(); return TRUE;};
private:
	RBTree<KEY,VALUE> m_rbTree;
};

template<class KEY, class VALUE>
BOOL32 Map<KEY,VALUE>::Find(const KEY& key, VALUE& value)
{
	RBNode<KEY,VALUE>* tmp = m_rbTree.Find(key);
	RBNode<KEY,VALUE>* pNullNode = m_rbTree.GetNullNode();

	if (tmp != pNullNode)
	{
		value = tmp->m_tData;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

template<class KEY, class VALUE>
BOOL32 Map<KEY,VALUE>::Exist(const KEY& key)
{
	RBNode<KEY,VALUE>* tmp = m_rbTree.Find(key);
	RBNode<KEY,VALUE>* pNullNode = m_rbTree.GetNullNode();
	
	if (tmp != pNullNode)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

template<class KEY, class VALUE>
VALUE& Map<KEY, VALUE>::operator[](const KEY& key)
{
	RBNode<KEY,VALUE>* tmp = m_rbTree.Find(key);
	RBNode<KEY,VALUE>* pNullNode = m_rbTree.GetNullNode();
	
	if (tmp != pNullNode)
	{
		return tmp->m_tData;
	}
	else
	{
		m_rbTree.Insert(key, VALUE());
		return m_rbTree.Find(key)->m_tData;
	}
};

template<class KEY, class VALUE>
BOOL32 Map<KEY, VALUE>::Foreach(Map<KEY, VALUE>::VisitFunc visit, void *ctx)
{
	if (m_rbTree.GetSize() == 0) return TRUE;
	
	RBNode<int,int>* node = m_rbTree.GetInOrderFirstNode();
	RBNode<int,int>* pNullNode = m_rbTree.GetNullNode();
	while(node != pNullNode)
	{
		visit(&(node->m_tKey), &(node->m_tData), ctx);
		node = m_rbTree.InOrderSuccessor(node);
	}
	return TRUE;
};

template<class KEY, class VALUE>
BOOL32 Map<KEY, VALUE>::Swap(Map<KEY, VALUE>& tOther)
{
	RBTree<KEY,VALUE> tTreeTmp = tOther.m_rbTree;
	tOther.m_rbTree = this->m_rbTree;
	this->m_rbTree = tTreeTmp;
	return TRUE;
};

#endif