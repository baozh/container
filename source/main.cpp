#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "type_def.h"
#include <stdio.h>
#include <string>
#include <cctype>
#include <algorithm>
#include "darray.h"
#include "list.h"
#include "queue.h"

using namespace std;


BOOL32 print(void *data, void *ctx)
{
	u32 *pData = (u32*)data;
	cout<<*pData<<"  ";
	return TRUE;
}


BOOL32 print_str(void *data, void *ctx)
{
	string *pData = (string*)data;
	cout<<*pData<<"  ";
	return TRUE;
}

BOOL32 toupper_str(void *data, void *ctx)
{
	string *pData = (string*)data;

	transform(pData->begin(), pData->end(), pData->begin(), ::toupper);
	return TRUE;
}

BOOL32 concat_str(void *data, void *ctx)
{
	string *pData = (string*)data;
	string *pCtx = (string*)ctx;
	pCtx->append(*pData);

	//*ctx = (string*)ctx;
	//*ctx = *ctx + *pData;
	
	return TRUE;
}

int main(void)
{
	Queue<u32> a;
	cout<<a.GetSize()<<endl;
	
	a.PushBack(45);
	cout<<a.GetSize()<<endl;

	a.PushBack(67);
	cout<<a.GetSize()<<endl;

	a.PushBack(34);
	cout<<a.GetSize()<<endl;

	a.PushBack(896);
	cout<<a.GetSize()<<endl;

	a.PushBack(345);
	cout<<a.GetSize()<<endl;


	a.PushBack(77);
	cout<<a.GetSize()<<endl;

	a.PushBack(55);
	cout<<a.GetSize()<<endl;

	a.PushBack(33);
	a.PushBack(88);
	a.PushBack(543);a.PushBack(234);
	a.PushBack(323);
		cout<<a.GetSize()<<endl;
	a.PushBack(103);
	a.PushBack(786);
	a.PushBack(367);

	a.PushBack(666);
	a.PushBack(777);
	a.PushBack(888);
		cout<<a.GetSize()<<endl;
	a.PushBack(444);
	a.PushBack(555);


	cout<<endl<<"size = "<<a.GetSize()<<endl;
	a.Foreach(print, NULL);

	//cout<<endl;
	//a.Sort();
	//a.Foreach(print, NULL);

	a.PopFront();


	a.PopFront();


	a.PopFront();
	cout<<endl<<"size = "<<a.GetSize()<<endl;
a.Foreach(print, NULL);

	a.PopFront();


	a.PopFront();


	a.PopFront();
	cout<<endl<<"size = "<<a.GetSize()<<endl;
a.Foreach(print, NULL);

// 	a.PopFront();
// 
// 
// 	a.PopFront();
// 
// 
// 	a.PopFront();
// 
// 	a.PopFront();
// 
// 
// 	a.PopFront();
// 
// 
// 	a.PopFront();
// 
// 
// 	a.PopFront();
// 
// 	a.PopFront();





	/*
	List<u32> l;

	l.InsertTail(15);
	l.InsertTail(32);
	l.InsertTail(56);
	l.InsertTail(8);
	l.InsertTail(55);
	l.InsertTail(99);
	l.InsertTail(33);
	l.InsertTail(48);
	l.InsertTail(9);
	l.InsertTail(23);
	l.InsertTail(77);
	l.InsertTail(67);
	l.InsertTail(345);
	l.InsertTail(897);
	l.InsertTail(543);
	l.InsertTail(21);
	l.InsertTail(12);
	l.InsertTail(11);
	l.InsertTail(56);
	l.InsertTail(45);
	l.InsertTail(33);
	l.InsertTail(101);
	l.InsertTail(145);
	l.InsertTail(178);
	l.InsertTail(234);
	l.InsertTail(45);
	l.InsertTail(6);
	l.InsertTail(98);
	l.InsertTail(77);
	l.InsertTail(34);
	l.InsertTail(56);
	l.InsertTail(854);

	printf("\nprint list:\n");
	l.ForeachList(print, NULL);

	List<u32>::Position pos1 = l.Find(99);
	List<u32>::Position pos2 = l.Find(8);

	//l.Swap(pos1, pos2);
	l.Sort();

	printf("\nprint list:\n");
	l.ForeachList(print, NULL);
	*/

	/*
	List<u32>::Position pos = l.Find(15);
	l.InsertAfter(pos, 17);

	printf("\n");
	pos = l.GetHead();
	//List<u32>::Position tail = l.GetTail();
	while (pos != NULL)
	{
		printf("%d ", l.GetData(pos));
		pos = l.GetNext(pos);
	}

	l.Delete(15);

	printf("\n");
	 pos = l.GetHead();
	// tail = l.GetTail();
	while (pos != NULL)
	{
		//l.SetData(pos, 45);
		printf("%d ", l.GetData(pos));
		pos = l.GetNext(pos);
	}
	
	//l.ForeachList(print, NULL);

	printf("\nprint string:\n");
	List<string> s;
	s.InsertTail("bao");
	s.InsertTail("zeng");
	s.InsertTail("hui");


	string tmp = "bao";
	s.InsertTail(tmp);
	tmp = tmp + "zeng";
	s.InsertTail(tmp);
	tmp = tmp + "hui";
	s.InsertTail(tmp);

	s.ForeachList(print_str, NULL);

	s.ForeachList(toupper_str, NULL);

	printf("\n print string:\n");
	s.ForeachList(print_str, NULL);

	string str;
	s.ForeachList(concat_str, (void *)&str);
	printf("\n print string:\n");
	cout<<str<<endl;
	*/

	system("pause");
	return 0;
}


