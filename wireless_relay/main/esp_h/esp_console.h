#ifndef ESP_CONSOLE_H
#define ESP_CONSOLE_H "ESP_CONSOLE_HEADER_FILE"

#include <iostream>
using namespace std;


 namespace Color {
//Black 0;30
//Dark Gray 1;30
//Red 0;31
//Light Red 1;31
//Green 0;32
//Light Green 1;32
//Brown/Orange 0;33 Yellow 1;33 Blue 0;34 Light Blue 1;34 Purple 0;35 Light Purple 1;35 Cyan 0;36 Light Cyan 1;36 Light Gray 0;37 White 1;37
   string   FG_RED      = "31",
   FG_GREEN    = "32",
    FG_YELLOW    = "33",
        FG_BLUE     = "34",
        FG_DEFAULT  = "39",

        BG_RED      = "41",
        BG_GREEN    = "42",
        BG_BLUE     = "44",
        BG_DEFAULT  = "49",


        RED="\033[1;31m",
        YELLOW="\033[1;33m",
        WHITE="\033[1;37m",
        GREEN="\033[1;32m";
 }

 /*
   HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
  */

void startColor(string col)
{
      printf("%s",col.c_str());
}

void startRed()
{
    startColor(Color::GREEN);
}

void endColor()
{
    printf("\033[0m");
}

void writelnColor (string text, string color)
{
  startColor(color);
  printf("%s\n",text.c_str() );
    endColor();
}

void writeColor (string text, string color)
{
  startColor(color);
  printf("%s",text.c_str() );
    endColor();
}

void writelnRed (string text) {
  startColor(Color::RED);
    printf("%s\n",text.c_str() );
    endColor();
}

void writeWhite (string text) {
  startColor(Color::WHITE);
    printf("%s",text.c_str() );
    endColor();
}

void writelnWhite (string text) {
  startColor(Color::WHITE);
    printf("%s\n",text.c_str() );
    endColor();
}


void writeGreen (string text) {
  startColor(Color::GREEN);
    printf("%s",text.c_str() );
    endColor();
}
void writelnGreen (string text) {
  startColor(Color::GREEN);
    printf("%s\n",text.c_str() );
    endColor();
}

void writelnYellow (string text)
{
    startColor(Color::YELLOW);
    printf("%s\n", text.c_str());
    endColor();
}

void writeYellow (string text)
{   startColor(Color::YELLOW);
    printf("%s", text.c_str());
    endColor();
}




#endif
