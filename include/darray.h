/*=====================================================================================
模块名  ：
文件名  ：darray.h
相关文件：
实现功能：动态数组的实现.
作者    ：包增辉
版权    ：< Copyright(C) 2010 Suzhou Keda Technology Co., Ltd. All rights reserved >
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/10/5   V1.0    包增辉                    创建
========================================================================================*/

#ifndef _DARRAY_H
#define _DARRAY_H

#include "type_def.h"
#include "assert.h"


template <typename T>
class DArray
{
public:
	typedef BOOL32 (*VisitFunc)(void *data, void *ctx);

	DArray(size_t dwCapacity = 1):m_pData(NULL), m_dwSize(0), m_dwCapacity(dwCapacity)
	{
		
		m_pData = new T[m_dwCapacity];
		if(m_pData == NULL)
		{
			std::cout<<"[DArray()]out of memory, failed to alloc space!"<<std::endl;
		}
	};

	DArray(const DArray& tOther) { __copy(tOther);};
	DArray& operator=(const DArray &tOther);
	BOOL32 operator==(const DArray &tOther) const;
	~DArray() { MakeEmpty();};

	BOOL32 IsEmpty() const {return (m_dwSize == 0);};
	BOOL32 MakeEmpty();   //删除所有元素，并释放分配的空间

	size_t GetSize() const {return m_dwSize;};
	size_t GetCapacity() const {return m_dwCapacity;};
	void SetCapacity(size_t dwCapacity);

	T At(size_t nIndex) const;
	T& operator[](size_t nIndex);
	T Front() const;
	T Back() const;
	s32 Find(const T &tData);

	BOOL32 Insert(size_t nIndex, const T &tData);   //将元素tData插入到nIndex位置，之后的元素后移.
	BOOL32 PushBack(const T &tData) {return Insert(m_dwSize, tData);};
	T PopBack();

	BOOL32 Delete(size_t nIndex);   //删除位置nIndex上的元素，之后的元素前移.
	size_t DeleteElem(const T& cElem);

	BOOL32 Foreach(VisitFunc visit, void *ctx);
	BOOL32 Sort() {__sort_array(0, m_dwSize-1); return TRUE;};
private:
	BOOL32 __set_capacity(size_t dwNewCapacity);
	BOOL32 __capacity_shrink();
	void __copy(const DArray& cOther);
	void __sort_array(size_t begin, size_t end);
	void __swap(T *data1, T *data2);
private:
	static const u32 dwMinPreAllocSize;
	T* m_pData;
	size_t m_dwSize;
	size_t m_dwCapacity;
};

template <class T>
const u32 DArray<T>::dwMinPreAllocSize = 15;

template <class T>
void DArray<T>::__swap(T *data1, T *data2)
{
	if (*data1 == *data2) return; 
	T tmp = *data1;
	*data1 = *data2;
	*data2 = tmp;
 };

template <typename T>
void DArray<T>::__copy(const DArray<T>& cOther)
{
	m_dwCapacity = cOther.m_dwCapacity;
	
	m_pData = new T[m_dwCapacity];
	if (m_pData == NULL)
	{
		std::cout<<"[__copy()]out of memory, failed to alloc space!"<<std::endl;
		return;
	}
	
	m_dwSize = cOther.m_dwSize;
	for(s32 i=0; i<cOther.m_dwSize; i++)
	{
		m_pData[i] = cOther.m_pData[i];
	}
};

template <typename T>
BOOL32 DArray<T>::__set_capacity(size_t dwNewCapacity)
{
	if (dwNewCapacity == 0) dwNewCapacity = 1;
	
	//if (dwNewCapacity <= m_dwSize) 
		//return FALSE;
	
	T* pNewData = new T[dwNewCapacity];
	if (pNewData == NULL) 
	{
		std::cout<<"[__copy()]out of memory, failed to alloc space!"<<std::endl;
		return FALSE;
	}	
	
	for(s32 i=0; i<m_dwSize; i++)
	{
		pNewData[i] = m_pData[i];
	}
	m_dwCapacity = dwNewCapacity;
	delete[] m_pData;
	m_pData = pNewData;
	
	return TRUE;
};

template <typename T>
BOOL32 DArray<T>::__capacity_shrink()
{
	//当使用大小比空闲大小小时，将空间调整为使用大小的1.5倍.
	if((m_dwSize < (m_dwCapacity >> 1)) && (m_dwSize > dwMinPreAllocSize))
	{
		size_t dwAllocSize = m_dwSize + (m_dwSize>>1);
		return __set_capacity(dwAllocSize);
	}
	
	return TRUE;
};

template <typename T>
BOOL32 DArray<T>::Foreach(VisitFunc visit, void *ctx)
{
	if (visit == NULL) return FALSE;
	
	BOOL32 bResult = FALSE;
	for (s32 i=0; i<m_dwSize; i++)
	{
		bResult = visit((void *)&(m_pData[i]), ctx);
	}
	return bResult;
};

