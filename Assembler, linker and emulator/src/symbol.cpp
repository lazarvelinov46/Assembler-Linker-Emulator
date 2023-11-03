
#include "../inc/symbol.h"

Symbol::Symbol(string name,int size,int offset,string section,string local,int num){
  this->name=name;
  this->size=size;
  this->offset=offset;
  this->section=section;
  this->local=local;
  this->defined=false;
  this->num=num;

}

void Symbol::setDefined(bool defined){
  this->defined=defined;
  
}
Symbol::Symbol(int value,int size,string type,string local,int index,string name,bool defined){
  
  this->value=value;
  this->size=size;
  this->type=type;
  this->local=local;
  this->index=index;
  this->name=name;
  this->defined=defined;
}