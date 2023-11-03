#include "section.h"
#include <fstream>
#include <iomanip>
#include <map>

class Linker{
private:
  SymTable* finalSymTab;
  int i=0;
  vector<string> infiles;
  vector<Symbol> globalSyms;
  vector<Symbol> externSyms;
  vector<Section> secInfo;
  map<string,SymTable> symTabs;
  vector<Section> sections;
  map<string,vector<RelocationTable>> relaTabs;
  vector<unsigned char> linkedData;
  ofstream outp;

  bool checkExtern();
  void collectAllSymbols();
  void sortSectionsOne();
  void reorderValues();
  void findAdding();
  void insertRelocations();
  void fillData(string out);
public:
  void readFile(string file);
  void link(string out);
  Linker(){
    finalSymTab=new SymTable();
  }
};