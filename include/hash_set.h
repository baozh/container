/*=====================================================================================
模块名  ：
文件名  ：hash_set.h
相关文件：
实现功能：使用hash table实现set.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/9   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _HASH_SET_H
#define _HASH_SET_H

#include "type_def.h"
#include "hash_map.h"

class CXVoid{};

template<class T, class HASH = CHashFunctor<T>, class EQUAL = CEqualFunctor<T> >
class HashSet
{
public:
	typedef HashNode<T, CXVoid>* Position;

	explicit HashSet(size_t nSetCapacity = DEFAULT_HASH_BUCKETS_NUM) : m_tHashMap(nSetCapacity){};
	~HashSet(){MakeEmpty();};
	
public:	
	HashSet(const HashSet<T, HASH, EQUAL>& tOther) { m_tHashMap = tOther.m_tHashMap;};

	HashSet<T, HASH, EQUAL>& operator=(const HashSet<T, HASH, EQUAL> &tOther)
	{
		if (this != &tOther)
		{
			m_tHashMap = tOther.m_tHashMap;
		}
		return *this;
	};
	BOOL32 Swap(HashSet<T, HASH, EQUAL>& tOther)
	{
		return m_tHashMap.Swap(tOther.m_tHashMap);
	};
	BOOL32 operator==(HashSet<T, HASH, EQUAL> &tOther)
	{
		return m_tHashMap == tOther.m_tHashMap;
	};

public:
	size_t GetSize() const {return m_tHashMap.GetSize();}
	
	void Insert(const T& key) {m_tHashMap.Insert(key, CXVoid());}
	BOOL32 Exist(const T& key) {return m_tHashMap.Exist(key);}
	BOOL32 Delete(const T& key) {return m_tHashMap.Delete(key);}
	
	void MakeEmpty() {m_tHashMap.MakeEmpty();}
	BOOL32 IsEmpty() const { return m_tHashMap.IsEmpty(); }

	Position GetBeginPos() {return m_tHashMap.GetBeginPos();};
	Position GetEndPos() {return NULL;}; 
	Position GetNextPos(Position pos) {return m_tHashMap.GetNextPos(pos);};
	T GetItem(Position pos) {return pos->m_tKey;};

	BOOL32 ForeachPrint()
	{
		if (m_tHashMap.IsEmpty()) return TRUE;
		
		HashNode<T, CXVoid>* pCur = m_tHashMap.GetBeginPos();
		HashNode<T, CXVoid>* pEnd = m_tHashMap.GetEndPos();
		while (pCur != pEnd)
		{
			std::cout<<"item:"<<pCur->m_tKey<<std::endl;
			pCur = m_tHashMap.GetNextPos(pCur);
		}
		return TRUE;
	};

private:
	HashMap<T, CXVoid, HASH, EQUAL> m_tHashMap;
};

typedef HashSet<String_sso, CMsHashXStr> CStrHashSet;

#endif
