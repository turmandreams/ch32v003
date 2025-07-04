// ===================================================================================
// SSD1306 128x64 Pixels OLED Terminal Functions                              * v1.5 *
// ===================================================================================
//
// Collection of the most necessary functions for controlling an SSD1306 128x64 pixels
// I2C OLED for the display of text in the context of emulating a terminal output.
//
// References:
// -----------
// - Neven Boyanov: https://github.com/tinusaur/ssd1306xled
//
// 2024 by Enol Matilla: https://github.com/rotura

#include "oled_segment.h"

#define BLACK false
#define WHITE true


// Screenbuffer
uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

// OLED initialisation sequence
const uint8_t OLED_INIT_CMD[] = {
  OLED_MULTIPLEX,   0x3F,                 // set multiplex ratio  
  OLED_CHARGEPUMP,  0x14,                 // set DC-DC enable  
  OLED_MEMORYMODE,  0x02,                 // set page addressing mode
  OLED_COMPINS,     0x12,                 // set com pins
  OLED_XFLIP, OLED_YFLIP,                 // flip screen
  OLED_DISPLAY_ON                         // display on
};

const uint8_t ssd1306_init_sequence [] = {	// Initialization Sequence
	0xAE,			// Set Display ON/OFF - AE=OFF, AF=ON
	0xD5, 0xF0,		// Set display clock divide ratio/oscillator frequency, set divide ratio
	0xA8, 0x3F,		// Set multiplex ratio (1 to 64) ... (height - 1)
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0x40 | 0x00,	// Set start line address, at 0.
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, 10=Page, 11=Invalid
	0xA0 | 0x01,	// Set Segment Re-map
	0xC8,			// Set COM Output Scan Direction
	0xDA, 0x12,		// Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
	0x81, 0x3F,		// Set contrast control register - 0x01 to 0xFF - Default: 0x3F
	0xD9, 0x22,		// Set pre-charge period (0x22 or 0xF1)
	0xDB, 0x20,		// Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
	0xA4,			// Entire Display ON (resume) - output RAM to display
	0xA6,			// Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
	0x2E,			// Deactivate Scroll command
	0xAF,			// Set Display ON/OFF - AE=OFF, AF=ON
	0x22, 0x00, 0x3f,	// Set Page Address (start,end) 0 - 63
	0x21, 0x00,	0x7f,	// Set Column Address (start,end) 0 - 127
};

// OLED global variables
uint8_t line, column, scroll;
uint8_t cursorX, cursorY;

// OLED set cursor to line start
void OLED_setline(uint8_t line) {
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_CMD_MODE);               // set command mode
  I2C_write(OLED_PAGE + line);            // set line
  I2C_write(0x00); I2C_write(0x10);       // set column to "0"
  I2C_stop();                             // stop transmission
}

// OLED clear line
void OLED_clearline(uint8_t line) {
  uint8_t i;
  OLED_setline(line);                     // set cursor to line start
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_DAT_MODE);               // set data mode
  for(i=128; i; i--) {                    // clear the line
    I2C_write(0x00);
    SSD1306_Buffer[i + (line / 8) * SSD1306_WIDTH] = 0x00;
  }     
  I2C_stop();                             // stop transmission
}

// OLED clear screen and buffer
void OLED_clear(void) {
  uint16_t i;
		OLED_setpos(0, 0);                            // set cursor to first digit
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  for(i=128*8; i; i--) {                          // clear screen and buffer
    I2C_write(0x00);
    SSD1306_Buffer[i-1] = 0x00;
    }           
  I2C_stop();                                     // stop transmission
}

// OLED clear the top line, then scroll the display up by one line
void OLED_scrollDisplay(void) {
  OLED_clearline(scroll);                 // clear line
  scroll = (scroll + 1) & 0x07;           // set next line
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_CMD_MODE);               // set command mode
  I2C_write(OLED_OFFSET);                 // set display offset:
  I2C_write(scroll << 3);                 // scroll up
  I2C_stop();                             // stop transmission
}

