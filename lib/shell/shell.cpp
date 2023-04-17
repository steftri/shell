#include <ctype.h>
#include <string.h>

#include "shell.h"



Shell::Shell(void)
{
  mp_PromptCallback = nullptr;
  mp_DefaultCmdCallback = nullptr;
  mu16_NumberOfCommands = 0;
  mu16_BufferPos = 0;
}



void Shell::setPromptCallback(const TTPromptCallback *p_PromptCallback)
{
  mp_PromptCallback = p_PromptCallback;
}



void Shell::setCommandNotFoundCallback(const TDefaultCmdCallback *p_DefaultCmdCallback)
{
  mp_DefaultCmdCallback = p_DefaultCmdCallback;
}



int16_t Shell::addCommandCallback(const char *pc_Cmd, const TCmdCallback *p_CmdCallback)
{
  if(mu16_NumberOfCommands>=SHELL_MAX_COMMANDS)
    return RcError;

  ma_Commands[mu16_NumberOfCommands].pc_Cmd = pc_Cmd;
  ma_Commands[mu16_NumberOfCommands].p_Callback = p_CmdCallback;
  mu16_NumberOfCommands++;

  return RcOK;
}


void Shell::begin(void)
{
  reset(true);
}



void Shell::reset(const bool b_DisplayPrompt)
{
  mu16_BufferPos = 0;
  if(b_DisplayPrompt && mp_PromptCallback)
    mp_PromptCallback();
}



void Shell::putChar(const char c_Char)
{
  if(c_Char==SHELL_EOL_CHARACTER)
  {
    _execCmd();
    mu16_BufferPos = 0;
  }
  else if(mu16_BufferPos<SHELL_MAX_BUFFER_LENGTH)
  {
    mac_Buffer[mu16_BufferPos++] = c_Char;
  }
}



void Shell::_execCmd(void)
{
  char ac_Buffer[SHELL_MAX_BUFFER_LENGTH+1];   // +1 because of trailing \0
  uint16_t u16_BufferPos = 0;
  bool b_ArgFound = false;
  char *argv[SHELL_MAX_ARGS];
  uint16_t u16_Args = 0;
  bool b_Escaped = false;
  bool b_Quoted = false;
  
  // first, parse the string - split it into words, but allow escape character (\) 
  // and quotes ("")
  for(auto i=0U; i<mu16_BufferPos; i++)
  {
    if(mac_Buffer[i]==SHELL_ESCAPE_CHARACTER && !b_Escaped)
    {
      b_Escaped=true;
    }
    else if(mac_Buffer[i]==SHELL_QUOTE_CHARACTER && !b_Escaped)
    {
      b_Quoted = !b_Quoted;
    }
    else
    { 
      if(isspace(mac_Buffer[i]) && !(b_Escaped||b_Quoted))
      {
        if(b_ArgFound) 
        {
          ac_Buffer[u16_BufferPos++] = 0;
          b_ArgFound = false;
        }
      }
      else
      {
        b_Escaped = false;
        if(!b_ArgFound && u16_Args<SHELL_MAX_ARGS)
        {
          argv[u16_Args++]=&ac_Buffer[u16_BufferPos];
          b_ArgFound = true;
        }
        ac_Buffer[u16_BufferPos++] = mac_Buffer[i];
      }
    }
  }
  ac_Buffer[u16_BufferPos++]=0;

  // if no command found, quit
  if(u16_Args == 0)
  {
    if(mp_PromptCallback)
      mp_PromptCallback();
    return;
  }

  // second, match the command with available ones
  for(auto i=0U; i<mu16_NumberOfCommands; i++)
  {
    if(!strcmp(ma_Commands[i].pc_Cmd, argv[0]))
    {
      if(ma_Commands[i].p_Callback)
        ma_Commands[i].p_Callback(u16_Args, argv);
      if(mp_PromptCallback)
        mp_PromptCallback();
      return;
    }
  }

  // if no matching command found, call default command callback
  if(mp_DefaultCmdCallback) 
    mp_DefaultCmdCallback(argv[0]);
  if(mp_PromptCallback)
    mp_PromptCallback();

  return;
}