template <typename T>
size_t DArray<T>::DeleteElem(const T& cElem)
{
	size_t nEraseNum = 0;
	for(s32 i=0; i<m_dwSize; i++)
	{
		if (cElem == m_pData[i])
		{
			if (Delete(i))
			{
				nEraseNum++;
				i--;
			}
		}		
	}
	__capacity_shrink();
	return nEraseNum;
};

template <typename T>
BOOL32 DArray<T>::Delete(size_t nIndex) 
{
	if (nIndex >= m_dwSize || nIndex < 0) 
	{
		return FALSE;
	}
	
	for(s32 i=nIndex; i<m_dwSize-1; i++)
	{
		m_pData[i] = m_pData[i+1];
	}
	m_dwSize--;
	__capacity_shrink();
	return TRUE;		
};

template <typename T>
T DArray<T>::PopBack()
{
	if (m_dwSize < 1)
	{
		std::cout<<"[PopBack()] Array don't have element!"<<std::endl;
		return T();
	}
	T tmp = m_pData[m_dwSize-1];
	m_dwSize--;
	__capacity_shrink();
	return tmp;
};

template <typename T>
BOOL32 DArray<T>::MakeEmpty()   //删除所有元素，并释放分配的空间
{
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	m_dwSize = 0;
	m_dwCapacity = 0;
	return TRUE;
};

template <typename T>	
void DArray<T>::SetCapacity(size_t dwCapacity)
{
    if (dwCapacity <= m_dwCapacity)
    {
        return;
    }
    else
    {
        __set_capacity(dwCapacity);
    }
};

template <typename T>
T DArray<T>::At(size_t nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_dwSize);
	return m_pData[nIndex];
};

template <typename T>
T DArray<T>::Front() const
{
	if (m_dwSize < 1)
	{
		std::cout<<"[Front()] Array don't have element!"<<std::endl;
		return T();
	}
	return m_pData[0];
};

template <typename T>
T DArray<T>::Back() const
{
	if (m_dwSize < 1)
	{
		std::cout<<"[Back()] Array don't have element!"<<std::endl;
		return T();
	}
	return m_pData[m_dwSize - 1];
};
	
template <typename T>
s32 DArray<T>::Find(const T &tData)
{
	for(s32 i=0; i<m_dwSize; i++)
	{
		if (m_pData[i] == tData)
		{
			return i;
		}
	}
	return -1;
};

template <typename T>
T& DArray<T>::operator[](size_t nIndex)
{
	assert(nIndex >= 0 && nIndex < m_dwSize);
	return m_pData[nIndex];
};

template <typename T>
BOOL32 DArray<T>::operator==(const DArray<T> &tOther) const
{
	if (m_dwSize != tOther.m_dwSize)
	{
		return FALSE;
	}

	for(s32 i=0; i < m_dwSize; i++)
	{
		if (m_pData[i] != tOther.m_pData[i])
		{
			return FALSE;
		}	
	}
	return TRUE;
};

template <typename T>
BOOL32 DArray<T>::Insert(size_t nIndex, const T &tData)
{
	if (nIndex > m_dwSize || nIndex < 0) 
		return FALSE;
	
	if (m_dwCapacity == m_dwSize)
	{
		if (!__set_capacity(m_dwCapacity * 2))
		{
			std::cout<<"[Insert()] __set_capacity() failed!"<<std::endl;
			return FALSE;
		}
	}
	
	for(s32 i=m_dwSize; i>nIndex; i--)
	{
		m_pData[i] = m_pData[i-1];			
	}
	
	m_dwSize++;
	m_pData[nIndex] = tData; 
	
	return TRUE;
};

template <typename T>
DArray<T>& DArray<T>::operator=(const DArray<T> &tOther)
{
	if (this != &tOther)
	{
		MakeEmpty();
		__copy(tOther);
	}
	return *this;
};

template <typename T>
void DArray<T>::__sort_array(size_t begin, size_t end)
{
	if (begin >= end) return;

	int i = 0, j = 0;
	T tPivot = m_pData[begin];      //选择第一个元素作为轴值
	__swap(&m_pData[begin], &m_pData[end]);   //将轴值存到最后一个位置

	i = begin; j = end -1;
	while(i<j)
	{
		while(m_pData[i]<tPivot) {i++;}
		while(tPivot<m_pData[j]) {j--;}
		if (i < j)
		{
			__swap(&m_pData[i], &m_pData[j]);
			i++;
			j--;
		}
		else
		{
			break;
		}
	}
	__swap(&m_pData[i], &m_pData[end]);
	
	if (i != begin)
	{
		__sort_array(begin, i-1);
	}

	if (i != end)
	{
		__sort_array(i+1, end);
	}
};

#endif