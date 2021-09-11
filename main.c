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


int main(int argc, char *argv[]) {
  Display *display;
  int event_rtrn, error_rtrn;
  int major_in_out = XkbMajorVersion;
  int minor_in_out = XkbMinorVersion;
  int reason_rtrn;
/*
  XkbControlsRec junk;
*/
  XkbDescPtr xkb;
  unsigned int mask = 0;
  Bool query, write_line, expire, query_expire, write_line_expire;
  Status status;

  if (argc == 1) {
    print_usage();
    exit(0);
  }
  query = argc==2 && strcmp(argv[1],"q")==0;
  write_line = argc==2 && strcmp(argv[1],"w")==0;
  query_expire = argc==3 && strcmp(argv[1],"q")==0 && strcmp(argv[2],"exp")==0;
  write_line_expire = argc==3 && strcmp(argv[1],"w")==0 && strcmp(argv[2],"exp")==0;;
  expire = argc>=2 && strcmp(argv[1],"exp")==0;

/*
  if (query || write_line)
    mask = XkbAllControlsMask;
   The first time we call get_arguments is just to get mask 
   No we don't - XkbGetControls seems buggy with repect to getting
   ax_options if the mask is selective 
  else if (query_expire || write_line_expire)
    mask = XkbAccessXTimeoutMask;
  else if (expire) {
    if (!get_expire_arguments(argc,argv, &junk, &mask)) {
      print_usage();
      exit(1);
    }
  }
  else if (!get_arguments(argc,argv, &junk, &mask)) {
    print_usage();
    exit(1);
  }
*/

  display = XkbOpenDisplay(NULL, 
                &event_rtrn, &error_rtrn, &major_in_out, 
                &minor_in_out, &reason_rtrn);
  if (reason_rtrn != XkbOD_Success) {
    switch(reason_rtrn) {
      case XkbOD_BadLibraryVersion:
      case XkbOD_BadServerVersion:
      case XkbOD_NonXkbServer:
        fprintf(stderr,"Non-existent or incompatible XKB library version\n");
        break;
      case XkbOD_ConnectionRefused:
        fprintf(stderr, "Unable to open display\n");
        break;
      default:
        fprintf(stderr, "Unknown error in XkbOpenDisplay\n");
        break;
    }
    exit(1);
  }

  xkb = XkbGetKeyboard(display,XkbControlsMask,XkbUseCoreKbd);
  if (xkb == 0) {
    fprintf(stderr, "XKB not supported for display %s\n", getenv("DISPLAY"));
    exit(1);
  }

  status = XkbGetControls(display, XkbAccessXTimeoutMask|XkbAllControlsMask, xkb);
  switch(status) {
    case Success:
      break;
    case BadAlloc:
      fprintf(stderr,"Allocation error in XkbGetControls\n");
      exit(1);
    case BadMatch:
      fprintf(stderr,"Bad match in  XkbGetControls\n");
      exit(1);
    case BadImplementation:
      fprintf(stderr,"Bad implementation in  XkbGetControls\n");
      exit(1);
    default:
      fprintf(stderr,"Unknown error in  XkbGetControls\n");
      exit(1);
  }

  if (query) {
    print_controls(xkb->ctrls);
  }
  else if (write_line) {
    print_controls_in_line(xkb->ctrls);
  }
  else if (query_expire) {
    print_expire_controls(xkb->ctrls);
  }
  else if (write_line_expire) {
    print_expire_controls_in_line(xkb->ctrls);
  }
  else if (expire) {
    if (!get_expire_arguments(argc,argv, xkb->ctrls, &mask)) {
      print_usage();
      exit(1);
    }
    XkbSetControls(display, mask, xkb);
  }
  else {
    if (!get_arguments(argc,argv, xkb->ctrls, &mask)) {
      print_usage();
      exit(1);
    }
    XkbSetControls(display, mask, xkb);
  }

  XCloseDisplay(display);
  exit(0);
}
