#include <Arduino.h>
#include <TFT_eSPI_ILI9488.h>
//#include <TFT_eSPI.h>

int8_t TFT_eSPI_miso;
int8_t TFT_eSPI_mosi;
int8_t TFT_eSPI_sclk;
int8_t TFT_eSPI_rst;
int8_t TFT_eSPI_cs;
int8_t TFT_eSPI_dc;

int16_t TFT_eSPI_width = 320;
int16_t TFT_eSPI_height = 480;

int8_t TFT_eSPI_init_delay = 0x80;

int8_t TFT_eSPI_nop = 0x00;
int8_t TFT_eSPI_swrst = 0x01;

int8_t TFT_eSPI_slpin = 0x10;
int8_t TFT_eSPI_slpout = 0x11;

int8_t TFT_eSPI_invoff = 0x20;
int8_t TFT_eSPI_invon = 0x21;

int8_t TFT_eSPI_dispoff = 0x28;
int8_t TFT_eSPI_dispon = 0x29;

int8_t TFT_eSPI_caset = 0x2A;
int8_t TFT_eSPI_paset = 0x2B;
int8_t TFT_eSPI_ramwr = 0x2C;

int8_t TFT_eSPI_ramrd = 0x2E;

int8_t TFT_eSPI_madctl = 0x36;

int8_t TFT_eSPI_mad_my = 0x80;
int8_t TFT_eSPI_mad_mx = 0x40;
int8_t TFT_eSPI_mad_mv = 0x20;
int8_t TFT_eSPI_mad_ml = 0x10;
int8_t TFT_eSPI_mad_rgb = 0x00;
int8_t TFT_eSPI_mad_bgr = 0x08;
int8_t TFT_eSPI_mad_mh = 0x04;
int8_t TFT_eSPI_mad_ss = 0x02;
int8_t TFT_eSPI_mad_gs = 0x01;

const uint8_t PROGMEM initcmd[] = {
  0XC0, 2, 0x17, 0x15,			// Power control VRH[5:0]  Vreg1out, Verg2out
  0xC1, 1, 0x41,				// Power control SAP[2:0];BT[3:0]  VGH,VGL
  0xC5, 3, 0x00, 0x12, 0x80,    // VCM control
  TFT_eSPI_madctl, 1, 0x48,			// Memory Access Control
  0x3A, 1, 0x66,				// 18 bit colour for SPI
  0xB0, 1, 0x00,				// Interface Mode Control  jaretburkett has 80=SDO NOT USE, Bodmer has 00
  0xB1, 1, 0xA0,				// Frame Rate Control
  0xB4, 1, 0x02,				// Display Inversion Control 2-dot
  0xB6, 3, 0x02, 0x02, 0x3B,    // Display Function Control
  0xB7, 1, 0xC6,				// Entry mode set
  0xF7, 4, 0xA9, 0x51, 0x2C, 0x82,
  0xE0, 15, 0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, // Set Gamma
	0x3F, 0x78, 0x4C, 0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F,
  0XE1, 15, 0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, // Set Gamma
	0x32, 0x45, 0x46, 0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F,
  TFT_eSPI_slpout, 0x80,                // Exit Sleep 0x80 = delay 150
  TFT_eSPI_dispon, 0x80,                // Display on 0x80 = delay 150
  0x00                             // End of list
};

int8_t TFT_eSPI_touch_cs;
/**************************************************************************/
/*!
	@brief  Instantiate TFT_eSPI ILI9488 driver with hardware SPI
	@param    cs    Chip select pin #
	@param    dc    Data/Command pin #
	@param    mosi  SPI MOSI pin #
	@param    sclk  SPI Clock pin #
	@param    rst   Reset pin # (optional, pass -1 if unused)
	@param    miso  SPI MISO pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
TFT_eSPI_ILI9488::TFT_eSPI_ILI9488(int8_t cs, int8_t dc, int8_t mosi,
							       int8_t sclk, int8_t rst, int8_t miso)
	: TFT_eSPI(cs, dc, mosi, sclk, rst, miso, TFT_WIDTH, TFT_HEIGHT)
{}

/***************************************************************************************
** Function name:           setRotation
** Description:             rotate the screen orientation m = 0-3 or 4-7 for BMP drawing
***************************************************************************************/
void TFT_eSPI_ILI9488::setRotation(uint8_t m)
{
	writecommand(TFT_MADCTL);
	rotation = m % 4;
	switch (rotation) {
	case 0: // Portrait
		writedata(TFT_MAD_MX | TFT_MAD_BGR);
		_width = TFT_WIDTH;
		_height = TFT_HEIGHT;
		break;
	case 1: // Landscape (Portrait + 90)
		writedata(TFT_MAD_MV | TFT_MAD_BGR);
		_width = TFT_HEIGHT;
		_height = TFT_WIDTH;
		break;
	case 2: // Inverter portrait
		writedata(TFT_MAD_MY | TFT_MAD_BGR);
		_width = TFT_WIDTH;
		_height = TFT_HEIGHT;
		break;
	case 3: // Inverted landscape
		writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_BGR);
		_width = TFT_HEIGHT;
		_height = TFT_WIDTH;
		break;
	}

	addr_row = 0xFFFF;
	addr_col = 0xFFFF;

	// Reset the viewport to the whole screen
	resetViewport();
}

void TFT_eSPI_ILI9488::initTouch(uint8_t _cs)
{
	TFT_eSPI_touch_cs = _cs;
}
