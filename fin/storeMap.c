#include "simpletools.h"
#define size 999

struct stack{
  float x[size];
  float y[size];
  int top;
}position;

void stack_init(){
  position.top = -1;
}

int stack_full(){
  if(position.top >= size - 1){
    return 1;
  }
  else{
    return 0;
  }

}


void push(float xPos,float yPos){
  position.top++;
  position.x[position.top] = xPos;
  position.y[position.top] = yPos;
}

int stack_empty(){
  if (position.top == -1){
    return 1;
  }
  else{
    return 0;
  }
}

float popX(){
  float xPos;
  xPos = position.x[position.top];
  return xPos;
}


float popY(){
  float yPos;
  yPos = position.y[position.top];
  return yPos;
}

void decrement(){
  position.top--;
}
  





 
