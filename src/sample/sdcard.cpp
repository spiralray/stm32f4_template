/*
 * sdcard.cpp
 *
 *  Created on: 2015/03/02
 *      Author: Kentaro Tanaka
 */




/* Includes ------------------------------------------------------------------*/
#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"

#include "board/main_v3.h"

#include "ff.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
DWORD AccSize;										/* Work register for fs command */
WORD AccFiles, AccDirs;								/* Work register for fs command */
FILINFO Finfo;										/* Work register for fs command */

#if _USE_LFN
char Lfname[512];
#endif
char Line[256];										/* Console input buffer */

FATFS FatFs;								/* File system object for each logical drive */
FIL File;										/* File objects */
DIR Dir;											/* Directory object */

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

int showFolder(const char *path){

  FRESULT res;
  char buff[512];

  /* Flush Input Buffer */
  Line[0] =0;

#if _USE_LFN
  Finfo.lfname = Lfname;
  Finfo.lfsize = sizeof(Lfname);
#endif

  DIR Dir;
  long p1;
  UINT s1, s2;

  p1 = s1 = s2 = 0;
  res = f_opendir(&Dir, path);
  for(;;) {
      res = f_readdir(&Dir, &Finfo);
      if ((res != FR_OK) || !Finfo.fname[0]){
          if( res != FR_OK ){
              sprintf(buff, "\r\n\r\nERROR\r\n%d\r\n\r\n",res);
              debug << buff;
          }
          return res;
      }
      if (Finfo.fattrib & AM_DIR) {
          s2++;
      } else {
          s1++; p1 += Finfo.fsize;
      }
#if 1
      sprintf(buff, "%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s  %s\r\n",
          (Finfo.fattrib & AM_DIR) ? 'D' : '-',
              (Finfo.fattrib & AM_RDO) ? 'R' : '-',
                  (Finfo.fattrib & AM_HID) ? 'H' : '-',
                      (Finfo.fattrib & AM_SYS) ? 'S' : '-',
                          (Finfo.fattrib & AM_ARC) ? 'A' : '-',
                              (Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
                              (Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
                              Finfo.fsize, Finfo.fname,
#if _USE_LFN
                              Lfname);
#else
      "");
#endif
      debug << buff;

#endif
  }

  return 0;

}

/**************************************************************************/
/*
    @brief  Main Program.
	@param  None.
    @retval None.
 */
/**************************************************************************/
int main(void)
{
  //Initialise Systick
  MillisecondTimer::initialise();

  MainV3 board;

  debug << "FatFs test\r\n";

#if _USE_LFN
	Finfo.lfname = Lfname;
	Finfo.lfsize = sizeof(Lfname);
#endif

  f_mount(&FatFs, "", 0);
  MillisecondTimer::delay(100);

  debug << "Show file list\r\n";
  showFolder("");
  debug << "End\r\n";

  uint32_t s;
  FRESULT res;

  //----------------------------------------------------------------Write test
  char *str = "This is a sample file of FatFs module.";

  debug << "\r\nWrite test\r\n";

  res = f_open(&File, "sample.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res){
	  debug << "f_open " << (uint32_t) res;
	  while(1);
  }

  res = f_write(&File, str, strlen(str)+1, &s);
  if (res){
	  debug << "f_write " << (uint32_t) res;
	  while(1);
  }
  f_close(&File);

  //----------------------------------------------------------------Read test
  char line[256];

  debug << "\r\nRead test\r\n";

  /* テキスト ファイルを開く */
  res = f_open(&File, "sample.txt", FA_READ);
  if (res){
	  debug << "f_open " << (uint32_t) res;
	  while(1);
  }

  /* 1行ずつ読み出して表示 */
  while (f_gets(line, sizeof line, &File))
	  debug << line;

  f_close(&File);


  while(1){
      MillisecondTimer::delay(500);
      board.led.On();
      MillisecondTimer::delay(500);
      board.led.Off();
  }

}

/* End Of File ---------------------------------------------------------------*/


