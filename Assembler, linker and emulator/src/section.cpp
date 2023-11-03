#include "../inc/section.h"

Section::Section(string name,int offset,int size, string file){
  this->name=name;
  this->offset=offset;
  this->size=size;
  this->file=file;
}/*
Section::~Section(){
  delete this->symTab;
  delete this->relaTab;
  delete this->data;
}
*/
void Section::addByte(unsigned char byte){
  this->values.push_back(byte);
  this->lc++;
}
void Section::addEmptyWord(){
  this->values.push_back(0x00);
  this->lc++;
  this->values.push_back(0x00);
  this->lc++;
}
void Section::addWord(int value){
  this->values.push_back(value&0x00ff);
  this->lc++;
  this->values.push_back(value>>8);
  this->lc++;
}

void Section::addWord(int start,int value){

  this->values.at(start++)=value&0x00ff;
  value=value>>8;
  this->values.at(start)=value&0x00ff;
}

void Section::addInstrData(int value){
  this->values.push_back(value>>8);
  this->lc++;
  this->values.push_back(value&0x00ff);
  this->lc++;
}

void Section::addInstrData(int start,int value){
  this->values.at(start++)=(value>>8)&0x00ff;
  this->values.at(start)=value&0x00ff;
}

void Section::addPcrelWord(int value,int offset){
  this->values[offset++]=value>>8;
  this->values[offset]=value;
}