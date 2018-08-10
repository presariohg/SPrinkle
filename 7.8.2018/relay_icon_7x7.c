/****************************************************************************
Font created by the LCD Vision V1.05 font & image editor/converter
(C) Copyright 2011-2013 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Font name: relay_status
Fixed font width: 7 pixels
Font height: 7 pixels
First character: 0x30
Last character: 0x31

Exported font data size:
18 bytes for displays organized as horizontal rows of bytes
18 bytes for displays organized as rows of vertical bytes.
****************************************************************************/

flash unsigned char relay_icon_7x7[]=
{
0x07, /* Fixed font width */
0x07, /* Font height */
0x30, /* First character */
0x02, /* Number of characters in font */

#ifndef _GLCD_DATA_BYTEY_
/* Font data for displays organized as
   horizontal rows of bytes */

/* Code: 0x30, ASCII Character: '0' */
0x1C, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C, 

/* Code: 0x31, ASCII Character: '1' */
0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C, 

#else
/* Font data for displays organized as
   rows of vertical bytes */

/* Code: 0x30, ASCII Character: '0' */
0x1C, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C, 

/* Code: 0x31, ASCII Character: '1' */
0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C, 

#endif
};

