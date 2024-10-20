
///////////////////
/* DAC Functions */
///////////////////

uint8_t dac_cs = 17;
uint8_t dac_load = 16;

SPISettings spisettings(2000000, MSBFIRST, SPI_MODE0);

void dac_init(void) {
  pinMode(dac_cs,OUTPUT);
  pinMode(dac_load,OUTPUT);
}

void dac_MCP49xx (uint8_t channel, uint8_t value) {
  uint8_t output_1, output_2; // represent 1st and 2nd bytes of 16 bits
  digitalWrite(dac_cs, LOW);
  if (channel == 1) {
    output_1=0x80;
  }
  else {
    output_1=0x00;
  }
  output_1 |= 0x70;
  output_2 = 0x00;
  output_1 = output_1 | (value >> 4);
  output_2 = value & 0x0f;
  output_2 = output_2 << 4;

  // Start an SPI transmission with the assigned spisettings
  SPI.beginTransaction(spisettings);  
  SPI.transfer(output_1);
  SPI.transfer(output_2);
  SPI.endTransaction(); // End a transmission
  digitalWrite(dac_cs, HIGH);
}

void dac_xy(uint8_t x, uint8_t y)
{
  digitalWrite(dac_load, HIGH);
  dac_MCP49xx(1,x);
  dac_MCP49xx(0,y);
  digitalWrite(dac_load, LOW);
}

