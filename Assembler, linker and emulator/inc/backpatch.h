#include <string>
#include <vector>

using namespace std;

class Backpatch{
  public:
  int offset;
  string type;
  string symbol;
  bool patched;
  Backpatch(int o,string t,string s,bool p=false){this->offset=o;this->type=t;this->symbol=s;this->patched=p;}
};

class BackpatchTable{
  private:
  vector<Backpatch> bplist;
  public:
  void addBackpatch(int o,string t,string s,bool p=false);
  vector<Backpatch>& getList(){return this->bplist;}
  int getSize(){return bplist.size();}
  Backpatch& getElement(int i){return this->bplist[i];}
};