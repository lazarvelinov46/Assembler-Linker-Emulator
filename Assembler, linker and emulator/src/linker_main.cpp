#include <iostream>
#include <string>
#include <exception>
#include "../inc/linker.h"

using namespace std;

int main(int argc,char **argv){
  if(argc<5)throw exception();
  Linker *l=new Linker();
  //hex je argv[1] -0 je argv[2]
  for(int i=4;i<argc;i++){
    l->readFile(argv[i]);
  }
  l->link(argv[3]);
  return 0;
}