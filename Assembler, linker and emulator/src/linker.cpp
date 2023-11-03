#include "../inc/linker.h"

void Linker::readFile(string file){
  ifstream ifs;
  ifs.open(file);
  string line;
  infiles.push_back(file);
  SymTable *locSymTab=new SymTable();
  getline(ifs,line);
  if(line!="symtab"){
    throw exception();
  }
  getline(ifs,line);
  while(getline(ifs,line)){
    if(line=="relatabs"){
      break;
    }
    vector<string> words;
    char *word=new char[50];
    int wc=0;
    for(int i=0;i<line.length();i++){
      if(line[i+1]=='\0'||line[i+1]=='\n'){
        word[wc++]=line[i];
        word[wc]='\0';
        words.push_back(word);
        wc=0;
        i++;
      }else if(line[i]=='\t'||line[i]==' '){

        word[wc]='\0';
        words.push_back(word);
        wc=0;
      }else{

        word[wc++]=line[i];
      }
    }
    Symbol *sym=new Symbol(stoi(words[1]),stoi(words[2]),words[3],words[4],stoi(words[5]),words[6]);
    locSymTab->addSymbol(*sym);
    words.clear();
  }
  symTabs.insert(pair<string,SymTable>(file,*locSymTab));
  string currSec="";
  bool cnt=false;

  RelocationTable *rt;
  vector<RelocationTable> secreltab;
  bool first=true;
  while(getline(ifs,line)){
    if(line=="sections"){
      break;
    }
    if(cnt){
      cnt=false;
      continue;
    }
    vector<string> words;
    char *word=new char[50];
    int wc=0;
    for(int i=0;i<line.length();i++){
      if(line[i+1]=='\0'||line[i+1]=='\n'){
        word[wc++]=line[i];
        word[wc]='\0';
        words.push_back(word);
        wc=0;
        i++;
      }else if(line[i]=='\t'||line[i]==' '){

        word[wc]='\0';
        words.push_back(word);
        wc=0;
      }else{

        word[wc++]=line[i];
      }
    }
    if(words[0]=="section"){
      if(first){
        rt=new RelocationTable();
        first=false;
      }else{
        secreltab.push_back(*rt);
        rt=new RelocationTable();
      }
      currSec=words[1];
      rt->setSection(currSec);
      cnt=true;
    }else{
      Relocation r(stoi(words[0],0,16),(RelaType)stoi(words[1]),stoi(words[2]),stoi(words[3]));
      rt->addMade(r);
    }
    words.clear();
    
  }

  secreltab.push_back(*rt);
  first=true;
  relaTabs.insert(std::pair<string,vector<RelocationTable>>(file,secreltab));
  vector<Section> secfiles;
  Section *s;
  while(getline(ifs,line)){
    if(line==""){
      break;
    }
    vector<string> words;
    char *word=new char[50];
    int wc=0;
    for(int i=0;i<line.length();i++){
      if(line[i+1]=='\0'||line[i+1]=='\n'){
        word[wc++]=line[i];
        word[wc]='\0';
        words.push_back(word);
        wc=0;
        i++;
      }else if(line[i]=='\t'||line[i]==' '){

        word[wc]='\0';
        words.push_back(word);
        wc=0;
      }else{

        word[wc++]=line[i];
      }
    }
    if(words[0]=="section:"){
      if(first){
        s=new Section(symTabs.at(file).getNumberByName(words[1]));
        first=false;
      }else{
        sections.push_back(*s);
        s=new Section(symTabs.at(file).getNumberByName(words[1]));
      }
      s->name=words[1];
      s->size=(symTabs.at(file).getSymbol(words[1]))->getSize();
      s->offset=(symTabs.at(file).getSymbol(words[1]))->getValue();
      s->setFile(file);
    }else{
      for(int j=0;j<words.size();j++){
      s->addByte(stoi(words[j],0,16));
      }
    }
    words.clear();
    
  }
  sections.push_back(*s);
  ifs.close();
}

void Linker::link(string out){
  //prvi prolaz dodati sve u finalnu tabelu osim eksternih
  collectAllSymbols();
  //provera eksternih

  
  
  if(!checkExtern()){
    //throw exception();
  }
  //proslo ako je sve ok
  //spoji sekcije valjda
  this->sortSectionsOne();
  //sredi value simbola
  this->reorderValues();
  this->insertRelocations();
  this->fillData(out);

}

bool Linker::checkExtern(){
  for(int i=0;i<externSyms.size();i++){
    Symbol *s=&externSyms.at(i);
    if(finalSymTab->getNumberByName(s->getName())==-20){
      return false;
    }
  }

  return true;
}

void Linker::collectAllSymbols(){

  for(int i=0;i<infiles.size();i++){

    SymTable *st=&symTabs.at(infiles[i]);
    for(int j=0;j<st->getSize();j++){
      Symbol *s=st->getSymbolByNum(j);
      if(s->getIndex()!=-1){
        if(s->getLocal()=="GLOBAL"&&finalSymTab->getNumberByName(s->getName())!=-20){
          throw exception();
        }else{
          finalSymTab->addSymbol(*s);
        }
      }else{
        externSyms.push_back(*s);
      }
    }
  }

}

