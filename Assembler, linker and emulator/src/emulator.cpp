#include "../inc/emulator.h"
#include <bitset>

void Emulator::readFile(string in){
  inf.open(in);
  off.open(this->izlaz);
  string line;
  adc=0;
  int i;
  int wc=0;
  vector<string> words;

    char *word=new char[10];
  while(getline(inf,line)){
    i=0;
    while(line[i]!=':'){
      i++;
    }
    i++;
    i++;
    for(i;i<line.length();i++){
      if(line[i]==' '){
        word[wc]='\0';
        words.push_back(word);
        adc++;
        wc=0;
      }else if(line[i]=='\n'||line[i]=='\0'){
      }else{
        word[wc++]=line[i];
      }
    }
  }
  for(int j=0;j<words.size();j++){
    mem[j]=stoi(words[j],nullptr,16);
    off<<setw(2) << setfill('0')<<hex<<(int)mem[j]<<" ";
    if(j%8==0){
      off<<endl;
    }
  }
  /*
  for(int i=0;i<adc;i++){
    cout<<mem[i];
    if(i%8==0){
      cout<<endl;
    }
  }
  */
  off.close();
  inf.close();
}

Emulator::Emulator(){
  for(int i=0;i<7;i++){
    reg[i]=0;
  }
  reg[7]=92;
  psw=0;
  this->end=false;
}

