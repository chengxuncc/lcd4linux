/* $Id: layout.c,v 1.7 2004/01/30 20:57:56 reinelt Exp $
 *
 * new layouter framework
 *
 * Copyright 1999-2003 Michael Reinelt <reinelt@eunet.at>
 * Copyright 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * $Log: layout.c,v $
 * Revision 1.7  2004/01/30 20:57:56  reinelt
 * HD44780 patch from Martin Hejl
 * dmalloc integrated
 *
 * Revision 1.6  2004/01/14 11:33:00  reinelt
 * new plugin 'uname' which does what it's called
 * text widget nearly finished
 * first results displayed on MatrixOrbital
 *
 * Revision 1.5  2004/01/13 08:18:19  reinelt
 * timer queues added
 * liblcd4linux deactivated turing transformation to new layout
 *
 * Revision 1.4  2004/01/12 03:51:01  reinelt
 * evaluating the 'Variables' section in the config file
 *
 * Revision 1.3  2004/01/11 18:26:02  reinelt
 * further widget and layout processing
 *
 * Revision 1.2  2004/01/11 09:26:15  reinelt
 * layout starts to exist...
 *
 * Revision 1.1  2004/01/10 20:22:33  reinelt
 * added new function 'cfg_list()' (not finished yet)
 * added layout.c (will replace processor.c someday)
 * added widget_text.c (will be the first and most important widget)
 * modified lcd4linux.c so that old-style configs should work, too
 *
 */

/* 
 * exported functions:
 *
 * layout_init (char *section)
 *    initializes the layouter
 *
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "cfg.h"
#include "widget.h"
#include "layout.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


int layout_addItem (char *name, int row, int col)
{
  // allocate widget
  widget_add (name, row-1, col-1);
  return 0;
}


int layout_init (char *layout)
{
  char *section;
  char *list;
  char *key;
  char *widget;
  int row, col;
  
  info ("initializing layout '%s'", layout);
  
  // prepare config section
  // strlen("Layout:")=7
  section=malloc(strlen(layout)+8);
  strcpy(section, "Layout:");
  strcat(section, layout);

  list=cfg_list(section);
  key=strtok(list, "|");
  while (key!=NULL) {
    int i, n;
    char *k;
    // map to lower char for scanf()
    for (k=key; *k!='\0'; k++) *k=tolower(*k);
    i=sscanf (key, "row%d.col%d%n", &row, &col, &n);
    if (i==2 && key[n]=='\0') {
      widget=cfg_get(section, key, NULL);
      if (widget!=NULL && *widget!='\0') {
	layout_addItem (widget, row, col);
      }
    }
    key=strtok(NULL, "|");
  }
  free (list);
  
  return 0;
}

