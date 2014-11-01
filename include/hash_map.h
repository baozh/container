/*=====================================================================================
模块名  ：
文件名  ：hash_map.h
相关文件：
实现功能：使用hash table实现map, string map.
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/9   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _HASH_MAP_H
#define _HASH_MAP_H

#include "type_def.h"
#include "paddingassert.h"
#include "darray.h"

typedef unsigned char      byte;

//hash functions
inline size_t hash_mysql(const char* key,   size_t len)  
{ 
	if (key == 0)  return 0;
	
	size_t nr=1, nr2=4; 
	while (len--) 
	{ 
		nr^= (((nr & 63)+nr2)*((size_t) (byte) *key++))+ (nr << 8); 
		nr2+=3; 
	} 
	return((size_t) nr);   
}

inline size_t hash_string(const char* pStr, size_t len)
{
	if (pStr == 0)  return 0;
	size_t nHash = 0;
	while (*pStr)  nHash = (nHash<<5) + nHash + *pStr++;
	
	return nHash;
}

//hash template
//primitive type hash function: int, short, long..., or object but not string
template<class KEY>
struct CHashFunctor
{
	size_t operator()(const KEY& key) const
	{
		//convert object to string buffer forcelly
		//KEY需要注意字节对齐，要么是一字节对齐，要么和编译器设定的对齐方式一致，
        //必须保证KEY对象中没有padding字节，否则对同一个KEY会有不同的hansh值
		
		//编译期检测KEY是否packed，如果不是会编译错误
		pack_assert<KEY>();
		int nLen = sizeof(KEY);
		const char* pKey = reinterpret_cast<const char*>(&key);
		return hash_mysql(pKey, nLen);
	}	
};

//equal template
template<class KEY>
struct CEqualFunctor
{
	BOOL32 operator()(const KEY& key1, const KEY& key2) const
	{
		return (key1 == key2);
	}
};

static const size_t      DEFAULT_HASH_BUCKETS_NUM = 100;
static const size_t      DEFAULT_HASH_FREELIST_SIZE = 100;

template<class KEY, class VALUE>
class HashNode
{
public:
	HashNode<KEY,VALUE> *m_pNext;
	size_t	m_dwHashValue;    //hash值,其实也是hash桶的索引
	KEY m_tKey;    //find(key)的时候，需要遍历链表，直到key==other.key
	VALUE m_tValue;

	HashNode():m_pNext(NULL), m_dwHashValue(0), m_tKey(KEY()), m_tValue(VALUE()) {};
	~HashNode() {m_pNext = NULL; m_dwHashValue=0; m_tKey = KEY(); m_tValue = VALUE();};
};

// 注意：假设 long 至少有 32 bits
static const u32 DEFAULT_PRIME_NUM = 28;
static const u32 __primlist[DEFAULT_PRIME_NUM] =
{
	53,         97,           193,         389,       769,
	1543,       3079,         6151,        12289,     24593,
	49157,      98317,        196613,      393241,    786433,
	1572869,    3145739,      6291469,     12582917,  25165843,
	50331653,   100663319,    201326611,   402653189, 805306457, 
	1610612741, 3221225473ul, 4294967291ul
};

// 以下找出上述28个质数之中，最接近并大于n的那个质数
inline u32 __hash_next_prime(u32 n)
{
	for (u32 dwIndex = 0; dwIndex < DEFAULT_PRIME_NUM; dwIndex++)
	{
		if (__primlist[dwIndex] >= n)
		{
			return __primlist[dwIndex];
		}
	}
	return __primlist[DEFAULT_PRIME_NUM-1];
}

//map
template<class KEY, class VALUE, class HASH = CHashFunctor<KEY>, class EQUAL = CEqualFunctor<KEY> >
class HashMap  
{
public:
	typedef HashNode<KEY, VALUE>* Position;
	typedef BOOL32 (*VisitFunc)(void *key, void *value, void *ctx);
	
	HashMap(u32 dwBucketNum = DEFAULT_HASH_BUCKETS_NUM):m_dwSize(0), m_pFreeList(NULL)
	{
		__initialize_buckets(dwBucketNum);
		if (m_pFreeList == NULL)
		{
			m_pFreeList = __alloc_freelist();
		}
	};

	HashMap(const HashMap<KEY, VALUE, HASH, EQUAL>& tOther);
	HashMap<KEY, VALUE, HASH, EQUAL>& operator=(const HashMap<KEY, VALUE, HASH, EQUAL> &tOther);
	BOOL32 Swap(HashMap<KEY, VALUE, HASH, EQUAL>& other);
	BOOL32 operator==(HashMap<KEY, VALUE, HASH, EQUAL> &tOther);
	~HashMap() {MakeEmpty();};
	
	BOOL32 IsEmpty() {return (m_dwSize == 0);};
	BOOL32 MakeEmpty();
	size_t GetSize() {return m_dwSize;};
	size_t GetBucketsNum() {return m_tBuckets.GetCapacity();};
	
	VALUE& operator[](const KEY& key);
	BOOL32 Insert(const KEY& key, const VALUE& value);

	Position Find(const KEY& key);
	BOOL32 Find(const KEY& key, VALUE& value);
	BOOL32 Exist(const KEY& key);

	Position GetBeginPos();
	Position GetEndPos() {return NULL;}; 
	Position GetNextPos(Position pos);
	KEY GetKey(Position pos) {return pos->m_tKey;};
	VALUE GetValue(Position pos) {return pos->m_tValue;};

	BOOL32 ResizeBuckets(u32 dwBucketNum) {__resize_buckets(dwBucketNum);};

	BOOL32 Delete(const KEY& key);

	BOOL32 Foreach(VisitFunc visit, void *ctx);
	BOOL32 ForeachPrint();
private:
	void __copy(const HashMap<KEY, VALUE, HASH, EQUAL>& tOther);
	void __initialize_buckets(u32 dwBucketsNum);
	Position __alloc_freelist(u32 dwNodeNum = DEFAULT_HASH_FREELIST_SIZE);
	Position __get_freenode();
	void ___destory_freenode(Position node);
	void __destory_freelist(Position listpos);
	size_t __get_hash_code(const KEY& key, u32 dwBucketsSize = 0);
	Position __get_key_node(const KEY& key, u32& dwHash);
	void __resize_buckets(u32 dwBucketsNum);
private:
	DArray<HashNode<KEY,VALUE>* > m_tBuckets;   //hash桶
	size_t m_dwSize;         //hash table中已存的 对象数量
	HashNode<KEY,VALUE>* m_pFreeList;   //已释放的结点,预先分配的空闲结点,组成的链表
};

template<class KEY, class VALUE, class HASH, class EQUAL>
void HashMap<KEY, VALUE, HASH, EQUAL>::__initialize_buckets(u32 dwBucketsNum)
{
	assert(m_dwSize == 0);
	
	dwBucketsNum = __hash_next_prime(dwBucketsNum);
	m_tBuckets.SetCapacity(dwBucketsNum);
	for (u32 dwIndex = 0; dwIndex < dwBucketsNum; dwIndex++)
	{
		m_tBuckets[dwIndex] = NULL;
	}
	m_dwSize = 0;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashNode<KEY, VALUE>* HashMap<KEY, VALUE, HASH, EQUAL>::__alloc_freelist(u32 dwNodeNum)
{
	dwNodeNum = (dwNodeNum == 0) ? DEFAULT_HASH_FREELIST_SIZE : dwNodeNum;
	assert(dwNodeNum > 0);
	
	HashNode<KEY, VALUE>* pListHead = NULL;
	for(u32 dwIndex = 0; dwIndex < dwNodeNum; dwIndex++)
	{
		HashNode<KEY, VALUE>* pFreeNode = new HashNode<KEY, VALUE>;
		if (pFreeNode == NULL)
		{
			__destory_freelist(pListHead);
			pListHead = NULL;
			std::cout<<"[__alloc_freelist()]out of memory, failed to alloc space!"<<std::endl;
			return NULL;
		}
		
		//插入作为链表的第一个结点
		pFreeNode->m_pNext = pListHead;
		pListHead = pFreeNode;
	}
	return pListHead;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
void HashMap<KEY, VALUE, HASH, EQUAL>::__destory_freelist(HashNode<KEY,VALUE>* listpos)
{
	if (listpos == NULL) return;

	HashNode<KEY,VALUE>* pCur = listpos;
	HashNode<KEY,VALUE>* pNext = pCur->m_pNext;
	while (pCur != NULL)
	{
		pNext = pCur->m_pNext;
		delete pCur;
		pCur = pNext;
	}
};

template<class KEY, class VALUE, class HASH, class EQUAL>
VALUE& HashMap<KEY, VALUE, HASH, EQUAL>::operator[](const KEY& key)
{
	//当hash桶数量 小于 存储的对象数量时，重新分配一个更多的hash桶数.
	if (m_dwSize + 1 > m_tBuckets.GetCapacity())
	{
		__resize_buckets(m_dwSize+1);		
	}
	
	u32 dwHashCode = 0;
	HashNode<KEY,VALUE>* pNode = NULL;
	if ((pNode = __get_key_node(key, dwHashCode)) == NULL)
	{			
		// it doesn't exist, add a new entry
		pNode = __get_freenode();
		assert(pNode);
		pNode->m_dwHashValue = dwHashCode;
		pNode->m_tKey = key;
		
		// put into hash table
		////插入作为链表的第一个结点
		pNode->m_pNext = m_tBuckets[dwHashCode];
		m_tBuckets[dwHashCode] = pNode;
	}
	
	return pNode->m_tValue;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
void HashMap<KEY, VALUE, HASH, EQUAL>::__resize_buckets(u32 dwBucketsNum)
{
	u32 dwOldCapacity = m_tBuckets.GetCapacity();
	if (dwBucketsNum <= dwOldCapacity)
		return;
	
	// create new hash table
	u32 dwNewCapacity = __hash_next_prime(dwBucketsNum);
	DArray<HashNode<KEY,VALUE>* > tNewBuckets(dwNewCapacity);
	u32 dwIndex = 0;
	for (dwIndex = 0; dwIndex < dwNewCapacity; dwIndex++)
	{
		tNewBuckets[dwIndex] = NULL;
	}

	// copy old table to new table one by one
	for (dwIndex = 0; dwIndex < dwOldCapacity; dwIndex++)
	{
		HashNode<KEY,VALUE>* pFirst = m_tBuckets[dwIndex];
		while(pFirst)
		{
			//注意：因为桶数变了，计算新的hash值，要用新的桶数
			u32 dwNewBucketsIndex = __get_hash_code(pFirst->m_tKey, dwNewCapacity);

			m_tBuckets[dwIndex] = pFirst->m_pNext;
			//插入作为链表的第一个结点
			pFirst->m_pNext = tNewBuckets[dwNewBucketsIndex];
			tNewBuckets[dwNewBucketsIndex] = pFirst;
            pFirst->m_dwHashValue = dwNewBucketsIndex;

			pFirst = m_tBuckets[dwIndex];  //pFirst后移
		}
	}
	m_tBuckets.MakeEmpty();
	m_tBuckets = tNewBuckets;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::Insert(const KEY& key, const VALUE& value)
{
	//这里一步拆为两步.
	//当VALUE为空类时一步的话会被编译器 错误地优化掉
	VALUE& oldValue = (*this)[key];
    oldValue = value; 

	return TRUE;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
size_t HashMap<KEY, VALUE, HASH, EQUAL>::__get_hash_code(const KEY& key, u32 dwBucketsSize)
{
	if ( 0 == dwBucketsSize )
	{
		dwBucketsSize = m_tBuckets.GetCapacity();
	}
    return HASH()(key) % dwBucketsSize;	
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashNode<KEY, VALUE>* HashMap<KEY, VALUE, HASH, EQUAL>::__get_key_node(const KEY& key, u32& dwHash)
{
	u32 dwBucketsIndex = __get_hash_code(key);
	dwHash = dwBucketsIndex;

	if (m_dwSize == 0) return NULL;

	HashNode<KEY,VALUE>* pNode;
	for (pNode = m_tBuckets[dwBucketsIndex]; pNode != NULL; pNode = pNode->m_pNext)
	{
		if (EQUAL()(pNode->m_tKey, key))
			return pNode;
	}
	return NULL;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashNode<KEY, VALUE>* HashMap<KEY, VALUE, HASH, EQUAL>::__get_freenode()
{
	if (m_pFreeList == NULL)
	{
		if ((m_pFreeList = __alloc_freelist()) == NULL)
		{
			return NULL;
		}
	}
	HashNode<KEY, VALUE>* pFreeNode = m_pFreeList;
	m_pFreeList = m_pFreeList->m_pNext;
	m_dwSize++;  //attention!
	return pFreeNode;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashNode<KEY, VALUE>* HashMap<KEY, VALUE, HASH, EQUAL>::Find(const KEY& key)
{
	u32 dwHashCode = 0;
	__get_key_node(key, dwHashCode);
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::Find(const KEY& key, VALUE& value)
{
	u32 dwHashCode = 0;
	HashNode<KEY, VALUE>* pNode = __get_key_node(key, dwHashCode);
	if (pNode!=NULL)
	{
		value = pNode->m_tValue;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::Exist(const KEY& key)
{
	u32 dwHashCode = 0;
	if (__get_key_node(key, dwHashCode) != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::Delete(const KEY& key)
{
	if (m_dwSize == 0) return TRUE;

	u32 dwBucketsIndex = __get_hash_code(key);
	
	HashNode<KEY,VALUE>* pCur = NULL;
	HashNode<KEY,VALUE>* pPrev = NULL;
	for (pCur = m_tBuckets[dwBucketsIndex]; pCur != NULL; pCur = pCur->m_pNext)
	{
		if (EQUAL()(pCur->m_tKey, key))
		{
			if (pCur == m_tBuckets[dwBucketsIndex])
			{
				m_tBuckets[dwBucketsIndex] = pCur->m_pNext;
			}
			else
			{
				pPrev->m_pNext = pCur->m_pNext;
			}
			___destory_freenode(pCur);
			return TRUE;
		}
		pPrev = pCur;
	}
	return TRUE;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
void HashMap<KEY, VALUE, HASH, EQUAL>::___destory_freenode(HashNode<KEY,VALUE>* node)
{
	if (node == NULL) return;
	
	node->m_dwHashValue = 0;
	node->m_tKey = KEY();
	node->m_tValue = VALUE();
	
	//插入作为m_pFreeList链表的第一个结点
	node->m_pNext = m_pFreeList;
	m_pFreeList = node;
	m_dwSize--;   //attention!
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::MakeEmpty()
{
	//先destory 每个桶链表的结点
	u32 dwCapacity = m_tBuckets.GetCapacity();
	for (u32 dwIndex = 0; dwIndex < dwCapacity; dwIndex++)
	{
		__destory_freelist(m_tBuckets[dwIndex]);
		m_tBuckets[dwIndex] = NULL;
	}

	//然后destory freelist
	__destory_freelist(m_pFreeList);
	m_pFreeList = NULL;

	//最后destory buckets.
	m_tBuckets.MakeEmpty();
	m_dwSize = 0;

	return TRUE;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashNode<KEY, VALUE>* HashMap<KEY, VALUE, HASH, EQUAL>::GetBeginPos()
{
	if (m_dwSize == 0) return NULL;

	u32 dwCapacity = m_tBuckets.GetCapacity();
	
	for (u32 dwIndex = 0; dwIndex < dwCapacity; dwIndex++)
	{
		HashNode<KEY,VALUE>* pFirst = m_tBuckets[dwIndex];
		if (pFirst != NULL)
		{
			return pFirst;
		}
	}
	return NULL;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashNode<KEY, VALUE>* HashMap<KEY, VALUE, HASH, EQUAL>::GetNextPos(HashNode<KEY, VALUE>* pos)
{
	if (m_dwSize == 0) return NULL;

	if (pos->m_pNext != NULL)
	{
		return pos->m_pNext;
	}
	else
	{
		u32 dwBucketsIndex = pos->m_dwHashValue + 1;   //下一个hash桶
		u32 dwBucketsNum = m_tBuckets.GetCapacity();
		for (dwBucketsIndex; dwBucketsIndex < dwBucketsNum; dwBucketsIndex++)
		{
			HashNode<KEY,VALUE>* pFirst = m_tBuckets[dwBucketsIndex];
			if (pFirst != NULL)
			{
				return pFirst;
			}
		}
	}
	return NULL;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
void HashMap<KEY, VALUE, HASH, EQUAL>::__copy(const HashMap<KEY, VALUE, HASH, EQUAL>& tOther)
{
	if (tOther.m_dwSize == 0) return;

	m_tBuckets.MakeEmpty();
	m_tBuckets.SetCapacity(tOther.m_tBuckets.GetCapacity());

	HashNode<KEY, VALUE>* pCur = tOther.GetBeginPos();
	HashNode<KEY, VALUE>* pEnd = tOther.GetEndPos();
	while (pCur != pEnd)
	{
		Insert(pCur->m_tKey, pCur->m_tValue);
		pCur = tOther.GetNextPos(pCur);
	}

	m_dwSize = tOther.m_dwSize;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
HashMap<KEY, VALUE, HASH, EQUAL>::HashMap(const HashMap<KEY, VALUE, HASH, EQUAL>& tOther)
{
	__copy(tOther);
};


template<class KEY, class VALUE, class HASH, class EQUAL>
HashMap<KEY, VALUE, HASH, EQUAL>& HashMap<KEY, VALUE, HASH, EQUAL>::operator=(const HashMap<KEY, VALUE, HASH, EQUAL> &tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		__copy(tOther);
	}
	return *this;
};


template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::Swap(HashMap<KEY, VALUE, HASH, EQUAL>& tOther)
{
	HashMap<KEY, VALUE, HASH, EQUAL> tTmp = tOther;
	tOther = *this;
	*this = tTmp;
	return TRUE;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::operator==(HashMap<KEY, VALUE, HASH, EQUAL> &tOther)
{
	if (m_dwSize != tOther.m_dwSize)
		return FALSE;

	HashMap<KEY, VALUE, HASH, EQUAL>::Position pCur = tOther.GetBeginPos();
	HashMap<KEY, VALUE, HASH, EQUAL>::Position pEnd = tOther.GetEndPos();
	while (pCur != pEnd)
	{
		if (pCur->m_tValue != (*this)[pCur->m_tKey])
		{
			return FALSE;
		}
		pCur = GetNextPos(pCur);
	}
	return TRUE;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::ForeachPrint()
{
	if (m_dwSize == 0) return TRUE;

	HashNode<KEY, VALUE>* pCur = GetBeginPos();
	HashNode<KEY, VALUE>* pEnd = GetEndPos();
	while (pCur != pEnd)
	{
		std::cout<<"key:"<<pCur->m_tKey<<", value:"<<pCur->m_tValue<<std::endl;
		pCur = GetNextPos(pCur);
	}
	return TRUE;
};

template<class KEY, class VALUE, class HASH, class EQUAL>
BOOL32 HashMap<KEY, VALUE, HASH, EQUAL>::Foreach(HashMap<KEY, VALUE, HASH, EQUAL>::VisitFunc visit, void *ctx)
{
	if (m_dwSize == 0) return TRUE;
	
	HashNode<KEY, VALUE>* pCur = GetBeginPos();
	HashNode<KEY, VALUE>* pEnd = GetEndPos();
	while (pCur != pEnd)
	{
		visit(&(pCur->m_tKey), &(pCur->m_tValue), ctx);
		pCur = GetNextPos(pCur);
	}
	return TRUE;
};

#include "string_sso.h"

//string hash fun
template<> struct CHashFunctor<String_sso>
{
	size_t operator()(const String_sso& key) const
	{
		return hash_string((const char *)key.c_str(), 0);
	}	
};
typedef CHashFunctor<String_sso> CMsHashXStr;

//string map
template<class VALUE, class HASH = CMsHashXStr> 
class CStrHashMap : public HashMap<String_sso, VALUE, HASH>
{
	typedef HashMap<String_sso, VALUE, HASH> BaseMap;
public:
	explicit CStrHashMap(size_t nHashCapacity = DEFAULT_HASH_BUCKETS_NUM) : BaseMap(nHashCapacity){};
};

#endif