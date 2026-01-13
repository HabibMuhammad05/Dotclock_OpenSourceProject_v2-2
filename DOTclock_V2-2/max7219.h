// MAX7219 functions by Pawel A. Hernik
// 2016.12.10 updated for rotated LED Martices, define ROTATE below (0,90 or 270)

#define NUM_MAX 4
#define LINE_WIDTH 16
#define ROTATE  90

#define MAX_DIGITS 16
byte dig[MAX_DIGITS]={0};
byte digold[MAX_DIGITS]={0};
byte digtrans[MAX_DIGITS]={0};
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
int dx=0;
int dy=0;
byte del=0;
int xPos, yPos;



// MAX7219 commands:
#define CMD_NOOP   0
#define CMD_DIGIT0 1
#define CMD_DIGIT1 2
#define CMD_DIGIT2 3
#define CMD_DIGIT3 4
#define CMD_DIGIT4 5
#define CMD_DIGIT5 6
#define CMD_DIGIT6 7
#define CMD_DIGIT7 8
#define CMD_DECODEMODE  9
#define CMD_INTENSITY   10
#define CMD_SCANLIMIT   11
#define CMD_SHUTDOWN    12
#define CMD_DISPLAYTEST 15

byte scr[NUM_MAX*8 + 8]; // +8 for scrolled char

// ============== INTERNAL FUNCTIONS FOR DISPLAYING =============//

void sendCmd(int addr, byte cmd, byte data){
  digitalWrite(CS_PIN, LOW);
  for (int i = NUM_MAX-1; i>=0; i--) {
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, i==addr ? cmd : 0);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, i==addr ? data : 0);
  }
  digitalWrite(CS_PIN, HIGH);
}

void sendCmdAll(byte cmd, byte data){
  digitalWrite(CS_PIN, LOW);
  for (int i = NUM_MAX-1; i>=0; i--) {
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, cmd);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  }
  digitalWrite(CS_PIN, HIGH);
}

void refresh(int addr){
  for (int i = 0; i < 8; i++)
    sendCmd(addr, i + CMD_DIGIT0, scr[addr * 8 + i]);
}

void refreshAllRot270(){
  byte mask = 0x01;
  for (int c = 0; c < 8; c++) {
    digitalWrite(CS_PIN, LOW);
    for(int i=NUM_MAX-1; i>=0; i--) {
      byte bt = 0;
      for(int b=0; b<8; b++) {
        bt<<=1;
        if(scr[i * 8 + b] & mask) bt|=0x01;
      }
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + c);
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, bt);
    }
    digitalWrite(CS_PIN, HIGH);
    mask<<=1;
  }
}

void refreshAllRot90(){
  byte mask = 0x80;
  for (int c = 0; c < 8; c++) {
    digitalWrite(CS_PIN, LOW);
    for(int i=NUM_MAX-1; i>=0; i--) {
      byte bt = 0;
      for(int b=0; b<8; b++) {
        bt>>=1;
        if(scr[i * 8 + b] & mask) bt|=0x80;
      }
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + c);
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, bt);
    }
    digitalWrite(CS_PIN, HIGH);
    mask>>=1;
  }
}

void refreshAll() {
#if ROTATE==270
  refreshAllRot270();
#elif ROTATE==90
  refreshAllRot90();
#else
  for (int c = 0; c < 8; c++) {
    digitalWrite(CS_PIN, LOW);
    for(int i=NUM_MAX-1; i>=0; i--) {
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + c);
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, scr[i * 8 + c]);
    }
    digitalWrite(CS_PIN, HIGH);
  }
#endif
}

void clr(){
  for (int i = 0; i < NUM_MAX*8; i++) scr[i] = 0;
}

void scrollLeft(){
  for(int i=0; i < NUM_MAX*8+7; i++) scr[i] = scr[i+1];
}

void invert(){
  for (int i = 0; i < NUM_MAX*8; i++) scr[i] = ~scr[i];
}

void initMAX7219(){
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  sendCmdAll(CMD_DISPLAYTEST, 0);
  sendCmdAll(CMD_SCANLIMIT, 7);
  sendCmdAll(CMD_DECODEMODE, 0);
  sendCmdAll(CMD_INTENSITY, 2); // minimum brightness
  sendCmdAll(CMD_SHUTDOWN, 0);
  clr();
  refreshAll();
}

