#include <iostream>
#include <string>
#include <exception>
#include "../inc/assembler.h"

using namespace std;

int main(int argc,char **argv){
  if(argc!=4)throw exception();
  Assembler *a=new Assembler();
  a->startAssembling(argv[3],argv[2]);
  return 0;
}