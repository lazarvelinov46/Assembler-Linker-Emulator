#include "../inc/symTable.h"
#include <iostream>
/*
SymTable::SymTable(){
  this->first=this->last=nullptr;
  order=0;
}

SymTable::~SymTable(){
  Symbol* temp;
  while(first){
    temp=first;
    first=first->next;
    delete temp;
  }
  first=last=nullptr;
}
*/
void SymTable::addSymbol(Symbol &s){
  s.setNum(cnt++);
  this->symbols.push_back(s);
}

Symbol* SymTable::getSymbol(string name){
  vector<Symbol>::iterator it;
  for (it = symbols.begin(); it != symbols.end(); ++it){
        if(it->getName()==name){
          return it->getSym();
        }
  }
  return nullptr;
}

void SymTable::addSymbol(int value,int size,string type,string local,int index,string name){
  Symbol* s=new Symbol(value,size,type,local,index,name);
  s->setNum(cnt++);
  this->symbols.push_back(*s);
}

int SymTable::getNumberByName(string name){
  for(int i=0;i<symbols.size();i++){
    if(symbols[i].getName()==name){
      return i;
    }
  }
  return -20;
}
int SymTable::getIndexByName(string name){
  for(int i=0;i<symbols.size();i++){
    if(symbols[i].getName()==name){
      return symbols[i].getIndex();
    }
  }
  return -20;
}
