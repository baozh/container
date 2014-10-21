#ifndef _XPACK_ASSERT_INCLUDED_
#define _XPACK_ASSERT_INCLUDED_

#include "type_def.h"

////防止有人 push 了 1, 导致计算不正确
#ifdef WIN32
#pragma pack( push, 16) //调大指定对齐值, 使指定对齐值失效,以保证 T 使用自身对齐值.
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
     *  如果b与c之间没有pading的话,则是1字节对齐的
     *  pading值 = sizeof(AlignmentFinder) - sizeof(T) - 1, 其实就是 T 的自身pack值.
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
struct MUST_ALIGN_WITH_ONE_BYTE;        //声明一个模板类，不定义，模版参数为fasle会有编译错误

template<>
struct MUST_ALIGN_WITH_ONE_BYTE<true> // 针对模版参数为true进行特化
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