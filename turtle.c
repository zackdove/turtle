//Zack Dove 2017 zd17646@bristol.ac.uk
//A program that moves a "turtle" round the screen based on user Input
//Uses the display.h file from week 9

#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

//Stores the value of pi as a global variable, intialised later
double pi;

//Structure for the current state
struct state{
  int prevx;
  int prevy;
  int x;
  int y;
  double angle;
  bool pendown;
  display *d;
};
typedef struct state state;

//Initialises the state, display and pi
state *initialiseState(){
  pi = 4.0 * atan(1.0);
  state *s = malloc(sizeof(state));
  s->prevx = 250;
  s->prevy = 400;
  s->x = 250;
  s->y = 400;
  s->angle = 0;
  s->pendown = true;
  s->d = newDisplay("turtle", 500, 500);
  return s;
}

//Converts an integer degree to radians
double degToRad(int degrees){
  double result =  (degrees * pi) / 180;
  return result;
}

//Rotates the turtle by an integer angle
void turn(state *s, int a){
  s->angle += degToRad(a);
}

//Rotates the turtle clockwise by an integer angle
void right(state *s, int a){
  turn(s, a);
}

//Rorates the turtle anticlockwise by an integer angle
void left(state *s, int a){
  turn(s, -a);
}

//Moves the turtle forward in the direction that it faces
void forward(state *s, int d){
  s->x = s->prevx + (d * sin(s->angle));
  s->y = s->prevy - (d * cos(s->angle));
  if (s->pendown) line(s->d, s->prevx, s->prevy, s->x, s->y);
  s->prevx = s->x;
  s->prevy = s->y;
}

//Toggles the pen up or down
void pen(state *s){
  s->pendown = !s->pendown;
}

//Changes the colour
void changeColour(state *s, int c){
  colour(s->d, c);
}

//Checks if a string is a command
bool checkOperation(char *cmnd){
  if (strcmp(cmnd, "FD")==0 || strcmp(cmnd, "CL")==0 || strcmp(cmnd, "RT")==0 || strcmp(cmnd, "LT")==0 || strcmp(cmnd, "COL")==0 || strcmp(cmnd, "HM")== 0 || strcmp(cmnd, "PEN")==0 || strcmp(cmnd, "END")==0){
    return true;
  } else return false;
}

//Checks is a string is one of the singular commands
bool checkSingular(char *cmnd){
  if (strcmp(cmnd, "PEN")==0 || strcmp(cmnd, "END")==0 || strcmp(cmnd, "CL")==0 || strcmp(cmnd, "HM")==0) return true;
  else return false;
}

//Performs an operation
void doOperation(char *cmnd, int operand, state *s){
  pause(s->d, 150);
  if (strcmp(cmnd, "FD")==0){
    forward(s, operand);
  }
  if (strcmp(cmnd, "RT")==0){
    right(s, operand);
  }
  if (strcmp(cmnd, "LT")==0){
    left(s, operand);
  }
  if (strcmp(cmnd, "PEN")==0){
    pen(s);
  }
  if (strcmp(cmnd, "END")==0){
    end(s->d);
    exit(0);
  }
  if (strcmp(cmnd, "COL") == 0){
    changeColour(s, operand);
  }
  if (strcmp(cmnd, "CL") == 0){
    clear(s->d);
    changeColour(s, 255);
  }
  if (strcmp(cmnd, "HM") == 0){
    line(s->d, s->x, s->y, 250, 400);
    s->prevx = 250;
    s->prevy = 400;
    s->x = 250;
    s->y = 400;
  }
}

//Checks if rthe operand is an integer
bool checkOperand(char operand[]){
  for (int i = 0; i<strlen(operand); i++){
    if (operand[i] < 48 || operand[i] > 57){
      return false;
    }
  }
  return true;
}

//Gets an integer operand from a string
int getOperand(char operand[]){
  return atoi(operand);
}

//Takes input from stdin, seperates into tokens and performs them
void input(state *s){
  char string[300];
  char *ptr;
  char operator[10];
  int operand;
  printf("Please enter a command or list of commands seperate by commas:\n");
  fgets(string, sizeof(string), stdin);
  ptr = strtok(string, " ,\n");
  while (ptr != NULL){
    if (checkOperation(ptr)){
      if (checkSingular(ptr)) doOperation(ptr, 0, s);
      strcpy(operator, ptr);
    } else if (checkOperand(ptr)){
      operand = getOperand(ptr);
      doOperation(operator, operand, s);
    } else {
      printf("Error: '%s' not allowed. Refer to the readme for more.\n",ptr);
      input(s);
    }
    ptr = strtok(NULL, " ,\n");
  }
  input(s);
}


//--------------Testing-------------------------------------------------

void testInitialiseState(){
  state *s = initialiseState();
  assert(floor(pi*1000000) == 3141592);
  assert(s->prevx == 250);
  assert(s->prevy == 400);
  assert(s->x == 250);
  assert(s->y == 400);
  assert(s->angle == 0);
  assert(s->pendown == true);
}

void testDegToRad(){
  assert(degToRad(0) == 0);
  assert(floor(degToRad(14)*1000000) == 244346);
  assert(degToRad(90) == pi/2);
  assert(floor(degToRad(155)*1000000) == 2705260);
  assert(degToRad(180) == pi);
  assert(floor(degToRad(200)*1000000) == 3490658);
  assert(degToRad(270) == 3*pi/2);
  assert(degToRad(360) == 2*pi);
  assert(floor(degToRad(444)*1000000) == 7749261);
}

void testTurn(){
  state *s = initialiseState();
  s->angle = 0;
  turn(s, 0);
  assert(s->angle == 0);
  turn (s, 45);
  assert(s->angle == degToRad(45));
  turn (s, -90);
  assert(s->angle == degToRad(-45));
  turn (s, 999);
  assert(floor(s->angle*1000000) == floor(degToRad(954)*1000000));
}

void testForward(){
  state *s = initialiseState();
  forward(s, 100);
  assert(s->x == 250);
  assert(s->y == 300);
  turn(s, 90);
  forward(s, 100);
  assert(s->x == 350);
  assert(s->y == 300);
  turn(s, 45);
  forward(s, 100);
  assert(s->x == 420);
  assert(s->y == 370);
}

void testPen(){
  state *s = initialiseState();
  s->pendown = true;
  pen(s);
  assert(s->pendown == false);
}

void test(){
  testInitialiseState();
  testDegToRad();
  testTurn();
  testForward();
  testPen();
  printf("All tests pass\n");
}


int main(int argc, char *argv[]){
  if (argc == 2 && strcmp(argv[1], "t")==0) test();
  else if (argc == 1){
    state *s = initialiseState();
    input(s);
    end(s->d);
  } else {
    printf("Please use ./turtle for normal running, or ./turtle t for testing\n");
  }

}
