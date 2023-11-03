#include <string>
#include <vector>
using namespace std;

enum lineType{INSTRUCTION, DIRECTIVE,NONELINE};

enum Instruction{
  HALT,INT,IRET,CALL,RET,JMP,JEQ,JNE,JGT,PUSH,POP,XCHG,ADD,SUB,MUL,DIV,CMP,NOT,AND,OR,XOR,TEST,SHL,SHR,LDR,STR,NONEINSTR
};

enum Directive{
  GLOBAL,EXTERN,SECTION,WORD,SKIP,END,NONEDIR
};

enum ArgumentType{
  LITERAL,REGISTER,SYMBOL
};

typedef struct Argument{
  ArgumentType type;
  int value;
  string name;
  string subType;
}Argument;
class FormatedLine{
private:
  bool label=false;
  string labelName;
  lineType type=lineType::NONELINE;
  Instruction instruction=Instruction::NONEINSTR;
  Directive directive=Directive::NONEDIR;
  vector<Argument> arguments;
  public:
  void setLabel(bool l){this->label=l;}
  void setLabelName(string name){this->labelName=name;}
  void setLineType(lineType t){this->type=t;}
  void setInstruction(Instruction i){this->instruction=i;}
  void setDirective(Directive d){this->directive=d;}
  void pushArgument(Argument arg){this->arguments.push_back(arg);}
  bool getLabel(){return this->label;}
  string getLabelName(){return this->labelName;}
  lineType getLineType(){return this->type;}
  Directive getDirective(){return this->directive;}
  int getNoArguments(){return this->arguments.size();}
  Argument getArgument(int i){return this->arguments[i];}
  Instruction getInstruction(){ return this->instruction;}
  unsigned char getRegNum(string reg);
  void setArguments(vector<Argument> args){this->arguments=args;}
};