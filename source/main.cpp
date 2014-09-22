#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "type_def.h"
#include <stdio.h>
#include "list.h"

using namespace std;

int main(void)
{
	List<u32> l;

	l.InsertHead(56);
	l.InsertHead(32);
	l.InsertHead(15);
	l.InsertHead(8);

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
		printf("%d ", l.GetData(pos));
		pos = l.GetNext(pos);
	}
	
	//system("pause");
	return 0;	
}


