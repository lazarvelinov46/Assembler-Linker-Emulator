#include <iostream>
#include <fstream>
#include <iomanip>
#include "../inc/assembler.h"

using namespace std;
/*
void Assembler::processLine(string line){
  StringList *words=split(line);
  

  if(words==nullptr || words->getFirst()==nullptr||words->getLast()){
    return;
  }
  lines->addString(line);

  Word* temp=words->getFirst();
  while(temp){
    //labela
    string str=temp->word;
    if(str.back()==':'){
      if(temp!=words->getFirst()){
        cout<<"labela nije na pocetku linije"<<endl;

      }else{
        str.pop_back();
        if(check(str)==false){
          cout<<"Labela je u losem formatu"
        }
        symTable->addSymbol()
      }
    }
    //global extern section word skip end
    else if(str.front()=='.'){
      if(str==".global"){
        //global
        if(str==words->getLast()->word){
          cout<<"Greska!!! Mora neki simbol uz global"<<endl;
        }
        temp=temp->next;
        while(temp){
          if(temp->next!= nullptr && temp->word.back()!=','){
            cout<<"Lose razdvojeni argumenti"<<endl;
          }
          if(temp->word.back()==','){
            temp->word.pop_back();
          }
          Symbol* s=new Symbol(temp->word,0,lc,".UND","global");
          if(this->symTable->getSymbol(s->getName())!=nullptr){
            cout<<"simbol vec definisan"<<endl;
          }
          else{
            this->symTable->addSymbol(*s);
          }

          //dodaj valjda nmp
        }
      }
      else if(str==".extern"){
        if(str==words->getLast()->word){
          cout<<"Greska!!! Mora neki simbol uz extern"<<endl;
        }
        temp=temp->next;
        while(temp){
          if(temp->next!= nullptr && temp->word.back()!=','){
            cout<<"Lose razdvojeni argumenti"<<endl;
          }
          if(temp->word.back()==','){
            temp->word.pop_back();
          }
          Symbol* s= new Symbol(temp->word,0,lc,".UND","global");
          if(this->symTable->getSymbol(s->getName())!=nullptr){
            cout<<"simbol vec definisan"<<endl;
          }
          else{
            this->symTable->addSymbol(*s);
          }
          //dodaj valjda nmp
        }
      }
      else if(str==".section"){
        //section
        if(str==words->getLast()->word){
          cout<<"Greska!!! Mora neki simbol uz extern"<<endl;
        }
        temp=temp->next;
        while(temp){
          if(temp->next!= nullptr && temp->word.back()!=','){
            cout<<"Lose razdvojeni argumenti"<<endl;
          }
          if(temp->word.back()==','){
            temp->word.pop_back();
          }
          if(sections->getSection(temp->word)){
            sections->setSection(temp->word);
          }else{
            this->sections->add(temp->word);
          }
        }
      }
    }
  }
}

StringList* Assembler::split(string line){
  StringList* words= new StringList();
  string current="";
  for(char c:line){
    if(c=='#'){
      break;
    }else if(c==' '|| c=='\t'|| c=='\n'){
      if(current!=""){
        words->addString(current);
        current="";
      }
    }else{
      current+=c;
    }
  }
  if(current!=""){
    words->addString(current);
  }
  return words;
}

bool Assembler::check(string str){

}

*/
void Assembler::startAssembling(string inputFile,string outputFile){
  inf.open(inputFile);
  off.open(outputFile);
  string line;
  while(getline(inf,line)){
    formator->formatLine(line);
    
    
    FormatedLine fl=formator->formatWords(formator->getWords());
    if(fl.getLabel()){
      this->addLabel(fl.getLabelName());
    }
    switch (fl.getLineType())
    {

    case lineType::DIRECTIVE:
    
      assembleDirective(fl);
      break;
    case lineType::INSTRUCTION:
      assembleInstruction(fl);
      break;
    default:
      break;
    }
    formator->clear();
  }
  inf.close();
  off.close();
  this->formator=new Formator();
  this->symTable=new SymTable();
  this->backpathTab.clear();
  this->sections.clear();
  this->relaTab.clear();
  currSection=-1;
}
void Assembler::addLabel(string label){
  if(this->symTable->getSymbol(label)){
    if(!(this->symTable->getSymbol(label)->getLocal()=="GLOBAL"&&this->symTable->getIndexByName(label)==-1)){
      throw exception();
    }
    this->symTable->getSymbol(label)->setValue(this->sections[this->currSection].getLocationCounter());
    this->symTable->getSymbol(label)->setIndex(this->sections[currSection].getBase());
    this->symTable->getSymbol(label)->setDefined(true);
  }
  else{
  this->symTable->addSymbol(this->sections[this->currSection].getLocationCounter(),0,"NOTYP","LOCAL",
    this->sections[this->currSection].getBase(),label);
  }
}

