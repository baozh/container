/*=====================================================================================
模块名  ：
文件名  ：rbtree.h
相关文件：
实现功能：红黑树的实现.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/23   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _RB_TREE_H
#define _RB_TREE_H 

#include <iostream>
#include "type_def.h"

template<class KEY,class VALUE>
class RBNode
{  
public:
	enum NodeColor{__COLOR_RED, __COLOR_BLACK}; 
	NodeColor m_tColor;
	RBNode* m_pRChild;
	RBNode* m_pLChild;
	RBNode* m_pParent;
	KEY m_tKey;
	VALUE m_tData;

	RBNode()  
	{  
		m_pRChild = 0;  
		m_pLChild = 0;  
		m_pParent = 0;
	};
};

template<class KEY,class VALUE>
class RBTree
{
public:  
        RBTree();
		RBTree(const RBTree& tOther);
        RBTree& operator=(const RBTree& tOther);
		BOOL32 operator==(const RBTree& tOther);
		~RBTree() {MakeEmpty();};

        BOOL32 IsEmpty();
		BOOL32 MakeEmpty();
        RBNode<KEY,VALUE>* Find(KEY key);
		BOOL32 IsUsefullNode(RBNode<KEY,VALUE>* node) {return node != m_pNullNode;};

        BOOL32 Insert(KEY key,VALUE data);
        BOOL32 Delete(KEY key);

		u32 GetSize() {return m_dwSize;};

		RBNode<KEY,VALUE>* GetInOrderFirstNode() const;
		RBNode<KEY,VALUE>* GetInOrderLastNode() const;
        RBNode<KEY,VALUE>* InOrderPredecessor(RBNode<KEY,VALUE>* node) const;  
        RBNode<KEY,VALUE>* InOrderSuccessor(RBNode<KEY,VALUE>* node) const;
		RBNode<KEY,VALUE>* GetNullNode() const {return m_pNullNode;};
        
        void InOrderPrintNode()  {__in_order_traverse(m_pRoot);};    //for debug 
    private:
		RBNode<KEY,VALUE>* __clone_node(RBNode<KEY,VALUE>* node);
		RBNode<KEY,VALUE>* __copy_nodes(RBNode<KEY,VALUE>* src, const RBTree& tOther, RBNode<KEY,VALUE>* dst);
		void __copy(const RBTree& tOther);
		void __insert_rebalance(RBNode<KEY,VALUE>* node);
        BOOL32 __rotate_left(RBNode<KEY,VALUE>* node);
        BOOL32 __rotate_right(RBNode<KEY,VALUE>* node);
		void __delete_rebalance(RBNode<KEY,VALUE>* node);
        void __free_tree_node(RBNode<KEY,VALUE>* node);
		void __in_order_traverse(RBNode<KEY,VALUE>* node);
    private:  
        RBNode<KEY,VALUE>* m_pNullNode;  
        RBNode<KEY,VALUE>* m_pRoot;  
		u32 m_dwSize;
};

template<class KEY,class VALUE>
RBTree<KEY, VALUE>::RBTree()
{
	m_dwSize = 0;
	this->m_pNullNode = new RBNode<KEY, VALUE>();
	this->m_pRoot = m_pNullNode;
	this->m_pNullNode->m_pRChild = this->m_pRoot;  
	this->m_pNullNode->m_pLChild = this->m_pRoot;  
	this->m_pNullNode->m_pParent = this->m_pRoot;  
	this->m_pNullNode->m_tColor = RBNode<KEY, VALUE>::__COLOR_BLACK;  
};

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::__clone_node(RBNode<KEY,VALUE>* node)
{
	RBNode<KEY,VALUE>* pNewNode = new RBNode<KEY,VALUE>();
	pNewNode->m_tKey = node->m_tKey;
	pNewNode->m_tData = node->m_tData;
	pNewNode->m_tColor = node->m_tColor;  
	pNewNode->m_pRChild = m_pNullNode; 
	pNewNode->m_pLChild = m_pNullNode;
	pNewNode->m_pParent = m_pNullNode;
	return pNewNode;
};

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::__copy_nodes(RBNode<KEY,VALUE>* src, const RBTree& tOther, RBNode<KEY,VALUE>* dst)
{
	RBNode<KEY,VALUE>* __top = __clone_node(src);
	__top->m_pParent = dst;
	
	if (src->m_pRChild)
		__top->m_pRChild = __copy_nodes(src->m_pRChild, tOther, __top);
	
	dst = __top;
	src = src->m_pLChild;
	
	while (src != tOther.m_pNullNode) 
	{
		RBNode<KEY,VALUE>* __y = __clone_node(src);
		dst->m_pLChild = __y;
		__y->m_pParent = dst;
		if (src->m_pRChild)
			__y->m_pRChild = __copy_nodes(src->m_pRChild,tOther, __y);
		dst = __y;
		src = src->m_pLChild;
	}
	
	return __top;
};

template<class KEY,class VALUE>
void RBTree<KEY, VALUE>::__copy(const RBTree& tOther)
{
	if (tOther.m_dwSize == 0) return;

	RBNode<int,int>* node = tOther.GetInOrderFirstNode();
	RBNode<int,int>* pNullNode = tOther.GetNullNode();
	while(node != pNullNode)
	{
	 	this->Insert(node->m_tKey, node->m_tData);
	  	node = tOther.InOrderSuccessor(node);
	}
	m_dwSize = tOther.m_dwSize;
};

template<class KEY,class VALUE>
RBTree<KEY, VALUE>::RBTree(const RBTree<KEY, VALUE>& tOther)
{
	m_dwSize = 0;
	this->m_pNullNode = new RBNode<KEY, VALUE>();
	this->m_pRoot = m_pNullNode;
	this->m_pNullNode->m_pRChild = this->m_pRoot;
	this->m_pNullNode->m_pLChild = this->m_pRoot;
	this->m_pNullNode->m_pParent = this->m_pRoot;
	this->m_pNullNode->m_tColor = RBNode<KEY, VALUE>::__COLOR_BLACK; 
	__copy(tOther);
};

template<class KEY,class VALUE>
RBTree<KEY,VALUE>& RBTree<KEY, VALUE>::operator=(const RBTree<KEY, VALUE>& tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		__copy(tOther);
	}
	return *this;
};

template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::operator==(const RBTree<KEY, VALUE>& tOther)
{
	if (m_dwSize != tOther.m_dwSize)
	{
		return FALSE;
	}

	RBNode<int,int>* node = tOther.GetInOrderFirstNode();
	RBNode<int,int>* pNullNode = tOther.GetNullNode();
	while(node != pNullNode)
	{
		RBNode<int,int>* tmp = Find(node->m_tKey);
		if (tmp == m_pNullNode || (!(tmp->m_tData == node->m_tData)))
		{
			return FALSE;
		}
		node = tOther.InOrderSuccessor(node);
	}
	return TRUE;
};

template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::IsEmpty()  
{  
	if(this->m_pRoot == this->m_pNullNode)
	{  
		return TRUE;
	}  
	else  
	{
		return FALSE;
	}  
};

template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::MakeEmpty()
{
	__free_tree_node(m_pRoot);
	
	this->m_pRoot = m_pNullNode;
	this->m_pNullNode->m_pRChild = this->m_pRoot;
	this->m_pNullNode->m_pLChild = this->m_pRoot;
	this->m_pNullNode->m_pParent = this->m_pRoot;
	this->m_pNullNode->m_tColor = RBNode<KEY, VALUE>::__COLOR_BLACK; 
	m_dwSize = 0;
	return TRUE;
};

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::Find(KEY key)
{
	if (m_dwSize == 0) return m_pNullNode;

	RBNode<KEY,VALUE>* pParent = m_pNullNode;  //保存 最后一个>=key的结点(最小的>=key的那个结点)
	RBNode<KEY,VALUE>* pCur = m_pRoot;
	while(pCur != m_pNullNode)
	{  
		if(!(pCur->m_tKey < key))
		{  
			pParent = pCur;
			pCur  = pCur->m_pLChild;  //比当前的小，往左
		}  
		else  
		{  
			pCur = pCur->m_pRChild;  //比当前的大，往右 
		}  
	}

	if (key < pParent->m_tKey)
	{
		return m_pNullNode;
	}
	else
	{
		return pParent;
	}
};  

template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::Insert(KEY key,VALUE data)  
{  
	if (Find(key) != m_pNullNode)
	{
		return FALSE;
	}

	RBNode<KEY,VALUE>* pInsertPos = m_pNullNode;  
	RBNode<KEY,VALUE>* pCur = m_pRoot;  
	while(pCur != m_pNullNode)  
	{  
		pInsertPos = pCur;  
		if(key < pCur->m_tKey)  
		{  
			pCur = pCur->m_pLChild;
		}  
		else
		{  
			pCur = pCur->m_pRChild;
		} 
	}  
	
	RBNode<KEY,VALUE>* pNewNode = new RBNode<KEY,VALUE>();
	pNewNode->m_tKey = key;
	pNewNode->m_tData = data;
	pNewNode->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
	pNewNode->m_pRChild = m_pNullNode; 
	pNewNode->m_pLChild = m_pNullNode;
	if(pInsertPos == m_pNullNode)   //插入的是一颗空树
	{  
		m_pRoot = pNewNode;  
		m_pRoot->m_pParent = m_pNullNode;  
		m_pNullNode->m_pLChild = m_pRoot;  
		m_pNullNode->m_pRChild = m_pRoot;  
		m_pNullNode->m_pParent = m_pRoot;
	}  
	else  
	{  
		if(key < pInsertPos->m_tKey)  
		{  
			pInsertPos->m_pLChild = pNewNode;  
		}  
		else  
		{  
			pInsertPos->m_pRChild = pNewNode;  
		}  
		pNewNode->m_pParent = pInsertPos;  
	}  
	__insert_rebalance(pNewNode);    //修复红黑树性质

	m_dwSize++;
	return TRUE;
};

template<class KEY,class VALUE>
void RBTree<KEY, VALUE>::__insert_rebalance(RBNode<KEY,VALUE>* node)  
{  
	while(node->m_pParent->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)  
	{  
		if(node->m_pParent == node->m_pParent->m_pParent->m_pLChild)
		{  
			RBNode<KEY,VALUE>* pUncle = node->m_pParent->m_pParent->m_pRChild;  
			if(pUncle->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)   //case1，z的叔叔y是红色
			{  
				node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
				pUncle->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
				node->m_pParent->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
				node = node->m_pParent->m_pParent;  
			}  
			else if(pUncle->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK )  //case2：z的叔叔y是黑色
			{  
				if(node == node->m_pParent->m_pRChild) //且z是右孩子
				{  
					node = node->m_pParent;  
					__rotate_left(node);  
				}  
				else                 //case3：z的叔叔y是黑色的，但z是左孩子
				{  
					node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					node->m_pParent->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
					__rotate_right(node->m_pParent->m_pParent);  
				}
			}
		}
		else //针对为case1中，z的父亲现在作为祖父的右孩子了的情况
		{  
			RBNode<KEY,VALUE>* pUncle = node->m_pParent->m_pParent->m_pLChild;  
			if(pUncle->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)  
			{  
				node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
				pUncle->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
				pUncle->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
				node = node->m_pParent->m_pParent;  
			}  
			else if(pUncle->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK)  
			{  
				if(node == node->m_pParent->m_pLChild)  
				{  
					node = node->m_pParent;  
					__rotate_right(node); 
				}  
				else  
				{  
					node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					node->m_pParent->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
					__rotate_left(node->m_pParent->m_pParent);
				}  
			}  
		}  
	}  
	m_pRoot->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
};  

template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::__rotate_left(RBNode<KEY,VALUE>* node)  
{  
	if(node==m_pNullNode || node->m_pRChild==m_pNullNode)  
	{  
		return FALSE; 
	}  
	RBNode<KEY,VALUE>* pLowerRight = node->m_pRChild;  
	pLowerRight->m_pParent = node->m_pParent;  
	node->m_pRChild = pLowerRight->m_pLChild;  
	if(pLowerRight->m_pLChild != m_pNullNode)  
	{  
		pLowerRight->m_pLChild->m_pParent = node;  
	}  
	if(node->m_pParent == m_pNullNode) //rotate node is root  
	{  
		m_pRoot = pLowerRight;  
		m_pNullNode->m_pLChild = m_pRoot;  
		m_pNullNode->m_pRChild = m_pRoot;
	}  
	else  
	{  
		if(node == node->m_pParent->m_pLChild)  
		{  
			node->m_pParent->m_pLChild = pLowerRight;  
		}  
		else  
		{  
			node->m_pParent->m_pRChild = pLowerRight;  
		}  
	}  
	node->m_pParent = pLowerRight;  
	pLowerRight->m_pLChild = node; 
	return TRUE;
};  
  
template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::__rotate_right(RBNode<KEY,VALUE>* node)  
{  
	if(node==m_pNullNode || node->m_pLChild==m_pNullNode)  
	{  
		return FALSE; 
	}  

	RBNode<KEY,VALUE>* pLowerRight = node->m_pLChild;  
	node->m_pLChild = pLowerRight->m_pRChild;  
	pLowerRight->m_pParent = node->m_pParent;  
	if(pLowerRight->m_pRChild != m_pNullNode)  
	{  
		pLowerRight->m_pRChild->m_pParent = node;  
	}  
	if(node->m_pParent == m_pNullNode) //node is root  
	{  
		m_pRoot = pLowerRight;  
		m_pNullNode->m_pLChild = m_pRoot;  
		m_pNullNode->m_pRChild = m_pRoot;
	}  
	else  
	{  
		if(node==node->m_pParent->m_pRChild)  
		{  
			node->m_pParent->m_pRChild = pLowerRight;  
		}  
		else  
		{  
			node->m_pParent->m_pLChild = pLowerRight;  
		}  
	}  
	node->m_pParent = pLowerRight;  
	pLowerRight->m_pRChild = node;  
	return TRUE;
};  

template<class KEY,class VALUE>
BOOL32 RBTree<KEY, VALUE>::Delete(KEY key)  
{  
	RBNode<KEY,VALUE>* pDeleteNode = Find(key);  
	if(pDeleteNode == m_pNullNode)  
	{  
		return FALSE;
	}  
	if(pDeleteNode->m_pLChild != m_pNullNode && pDeleteNode->m_pRChild != m_pNullNode)  
	{  
		RBNode<KEY,VALUE>* pSuccessor = InOrderSuccessor(pDeleteNode);  
		pDeleteNode->m_tData = pSuccessor->m_tData;  
		pDeleteNode->m_tKey = pSuccessor->m_tKey;  
		pDeleteNode = pSuccessor;  
	}
	RBNode<KEY,VALUE>* pDeleteNodeChild;  
	if(pDeleteNode->m_pRChild != m_pNullNode)  
	{  
		pDeleteNodeChild = pDeleteNode->m_pRChild;  
	}  
	else if(pDeleteNode->m_pLChild != m_pNullNode)  
	{  
		pDeleteNodeChild = pDeleteNode->m_pLChild;  
	}  
	else  
	{  
		pDeleteNodeChild = m_pNullNode;  
	}  
	pDeleteNodeChild->m_pParent = pDeleteNode->m_pParent;  
	if(pDeleteNode->m_pParent == m_pNullNode)   //delete root node  
	{  
		m_pRoot = pDeleteNodeChild;
		m_pNullNode->m_pParent = m_pRoot;
		m_pNullNode->m_pLChild = m_pRoot;
		m_pNullNode->m_pRChild = m_pRoot;
	}  
	else if(pDeleteNode == pDeleteNode->m_pParent->m_pRChild)  
	{  
		pDeleteNode->m_pParent->m_pRChild = pDeleteNodeChild;  
	}  
	else  
	{  
		pDeleteNode->m_pParent->m_pLChild = pDeleteNodeChild;  
	}  
	if(pDeleteNode->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK && 
		!(pDeleteNodeChild == m_pNullNode && pDeleteNodeChild->m_pParent == m_pNullNode))  
	{  
		__delete_rebalance(pDeleteNodeChild);  
	}  
	delete pDeleteNode;  

	m_dwSize--;
	return TRUE;  
};  

template<class KEY,class VALUE>
void RBTree<KEY, VALUE>::__delete_rebalance(RBNode<KEY,VALUE>* node)  
{  
	while(node != m_pRoot && node->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK)  
	{  
		if(node == node->m_pParent->m_pLChild)  
		{  
			RBNode<KEY,VALUE>* pBrother = node->m_pParent->m_pRChild;
			if(pBrother->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)   //case1：x的兄弟w是红色的
			{  
				pBrother->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
				node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
				__rotate_left(node->m_pParent);  
			}  
			else     //case2：x的兄弟w是黑色的
			{  
				if(pBrother->m_pLChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK 
					&& pBrother->m_pRChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK)  
					//且w的俩个孩子都是黑色的  
				{  
					pBrother->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
					node = node->m_pParent;  
				}  
				else if(pBrother->m_pRChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK)  
					//case3：x的兄弟w是黑色的，w的右孩子是黑色(w的左孩子是红色)
				{  
					pBrother->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
					pBrother->m_pLChild->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					__rotate_right(pBrother);  
				}  
				else if(pBrother->m_pRChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)  
					//case4：x的兄弟w是黑色的，且w的右孩子时红色的
				{  
					pBrother->m_tColor = node->m_pParent->m_tColor;  
					node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					pBrother->m_pRChild->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					__rotate_left(node->m_pParent);  
					node = m_pRoot;  
				}  
			}  
		}  
		else  //下述情况针对上面的case1中,node作为右孩子而阐述  
			//同样,原理一致,只是遇到左旋改为右旋，遇到右旋改为左旋,即可.其它代码不变.
		{  
			RBNode<KEY,VALUE>* pBrother = node->m_pParent->m_pLChild;  
			if(pBrother->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)  
			{  
				pBrother->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
				node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
				__rotate_right(node->m_pParent);  
			}  
			else  
			{  
				if(pBrother->m_pLChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK 
					&& pBrother->m_pRChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK)  
				{  
					pBrother->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
					node = node->m_pParent;  
				}  
				else if(pBrother->m_pLChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_BLACK)  
				{  
					pBrother->m_tColor = RBNode<KEY,VALUE>::__COLOR_RED;  
					pBrother->m_pRChild->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					__rotate_left(pBrother);  
				}  
				else if(pBrother->m_pLChild->m_tColor == RBNode<KEY,VALUE>::__COLOR_RED)  
				{  
					pBrother->m_tColor = node->m_pParent->m_tColor;  
					node->m_pParent->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					pBrother->m_pLChild->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;  
					__rotate_right(node->m_pParent);  
					node = m_pRoot;  
				}  
			}  
		}  
	}  
	m_pNullNode->m_pParent = m_pRoot;
	node->m_tColor = RBNode<KEY,VALUE>::__COLOR_BLACK;
}; 

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::GetInOrderFirstNode() const
{
	RBNode<KEY,VALUE>* node = m_pRoot;
	while(node->m_pLChild != m_pNullNode)
	{
		node = node->m_pLChild;
	}
	return node;
};

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::GetInOrderLastNode() const
{
	RBNode<KEY,VALUE>* node = m_pRoot;
	while(node->m_pRChild != m_pNullNode)
	{
		node = node->m_pRChild;
	}
	return node;
};

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::InOrderPredecessor(RBNode<KEY,VALUE>* node) const  
{  
	if(node==m_pNullNode)
	{  
		return m_pNullNode;  
	}  
	RBNode<KEY,VALUE>* pResult = node->m_pLChild;
	while(pResult != m_pNullNode) 
	{  
		if(pResult->m_pRChild != m_pNullNode)       
		{  
			pResult = pResult->m_pRChild;  
		}  
		else  
		{  
			break;  
		}  
	}            //after while loop result==null or result's right child is null  
	if(pResult == m_pNullNode)  
	{  
		RBNode<KEY,VALUE>* pCur = node->m_pParent;  
		pResult = node;  
		while(pCur != m_pNullNode && pResult == pCur->m_pLChild)  
		{  
			pResult = pCur;  
			pCur = pCur->m_pParent;  
		}  
		pResult = pCur;         // first right parent or null  
	}  
	return pResult;  
};  

template<class KEY,class VALUE>
RBNode<KEY,VALUE>* RBTree<KEY, VALUE>::InOrderSuccessor(RBNode<KEY,VALUE>* node) const
{  
	if(node == m_pNullNode)
	{  
		return m_pNullNode;  
	}  
	RBNode<KEY,VALUE>* pResult = node->m_pRChild; 
	while(pResult != m_pNullNode) 
	{  
		if(pResult->m_pLChild != m_pNullNode)       
		{  
			pResult = pResult->m_pLChild;  
		}  
		else  
		{  
			break;  
		}  
	}     //after while loop result==null or result's left child is null  
	if(pResult == m_pNullNode)  
	{  
		RBNode<KEY,VALUE>* pCur = node->m_pParent;  
		pResult = node;  
		while(pCur!=m_pNullNode && pResult == pCur->m_pRChild)  
		{  
			pResult = pCur;  
			pCur = pCur->m_pParent;  
		}  
		pResult = pCur;         //first parent's left or null  
	}
	return pResult;  
};  

template<class KEY,class VALUE>
void RBTree<KEY, VALUE>::__free_tree_node(RBNode<KEY,VALUE>* node)  
{  
	if(node==m_pNullNode)  
	{  
		return;  
	}  
	else  
	{  
		__free_tree_node(node->m_pLChild);
		__free_tree_node(node->m_pRChild);
		delete node;
	}
};

template<class KEY,class VALUE>
void RBTree<KEY, VALUE>::__in_order_traverse(RBNode<KEY,VALUE>* node)  
{  
	if(node==m_pNullNode)  
	{  
		return;  
	}  
	else  
	{  
		__in_order_traverse(node->m_pLChild);
		std::cout<<"key:"<<node->m_tKey<<std::endl;
		__in_order_traverse(node->m_pRChild);
	}  
};

#endif /*_RB_TREE_H*/