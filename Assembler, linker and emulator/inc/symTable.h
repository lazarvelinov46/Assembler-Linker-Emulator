#include <string>
#include "symbol.h"
#include <list>
#include <iterator>
#include <vector>
using namespace std;

class SymTable{
  private:
    int cnt=0;
    vector<Symbol> symbols;
    
  public:
    SymTable(bool numerical);
    SymTable(){}
    Symbol* getSymbol(string name);
    void printTable();
    void overwriteSymbol(string section,int base,long offset);
    void addSymbol(Symbol &s);
    void addSymbol(int value,int size,string type,string local,int index,string name);
    int getSize(){return this->cnt;}
    int getNumberByName(string name);
    void setLocal(int num,string local){this->symbols[num].setLocal(local);}
    int getIndexByName(string name);
    Symbol* getSymbolByNum(int i){return &symbols[i];}
};