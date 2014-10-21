/*=====================================================================================
模块名  ：
文件名  ：string_sso.h
相关文件：
实现功能：实现string类，sso版本(特殊处理)
作者    ：包增辉(bao.z.h.2008@gmail.com)
版权    ：
---------------------------------------------------------------------------------------
修改记录：
日期        版本    修改人      走读人      修改记录
2014/1/4    V1.0    包增辉                    创建
========================================================================================*/


#ifndef STRING_SSO_H
#define STRING_SSO_H


#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
//#include "new_handler_support.h"
//#include "type_def.h"


static const u32 dwLocalSize = 15;

/*==============================================================================
类名    ：String_sso
功能    ：
主要接口：
备注    ：
--------------------------------------------------------------------------------
修改记录：
日  期        版本        修改人            走读人            修改记录
2014/1/26     V1.0        包增辉
==============================================================================*/
class String_sso
{
public:
	String_sso()
	{
		m_pcszData = NULL;
		m_dwSize = 0;
		memset(data.m_achBuffer, 0, dwLocalSize + 1);
		data.m_achBuffer[0] = '\0';
		//SetCurrHandler(OutOfMem);    //设置new失败时要调用的new handler.
	}

	String_sso(const s8* str)
	{
	    u32 dwStrLength = strlen(str);
		if (str)
		{
            if (dwStrLength <= dwLocalSize)
            {
                strcpy(data.m_achBuffer, str);
                m_dwSize = dwStrLength;
                m_pcszData = NULL;
            }
            else
            {
                data.m_dwCapacity = dwLocalSize;
                m_dwSize = dwStrLength;

                ////因为要分配的空间肯定大于15，所以再分配2倍的空间。
				m_pcszData = NewCapacityAlloc(dwStrLength);
                strcpy(m_pcszData, str);
            }
		}
		//SetCurrHandler(OutOfMem);     //设置new失败时要调用的new handler.
	}

	String_sso(const String_sso& rhs)
	{
        if (rhs.m_dwSize <= dwLocalSize)
        {
            strcpy(data.m_achBuffer, rhs.data.m_achBuffer);
            m_dwSize = rhs.m_dwSize;
            m_pcszData = NULL;
        }
        else
        {
            data.m_dwCapacity = dwLocalSize;
            m_dwSize = rhs.m_dwSize;

            //因为要分配的空间肯定大于15，所以再分配2倍的空间。
            m_pcszData = NewCapacityAlloc(rhs.m_dwSize);
            strcpy(m_pcszData, rhs.m_pcszData);
        }
	}

	~String_sso()
	{
	    if (m_pcszData != NULL)
	    {
	        delete[] m_pcszData;
            m_pcszData = NULL;
	    }
		m_dwSize = 0;
		memset(data.m_achBuffer, 0, dwLocalSize + 1);
	}

	String_sso& operator=(const s8* str)
	{
	    u32 dwStrLength = strlen(str);
		if (str != m_pcszData && str != data.m_achBuffer && str != NULL)
        {
            if (dwStrLength <= dwLocalSize)
            {
                strcpy(data.m_achBuffer, str);
                m_dwSize = dwStrLength;

                if (m_pcszData != NULL)
                {
                    delete[] m_pcszData;
                    m_pcszData = NULL;
                }
            }
            else
            {
                if (m_pcszData == NULL)  //如果之前是保存在achBuffer中，没有分配空间
                {
                    data.m_dwCapacity = dwLocalSize;
                    m_dwSize = dwStrLength;

                    //因为要分配的空间肯定大于15，所以再分配2倍的空间                    
					m_pcszData = NewCapacityAlloc(dwStrLength);
                    strcpy(m_pcszData, str);
                }
                else if (data.m_dwCapacity >= dwStrLength)  //已分配的空间足够存储了
                {
                    strcpy(m_pcszData, str);
                    m_dwSize = dwStrLength;    //注：m_dwCapacity不用更新
                }
                else   //已分配的空间不够存储，需要重新分配
                {
                    s8* pszStr = NewCapacityAlloc(dwStrLength);
                    strcpy(pszStr, str);

                    m_dwSize = dwStrLength;

                    if (m_pcszData != NULL)
                    {
                        delete[] m_pcszData;
                    }
                    m_pcszData = pszStr;
                }
            }
		}

		return *this;
	}

