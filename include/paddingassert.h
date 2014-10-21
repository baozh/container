#ifndef _XPACK_ASSERT_INCLUDED_
#define _XPACK_ASSERT_INCLUDED_

#include "type_def.h"

////��ֹ���� push �� 1, ���¼��㲻��ȷ
#ifdef WIN32
#pragma pack( push, 16) //����ָ������ֵ, ʹָ������ֵʧЧ,�Ա�֤ T ʹ���������ֵ.
#endif //WIN32


template <class T>
class ispointer_traits
{
private:   
    class Small
    {
    public:
        unsigned char by;
    };
    
    
    class Big
    {
    public:
        unsigned char by;
        unsigned char by1;
    };

    static Small IsPointerHelper(...);    
    static Big IsPointerHelper(void*);

    static T makeT();
public:
    enum {
        ispointer = 
            sizeof(IsPointerHelper(makeT())) - 1
    };
};

template <class T>
class CAlignmentTraits
{
public:
    /**
     *  ���b��c֮��û��pading�Ļ�,����1�ֽڶ����
     *  padingֵ = sizeof(AlignmentFinder) - sizeof(T) - 1, ��ʵ���� T ������packֵ.
     */
    struct AlignmentFinder
    {
        char b;
        T c;
    };
    enum {ispointer = ispointer_traits<T>::ispointer};
    enum {IsAlignByOneByte = ispointer || ( 1 == sizeof(AlignmentFinder) - sizeof(T) ) };
};

template <>
class CAlignmentTraits<signed char>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<unsigned char>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<signed short>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<unsigned short>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<signed long>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<unsigned long>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<signed int>
{
	public:    
		enum {IsAlignByOneByte = 1};
};


template <>
class CAlignmentTraits<unsigned int>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<s64>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

template <>
class CAlignmentTraits<u64>
{
	public:    
		enum {IsAlignByOneByte = 1};
};

#ifdef _LINUX_

template<class T> 
class CAlignmentTraits<T*>
{
public:    
		enum {IsAlignByOneByte = 1};
};

#endif

template<bool>
struct MUST_ALIGN_WITH_ONE_BYTE;        //����һ��ģ���࣬�����壬ģ�����Ϊfasle���б������

template<>
struct MUST_ALIGN_WITH_ONE_BYTE<true> // ���ģ�����Ϊtrue�����ػ�
{
};

template<class T>
struct pack_assert
{
	pack_assert()
	{
		MUST_ALIGN_WITH_ONE_BYTE<CAlignmentTraits<T>::IsAlignByOneByte>();
	}
};

#ifdef WIN32
#pragma pack(pop)
#endif //WIN32

#endif  //#ifndef _XPACK_ASSERT_INCLUDED_