// OLED init function
void OLED_init(void) {
  uint8_t i;
  I2C_init();                             // initialize I2C first
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_CMD_MODE);               // set command mode
  for(i = 0; i < sizeof(ssd1306_init_sequence); i++)
    I2C_write(ssd1306_init_sequence[i]);          // send the command bytes
  I2C_stop();                             // stop transmission
  scroll = 0;                             // start with zero scroll
  OLED_clear();                           // clear screen
  OLED_setpos(0,0);
}

// OLED plot a single character
void OLED_plotChar(char c, bool inverted) {
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_DAT_MODE);               // set data mode
  short pos;
  for(short i=0 ; i!=5; i++){
    I2C_write(inverted? ~font5x8[c-32][i] : font5x8[c-32][i]);
    pos = cursorX + i + (cursorY / 8) * SSD1306_WIDTH;
    SSD1306_Buffer[pos] = inverted? ~font5x8[c-32][i] : font5x8[c-32][i];
  }
  SSD1306_Buffer[++pos] = 0;
  I2C_write(0x00);                        // write space between characters
  I2C_stop();                             // stop transmission
}

// OLED write a character or handle control characters
void OLED_write(char c, bool inverted) {
  c = c & 0x7F;                           // ignore top bit
  // normal character
  if(c >= 32) {
    OLED_plotChar(c, inverted);
    if(++column > 20) {
      column = 0;
      if(line == 7) OLED_scrollDisplay();
      else line++;
      OLED_setline((line + scroll) & 0x07);
    }
    cursorX += 6;
    if(cursorX > 128-6){
        cursorX = 0;
        cursorY += 8;
    }
  }
  // new line
  else if(c == '\n') {
    cursorX = 0;
    cursorY += 8;
    column = 0;
    if(line == 7) OLED_scrollDisplay();
    else line++;
    OLED_setline((line + scroll) & 0x07);
  }
  // carriage return
  else if(c == '\r') {
    column = 0;
    OLED_setline((line + scroll) & 0x07);
  }
}

// OLED print string
void OLED_print(char* str) {
  OLED_printS(str, false);
}

// OLED print string
void OLED_printS(char* str, bool inverted) {
  while(*str) {
    OLED_write(*str++, inverted);
  }
}

// OLED print string with newline
void OLED_println(char* str, bool inverted) {
  OLED_printS(str, inverted);
  OLED_write('\n', inverted);
}

// For BCD conversion
const uint32_t DIVIDER[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

// Print decimal value (BCD conversion by substraction method)
void OLED_printD(uint32_t value, bool inverted) {
  uint8_t digits   = 10;                          // print 10 digits
  uint8_t leadflag = 0;                           // flag for leading spaces
  while(digits--) {                               // for all digits
    uint8_t digitval = 0;                         // start with digit value 0
    uint32_t divider = DIVIDER[digits];           // read current divider
    while(value >= divider) {                     // if current divider fits into the value
      leadflag = 1;                               // end of leading spaces
      digitval++;                                 // increase digit value
      value -= divider;                           // decrease value by divider
    }
    if(!digits)  leadflag++;                      // least digit has to be printed
    if(leadflag) OLED_write(digitval + '0', inverted);      // print the digit
  }
}

// Convert byte nibble into hex character and print it
void OLED_printN(uint8_t nibble, bool inverted) {
  OLED_write((nibble <= 9) ? ('0' + nibble) : ('A' - 10 + nibble), inverted);
}

// Convert byte into hex characters and print it
void OLED_printB(uint8_t value, bool inverted) {
  OLED_printN(value >> 4, inverted);
  OLED_printN(value & 0x0f, inverted);
}

// Convert word into hex characters and print it
void OLED_printW(uint16_t value, bool inverted) {
  OLED_printB(value >> 8, inverted);
  OLED_printB(value, inverted);
}

// Convert long into hex characters and print it
void OLED_printL(uint32_t value, bool inverted) {
  OLED_printW(value >> 16, inverted);
  OLED_printW(value, inverted);
}

// OLED set cursor position
void OLED_setpos(uint8_t x, uint8_t y) {
  cursorY = y;
  cursorX = x;
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_CMD_MODE);               // set command mode
  I2C_write(OLED_PAGE | y);	              // set page start address
  I2C_write(x & 0x0F);			              // set lower nibble of start column
  I2C_write(OLED_COLUMN_HIGH | (x >> 4)); // set higher nibble of start column
  I2C_stop();                             // stop transmission
}

