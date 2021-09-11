/*
Copyright (c) 2000, 2002 Stephen Montgomery-Smith
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of Stephen Montgomery-Smith nor the names of his 
   contributors may be used to endorse or promote products derived from 
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE STEPHEN MONTGOMERY-SMITH AND CONTRIBUTORS 
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STEPHEN MONTGOMERY-SMITH OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/

#include "xkbset.h"

Bool get_number(char *arg, int low, int high, int *val) {
  int i;

  for (i=0;i<(int)strlen(arg);i++)
    if ((arg[i]<'0' || arg[i]>'9') && (i!=0 || arg[0]!='-'))
      return 0;
  *val = atoi(arg);
  if (*val<low || *val>high)
    return 0;
  return 1;
}

#define digit(c) ( ((c)>='0'&&(c)<='9')?(c)-'0': \
                   ((c)>='a'&&(c)<='f')?(c)-'a'+10: \
                   ((c)>='A'&&(c)<='F')?(c)-'A'+10: \
                   (success = 0) )
Bool get_64_hex(char *arg, char array[32]) {
  int i, success = 1;

  for(i=0;i<64&&success;i++)
    if (i%2==0)
      array[i/2] = digit(arg[i])*16;
    else
      array[i/2] += digit(arg[i]);
  return success;
}
