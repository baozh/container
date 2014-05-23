
#include <iostream>
#include <vector>
#include <deque>
#include <map>


using namespace std;

int main(void)

{
	vector<int> ver;
	
	ver.push_back(12);
	
	ver.push_back(22);
	ver.push_back(22);
	
	//ver.end();

	deque<int> deq;
	deq.push_back(22);
	//deq.begin();

	map<int, string> m;
	//m.begin();
	//m.begin();


	string str = "bao";
	const char *pStr = str.c_str();
	printf("%s\n", pStr);

	

	return 0;	
}


