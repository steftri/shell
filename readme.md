# Shell

Library for supplying a shell command interface 


## General

The Shell command parser supports the backslash and quotes, the command interface match the main(int argc, char *argv[]) mechanism.

The library does not have any dependencies to other libraries.
It does not use dynamic memory allocation.

## Features

* Command parsing as generally known from Linux shells 
* Quote support for suppressing the treatment of blanks as separators 
* Support of the backslash as escape character to suppress the recognition of control characters
* Implemented as C++ class with common Arduino style guide
* Low memory consumption overhead

## Integration

* Include the library

  ```C++
  #include <shell.h>
  ```
  
* Create an instance of the class
  
  ```C++
  Shell myShell;
  ```
 
* Create general callbacks

  ```C++
  // This callback is needed to output the command 
  // prompt ("> " in this case) to the serial device. 
  void shell_display_prompt(void)
  {
    Serial.print("> ");
    return;
  }
  
  // This callback is called when an unknown command is received. 
  void shell_cmd_not_found(char *pc_Cmd)
  {
    Serial.print("Command '");
    Serial.print(pc_Cmd);
    Serial.println("' not found");
    return;
  }
  ```
  
* Create command callbacks

  Each command should be implemented as a separate callback. The parameter "argc" holds the number of arguments, 
  the parameter argv hold an array of arguments (including the command itself as the first parameter). 
 
  As an example, the following is the implementation of the "echo" command. 

  ```C++
  // This is the callback for the command "echo". 
  int shell_cmd_echo(int argc, char *argv[])
  {
    if(argc<2) {
      Serial.println("usage: echo <text>");
      return 0;
    }

    for(int i=1; i<argc; i++) {
      Serial.print(argv[i]);
      Serial.print(" ");
    }
    Serial.println();

    return 0;
  }  
  ```  
  
* Setup

  Register the callbacks to the Shell instance.
  
  ```C++
  myShell.setCommandNotFoundCallback(&shell_cmd_not_found);
  myShell.setPromptCallback(&shell_display_prompt);
  myShell.addCommandCallback("echo", &shell_cmd_echo);
  ```
  
## Usage

* Begin

  Trigger the output of the initial command prompt:

  ```C++
  myShell.begin();
  ```

* Provide user input

  Call the `putChar()` method to handover the received character to the 
  shell; the shell parses the input and calls the command callback as soon 
  a command terminated by a carriage return character (`\r`) is detected.

  ```C++
  myShell.putChar(c);
  ```


## Limitations
                                               
* The number of commands is limited to 32 
* The length of a command (including arguments) is limited to 80
* The number of arguments per command is limited to 8

## Diagrams

### Class Diagram

This diagram illustrates the shell class. There are no other classes or important structures within this library.

![Class Diagram](doc/class_diagram.png)

### Sequence Diagram

This sequence diagram illustrates the initialization and the usage of the shell class. 

![Sequence Diagram](doc/sequence_diagram.png)


## Known Anomalies

* none


## Files & Configuration

* `shell.h`:  the include file; within this file the limitations (see above) can be changed.


## Changelog

### 1.0.1

* Documentation enhanced

### 1.0.0

* Initial release

## Website

Further information can be found on [GitHub](https://github.com/steftri/shell).

