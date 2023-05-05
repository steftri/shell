# Shell

This is a library for supplying a shell command interface. 
It supports the backslash and quotes, command interface match the main(int argc, char *argv[]) mechanism.

## General

The library does not have any dependencies to other libraries.
It does not use dynamic memory allocation.

## Usage

```// for configuration
```void setPromptCallback(const TTPromptCallback *p_PromptCallback);
```void setCommandNotFoundCallback(const TDefaultCmdCallback *p_DefaultCmdCallback);
```int16_t addCommandCallback(const char *pc_Cmd, const TCmdCallback *p_CmdCallback);

```// for operation
```void begin(void);
```void putChar(const char c_Char);
```void reset(const bool b_DisplayPrompt = false);


## Limitations
                                               
* The number of commands is limited to 32 
* The length of a command (including arguments) is limited to 80
* The number of arguments per command is limited to 8


## Known Anomalies

* none


## Files & Configuration

* `shell.h`:  the include file; within this file the limitations (see above) can be changed.



## License


## Website

Further information can be found on [GitHub](https://github.com/steftri/shell).

