/*=====================================================================================
ģ����  ��
�ļ���  ��string_sso.h
����ļ���
ʵ�ֹ��ܣ�ʵ��string�࣬sso�汾(���⴦��)
����    ��������(bao.z.h.2008@gmail.com)
��Ȩ    ��
---------------------------------------------------------------------------------------
�޸ļ�¼��
����        �汾    �޸���      �߶���      �޸ļ�¼
2014/1/4    V1.0    ������                    ����
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
����    ��String_sso
����    ��
��Ҫ�ӿڣ�
��ע    ��
--------------------------------------------------------------------------------
�޸ļ�¼��
��  ��        �汾        �޸���            �߶���            �޸ļ�¼
2014/1/26     V1.0        ������
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
		//SetCurrHandler(OutOfMem);    //����newʧ��ʱҪ���õ�new handler.
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

                ////��ΪҪ����Ŀռ�϶�����15�������ٷ���2���Ŀռ䡣
				m_pcszData = NewCapacityAlloc(dwStrLength);
                strcpy(m_pcszData, str);
            }
		}
		//SetCurrHandler(OutOfMem);     //����newʧ��ʱҪ���õ�new handler.
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

            //��ΪҪ����Ŀռ�϶�����15�������ٷ���2���Ŀռ䡣
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
                if (m_pcszData == NULL)  //���֮ǰ�Ǳ�����achBuffer�У�û�з���ռ�
                {
                    data.m_dwCapacity = dwLocalSize;
                    m_dwSize = dwStrLength;

                    //��ΪҪ����Ŀռ�϶�����15�������ٷ���2���Ŀռ�                    
					m_pcszData = NewCapacityAlloc(dwStrLength);
                    strcpy(m_pcszData, str);
                }
                else if (data.m_dwCapacity >= dwStrLength)  //�ѷ���Ŀռ��㹻�洢��
                {
                    strcpy(m_pcszData, str);
                    m_dwSize = dwStrLength;    //ע��m_dwCapacity���ø���
                }
                else   //�ѷ���Ŀռ䲻���洢����Ҫ���·���
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

	//����==,>,<    Ҫͬʱ����const char *, String_sso���ֶ���
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

	//����+=,+��������//��Ӧ���޸��Լ���������ֵ������
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
            if (m_pcszData == NULL)  //���֮ǰ�Ǳ�����achBuffer�У�û�з���ռ�
            {
                s8 achBufferTemp[dwLocalSize + 1];
                strcpy(achBufferTemp, data.m_achBuffer);   //�ȱ���Buffer���Է��޸�dwCapacity��ֵ�����

                data.m_dwCapacity = dwLocalSize;
                m_dwSize = dwNewSize;

                //��ΪҪ����Ŀռ�϶�����15�������ٷ���2���Ŀռ�
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
            else if (data.m_dwCapacity >= dwNewSize)  //�ѷ���Ŀռ��㹻�洢��
            {
                if (rhs.m_dwSize <= dwLocalSize)
                {
                    strcat(m_pcszData, rhs.data.m_achBuffer);
                }
                else
                {
                    strcat(m_pcszData, rhs.m_pcszData);
                }
                m_dwSize = dwNewSize; //ע��m_dwCapacity���ø���
            }
            else   //�ѷ���Ŀռ䲻���洢����Ҫ���·���
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

	//����[]
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

    //�����㹻�Ŀռ䣨dwNewSize����������m_dwCapacity
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

	//����String_sso new ʧ��ʱҪ���õ�new-handler.
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

//����������Ķ������cpp�ļ��У�.h�ļ�ֻ������(���ˣ�����ͷ�ļ���Ҳ����)
std::ostream& operator<<(std::ostream &out,const String_sso &str)
{
    out << str.c_str();
    return out;
}

#endif