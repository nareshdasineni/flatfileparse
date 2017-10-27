#ifndef DATA_H
#define DATA_H

#include<map>
#include<utility>
#include<pthread.h>
using namespace std;

class Data;

struct ThreadArgs
{
   Data *d;
   int fd;
};
typedef pair<string,string> StCity;
typedef map<string, StCity >RgStCty;
typedef pair<float,float> TempData;

class Data
{

public:
 string fN;// filename
RgStCty CityInfo;

map<string, TempData> CityMinTemp;
map<string, TempData> CityMaxTemp;
map<string, int> CityCount;
pthread_t *Threads;
int *FDs;
int numThreads;


 Data(string & fN);

 int getFileSize();
 void prepareFds(int n);

 void printAll();
	
 ~Data();

};
#endif
