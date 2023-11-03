#include <string>
using namespace std;

class Symbol{
private:
  int value;
  int size;
  string type;//NO_TYP,
  string local;
  int index;
  string name;
  int num;
  int offset;
  string section;

  bool defined;

  int sectionNum=0;
public:
  Symbol(int value,int size,string type,string local,int index,string name,bool defined=true);
  Symbol(string name,int size,int offset,string section,string local,int num=0);
  void setDefined(bool defined);
  string getName(){return this->name;}
  bool compName(string name){return name==this->name?true:false;}
  Symbol* getSym(){return this;}
  void setNum(int num){this->num=num;}
  void setLocal(string local){this->local=local;}
  string getLocal(){return this->local;}
  int getValue(){return this->value;}
  int getIndex(){return this->index;}
  int getSize(){return this->size;}
  string getType(){return this->type;}
  int getNum(){return this->num;}
  void addValue(int value){this->value+=value;}
  void setValue(int value){this->value=value;}
  void setIndex(int ind){this->index=ind;}
};