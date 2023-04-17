/*
 * main.cpp
 *
 * Created on: 2023-04-13
 *     Author: Stefan Trippler TRST
 */

#include <Arduino.h>

#include "shell.h"



#define PROJECT_NAME "ERNI Shell"


Shell myShell;



void cmd_prompt_callback(void)
{
  Serial.print("> ");
  return;
}

void cmd_not_found(char *pc_Cmd)
{
  Serial.print(pc_Cmd); 
  Serial.println(": command not found");
  return;
}



int cmd_help(int argc, char *argv[])
{
  Serial.println("Commands: ");
  Serial.println("help");
  Serial.println("version");  
  Serial.println("example [args]"); 
  return 0;
}



int cmd_version(int argc, char *argv[])
{
  Serial.println(PROJECT_NAME);
  Serial.println(__DATE__ ", " __TIME__); 
  return 0;
}



int cmd_example(int argc, char *argv[])
{
  Serial.print("Command name: ");
  Serial.println(argv[0]);
  Serial.print("Arguments (including command name): ");
  Serial.println(argc);
  for(int i=1; i<argc; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(argv[i]);
  }
  return 0;
}



void setup()
{
  Serial.begin(115200);
  Serial.println(PROJECT_NAME);
  Serial.println(__DATE__ ", " __TIME__);

  myShell.setPromptCallback(&cmd_prompt_callback);
  myShell.setCommandNotFoundCallback(&cmd_not_found);
  myShell.addCommandCallback("help", &cmd_help);
  myShell.addCommandCallback("version", &cmd_version);
  myShell.addCommandCallback("example", &cmd_example);  

  // clear incoming data from serial port
  while(Serial.available())
    Serial.read();

  myShell.begin();
}



void loop()
{
  char c_Char;

  while(Serial.available())
  {
    c_Char = Serial.read();
    if(c_Char=='\r')
      Serial.println();
    else
      Serial.write(c_Char);
    
    myShell.putChar(c_Char);
  }
}
