#include "../inc/formatedLine.h"

unsigned char FormatedLine::getRegNum(string reg){
  if(reg=="r0"){
    return 0;
  }else if(reg=="r1"){
    return 1;
  }else if(reg=="r2"){
    return 2;
  }else if(reg=="r3"){
    return 3;
  }else if(reg=="r4"){
    return 4;
  }else if(reg=="r5"){
    return 5;
  }else if(reg=="r6"){
    return 6;
  }else if(reg=="r7"){
    return 7;
  }
  return 0;
}