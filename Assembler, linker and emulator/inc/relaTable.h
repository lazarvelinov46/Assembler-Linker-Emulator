#include <string>
#include <vector>
using namespace std;

enum RelaType{
  LITTLE,BIG,PCREL
};

typedef struct Relocation{
  string section;
  int offset;
  int addend;
  int symbol;
  RelaType type;
  Relocation(int o,RelaType rt,int s,int a){
    offset=o;
    type=rt;
    symbol=s;
    addend=a;
  }
}Relocation;

class RelocationTable{
  private:
  vector<Relocation> content;
  public:
  void addRelocation(int offset,RelaType type,int symbol,int addend);
  void add(string section,int offset,int addend,string type);
  void addMade(Relocation r){this->content.push_back(r);}
  void addRel(int o,RelaType rt,int s,int a);
  int getSize(){return content.size();}
  string section;
  void setSection(string section){this->section=section;}
  string getSection(){return this->section;}
  Relocation* getRel(int i){return &content[i];}
  RelocationTable(){}
};