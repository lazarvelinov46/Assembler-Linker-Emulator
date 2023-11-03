#include <string>
#include <vector>
#include "formatedLine.h"
using namespace std;


class Formator{
private:
  vector<string> words;
  int wordsNum=0;
  bool end=false;
public:
  Formator(){};
  void formatLine(string line);
  FormatedLine formatWords(vector<string> words);
  Instruction checkInstruction(string word);
  Directive checkDirective(string word);
  ArgumentType checkArgType(string word);
  vector<string> getWords(){return this->words;}
  vector<Argument> formatArguments(vector<string> words,int start);
  void clear();
};