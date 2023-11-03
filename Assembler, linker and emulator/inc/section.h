#include <string>
#include "symTable.h"
#include "relaTable.h"
using namespace std;

class Section{
private:
  string file;
  int base;
  int lc;

  vector<unsigned char> values;
  SymTable* symTab;
  RelocationTable* relaTab;
public:
  Section(int base){this->base=base;this->lc=0;}
  string name;
  Section(){}
  //Section* next;
  int size;
  int offset;
  Section(string name,int offset,int size,string file);
  void addByte(unsigned char byte);
  vector<unsigned char>& getValues(){return this->values;}
  int getLocationCounter(){return this->lc;}
  int getBase(){return this->base;}
  void addEmptyWord();
  void addWord(int word);
  void addInstrData(int value);
  void addInstrData(int start,int value);
  void addWord(int start,int value);
  void addPcrelWord(int value,int offset);
  void setFile(string file){this->file=file;}
  string getFile(){return this->file;}
};