	String_sso& operator=(const String_sso& rhs)
	{
		return operator=(rhs.c_str());
	}

	size_t size() const
	{
		return m_dwSize;
	}

	const s8* c_str() const
	{
	    if (m_dwSize <= dwLocalSize)
	    {
	        return data.m_achBuffer;
	    }
	    else
	    {
	        return m_pcszData;
	    }
	}

    operator const char*() const
	{
		return c_str();
	}

	const s8* begin() const
	{
        if (m_dwSize <= dwLocalSize)
	    {
	        return data.m_achBuffer;
	    }
	    else
	    {
	        return m_pcszData;
	    }
    }

	const s8* end() const
	{
        if (m_dwSize <= dwLocalSize)
	    {
	        return data.m_achBuffer + m_dwSize;
	    }
	    else
	    {
	        return m_pcszData + m_dwSize;
	    }
	}

	//empty()
	BOOL32 empty() const
	{
		return m_dwSize == 0;
	}

	//重载==,>,<    要同时处理const char *, String_sso两种对象
	BOOL32 operator==(const String_sso& rhs) const
	{
	    if (rhs.m_dwSize <= dwLocalSize)
	    {
	        if (m_dwSize <= dwLocalSize)
	        {
	            return strcmp(data.m_achBuffer, rhs.data.m_achBuffer) == 0;
	        }
	        else
	        {
	            return FALSE;
	        }
	    }
	    else
	    {
	        if (m_dwSize > dwLocalSize)
	        {
	            return strcmp(m_pcszData, rhs.m_pcszData) == 0;
	        }
	        else
	        {
	            return FALSE;
	        }
	    }
	}

	BOOL32 operator==(const s8* str) const
	{
		if (str)
		{
		    String_sso strTemp(str);
		    return operator==(strTemp);
		}
		else
		{
		    return FALSE;
        }
    }

	BOOL32 operator<(const String_sso& rhs) const
	{
        if (m_dwSize <= dwLocalSize && rhs.m_dwSize <= dwLocalSize)
        {
            return strcmp(data.m_achBuffer, rhs.data.m_achBuffer) < 0;
        }
        else if (m_dwSize <= dwLocalSize && rhs.m_dwSize > dwLocalSize)
        {
            return strcmp(data.m_achBuffer, rhs.m_pcszData) < 0;
        }
        else if (m_dwSize > dwLocalSize && rhs.m_dwSize <= dwLocalSize)
        {
            return strcmp(m_pcszData, rhs.data.m_achBuffer) < 0;
        }
        else if (m_dwSize > dwLocalSize && rhs.m_dwSize > dwLocalSize)
        {
            return strcmp(m_pcszData, rhs.m_pcszData) < 0;
        }
	}

	BOOL32 operator<(const s8* str) const
	{
		if (str)
		{
		    String_sso strTemp(str);
		    return operator<(strTemp);
		}
		else
		{
		    return FALSE;
		}
	}

	BOOL32 operator>(const String_sso& rhs) const
	{
	    if (m_dwSize <= dwLocalSize && rhs.m_dwSize <= dwLocalSize)
        {
            return strcmp(data.m_achBuffer, rhs.data.m_achBuffer) > 0;
        }
        else if (m_dwSize <= dwLocalSize && rhs.m_dwSize > dwLocalSize)
        {
            return strcmp(data.m_achBuffer, rhs.m_pcszData) > 0;
        }
        else if (m_dwSize > dwLocalSize && rhs.m_dwSize <= dwLocalSize)
        {
            return strcmp(m_pcszData, rhs.data.m_achBuffer) > 0;
        }
        else if (m_dwSize > dwLocalSize && rhs.m_dwSize > dwLocalSize)
        {
            return strcmp(m_pcszData, rhs.m_pcszData) > 0;
        }
	}

	BOOL32 operator>(const s8* str) const
	{
	    if (str)
		{
		    String_sso strTemp(str);
		    return operator>(strTemp);
		}
		else
		{
		    return FALSE;
		}
	}

	//重载+=,+　　　　//不应该修改自己缓冲区的值！！！
	String_sso operator+(const String_sso& rhs)
	{
	    String_sso strTemp(*this);
        strTemp.operator+=(rhs);

        return strTemp;
	}

