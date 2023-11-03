#include <iostream>
#include <string>
#include <exception>
#include "../inc/emulator.h"

using namespace std;

int main(int argc,char **argv){
  if(argc>2)throw exception();
  Emulator *e=new Emulator();
  e->emulate(argv[1]);
  return 0;
}