void Assembler::assembleDirective(FormatedLine fl){

    int secnumber=0;
  switch (fl.getDirective())
  {
  case Directive::GLOBAL:
    /* code */
    if(fl.getNoArguments()>0){
      for(int i=0;i<fl.getNoArguments();i++){
        if(fl.getArgument(i).type==ArgumentType::LITERAL){
          throw exception();
        }else if(fl.getArgument(i).type==ArgumentType::REGISTER){
          throw exception();
        }else if(fl.getArgument(i).type==ArgumentType::SYMBOL){
          //regulare
          string name =fl.getArgument(i).name;
          int ind=symTable->getNumberByName(name);
          if(ind!=-20){
            symTable->setLocal(ind,"GLOBAL");
          }else{
            Symbol *s=new Symbol(0,0,"NOTYP","GLOBAL",-1,name,false);
            this->symTable->addSymbol(*s);

          }
        }else{
          throw exception();
        }
      }
    }else{
      throw exception();
    }
    break;
  case Directive::EXTERN:
    if(fl.getNoArguments()>0){
      //cout<<"extern"<<endl;
      for(int i=0;i<fl.getNoArguments();i++){
        if(fl.getArgument(i).type!=ArgumentType::SYMBOL){
          throw exception();
        }
        //proveri tip simbola
        int ind=symTable->getNumberByName(fl.getArgument(i).name);
        if(ind!=-20){
          //vec deklarisan
          cout<<"extern declared"<<endl;
          throw exception();
        }else{
          Symbol *s=new Symbol(0,0,"NOTYP","GLOBAL",-1,fl.getArgument(i).name,false);
          symTable->addSymbol(*s);
        }

      }
    }else{
      throw exception();
    }
    break;
  case Directive::SECTION:
    if(fl.getNoArguments()==1){
      if(fl.getArgument(0).type!=ArgumentType::SYMBOL){
        throw exception();
      }
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
        if(ind!=-20){
          //vec deklarisan
          throw exception();
        }

      Section sec=Section(symTable->getSize());
      Symbol *s=new Symbol(0,0,"SECTION","LOCAL",symTable->getSize(),fl.getArgument(0).name);
      sec.name=fl.getArgument(0).name;

      currSection=sec.name;
      this->sections.insert(std::pair<string,Section>(currSection,sec));
      this->secpom.push_back(currSection);
      RelocationTable *rt=new RelocationTable();
      this->relaTab.insert(std::pair<string,RelocationTable>(currSection,*rt));
      BackpatchTable *bpt=new BackpatchTable();
      this->backpathTab.insert(std::pair<string,BackpatchTable>(currSection,*bpt));
      this->symTable->addSymbol(*s);
    }else{
      cout<<"section bad atguments"<<endl;
      throw exception();
    }
    break;
  case Directive::WORD:
    if(fl.getNoArguments()>0){
      for(int i=0;i<fl.getNoArguments();i++){
        if(fl.getArgument(i).type==ArgumentType::LITERAL){

          int value=fl.getArgument(i).value;
          this->sections[currSection].addWord(value);
        }else if(fl.getArgument(i).type==ArgumentType::REGISTER){

          throw exception();
        }else if(fl.getArgument(i).type==ArgumentType::SYMBOL){

          int ind=symTable->getNumberByName(fl.getArgument(0).name);

          if(ind!=-20){
            //vec deklarisan dodaj u 
            
            Relocation r(this->sections[currSection].getLocationCounter(),RelaType::LITTLE,ind,0);

            if(symTable->getSymbol(fl.getArgument(i).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(i).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(i).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }

            relaTab[currSection].addMade(r);

          }else{
            
              backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"LITTLE",
              fl.getArgument(i).name,false);
          }

          sections[currSection].addEmptyWord();

        }
      }
    }else{
      throw exception();
    }
    break;
  case Directive::SKIP:
    if(fl.getNoArguments()==1){
      if(fl.getArgument(0).type==ArgumentType::LITERAL){
        //proveri tip
        for(int i=0;i<fl.getArgument(0).value;i++){
          sections[currSection].addByte(0x00);
        }
      }else{
        throw exception();
      }
    }else{
      throw exception();
    }
    break;
  case Directive::END:
    backpatch();
    //ispis
    off<<"symtab"<<endl;
    off<<"num"<<'\t'<<"value"<<'\t'<<"size"<<'\t'<<"type"<<'\t'<<"bind"<<'\t'<<"index"<<'\t'<<"name"<<endl;
    for(int i=0;i<symTable->getSize();i++){

      Symbol *s=symTable->getSymbolByNum(i);

      if(s->getType()=="SECTION"){
        
        off<<dec<<s->getNum()<<'\t'<<s->getValue()<<'\t'<<sections[s->getName()].getLocationCounter()<<'\t'
        <<s->getType()<<'\t'<<s->getLocal()<<'\t'<<s->getIndex()
        <<'\t'<<s->getName()<<endl;
      }else{
        off<<dec<<s->getNum()<<'\t'<<s->getValue()<<'\t'<<s->getSize()<<'\t'
        <<s->getType()<<'\t'<<s->getLocal()<<'\t'<<s->getIndex()
        <<'\t'<<s->getName()<<endl;
      }
    }
    
    off<<"relatabs"<<endl;
    for(int i=0;i<sections.size();i++){
      string secp=secpom[i];
      off<<"section "<<secpom[i]<<endl;
      off<<"offset"<<'\t'<<"type"<<'\t'<<"symbol"<<'\t'<<"addend"<<endl;
      for(int j=0;j<relaTab[secp].getSize();j++){
        Relocation *r=relaTab[secp].getRel(j);
        off<<hex<<r->offset<<'\t';
        off<<dec<<r->type<<'\t'<<r->symbol<<'\t'<<r->addend<<endl;
      }
    }
    off<<"sections"<<endl;
    for(int i=0;i<sections.size();i++){
      off<<"section: "<<secpom[i]<<endl;
      vector<unsigned char> values=sections[secpom[i]].getValues();
      for(int j=0;j<values.size();j++){
        if(j%8==0&&j!=0){
          off<<endl;
        }
        //off<<static_cast<char>(values[j]);
        off<<setw(2) << setfill('0') << hex << (int) values[j]<<" ";
      }
      off<<endl;
    }
    break;
  default:

  throw exception();
    break;
  }
}

