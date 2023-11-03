#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class Emulator{
private:
  unsigned char mem[INT16_MAX];
  unsigned short reg[8];
  unsigned int psw;
  unsigned int ivtp=0x0000;
  string inputFile;
  ifstream inf;
  int adc;
  ofstream off;
  string izlaz;
  vector<unsigned char> instr;
  bool end;
public:
void readFile(string input);
Emulator();
void emulate(string input);
int checkLen(string instr);
string checkInstr(unsigned char word);
};