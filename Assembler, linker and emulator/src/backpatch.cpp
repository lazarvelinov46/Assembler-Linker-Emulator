#include "../inc/backpatch.h"

void BackpatchTable::addBackpatch(int o,string t,string s,bool p){
  Backpatch *b=new Backpatch(o,t,s,p);
  this->bplist.push_back(*b);
}