void ssd1306_start_data(void) {
  I2C_start(OLED_ADDR);   
	I2C_write(0x40);			// Control byte: D/C=1 - write data
}

// OLED fill screen
void OLED_fill(uint8_t p) {
  OLED_setpos(0, 0);                      // set cursor to display start
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_DAT_MODE);               // set data mode
  for(uint16_t i=128*8; i; i--){          // send pattern
    I2C_write(p);
    SSD1306_Buffer[i-1] = p;
  } 
  I2C_stop();                             // stop transmission
}

// OLED draw bitmap
void OLED_DrawBitmap(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, const uint8_t* bmp, bool inverted) {
	int z=0;
  for(uint8_t y = y0; y < y0+(h/8); y++) {
    OLED_setpos(x0, y);
    I2C_start(OLED_ADDR);
    I2C_write(OLED_DAT_MODE);
    for(uint8_t x = x0; x < x0+w; x++)
		{
      I2C_write(inverted? ~bmp[z] : bmp[z]);
      SSD1306_Buffer[cursorX + ((cursorY / 8) + cursorY%8 - 1) * SSD1306_WIDTH] = inverted? ~bmp[z] : bmp[z];
      cursorX++;
			z++;
		}
    I2C_stop();
  }
}


/*
 * Draw one pixel in the screenbuffer
 * X => X Coordinate
 * Y => Y Coordinate
 * white => On or Off pixel
 */
void OLED_DrawPixel(uint8_t x, uint8_t y, bool white) {
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        // Don't write outside the buffer
        return;
    }
    // Draw in the right color
    if(white) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

/*
 * Draw one line or column of pixels in the screenbuffer
 * vertical => True for columns, false for rows
 * X => Selected row or column
 * white => On or Off pixel
 */
void OLED_DrawLine(bool vertical, short x, bool white){
    if(vertical){
      for(short y = 0; y < SSD1306_HEIGHT; y++){
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] = white? 255 : 0;
      }
    } else {
      for(short y = 0; y < SSD1306_WIDTH; y++){
        //SSD1306_Buffer[y + (x / 8) * SSD1306_WIDTH] = white? 1 << (x % 8) : 0;
        if(white) {
            SSD1306_Buffer[y + (x / 8) * SSD1306_WIDTH] |= 1 << (x % 8);
        } else {
            SSD1306_Buffer[y + (x / 8) * SSD1306_WIDTH] &= ~(1 << (x % 8));
        }
      }
    }
}

/*
 * Draw multiples lines or columns of pixels in the screenbuffer
 * vertical => True for columns, false for rows
 * X => Selected rows or columns
 * size => Size of X array
 * white => On or Off pixel
 */
void OLED_DrawMultipleLines(bool vertical, short* x, short size, bool white){
    for (short i = 0; i < size; i++) {
      OLED_DrawLine(vertical, x[i], white);
    }
}

/* Write the screenbuffer to the screen */
void OLED_DisplayBuffer(void) {
		//OLED_setpos(0,0);                      // set cursor to display start
		I2C_start(OLED_ADDR);                   // start transmission to OLED
		I2C_write(OLED_DAT_MODE);               // set data mode
		for(uint16_t i=0; i<128*8; i++) {
			I2C_write(SSD1306_Buffer[i]);         // send buffer
		}
		I2C_stop();                             // stop transmission
}


