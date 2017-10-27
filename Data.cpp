#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<pthread.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sstream>
#include "Data.h"
using namespace std;

int chunk=1024*1024;


void *readFile(void * args)
{

 ThreadArgs *tA=(ThreadArgs *)args;
 
  int sum=0;
  char buf;

  read(tA->fd,&buf,1);
  if(isalnum(buf)){
	lseek(tA->fd,-2,SEEK_CUR);
	read(tA->fd,&buf,1);
	if(isalnum(buf))
	{
		while(read(tA->fd,&buf,1)>0)
			if(buf=='\n')
				break;
	}
  }
  
  while(1)
  {
        string line;
	stringstream ss;
        while(read(tA->fd,&buf,1)>0)
	{
		if(isalnum(buf)||buf=='.'||buf==',')
			ss<<buf;
		else if(buf=='\n')
			break;
		else if(sum>=chunk)
			break;
		sum+=1;
	}
	
	if(sum>=chunk)
	{
		while(read(tA->fd,&buf,1))
		{
			if(buf=='\n')
				break;
			ss<<buf;
		}
	}
	int pos1=0,pos2=-1;
	line.assign(ss.str());

	string temp[3];
        float temperature[2];
 	for(int i=0;i<3;i++)
	{
            pos1=pos2+1;
            pos2=line.find(',',pos1);
	    temp[i]=line.substr(pos1,pos2-pos1);
	}
	
 	pos1=line.find(',',pos1)+1;
 	pos1=line.find(',',pos1)+1;
 	pos1=line.find(',',pos1)+1;
 	pos2=line.find(',',pos1);

        for(int i=0;i<2;i++){
	        pos1=pos2+1;
        	pos2=line.find(',',pos1);
      	   	temperature[i]=atof(line.substr(pos1,pos2-pos1).c_str());
	}

        StCity s=make_pair(temp[1],temp[2]);
        tA->d->CityInfo.insert(make_pair(temp[0],s));

        //Update CityMaxTemp map with maxTemperature info
	tA->d->CityMaxTemp[temp[2]].second += temperature[0];

        if(tA->d->CityMaxTemp[temp[2]].first < temperature[0])
		tA->d->CityMaxTemp[temp[2]].first=temperature[0];
		
        //Update CityMinTemp map with minTemperature info
	tA->d->CityMinTemp[temp[2]].second += temperature[1];

	if(tA->d->CityMinTemp[temp[2]].first == 0)
		tA->d->CityMinTemp[temp[2]].first=temperature[1];
        else if(tA->d->CityMinTemp[temp[2]].first > temperature[1])
		tA->d->CityMinTemp[temp[2]].first=temperature[1];

        tA->d->CityCount[temp[2]] += 1;

       
	if(sum>=chunk)
		break;
  }

}

Data::Data(string & fileName)
{
  fN.assign(fileName);
}

void Data::printAll()
{

  cout<<"City with Min Temperature Details:"<<endl;
  for(map<string,TempData>::iterator iter=CityMinTemp.begin();iter!=CityMinTemp.end();iter++)
	cout<<iter->first<<":"<<iter->second.first<<","<<iter->second.second<<endl;

  cout<<"City with Max Temperature Details:"<<endl;
  for(map<string,TempData>::iterator iter=CityMaxTemp.begin();iter!=CityMaxTemp.end();iter++)
	cout<<iter->first<<":"<<iter->second.first<<","<<iter->second.second<<endl;

  cout<<"City Data count:"<<endl;
  for(map<string,int>::iterator itr=CityCount.begin();itr!=CityCount.end();itr++)
	cout<<itr->first<<":"<<itr->second<<endl;
}


int Data::getFileSize()
{
   struct stat buf;
   lstat(fN.c_str(),&buf);

   return buf.st_size;

}


void Data::prepareFds(int n)
{
  numThreads=n;
  int offset=0;
  Threads = new pthread_t[n];
  FDs=new int[n];
  ThreadArgs ta[n];

  for(int i=0;i<numThreads;i++)
  {
	FDs[i]=open(fN.c_str(),O_RDONLY);
        lseek(FDs[i],offset,SEEK_SET);
        offset += (chunk+1);
	ta[i].d=this;
	ta[i].fd=FDs[i];
	
        pthread_create(&Threads[i],NULL, &readFile,&ta[i]); 
        
  }
  
}


Data::~Data()
{
  delete [] Threads;
}
