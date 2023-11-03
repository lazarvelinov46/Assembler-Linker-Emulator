#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <map>
#include "formator.h"
#include "section.h"
#include "backpatch.h"
using namespace std;

class Assembler{
  private:
  /*
    int lc=0;
    StringList* lines;
    SectionList* sections;
    List* globalList;
    List* externList;
    SymTable* symTable;
    TableEntry* current;
    int symbol;
    int section;
    struct Element{
      string symbol;
      string operand;
      string operation;
      int section;
      int value;
      int sections[100]={0};
      int TNSlvl
    }
    List<Element> elements;
    */
  Formator* formator;
  SymTable* symTable;
  string currSection="";
  vector<string> secpom;
  map<string,Section> sections;
  map<string,RelocationTable> relaTab;
  map<string,BackpatchTable> backpathTab;
  ifstream inf;
  ofstream off;
  public:
    void processLine(string line);
    bool check(string str);
    void startAssembling(string inFile,string outFile);
    Assembler(){ this->formator=new Formator();this->symTable=new SymTable();}
    void addLabel(string label);
    void addSection(string section);
    void assembleDirective(FormatedLine fl);
    void assembleInstruction(FormatedLine fl);
    void backpatch();
};