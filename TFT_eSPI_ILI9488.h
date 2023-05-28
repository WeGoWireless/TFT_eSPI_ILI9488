#ifndef _TFT_eSPI_ILI9488H_
#define _TFT_eSPI_ILI9488H_

#include <Arduino.h>
#include <TFT_eSPI.h>

class TFT_eSPI_ILI9488 : public TFT_eSPI {
public:
    TFT_eSPI_ILI9488(int8_t _CS = -1, int8_t _DC = -1, int8_t _MOSI = -1,
        int8_t _SCLK = -1, int8_t _RST = -1, int8_t _MISO = -1);

    void setRotation(uint8_t r); // Set the display image orientation to 0, 1, 2 or 3
    void initTouch(uint8_t _CS);
private:

};

#endif