#include "../inc/relaTable.h"
/*
RelocationTable::RelocationTable(){
  this->head=nullptr;
  this->tail=nullptr;
}

RelocationTable::~RelocationTable(){
  Relocation* temp;
  while(head){
    temp=head;
    head=head->next;
    delete temp;
  }
  head=tail=nullptr;
}

void RelocationTable::add(string section,int offset,int addend,string type){
  Relocation *rel= new Relocation(section,offset,addend,type);
  if(!head){
    head=tail=rel;
  }else{
    tail->next=rel;
    tail=tail->next;
  }
}
*/
void RelocationTable::addRelocation(int offset,RelaType type,int symbol,int addend){
  Relocation *r = new Relocation(offset,type,symbol,addend);
  this->content.push_back(*r);
}