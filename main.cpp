#include <iostream>
#include "Data.h"
#include<math.h>
using namespace std;

int main(void)
{

 string fN("climate_change.csv");
 Data d(fN);
 
  //cout<<"enter file name:";
  //cin>>fN;
  int fileSize=d.getFileSize();
  int threads=ceil(fileSize/(1024.0*1024.0));


  cout<<"File Size:"<<fileSize<<endl;
  cout<<"Total Threads:"<<threads<<endl;
//  d.readFile();
 // d.printAll();
 
 return 0;
}