//////////////////////////
/////Funciones Turman ////
/// //////////////////////



uint8_t bitRead(uint8_t byte, uint8_t bit) {
    return (byte >> bit) & 0x01;
}


void limpiabuffer(){

	for(int i=0;i<SSD1306_BUFFER_SIZE;i++){
		SSD1306_Buffer[i]=0;
	}


}

void pinta(const uint8_t *bitmap,int x,int y,int w,int h){


  int i=0;
  int contadorbits=7;

  bool sigue=true;
  int x1=x;
  int y1=y;


  while(sigue){

	  if(bitRead(bitmap[i],contadorbits)==1){
		  OLED_DrawPixel(x1,y1,true);
	  }else{
		  OLED_DrawPixel(x1,y1,false);
	  }

      contadorbits--;
      if(contadorbits<0){ contadorbits=7;i++;}

      x1++;
      if(x1>=x+w){
        if(contadorbits!=7){i++;}
        contadorbits=7;

        y1++;
        x1-=w;
        if(y1>=y+h){ sigue=false;}
      }

  }

}



void pintaescalado(const uint8_t *bitmap,int x,int y,int w,int h,int rw,int rh){

  int ii,jj;

  int i=0;
  int contadorbits=7;

  bool sigue=true;
  int x1=x;
  int y1=y;

  int rx=x*1000;
  int ry=y*1000;

  int rx2;
  int ry2;

  int aw=(rw*1000)/w;
  int ah=(rh*1000)/h;

  int aw2=rw/w;
  int ah2=rh/h;


  while(sigue){

	  rx2=rx/1000;
	  ry2=ry/1000;

	  if(bitRead(bitmap[i],contadorbits)==1){
		  for(ii=0;ii<=aw2;ii++){
			  for(jj=0;jj<=ah2;jj++){
				  OLED_DrawPixel(rx2+ii,ry2+jj,true);
			  }
		  }
		  //fillRect(rx2,ry2,aw2+1,ah2+1,true);
		  //OLED_DrawPixel(rx2,ry2,true);
	  }else{
		  /*
		  for(ii=0;ii<=aw2;ii++){
			  for(jj=0;jj<=ah2;jj++){
				  OLED_DrawPixel(rx2+ii,ry2+jj,false);
		  	  }
		  }
		  */
		  //fillRect(rx2,ry2,aw2+1,ah2+1,false);
		  //OLED_DrawPixel(rx2,ry2,false);
	  }

      contadorbits--;
      if(contadorbits<0){ contadorbits=7;i++;}

      x1++;
      rx+=aw;
      if(x1>=x+w){
        if(contadorbits!=7){i++;}
        contadorbits=7;

        y1++;
        ry+=ah;
        rx-=(rw*1000);
        x1-=w;
        if(y1>=y+h){ sigue=false;}
      }

  }

}

void pintaescaladodifuso(const uint8_t *bitmap,int x,int y,int w,int h,int rw,int rh){

  int ii,jj;

  int i=0;
  int contadorbits=7;

  bool sigue=true;
  int x1=x;
  int y1=y;

  int rx=x*1000;
  int ry=y*1000;

  int rx2;
  int ry2;

  int aw=(rw*1000)/w;
  int ah=(rh*1000)/h;

  int aw2=rw/w;
  int ah2=rh/h;


  while(sigue){

	  rx2=rx/1000;
	  ry2=ry/1000;

	  if(bitRead(bitmap[i],contadorbits)==1){
		  OLED_DrawPixel(rx2,ry2,true);
	  }else{
		  OLED_DrawPixel(rx2,ry2,false);
	  }

      contadorbits--;
      if(contadorbits<0){ contadorbits=7;i++;}

      x1++;
      rx+=aw;
      if(x1>=x+w){
        if(contadorbits!=7){i++;}
        contadorbits=7;

        y1++;
        ry+=ah;
        rx-=(rw*1000);
        x1-=w;
        if(y1>=y+h){ sigue=false;}
      }

  }

}



