#include <cmath>
#include <cstring>
#include "lps305.h"

LPS305::LPS305() {
  tty = NULL;
}

LPS305::~LPS305() {
  if(tty)ttym_close(tty);
}

int LPS305::connect(char portName[] , int baud) {
  tty = ttym_open(portName, baud);
  if(tty == NULL) {
    printf("No connect!\n");
    return (int)NULL;
  }
  ttym_timeout(tty, 1000);
  
  setOutput(1);
  setVoltage(1, 0);
  setVoltage(2, 0);
  setCurrent(1, 0);
  setCurrent(2, 0);
  
  return 1;
}
  
void LPS305::setVoltage(int channel, double volt) {
  if(channel < 1 || channel > 2 || volt < (-30) || volt > 30) {
    printf("Setup voltage error");
    return;
  }
  char buff[256];
  sprintf(buff, "VSET%i %.3f\r\n", channel, volt);
  std::cout << buff << std::endl;
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
}

void LPS305::setCurrent(int channel, double curr) {
  if(channel < 1 || channel > 2 || curr < (-2.5) || curr > 2.5) {
    printf("Setup current error");
    return;
  }
  char buff[256];
  sprintf(buff, "ISET%i %f\r\n", channel, curr);
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
}

void LPS305::setOutput(int mode) {
  if(mode < 0 || mode > 1) {
    printf("Setup output error");
    return;
  }
  char buff[256];
  sprintf(buff, "OUT%i\r\n", mode);
  ttym_write(tty, buff, strlen(buff));
  //ttym_read(tty,buff,200);
  sprintf(buff, getstr().c_str());
}

void LPS305::setModeTracking(int mode) {
  if(mode < 0 || mode > 2) {
    printf("Setup tracking error");
    return;
  }
  char buff[256];
  sprintf(buff, "TRACK%i\n", mode);
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
}

void LPS305::setBeeper(int mode) {
  if(mode < 0 || mode > 1) {
    printf("Setup beeper error");
    return;
  }
  char buff[256];
  sprintf(buff, "BEEP%i\n", mode);
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
} 

void LPS305::setDigOutput(int mode) {
  if(mode != 0 || mode != 3 || mode != 5) {
    printf("Setup digital output error");
    return;
  }
  char buff[256];
  sprintf(buff, "VDD%i\n", mode);
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
}

void LPS305::setCompensatedOutput(int mode) {
  if(mode < 0 || mode > 1) {
    std::cout << "Setup compensated output error" << std::endl;
    return;
  }
  char buff[256];
  sprintf(buff, "LOWA%i\n", mode);
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
}
  
double LPS305::getVoltage(int channel) {
  if(channel < 1 || channel > 2) {
    printf("Setup output error");
    return -101;
  }
  double res;
  char buff[256];
  sprintf(buff, "VOUT%i\r\n", channel);
  ttym_write(tty, buff, 6);
  sprintf(buff, getstr().c_str());

  res = atof(buff);
  return res;
}

double LPS305::getCurrent(int channel) {
  if(channel < 1 || channel > 2) {
    printf("Setup output error");
    return -101;
  }
  double res;
  char buff[256];
  sprintf(buff, "IOUT%i\n", channel);
  ttym_write(tty, buff, strlen(buff));
  sprintf(buff, getstr().c_str());
  
  res = atof(buff);
  return res;
}

int LPS305::getStatus() {
  int res;
  char buff[256];
  ttym_write(tty, (void*)"STATUS\n", 7);
  sprintf(buff, getstr().c_str());
  
  res = atoi(buff);
  return res;
}

char* LPS305::getModel() {
  char *buff = (char*)malloc(sizeof(char)*256);
  ttym_write(tty, (void*)"MODEL\n", 6);
  sprintf(buff, getstr().c_str());
  return buff;
}

char* LPS305::getVersion() {
  char *buff = (char*)malloc(sizeof(char)*256);
  ttym_write(tty, (void*)"VERSION\n", 8);
  sprintf(buff, getstr().c_str());

  return buff;
}

char* LPS305::getHelp() {
  char *buff = (char*)malloc(sizeof(char)*256);
  ttym_write(tty, (void*)"HELP\n", 5);
  sprintf(buff, getstr().c_str());
  return buff;
}

void LPS305::close() {
  if(tty != NULL) {
    ttym_close(tty);
    tty = NULL;
  }
}

std::string LPS305::getstr() {
  std::string res_str("", 256);
  size_t sz = 0;
  char data;
  while(1) {
    data = ttym_readchar(tty);
    if(sz >= res_str.size()) {
      std::cout << std::endl;
      std::cout << "out of range" << std::endl;
      return res_str;
    }
    if(data == ' ') continue;
    sz++;
    std::cout << data;
    if(data <= 0 && res_str.size() > 0){ //timeout
      std::cout << std::endl;
      return res_str;
    } else if(data <= 0 && res_str.size() <= 0) {
      std::cout << std::endl;
      return "";
    }
    if(data == 'O') {
      res_str[sz] = data;
      data = ttym_readchar(tty);
      sz++;
      if(data == 'K') {
        res_str[sz] = data;
        data = ttym_readchar(tty);
        sz++;
        if(data == '\n')
          break;
      }
    }
    res_str[sz] = data;
  }
  std::cout << std::endl;
  return res_str;
}