void Linker::sortSectionsOne(){
  for(int i=0;i<sections.size()-1;i++){

    //cout<<sections[i].name<<endl;
    
    for(int j=i+1;j<sections.size();j++){
      if(i+1!=j){
        if(sections.at(i).name==sections.at(j).name){
          while(sections.at(i).name==sections.at(j).name&&j<sections.size()-1){
          j++;
          }
          if(sections.at(i).name==sections.at(j).name&&j==sections.size()-1){
            break;
          }
          Section s=sections.at(j);
          for(int k=j;k>i;k--){
            sections.at(k)=sections.at(k-1);
          }
          sections.at(i)=s;
          
        }
        
      }
    }
    
  }

  for(int i=0;i<sections.size();i++){
    if(i==0){
      sections.at(i).offset=0;
      symTabs.at(sections.at(i).getFile()).getSymbol(sections.at(i).name)->addValue(0);
    }else{
      int n=sections.at(i-1).offset+sections.at(i-1).getLocationCounter();
      sections.at(i).offset=sections.at(i-1).offset+sections.at(i-1).getLocationCounter();

      symTabs.at(sections.at(i).getFile()).getSymbol(sections.at(i).name)->addValue(n);
    }
  }
}

void Linker::reorderValues(){
  //prvo svi posle eksterni
  for(int i=0;i<infiles.size();i++){
    SymTable *st=&symTabs.at(infiles[i]);
    for(int j=0;j<st->getSize();j++){
      Symbol *s=st->getSymbolByNum(j);
      if(s->getType()!="SECTION"&&s->getIndex()!=-1){
        int addn=st->getSymbolByNum(s->getIndex())->getValue();
        st->getSymbolByNum(j)->addValue(addn);
      }
    }
  }

  //sad eksterni
  for(int i=0;i<infiles.size();i++){
    SymTable *st=&symTabs.at(infiles[i]);
    for(int j=0;j<st->getSize();j++){
      Symbol *s=st->getSymbolByNum(j);
      if(s->getIndex()==-1){
        string nam=s->getName();
        int addn=0;
        for(int k=0;k<infiles.size();k++){
          SymTable *st2=&symTabs.at(infiles[k]);
          for(int l=0;l<st2->getSize();l++){
            Symbol *s2=st2->getSymbolByNum(l);
            if(s2->getName()==nam){
              if(s2->getIndex()==-1){
                continue;
              }
              else{
                addn=s2->getValue();
              }
            }
          }
        }
        st->getSymbolByNum(j)->addValue(addn);
      }
    }
  }
}

void Linker::insertRelocations(){
  int pom;
  for(int i=0;i<infiles.size();i++){
    vector<RelocationTable> *rels=&this->relaTabs.at(infiles[i]);

    for(int j=0;j<rels->size();j++){
      RelocationTable *rt=&rels->at(j);

      for(int k=0;k<rt->getSize();k++){
        Relocation *r=rt->getRel(k);
        switch (r->type)
        {
        case RelaType::BIG:
          pom=symTabs.at(infiles[i]).getSymbolByNum(r->symbol)->getValue();
          pom+=r->addend;
          for(int l=0;l<sections.size();l++){
            
            if(sections.at(l).getFile()==infiles[i]&&sections.at(l).name==rt->section){
              sections.at(l).addInstrData(r->offset,pom);
            }
          }
          break;
        case RelaType::LITTLE:
          pom=symTabs.at(infiles[i]).getSymbolByNum(r->symbol)->getValue();
          pom+=r->addend;
          for(int l=0;l<sections.size();l++){
            if(sections.at(l).getFile()==infiles[i]&&sections.at(l).name==rt->section){
              sections.at(l).addWord(r->offset,pom);
            }
          }
          break;
        case RelaType::PCREL:
          pom=symTabs.at(infiles[i]).getSymbolByNum(r->symbol)->getValue();
          if(r->offset==0xd){
          }
          
          pom+=r->addend;
          if(r->offset==0xd){
          }
          for(int l=0;l<sections.size();l++){
            if(sections.at(l).getFile()==infiles[i]&&sections.at(l).name==rt->section){

              pom=pom-r->offset-sections.at(l).offset;
              sections.at(l).addInstrData(r->offset,pom);
            }
          }
          break;
        default:
          throw exception();
          break;
        }
      }
    }
  }
}

void Linker::fillData(string out){
  for(int i=0;i<sections.size();i++){
    for(int j=0;j<sections.at(i).getValues().size();j++){
      linkedData.push_back(sections.at(i).getValues().at(j));
    }
  }
  outp.open(out);

  outp<<setw(4) << setfill('0') << hex <<0<<": ";

  outp<<setw(2) << setfill('0') << hex << (int) linkedData[0]<<" ";
  for(int i=1;i<linkedData.size();i++){
    if(i%8==0){
      outp<<endl<<setw(4) << setfill('0') << hex << i<<": ";
    }
    outp<<setw(2)<<setfill('0')<<hex<<(int) linkedData[i]<<" ";
  }
  outp.close();
}