void pintaBitmap(const uint8_t *bitmap,const uint8_t *bitmapmask,int x,int y,int w,int h,int rw,int rh){

   int i=0;
   int contadorbits=7;

   bool sigue=true;
   int x1=x;
   int y1=y;

   int rx=x*1000;
   int ry=y*1000;

   int rx2;
   int ry2;

   int aw=(rw*1000)/w;
   int ah=(rh*1000)/h;

   while(sigue){

 	   rx2=rx/1000;
 	   ry2=ry/1000;

 	   if(bitRead(bitmap[i],contadorbits)==1){ OLED_DrawPixel(rx2,ry2,true); }
 	   else if(bitRead(bitmapmask[i],contadorbits)==1){   OLED_DrawPixel(rx2,ry2,false); }


       contadorbits--;
       if(contadorbits<0){ contadorbits=7;i++;}

       x1++;
       rx+=aw;
       if(x1>=x+w){
         if(contadorbits!=7){i++;}
         contadorbits=7;

         y1++;
         ry+=ah;
         rx-=(rw*1000);
         x1-=w;
         if(y1>=y+h){ sigue=false;}
       }

   }

}

int abs(int dato){
	if(dato<0){ return -dato; }
	return dato;
}

void drawLine(int x1,int y1,int x2,int y2,bool color){

	x1=x1*100;
	y1=y1*100;
	x2=x2*100;
	y2=y2*100;

	int x=x1;
	int y=y1;

	int difx=abs(x1-x2);
	int dify=abs(y1-y2);

	if(difx>dify){

		int ay=dify/(difx/100);
		for(int i=0;i<(difx/100);i++){
			OLED_DrawPixel(x/100,y/100,color);

			if(x1<x2) { x+=100; }  else{ x-=100; }
			if(y1<y2) { y+=ay; }else{ y-=ay; }
		}
	}else{

		int ax=difx/(dify/100);
		for(int i=0;i<(dify/100);i++){
			OLED_DrawPixel(x/100,y/100,color);
			if(x1<x2) { x+=ax; }  else{ x-=ax; }
			if(y1<y2) { y+=100; }else{ y-=100; }
		}
	}

	//



}

void fillRect(int x,int y,int w,int h,bool color){
  for(int i=y;i<(y+h);i++){ drawLine(x,i,x+w,i,color); }
}


void fillRectBlack(int x,int y,int w,int h){
  for(int i=y;i<(y+h);i++){ drawLine(x,i,x+w,i,BLACK); }
}


void pintatexto(char* str,int x,int y,bool inverted) {

  while(*str) {
	  pintachar(*str++,x,y,inverted);
	  x+=6;
  }
}

void pintaint(int num,int x,int y,bool inverted) {

	  uint8_t digits   = 10;                          // print 10 digits
	  uint8_t leadflag = 0;                           // flag for leading spaces
	  while(digits--) {                               // for all digits
	    uint8_t digitval = 0;                         // start with digit value 0
	    uint32_t divider = DIVIDER[digits];           // read current divider
	    while(num >= divider) {                     // if current divider fits into the value
	      leadflag = 1;                               // end of leading spaces
	      digitval++;                                 // increase digit value
	      num -= divider;                           // decrease value by divider
	    }
	    if(!digits)  { leadflag++; }                      // least digit has to be printed
	    if(leadflag) {
	    	pintachar(digitval+'0',x,y,inverted);     // print the digit
	    	x+=6;
	    }
	  }


}

void pintachar(char c,int x,int y,bool inverted) {
	  int pos=0;
	  c = c & 0x7F;

	  for(short i=0 ; i!=5; i++){
	    pos = x + i + (y / 8) * SSD1306_WIDTH;
	    SSD1306_Buffer[pos] = inverted? ~font5x8[c-32][i] : font5x8[c-32][i];
	  }

}