void Assembler::assembleInstruction(FormatedLine fl){
  Instruction instr=fl.getInstruction();
  int r1,r2;
  unsigned char regpush,regpop;
  switch (instr)
  {
  case Instruction::HALT:
    if(fl.getNoArguments()==0){
      sections[currSection].addByte(0x00);
    }else{
      throw exception();
    }
    break;
  case Instruction::INT:
    if(fl.getNoArguments()==1){
      if(fl.getArgument(0).type==ArgumentType::REGISTER){
        
        if(fl.getArgument(0).subType!="regdir"){
          throw exception();
        }
        sections[currSection].addByte(0x10);
        string regname=fl.getArgument(0).name;
        unsigned char reg=fl.getRegNum(regname);
        reg=reg<<4;
        reg=reg|0x0f;
        sections[currSection].addByte(reg);
      }else{
        throw exception();
      }
    }else{
      throw exception();
    }
    break;
  case Instruction::IRET:
    if(fl.getNoArguments()!=0){
      throw exception();
    }
    this->sections[currSection].addByte(0x20);
    break;
  case Instruction::CALL:

    if(fl.getNoArguments()!=1&&fl.getArgument(0).subType!="regpom"){
      
      throw exception();
    }

    if(fl.getNoArguments()!=2&&fl.getArgument(0).subType=="regpom"){
      
      throw exception();
    }

    sections[currSection].addByte(0x30);
    if(fl.getArgument(0).subType=="litimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(0).value);
    }else if(fl.getArgument(0).subType=="symimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(0).subType=="pcrel"){
      
      sections[currSection].addByte(0xf7);

      sections[currSection].addByte(0x05);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){
        int sec=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
        if(sections[currSection].getBase()==sec){
          int pom=symTable->getSymbol(fl.getArgument(0).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue()-2;
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(0).name,false);
      sections[currSection].addEmptyWord();
      }


    }else if(fl.getArgument(0).subType=="litdir"){
      sections[currSection].addByte(0xff);
      sections[currSection].addByte(0x04);
      sections[currSection].addInstrData(fl.getArgument(0).value);

    }else if(fl.getArgument(0).subType=="symdir"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }

      sections[currSection].addEmptyWord();
      
    }else if(fl.getArgument(0).subType=="regdir"){

      
      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(0).subType=="regind"){


      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(0).subType=="regpom"){
      if(fl.getArgument(1).type==ArgumentType::LITERAL){
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(1).value);
      }else if(fl.getArgument(1).type==ArgumentType::SYMBOL){
        
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(1).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(1).name,false);
        }

        sections[currSection].addEmptyWord();
    }
    }
    break;
  case Instruction::RET:
    if(fl.getNoArguments()!=0){
      throw exception();
    }
    sections[currSection].addByte(0x40);
    break;
  case Instruction::JMP:

    if(fl.getNoArguments()!=1&&fl.getArgument(0).subType!="regpom"){
      
      throw exception();
    }

    if(fl.getNoArguments()!=2&&fl.getArgument(0).subType=="regpom"){
      
      throw exception();
    }

    sections[currSection].addByte(0x50);
    if(fl.getArgument(0).subType=="litimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(0).value);
    }else if(fl.getArgument(0).subType=="symimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){
        
        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(0).subType=="pcrel"){
      
      sections[currSection].addByte(0xf7);

      sections[currSection].addByte(0x05);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){
        if(sections[currSection].getBase()==symTable->getIndexByName(fl.getArgument(0).name)){
          int pom=symTable->getSymbol(fl.getArgument(0).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation rel(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              rel.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue()-2;
              rel.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();//prepraviiiiiiiiiiiiiii
        }


        relaTab[currSection].addMade(rel);

      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(0).name,false);
      sections[currSection].addEmptyWord();
      }

    }else if(fl.getArgument(0).subType=="litdir"){
      sections[currSection].addByte(0xff);
      sections[currSection].addByte(0x04);
      sections[currSection].addInstrData(fl.getArgument(0).value);

    }else if(fl.getArgument(0).subType=="symdir"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }

      sections[currSection].addEmptyWord();
      
    }else if(fl.getArgument(0).subType=="regdir"){

      
      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(0).subType=="regind"){


      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(0).subType=="regpom"){
      if(fl.getArgument(1).type==ArgumentType::LITERAL){
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(1).value);
      }else if(fl.getArgument(1).type==ArgumentType::SYMBOL){
        
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(1).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(1).name,false);
        }

        sections[currSection].addEmptyWord();
    }
    }
    break;
  case Instruction::JEQ:


    if(fl.getNoArguments()!=1&&fl.getArgument(0).subType!="regpom"){
      
      throw exception();
    }

    if(fl.getNoArguments()!=2&&fl.getArgument(0).subType=="regpom"){
      
      throw exception();
    }

    sections[currSection].addByte(0x51);
    if(fl.getArgument(0).subType=="litimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(0).value);
    }else if(fl.getArgument(0).subType=="symimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(0).subType=="pcrel"){
      
      sections[currSection].addByte(0xf7);

      sections[currSection].addByte(0x05);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){
        if(sections[currSection].getBase()==symTable->getIndexByName(fl.getArgument(0).name)){
          int pom=symTable->getSymbol(fl.getArgument(0).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation rel(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              rel.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue()-2;
              rel.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
        }


        relaTab[currSection].addMade(rel);

      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(0).name,false);
      sections[currSection].addEmptyWord();
      }

    }else if(fl.getArgument(0).subType=="litdir"){
      sections[currSection].addByte(0xff);
      sections[currSection].addByte(0x04);
      sections[currSection].addInstrData(fl.getArgument(0).value);

    }else if(fl.getArgument(0).subType=="symdir"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }

      sections[currSection].addEmptyWord();
      
    }else if(fl.getArgument(0).subType=="regdir"){

      
      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(0).subType=="regind"){


      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(0).subType=="regpom"){
     if(fl.getArgument(1).type==ArgumentType::LITERAL){
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(1).value);
      }else if(fl.getArgument(1).type==ArgumentType::SYMBOL){
        
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(1).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();
            }
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(1).name,false);
        }

        sections[currSection].addEmptyWord(); 
    }
    }
    break;
  case Instruction::JNE:


    if(fl.getNoArguments()!=1&&fl.getArgument(0).subType!="regpom"){
      
      throw exception();
    }

    if(fl.getNoArguments()!=2&&fl.getArgument(0).subType=="regpom"){
      
      throw exception();
    }

    sections[currSection].addByte(0x52);
    if(fl.getArgument(0).subType=="litimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(0).value);
    }else if(fl.getArgument(0).subType=="symimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(0).subType=="pcrel"){
      
      sections[currSection].addByte(0xf7);

      sections[currSection].addByte(0x05);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){
        if(sections[currSection].getBase()==symTable->getIndexByName(fl.getArgument(0).name)){
          int pom=symTable->getSymbol(fl.getArgument(0).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation rel(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              rel.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue()-2;
              rel.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
        }


        relaTab[currSection].addMade(rel);

      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(0).name,false);
      sections[currSection].addEmptyWord();
      }

    }else if(fl.getArgument(0).subType=="litdir"){
      sections[currSection].addByte(0xff);
      sections[currSection].addByte(0x04);
      sections[currSection].addInstrData(fl.getArgument(0).value);

    }else if(fl.getArgument(0).subType=="symdir"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }

      sections[currSection].addEmptyWord();
      
    }else if(fl.getArgument(0).subType=="regdir"){

      
      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(0).subType=="regind"){


      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(0).subType=="regpom"){
      if(fl.getArgument(1).type==ArgumentType::LITERAL){
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(1).value);
      }else if(fl.getArgument(1).type==ArgumentType::SYMBOL){
        
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(1).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(1).name,false);
        }

        sections[currSection].addEmptyWord();
    }
    }
    break;
  case Instruction::JGT:


    if(fl.getNoArguments()!=1&&fl.getArgument(0).subType!="regpom"){
      
      throw exception();
    }

    if(fl.getNoArguments()!=2&&fl.getArgument(0).subType=="regpom"){
      
      throw exception();
    }

    sections[currSection].addByte(0x53);
    if(fl.getArgument(0).subType=="litimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(0).value);
    }else if(fl.getArgument(0).subType=="symimm"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(0).subType=="pcrel"){
      
      sections[currSection].addByte(0xf7);

      sections[currSection].addByte(0x05);
      int ind=symTable->getIndexByName(fl.getArgument(0).name);
      if(ind!=-20){
        if(sections[currSection].getBase()==symTable->getIndexByName(fl.getArgument(0).name)){
          int pom=symTable->getSymbol(fl.getArgument(0).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation rel(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              rel.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue()-2;
              rel.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
        }


        relaTab[currSection].addMade(rel);

      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(0).name,false);
      sections[currSection].addEmptyWord();
      }

    }else if(fl.getArgument(0).subType=="litdir"){
      sections[currSection].addByte(0xff);
      sections[currSection].addByte(0x04);
      sections[currSection].addInstrData(fl.getArgument(0).value);

    }else if(fl.getArgument(0).subType=="symdir"){

      sections[currSection].addByte(0xff);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(0).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(0).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(0).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(0).name,false);
      }

      sections[currSection].addEmptyWord();
      
    }else if(fl.getArgument(0).subType=="regdir"){

      
      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(0).subType=="regind"){


      sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(0).subType=="regpom"){
      if(fl.getArgument(1).type==ArgumentType::LITERAL){
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(1).value);
      }else if(fl.getArgument(1).type==ArgumentType::SYMBOL){
        
        sections[currSection].addByte(0xf0|fl.getRegNum(fl.getArgument(0).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(1).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(0).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(1).name,false);
        }

        sections[currSection].addEmptyWord();
    }
    }
    break;
  case Instruction::PUSH:
    if(fl.getNoArguments()!=1){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"){
      throw exception();
    }
    sections[currSection].addByte(0xb0);
    regpush=fl.getRegNum(fl.getArgument(0).name);
    regpush=regpush<<4;
    regpush=regpush|0x06;
    sections[currSection].addByte(regpush);
    sections[currSection].addByte(0x12);
    break;
  case Instruction::POP:
    if(fl.getNoArguments()!=1){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"){
      throw exception();
    }
    sections[currSection].addByte(0xa0);
    regpop=fl.getRegNum(fl.getArgument(0).name);
    regpop=regpop<<4;
    regpop=regpop|0x06;
    sections[currSection].addByte(regpop);
    sections[currSection].addByte(0x42);
    /* code */
    break;
  case Instruction::XCHG:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }
    sections[currSection].addByte(0x60);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::ADD:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x70);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::SUB:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x71);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::MUL:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x72);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::DIV:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x73);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::CMP:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x74);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::NOT:
    if(fl.getNoArguments()==1){
      if(fl.getArgument(0).type==ArgumentType::REGISTER){
        if(fl.getArgument(0).subType!="regdir"){
          throw exception();
        }

        sections[currSection].addByte(0x80);
        r1=fl.getRegNum(fl.getArgument(0).name);
        sections[currSection].addByte(r1<<4||0x0f);
      }else{
        throw exception();
      }
    }else{
      throw exception();
    }
    break;
  case Instruction::AND:

    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x81);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::OR:

    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x82);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::XOR:

    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x83);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::TEST:

    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x84);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::SHL:

    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x90);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::SHR:

    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER||fl.getArgument(1).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"||fl.getArgument(1).subType!="regdir"){
      throw exception();
    }

    sections[currSection].addByte(0x91);
    r1=fl.getRegNum(fl.getArgument(0).name);
    r2=fl.getRegNum(fl.getArgument(1).name);
    sections[currSection].addByte(r1<<4|r2);
    break;
  case Instruction::LDR:
    if(fl.getNoArguments()!=2&&fl.getArgument(1).subType!="regpom"){
      throw exception();
    }

    if(fl.getNoArguments()!=3&&fl.getArgument(1).subType=="regpom"){
      
      cout<<"drug"<<endl;
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER){
      cout<<"tre"<<endl;
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"){
      cout<<"cet"<<endl;
      throw exception();
    }
    sections[currSection].addByte(0xa0);
    if(fl.getArgument(1).subType=="litimm$"){
      unsigned char r=fl.getRegNum(fl.getArgument(0).name);
      r=r<<4;
      r=r|0x0f;
      sections[currSection].addByte(r);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(1).value);
    }
    else if(fl.getArgument(1).subType=="symimm$"){
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(1).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(1).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(1).subType=="symimm"){

      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(1).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(1).name,false);
      }

      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(1).subType=="litimm"){
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);
      sections[currSection].addByte(0x04);
      //cout<<fl.getArgument(1).value<<endl;
      sections[currSection].addInstrData(fl.getArgument(1).value);

    }else if(fl.getArgument(1).subType=="pcrel"){
      
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x07);

      sections[currSection].addByte(0x03);
      int ind=symTable->getNumberByName(fl.getArgument(1).name);
      if(ind!=-20){
        if(sections[currSection].getBase()==symTable->getIndexByName(fl.getArgument(0).name)){
          int pom=symTable->getSymbol(fl.getArgument(1).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation rel(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              rel.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue()-2;
              rel.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();
        }


        relaTab[currSection].addMade(rel);

      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(1).name,false);

      sections[currSection].addEmptyWord();
      }

    }else if(fl.getArgument(1).subType=="regdir"){

      
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(1).subType=="regind"){


      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(1).subType=="regpom"){
      if(fl.getArgument(2).type==ArgumentType::LITERAL){
        sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(2).value);
      }else if(fl.getArgument(2).type==ArgumentType::SYMBOL){
        sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(2).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(2).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(2).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(2).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(2).name,false);
        }

        sections[currSection].addEmptyWord();
    }
    }
    break;
  case Instruction::STR:
    if(fl.getNoArguments()!=2){
      throw exception();
    }
    if(fl.getArgument(0).type!=ArgumentType::REGISTER){
      throw exception();
    }
    if(fl.getArgument(0).subType!="regdir"){
      throw exception();
    }
    sections[currSection].addByte(0xb0);
    if(fl.getArgument(1).subType=="litimm$"){

      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);

      sections[currSection].addByte(0x00);
      sections[currSection].addInstrData(fl.getArgument(1).value);
    }
    else if(fl.getArgument(1).subType=="symimm$"){
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);

      sections[currSection].addByte(0x00);
      int ind=symTable->getNumberByName(fl.getArgument(1).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(1).name,false);
      }
      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(1).subType=="symimm"){

      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);

      sections[currSection].addByte(0x04);
      int ind=symTable->getNumberByName(fl.getArgument(1).name);
      if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
              fl.getArgument(1).name,false);
      }

      sections[currSection].addEmptyWord();
    }else if(fl.getArgument(1).subType=="litimm"){
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x0f);
      sections[currSection].addByte(0x04);
      sections[currSection].addInstrData(fl.getArgument(1).value);

    }else if(fl.getArgument(1).subType=="pcrel"){
      
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|0x07);

      sections[currSection].addByte(0x03);
      int ind=symTable->getNumberByName(fl.getArgument(1).name);
      if(ind!=-20){
        if(sections[currSection].getBase()==symTable->getIndexByName(fl.getArgument(0).name)){
          int pom=symTable->getSymbol(fl.getArgument(1).name)->getValue()-sections[currSection].getLocationCounter()-2;
          sections[currSection].addInstrData(pom);
        }else{
        Relocation rel(this->sections[currSection].getLocationCounter(),RelaType::PCREL,ind,-2);
        if(symTable->getSymbol(fl.getArgument(1).name)->getLocal()=="LOCAL"){
              rel.addend=symTable->getSymbol(fl.getArgument(1).name)->getValue()-2;
              rel.symbol=symTable->getSymbol(fl.getArgument(1).name)->getIndex();
        }


        relaTab[currSection].addMade(rel);

      sections[currSection].addEmptyWord();
        }
      }else{
        backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"PCREL",
              fl.getArgument(1).name,false);
      sections[currSection].addEmptyWord();
      }

    }else if(fl.getArgument(1).subType=="regdir"){

      
      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));

      sections[currSection].addByte(0x01);      
      
    }else if(fl.getArgument(1).subType=="regind"){


      sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));

      sections[currSection].addByte(0x02);      
      
      
    }else if(fl.getArgument(1).subType=="regpom"){
      if(fl.getArgument(2).type==ArgumentType::LITERAL){
        sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));
        sections[currSection].addByte(0x03);
        sections[currSection].addInstrData(fl.getArgument(2).value);
      }else if(fl.getArgument(2).type==ArgumentType::SYMBOL){
        sections[currSection].addByte(fl.getRegNum(fl.getArgument(0).name)<<4|fl.getRegNum(fl.getArgument(1).name));
        sections[currSection].addByte(0x03);

        int ind=symTable->getNumberByName(fl.getArgument(2).name);
        if(ind!=-20){

        Relocation r(this->sections[currSection].getLocationCounter(),RelaType::BIG,ind,0);
        if(symTable->getSymbol(fl.getArgument(2).name)->getLocal()=="LOCAL"){
              r.addend=symTable->getSymbol(fl.getArgument(2).name)->getValue();
              r.symbol=symTable->getSymbol(fl.getArgument(2).name)->getIndex();//prepraviiiiiiiiiiiiiii
            }
        relaTab[currSection].addMade(r);
        }else{
          backpathTab[currSection].addBackpatch(sections[currSection].getLocationCounter(),"BIG",
                fl.getArgument(2).name,false);
        }

        sections[currSection].addEmptyWord();
      }
    }
    break;
  default:
  throw exception();
    break;
  }
}
void Assembler::backpatch(){
  //prodji kroz svaku bekpec tabelu za sve sekcije
  for(int sec=0;sec<backpathTab.size();sec++){
    //prolazak tabele za jednu sekciju
      BackpatchTable b=backpathTab[secpom[sec]];
    for(int i=0;i<this->backpathTab[secpom[sec]].getSize();i++){
      int symnum=symTable->getNumberByName(b.getElement(i).symbol);
      if(symnum==-20){
        //cout<<b.getElement(i).symbol<<endl;
        throw exception();
      }
      if(b.getElement(i).type=="LITTLE"){

        Relocation r(b.getElement(i).offset,RelaType::LITTLE,symnum,0);
        if(symTable->getSymbol(b.getElement(i).symbol)->getLocal()=="LOCAL"){
          r.symbol=symTable->getIndexByName(b.getElement(i).symbol);
          r.addend=symTable->getSymbol(b.getElement(i).symbol)->getValue();
        }
        this->relaTab[secpom[sec]].addMade(r);
        symTable->getSymbol(b.getElement(i).symbol)->setDefined(true);
      }else if(b.getElement(i).type=="BIG"){
        Relocation r(b.getElement(i).offset,RelaType::BIG,symnum,0);
        if(symTable->getSymbol(b.getElement(i).symbol)->getLocal()=="LOCAL"){
          r.symbol=symTable->getIndexByName(b.getElement(i).symbol);
          r.addend=symTable->getSymbol(b.getElement(i).symbol)->getValue();
        }
        this->relaTab[secpom[sec]].addMade(r);
        symTable->getSymbol(b.getElement(i).symbol)->setDefined(true);
      }else{
        //pcrel
        int ind=symTable->getIndexByName(b.getElement(i).symbol);
        if(sections[secpom[sec]].getBase()==ind){
          int pom=symTable->getSymbol(b.getElement(i).symbol)->getValue()-b.getElement(i).offset-2;
          sections[secpom[sec]].addPcrelWord(pom,b.getElement(i).offset);
        }else{
          Relocation r(b.getElement(i).offset,RelaType::PCREL,symnum,-2);
        if(symTable->getSymbol(b.getElement(i).symbol)->getLocal()=="LOCAL"){
          r.symbol=symTable->getIndexByName(b.getElement(i).symbol);
          r.addend=symTable->getSymbol(b.getElement(i).symbol)->getValue()-2;
        }
        this->relaTab[secpom[sec]].addMade(r);
        }

        symTable->getSymbol(b.getElement(i).symbol)->setDefined(true);
      }

    }

  }
}