void Emulator::emulate(string file){
  this->readFile(file);
  int instrlen;
  unsigned char first;
  while(!end){
    first=mem[reg[7]++];
    instr.push_back(mem[reg[7]]);
    string instr=checkInstr(first);
    int len=checkLen(instr);
    if(len==1){
      if(instr=="HALT"){
        end=true;
      }else if(instr =="IRET"){
        this->psw=mem[reg[6]++];
        int pom=mem[reg[6]++];
        pom=pom<<8;
        psw=psw|pom;
        reg[7]=mem[reg[6]++];
        pom=mem[reg[6]++];
        pom=pom<<8;
        reg[7]=reg[7]|pom;
      }else{
        reg[7]=mem[reg[6]++];
        int pom=mem[reg[6]++];
        pom=pom<<8;
        reg[7]=reg[7]|pom;
      }
    }else if(len==2){
      int instrreg=mem[reg[7]++];
      int destination=(instrreg>>4)&0xf;
      int source=instrreg&0xf;
      if(instr=="INT"){
        mem[--reg[6]]=(reg[7]>>8)&0xff;
        mem[--reg[6]]=reg[7]&0xff;
        mem[--reg[6]]=(psw>>8)&0xff;
        mem[--reg[6]]=psw&0xff;
        int adr=this->ivtp+(reg[destination]%8)*2;
        reg[7]=mem[adr++];
        int pom=mem[adr];
        pom=pom<<8;
        reg[7]=reg[7]|pom;
      }else if (instr=="XCHG"){
        int pom=reg[destination];
        reg[destination]=reg[source];
        reg[source]=pom;
      }else if(instr=="ADD"){
        reg[destination]=reg[destination]+reg[source];
      }else if(instr=="SUB"){
        reg[destination]=reg[destination]-reg[source];
      }else if(instr=="MUL"){
        reg[destination]=reg[destination]*reg[source];
      }else if(instr=="DIV"){
        reg[destination]=reg[destination]/reg[source];
      }else if(instr=="CMP"){
        int pom=reg[destination]-reg[source];
        if (pom==0) psw|=0x0001;
        else psw&=0xfffe;
        if(pom&0x8000) psw|=0x0008;
        else psw&=0xfff7;
        //dest pozitivan sourc negativan rez negativan --> overflou
        //dest negativan sors pozitivan rez pozitivan ---overflou
        if((pom&0x8000>0&&reg[destination]&0x8000==0&&reg[source]&0x8000>0)
          ||(pom&0x8000==0&&reg[destination]&0x8000>0&&reg[source]&0x8000==0)) psw|=0x0002;
        else psw&=0xfffd;
        if((reg[destination]&0x8000==0&&reg[source]&0x8000>0&&pom&0x8000==0)||
          (reg[destination]&0x8000>0 && reg[source]&0x8000>0 && pom&0x8000>0)||
          (reg[destination]&0x8000>0  && reg[source]&0x8000==0 && pom&0x8000>0)){
            psw|=0x0004;
          }
        else{
          psw&=0xfffb;
        }
        //treba li c?
      }else if(instr=="NOT"){
        reg[destination]=~reg[destination];
      }else if(instr=="AND"){
        reg[destination]=reg[destination]&reg[source];
      }else if(instr=="OR"){
        reg[destination]=reg[destination]|reg[source];
      }else if(instr=="XOR"){
        reg[destination]=reg[destination]^reg[source];
      }else if(instr=="TEST"){
        int pom=reg[destination]&reg[source];
        if(pom==0)psw|=0x0001;
        else psw&=0xfffe;
        if(pom&0x8000)psw|=0x0008;
        else psw&=0xfff7;
      }else if(instr=="SHL"){
        bool c;
        for(int i=0;i<reg[source]&&i<16;i++){
          if(reg[destination]&0x8000){
            c=true;
          }
          reg[destination]<<1;
        }
        if(reg[destination]==0) psw|=0x0001;
        else psw&=0xfffe;
        if(reg[destination]&0x8000)psw|=0x0008;
        else psw&=0xfff7;
        if(c)psw|=0x0004;
        else psw&=0xfffb;

      }else if(instr=="SHR"){
        bool c;
        for(int i=0;i<reg[source]&&i<16;i++){
          if(reg[destination]&0x0001){
            c=true;
          }
          reg[destination]>>1;
        }
        if(reg[destination]==0) psw|=0x0001;
        else psw&=0xfffe;
        if(reg[destination]&0x8000)psw|=0x0008;
        else psw&=0xfff7;
        if(c)psw|=0x0004;
        else psw&=0xfffb;
      }
    }else{
      int instrreg=mem[reg[7]++];
      int dest=instrreg>>4;
      dest&=0x0f;
      int src=instrreg&0x0f;
      unsigned char addr=mem[reg[7]++];
      unsigned short addrmod=addr&0x0f;
      unsigned short updt=(addr>>4)&0x0f;
      short op;
      if(addrmod!=2&&addrmod!=1){
        int pom1=mem[reg[7]++]<<8;
        int pom2=mem[reg[7]++];
        op=pom1|pom2;
      }
      if(addrmod==0){
        if(instr=="STR")throw exception();
        src=op;
      }else if(addrmod==1){
        if(instr=="STR")dest=src;
        else src=reg[src];
      }else if(addrmod==2){
        if(updt==1){
          reg[src]-=2;
        }else if(updt==2){
          reg[src]+=2;
        }
        if(instr=="STR"){
          int a=dest;
          dest=reg[src];
          if(a<8){
            src=reg[a];
          }
        }
        else{
          int a=reg[src];
          src=mem[a];
          int pom=mem[a+1];
          src=(pom<<8)|src;
          
        }
        if(updt==3){
          reg[6]-=2;
        }else if(updt==4){
          reg[6]+=2;
        }
      }else if(addrmod==3){
        if(updt==1){
          reg[src]-=2;
        }else if(updt==2){
          reg[src]+=2;
        }
        if(instr=="STR"){
          dest=reg[src]+op;
        }
        else{
          int a=src;
          src=mem[reg[a]+op];
          int pom=mem[reg[a]+op+1];
          src=(pom<<8)|src;
          
        }
        if(updt==3){
          reg[6]-=2;
        }else if(updt==4){
          reg[6]+=2;
        }
      }else if(addrmod==4){
        if(instr=="STR"){
          src=reg[dest];
          dest=op;
        }
        else{
          src=mem[op];
          int pom=mem[op+1];
          src=(pom<<8)|src;
        }
      }else if(addrmod==5){
        if(instr=="STR"){
          dest=reg[src]+op;
        }else{
          src=reg[src]+op;
        }
      }
      if(instr=="CALL"){
        mem[--reg[6]]=(reg[7]>>8)&0xff;
        mem[--reg[6]]=reg[7]&0xff;
        reg[7]=src;
      }else if(instr=="JMP"){
        reg[7]=src;
      }else if(instr=="JEQ"){
        if(psw&0x0001) reg[7]=src;
      }else if(instr=="JNE"){
        if(psw&0x0001==0) reg[7]=src;
      }else if(instr=="JGT"){
        if((psw&0x0008&&psw&0x0002)||(~psw&0x0008&&~psw&0x0002)){
          reg[7]=src;
        }
      }else if(instr=="LDR"){
        reg[dest]=src;
      }else if(instr=="STR"){
        //
        if(addrmod==1){
          reg[dest]=src;
        }else if(addrmod==2){
          mem[dest]=src&0x00ff;
          mem[dest+1]=(src>>8)&0x00ff;
        }else if(addrmod==3){
          
          mem[dest]=src&0x00ff;
          mem[dest+1]=(src>>8)&0x00ff;
        }else if(addrmod==4){
          mem[dest]=src&0x00ff;
          mem[dest+1]=(src>>8)&0x00ff;
        }
      }
    }
  }
  cout<<"Emulated processor executed halt instruction"<<endl;
  cout<<"Emulated processor state: psw=0b"<<bitset<16>(psw)<<endl;

  cout << "r0=0x" <<setw(4)<<setfill('0')<< hex<< reg[0] << endl;
  cout << "r1=0x" <<setw(4)<<setfill('0')<< hex<< reg[1] << endl;
  cout << "r2=0x" <<setw(4)<<setfill('0')<< hex<< reg[2] << endl;
  cout << "r3=0x" <<setw(4)<<setfill('0')<< hex<< reg[3] << endl;
  cout << "r4=0x" <<setw(4)<<setfill('0')<< hex<< reg[4] << endl;
  cout << "r5=0x" <<setw(4)<<setfill('0')<< hex<< reg[5] << endl;
  cout << "r6=0x" <<setw(4)<<setfill('0')<< hex<< reg[6] << endl;
  cout << "r7=0x" <<setw(4)<<setfill('0')<< hex<< reg[7] << endl;
}
string Emulator::checkInstr(unsigned char word){
  switch (word)
  {
  case 0x00:
    return "HALT";
    break;
  case 0x10:
    return "INT";
    break;
  case 0x20:
    return "IRET";
    break;
  case 0x30:
    return "CALL";
    break;
  case 0x40:
    return "RET";
    break;
  case 0x50:
    return "JMP";
    break;
  case 0x51:
    return "JEQ";
    break;
  case 0x52:
    return "JNE";
    break;
  case 0x53:
    return "JGT";
    break;
  case 0x60:
    return "XCHG";
    break;
  case 0x70:
    return "ADD";
    break;
  case 0x71:
    return "SUB";
    break;
  case 0x72:
    return "MUL";
    break;
  case 0x73:
    return "DIV";
    break;
  case 0x74:
    return "CMP";
    break;
  case 0x80:
    return "NOT";
    break;
  case 0x81:
    return "AND";
    break;
  case 0x82:
    return "OR";
    break;
  case 0x83:
    return "XOR";
    break;
  case 0x84:
    return "TEST";
    break;
  case 0x90:
    return "SHL";
    break;
  case 0x91:
    return "SHR";
    break;
  case 0xA0:
    return "LDR";
    break;
  case 0XB0:
    return "STR";
    break;
  default:
    throw exception();
    break;
  }
}
int Emulator::checkLen(string instr){
  if(instr=="HALT"||instr=="IRET"||instr=="RET") return 1;
  else if(instr=="INT"||instr=="XCHG"||instr=="ADD"||instr=="SUB"||instr=="MUL"||instr=="DIV"||instr=="CMP"||
  instr=="NOT"||instr=="AND"||instr=="OR"||instr=="XOR"||instr=="TEST"||instr=="SHL"||instr=="SHR") return 2;
  else return 3;
}