//=============== ACTUAL FUNCTIONS FOR DISPLAYING THE VISUAL ==============//

void setCol(int col, byte v){
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<8*NUM_MAX)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}

void showDigit(char ch, int col, const uint8_t *data){
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<8*NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;
    }
}

void showSimpleClock(){
  dx=dy=0;
  clr();
  showDigit(h/10,  4, dig7x16);
  showDigit(h%10,  12, dig7x16);
  showDigit(m/10, 21, dig7x16);
  showDigit(m%10, 29, dig7x16);
  showDigit(s/10, 38, dig7x16);
  showDigit(s%10, 46, dig7x16);
  setCol(19,dots ? B00100100 : 0);
  setCol(36,dots ? B00100100 : 0);
  refreshAll();
}

void showAnimClock(){
  byte digPos[4]={1,8,17,25};
  int digHt = 12;
  int num = 4; 
  int i;
  if(del==0) {
    del = digHt;
    for(i=0; i<num; i++) digold[i] = dig[i];
    dig[0] = h/10 ? h/10 : 10;
    dig[1] = h%10;
    dig[2] = m/10;
    dig[3] = m%10;
    for(i=0; i<num; i++)  digtrans[i] = (dig[i]==digold[i]) ? 0 : digHt;
  } else
    del--;
  
  clr();
  for(i=0; i<num; i++) {
    if(digtrans[i]==0) {
      dy=0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy = -digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;
    }
  }
  dy=0;
  setCol(15,dots ? B00100100 : 0);
  refreshAll();
 delay(30);
}

int showChar(char ch, const uint8_t *data){
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

void printCharWithShift(unsigned char c, int shiftDelay){
  
  if (c < ' ' || c > '~'+25) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}

void printStringWithShift(const char* s, int shiftDelay){
  while (*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
  }
}
int printCharX(char ch, const uint8_t *font, int x){
  int fwd = pgm_read_byte(font);
  int fht = pgm_read_byte(font+1);
  int offs = pgm_read_byte(font+2);
  int last = pgm_read_byte(font+3);
  if(ch<offs || ch>last) return 0;
  ch -= offs;
  int fht8 = (fht+7)/8;
  font+=4+ch*(fht8*fwd+1);
  int j,i,w = pgm_read_byte(font);
  for(j = 0; j < fht8; j++) {
    for(i = 0; i < w; i++) scr[x+LINE_WIDTH*(j+yPos)+i] = pgm_read_byte(font+1+fht8*i+j);
    if(x+i<LINE_WIDTH) scr[x+LINE_WIDTH*(j+yPos)+i]=0;
  }
  return w;
}

void printChar(unsigned char c, const uint8_t *font){
  if(xPos>NUM_MAX*8) return;
  int w = printCharX(c, font, xPos);
  xPos+=w+1;
}

void printString(const char *s, const uint8_t *font){
  while(*s) printChar(*s++, font);
  //refreshAll();
}

void printString(String str, const uint8_t *font){
  printString(str.c_str(), font);
}


void ScrollJadwalSholat() {
  static bool built = false;
  static char txtScroll[200];

  if (built) {
    // just scroll
    printStringWithShift(txtScroll, 40);
    delay(10);
    return;
  }

  built = true;   // build only once
  txtScroll[0] = '\0';

  char jam[6];
  int hours, minutes;

  const char TimeName[][10] = {
    "SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TERBENAM", "MAGHRIB", "ISYA"
  };

  for (uint8_t i = 0; i < 7; i++) {

    // Skip TERBIT and TERBENAM
    if (i == 1 || i == 4) continue;

    get_float_time_parts(times[i], hours, minutes);
    minutes += ihti;

    if (minutes >= 60) {
      minutes -= 60;
      hours++;
    }

    sprintf(jam, "%02d:%02d", hours, minutes);

    strcat(txtScroll, TimeName[i]);
    strcat(txtScroll, " ");
    strcat(txtScroll, jam);
    strcat(txtScroll, " | ");
  }

  // ===== IMSAK =====
  get_float_time_parts(times[0], hours, minutes);
  minutes += ihti;

  if (minutes < 11) {
    minutes = 60 - minutes;
    hours--;
  } else {
    minutes -= 10;
  }

  sprintf(jam, "%02d:%02d", hours, minutes);
  strcat(txtScroll, "IMSAK ");
  strcat(txtScroll, jam);
  strcat(txtScroll, "               "); // spacing at end
}
