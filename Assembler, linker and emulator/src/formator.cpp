#include "../inc/formator.h"
#include <regex>
#include <iostream>

void Formator::formatLine(string line){
  bool put=false;
  char* word= new char[100];
  int wc=0;
  for(int i=0;i<line.length();i++){
    
    char c=line[i];
    
    if((line[i+1]=='\0'||line[i+1]=='\n')&&(c!=':'&&c!=']')){
      word[wc++]=line[i];
      word[wc]='\0';
      words.push_back(word);
      wc=0;
    }

    else if(c=='#'){
      word[wc]='\0';
      words.push_back(word);
      wc=0;
      break;
    }
    else if(c==':'||c==']'){

      word[wc]='\0';
      words.push_back(word);
      wc=0;
      word[wc++]=c;
      word[wc]='\0';
      words.push_back(word);
      wc=0;
    }
    else if((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='.'||c=='_'){
      word[wc++]=c;
    }
    else if(c==' '||c=='\t'){
      if(wc==0){
        continue;
      }
      word[wc]='\0';
      words.push_back(word);
      wc=0;
    }else{
      //ovo znaci da je neki znak
      if(wc!=0){
      word[wc]='\0';
      words.push_back(word);
      wc=0;
      }
      word[wc++]=c;
      word[wc]='\0';
      words.push_back(word);
      wc=0;
    }
  }

}

FormatedLine Formator::formatWords(vector<string> w){
  FormatedLine line;
  bool first=true;bool labela=false;bool zapeta=false;
  int argcnt=0;
  for(int i=0;i<w.size();i++){
    if(w.size()>=2&&!labela){
      if(w[1]==":"){
        labela=true;
        line.setLabel(true);
        line.setLabelName(w[0]);
      }
    }
    if(labela&&i<2){
      continue;
    }else{
      if(first){
        //odredjivanje da li je instrukcija ili direktiva
        Instruction in=checkInstruction(w[i]);
        line.setInstruction(in);
        Directive dir=checkDirective(w[i]);
        line.setDirective(dir);
        if(in!=Instruction::NONEINSTR){
          line.setLineType(lineType::INSTRUCTION);
        }else if(dir!=Directive::NONEDIR){
          line.setLineType(lineType::DIRECTIVE);
        }else{
          line.setLineType(lineType::NONELINE);
        }
        first=false;
      }
      else{
        vector<Argument> args=formatArguments(w,i);
        line.setArguments(args);
        break;
      }
      
    }
  }
  return line;
}

Instruction Formator::checkInstruction(string word){
  if(word=="halt"){
    return Instruction::HALT;
  }else if(word=="int"){
    return Instruction::INT;
  }else if(word=="iret"){
    return Instruction::IRET;
  }else if(word=="call"){
    return Instruction::CALL;
  }else if(word=="ret"){
    return Instruction::RET;
  }else if(word=="jmp"){
    return Instruction::JMP;
  }else if(word=="jeq"){
    return Instruction::JEQ;
  }else if(word=="jne"){
    return Instruction::JNE;
  }else if(word=="jgt"){
    return Instruction::JGT;
  }else if(word=="push"){
    return Instruction::PUSH;
  }else if(word=="pop"){
    return Instruction::POP;
  }else if(word=="xchg"){
    return Instruction::INT;
  }else if(word=="add"){
    return Instruction::ADD;
  }else if(word=="sub"){
    return Instruction::SUB;
  }else if(word=="mul"){
    return Instruction::MUL;
  }else if(word=="div"){
    return Instruction::DIV;
  }else if(word=="cmp"){
    return Instruction::CMP;
  }else if(word=="not"){
    return Instruction::NOT;
  }else if(word=="and"){
    return Instruction::AND;
  }else if(word=="or"){
    return Instruction::OR;
  }else if(word=="xor"){
    return Instruction::XOR;
  }else if(word=="test"){
    return Instruction::TEST;
  }else if(word=="shl"){
    return Instruction::SHL;
  }else if(word=="shr"){
    return Instruction::SHR;
  }else if(word=="ldr"){
    return Instruction::LDR;
  }else if(word=="str"){
    return Instruction::STR;
  }else{
    return Instruction::NONEINSTR;
  }
}
Directive Formator::checkDirective(string word){
  if(word==".global"){
    return Directive::GLOBAL;
  }else if(word==".extern"){
    return Directive::EXTERN;
  }else if(word==".section"){
    return Directive::SECTION;
  }else if(word==".word"){
    return Directive::WORD;
  }else if(word==".skip"){
    return Directive::SKIP;
  }else if(word==".end"){
    return Directive::END;
  }else{
    return Directive::NONEDIR;
  }
}

//literal 
  regex literal_r{"(^[0-9][0-9]*$)|(^0x[0-9a-fA-F]*$)"};
//reg
  regex reg_r{"^r[0-9][l|h]*$"};
//symbol
  regex symbol_r{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
  
  regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
  string adresiranja[]={"litimm$","symimm$","pcrel","litdir","regdir","symdir","regind"};
vector<Argument> Formator::formatArguments(vector<string> words,int start){
  bool zapeta=false;
  bool pocetArg=false;
  int skip=0;
  int i=start;
  vector<Argument> args;
  while(i<words.size()){
    if(words[i]==","){
      i++;
    }
    else{

      if(words[i]=="$"){

        if(regex_match(words[i+1],literal_r)){
          Argument a;
          a.name="";
          a.subType="litimm$";
          a.type=ArgumentType::LITERAL;
          int pom=10;
          if(words[i+1].length()>1){
            if(words[i+1][0]=='0'){
              if(words[i+1].length()>2){
                if(words[i+1][1]=='x'){
                  pom=16;
                }else if(words[i+1][1]=='b'){
                  pom=2;
                }else{
                  pom=8;
                }
              }
            }
          }
          a.value=stoi(words[i+1],nullptr,pom);
          args.push_back(a);
          skip=1;
        }else if(regex_match(words[i+1],symbol_r)){
          Argument a;
          a.name=words[i+1];
          a.subType="symimm$";
          a.type=ArgumentType::SYMBOL;
          a.value=0;
          args.push_back(a);
          skip=1;
        }
      }else if(words[i]=="%"){
        if(regex_match(words[i+1],symbol_r)){
          Argument a;
          a.name=words[i+1];
          a.subType="pcrel";
          a.type=ArgumentType::SYMBOL;
          a.value=0;
          args.push_back(a);
          skip=1;

        }
      }else if(words[i]=="*"){
        if(regex_match(words[i+1],literal_r)){
          Argument a;
          a.name="";
          a.subType="litdir";
          a.type=ArgumentType::LITERAL;int pom=10;
          if(words[i+1].length()>1){
            if(words[i+1][0]=='0'){
              if(words[i+1].length()>2){
                if(words[i+1][1]=='x'){
                  pom=16;
                }else if(words[i+1][1]=='b'){
                  pom=2;
                }else{
                  pom=8;
                }
              }
            }
          }
          a.value=stoi(words[i+1],nullptr,pom);
          args.push_back(a);
          skip=1;
        }else if(regex_match(words[i+1],reg_r)){
          Argument a;
          a.name=words[i+1];
          a.subType="regdir";
          a.type=ArgumentType::REGISTER;
          a.value=0;
          args.push_back(a);
          skip=1;
        }else if(regex_match(words[i+1],symbol_r)){
          Argument a;
          a.name=words[i+1];
          a.subType="symdir";
          a.type=ArgumentType::SYMBOL;
          a.value=0;
          args.push_back(a);
          skip=1;
        }else if(words[i+1]=="["){
          if(words[i+3]=="]"){
            Argument a;
            a.name=words[i+2];
            a.subType="regind";
            a.type=ArgumentType::REGISTER;
            a.value=0;
            args.push_back(a);
            skip=3;
          }else if(words[i+3]=="+"){
            Argument a1;
            a1.name=words[i+2];
            a1.subType="regpom";
            a1.type=ArgumentType::REGISTER;
            a1.value=0;
            args.push_back(a1);
            Argument a2;
            a2.subType="regpom";
            if(regex_match(words[i+4],literal_r)){
              a2.type=ArgumentType::LITERAL;
              a2.name="";int pom=10;
              if(words[i+4].length()>1){
                if(words[i+4][0]=='0'){
                  if(words[i+4].length()>2){
                    if(words[i+4][1]=='x'){
                      pom=16;
                    }else if(words[i+4][1]=='b'){
                      pom=2;
                    }else{
                      pom=8;
                    }
                  }
                }
              }
              a2.value=stoi(words[i+4],nullptr,pom);
              
            }else if(regex_match(words[i+4],symbol_r)){
              a2.type=ArgumentType::SYMBOL;
              a2.name=words[i+4];
              a2.value=0;
            }
            args.push_back(a2);
            skip=5;
          }
        }
      }else if(words[i]=="["){
        if(words[i+2]=="]"){
          if(regex_match(words[i+1],reg_r)){
            Argument a;
            a.name=words[i+1];
            a.type=ArgumentType::REGISTER;
            a.subType="regind";
            a.value=0;
            args.push_back(a);
            skip=2;
          }
        }else if(words[i+2]=="+"){

          Argument a1;
            a1.name=words[i+1];
            a1.subType="regpom";
            a1.type=ArgumentType::REGISTER;
            a1.value=0;
            args.push_back(a1);
            Argument a2;
            a2.subType="regpom";
            if(regex_match(words[i+3],literal_r)){
              a2.type=ArgumentType::LITERAL;
              a2.name="";int pom=10;
              if(words[i+3].length()>1){
                if(words[i+3][0]=='0'){
                  if(words[i+3].length()>2){
                    if(words[i+3][1]=='x'){
                      pom=16;
                    }else if(words[i+3][1]=='b'){
                      pom=2;
                    }else{
                      pom=8;
                    }
                  }
                }
              }
              a2.value=stoi(words[i+3],nullptr,pom);
              
            }else if(regex_match(words[i+3],symbol_r)){
              a2.type=ArgumentType::SYMBOL;
              a2.name=words[i+3];
              a2.value=0;
            }
            args.push_back(a2);
            skip=4;
        }
      }else if(regex_match(words[i],literal_r)){
        Argument a;
        a.name="";
        a.type=ArgumentType::LITERAL;
        a.subType="litimm";int pom=10;
          if(words[i].length()>1){
            if(words[i][0]=='0'){
              if(words[i].length()>2){
                if(words[i][1]=='x'){
                  pom=16;
                }else if(words[i][1]=='b'){
                  pom=2;
                }else{
                  pom=8;
                }
              }
            }
          }
          a.value=stoi(words[i],nullptr,pom);
        args.push_back(a);
      }else if(regex_match(words[i],reg_r)){
        Argument a;
        a.name=words[i];
        a.type=ArgumentType::REGISTER;
        a.subType="regdir";
        a.value=0;
        args.push_back(a);
      }else if(regex_match(words[i],symbol_r)){
        Argument a;
        a.name=words[i];
        a.type=ArgumentType::SYMBOL;
        a.subType="symimm";
        a.value=0;
        args.push_back(a);
      }
      zapeta=true;
      i+=skip+1;
    }
    
  }
  return args;
}

void Formator::clear(){
  this->words.clear();
  this->wordsNum=0;
}