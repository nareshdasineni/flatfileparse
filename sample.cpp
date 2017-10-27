#include<iostream>
#include<fstream>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
using namespace std;

int main(void)
{
  int fd=open("sample.csv",O_RDONLY);
  lseek(fd,45,SEEK_SET);
  
  char buf;
  read(fd,&buf,1);
  cout<<"Before lseek:"<<buf<<endl;
  if(!isalnum(buf))
	cout<<"WhiteSpace"<<endl;
  lseek(fd,-2,SEEK_CUR);
  read(fd,&buf,1);
  cout<<"After lseek:"<<buf<<endl;
  if(!isalnum(buf))
	cout<<"WhiteSpace"<<endl;
 
  while(read(fd,&buf,1)>0)
  {
     cout<<buf;
     if(buf=='\n')
        break;
  } 

 /* while(read(fd,buf,1) > 0)
    cout<<buf;
 */

  close(fd);
  return 0;
  
}