	String_sso operator+(const s8* str)
	{
		if (str)
		{
            String_sso strRhs(str);

            String_sso strTemp(*this);
            strTemp.operator+=(strRhs);

            return strTemp;
		}
	}

	String_sso& operator+=(const String_sso& rhs)
	{
        u32 dwNewSize = m_dwSize + rhs.m_dwSize;

	    if (dwNewSize <= dwLocalSize)
        {
            strcat(data.m_achBuffer, rhs.data.m_achBuffer);
            m_dwSize = dwNewSize;
            m_pcszData = NULL;
        }
        else
        {
            if (m_pcszData == NULL)  //如果之前是保存在achBuffer中，没有分配空间
            {
                s8 achBufferTemp[dwLocalSize + 1];
                strcpy(achBufferTemp, data.m_achBuffer);   //先保存Buffer，以防修改dwCapacity后，值被冲掉

                data.m_dwCapacity = dwLocalSize;
                m_dwSize = dwNewSize;

                //因为要分配的空间肯定大于15，所以再分配2倍的空间
                m_pcszData = NewCapacityAlloc(dwNewSize);
                strcpy(m_pcszData, achBufferTemp);

                if (rhs.m_dwSize <= dwLocalSize)
                {
                    strcat(m_pcszData, rhs.data.m_achBuffer);
                }
                else
                {
                    strcat(m_pcszData, rhs.m_pcszData);
                }
            }
            else if (data.m_dwCapacity >= dwNewSize)  //已分配的空间足够存储了
            {
                if (rhs.m_dwSize <= dwLocalSize)
                {
                    strcat(m_pcszData, rhs.data.m_achBuffer);
                }
                else
                {
                    strcat(m_pcszData, rhs.m_pcszData);
                }
                m_dwSize = dwNewSize; //注：m_dwCapacity不用更新
            }
            else   //已分配的空间不够存储，需要重新分配
            {
                s8* pszStr = NewCapacityAlloc(dwNewSize);
                strcpy(pszStr, m_pcszData);

                if (rhs.m_dwSize <= dwLocalSize)
                {
                    strcat(pszStr, rhs.data.m_achBuffer);
                }
                else
                {
                    strcat(pszStr, rhs.m_pcszData);
                }

                m_dwSize = dwNewSize;
                if (m_pcszData != NULL)
                {
                    delete[] m_pcszData;
                }
                m_pcszData = pszStr;
            }
        }

		return *this;
	}

	String_sso& operator+=(const s8* str)
	{
		if (str)
		{
		    String_sso strTemp(str);
		    operator+=(strTemp);
		}

		return *this;
	}

	//重载[]
    const s8& operator[](u8 pos) const
	{
	    if ( pos < m_dwSize)
	    {
            if (m_dwSize <= dwLocalSize)
            {
                return data.m_achBuffer[pos];
            }
            else
            {
                return m_pcszData[pos];
            }

	    }

		return 0;
	}

    //分配足够的空间（dwNewSize），并更新m_dwCapacity
    s8* NewCapacityAlloc(u32 dwNewSize)
    {
        u32 dwNewCap = 0;
        if (data.m_dwCapacity >= dwNewSize)
        {
            dwNewCap = data.m_dwCapacity;
        }
        else
        {
            u32 dwCapTmp = data.m_dwCapacity;

            while (dwCapTmp <= dwNewSize)
            {
                dwCapTmp = dwCapTmp * 2;
            }

            dwNewCap = dwCapTmp;
        }
        data.m_dwCapacity = (((dwNewCap) > (dwLocalSize)) ? (dwNewCap) : (dwLocalSize));
        s8* pszNewStart = new s8[data.m_dwCapacity+1];
        return pszNewStart;
    }

	//定义String_sso new 失败时要调用的new-handler.
	static void OutOfMem()
	{
		printf("alloc String_sso error.Unable to satisfy request for memory.\n");
		abort();
	};

private:
	s8* m_pcszData;
	u32 m_dwSize;
	//static const u32 dwLocalSize;
	union
	{
        s8 m_achBuffer[dwLocalSize + 1];
        u32 m_dwCapacity;
    } data;
};

//把这个函数的定义放在cpp文件中，.h文件只放声明(错了，放在头文件中也可以)
std::ostream& operator<<(std::ostream &out,const String_sso &str)
{
    out << str.c_str();
    return out;
}

#endif