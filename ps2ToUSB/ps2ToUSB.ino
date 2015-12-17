/*  PS2Keyboard to USB */
   
#include <PS2Keyboard.h>

const int DataPin = 6;
const int IRQpin =  5;  // Teensy-LC can't use 0 or 1, at least. 

// State machine tables
uint16_t keyFSM[4][256] = {
  // STATE 0
  {
    /* 0x00 - 0x07 */ 0x0000, 0x0142, 0x0000, 0x013E, 0x013C, 0x013A, 0x013B, 0x0145,
    /* 0x08 - 0x0F */ 0x0000, 0x0143, 0x0141, 0x013F, 0x013D, 0x012B, 0x0135, 0x0000,
    /* 0x10 - 0x17 */ 0x0000, 0x05E2, 0x05E1, 0x0000, 0x05E0, 0x0114, 0x011E, 0x0000,
    /* 0x18 - 0x1F */ 0x0000, 0x0000, 0x011D, 0x0116, 0x0104, 0x011A, 0x011F, 0x0000,
    /* 0x20 - 0x27 */ 0x0000, 0x0106, 0x011B, 0x0107, 0x0108, 0x0121, 0x0120, 0x0000,
    /* 0x28 - 0x2F */ 0x0000, 0x012C, 0x0119, 0x0109, 0x0117, 0x0115, 0x0122, 0x0000,
    /* 0x30 - 0x37 */ 0x0000, 0x0111, 0x0105, 0x010B, 0x010A, 0x011C, 0x0123, 0x0000,
    /* 0x38 - 0x3F */ 0x0000, 0x0000, 0x0110, 0x010D, 0x0118, 0x0124, 0x0125, 0x0000,
    /* 0x40 - 0x47 */ 0x0000, 0x0136, 0x010E, 0x010C, 0x0112, 0x0127, 0x0126, 0x0000,
    /* 0x48 - 0x4F */ 0x0000, 0x0137, 0x0138, 0x010F, 0x0133, 0x0113, 0x0134, 0x0000,
    /* 0x50 - 0x57 */ 0x0000, 0x0000, 0x0131, 0x0000, 0x012D, 0x0130, 0x0000, 0x0000,
    /* 0x58 - 0x5F */ 0x0139, 0x05E5, 0x0128, 0x012E, 0x0000, 0x012F, 0x0000, 0x0000,
    /* 0x60 - 0x67 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x012A, 0x0000,
    /* 0x68 - 0x6F */ 0x0000, 0x0159, 0x0000, 0x015C, 0x015F, 0x0000, 0x0000, 0x0000,
    /* 0x70 - 0x77 */ 0x0162, 0x0163, 0x015A, 0x015D, 0x015E, 0x0160, 0x0129, 0x0153,
    /* 0x78 - 0x7F */ 0x0144, 0x0157, 0x015B, 0x0156, 0x0155, 0x0161, 0x0147, 0x0000,
    /* 0x80 - 0x87 */ 0x0000, 0x0000, 0x0000, 0x0140, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x88 - 0x8F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x90 - 0x97 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x98 - 0x9F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA0 - 0xA7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA8 - 0xAF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB0 - 0xB7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB8 - 0xBF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC0 - 0xC7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC8 - 0xCF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD0 - 0xD7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD8 - 0xDF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE0 - 0xE7 */ 0x0002, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE8 - 0xEF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF0 - 0xF7 */ 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF8 - 0xFF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  },

  // STATE 1
  {
    /* 0x00 - 0x07 */ 0x0000, 0x0242, 0x0000, 0x023E, 0x023C, 0x023A, 0x023B, 0x0245,
    /* 0x08 - 0x0F */ 0x0000, 0x0243, 0x0241, 0x023F, 0x023D, 0x022B, 0x0235, 0x0000,
    /* 0x10 - 0x17 */ 0x0000, 0x06E2, 0x06E1, 0x0000, 0x06E0, 0x0214, 0x021E, 0x0000,
    /* 0x18 - 0x1F */ 0x0000, 0x0000, 0x021D, 0x0216, 0x0204, 0x021A, 0x021F, 0x0000,
    /* 0x20 - 0x27 */ 0x0000, 0x0206, 0x021B, 0x0207, 0x0208, 0x0221, 0x0220, 0x0000,
    /* 0x28 - 0x2F */ 0x0000, 0x022C, 0x0219, 0x0209, 0x0217, 0x0215, 0x0222, 0x0000,
    /* 0x30 - 0x37 */ 0x0000, 0x0211, 0x0205, 0x020B, 0x020A, 0x021C, 0x0223, 0x0000,
    /* 0x38 - 0x3F */ 0x0000, 0x0000, 0x0210, 0x020D, 0x0218, 0x0224, 0x0225, 0x0000,
    /* 0x40 - 0x47 */ 0x0000, 0x0236, 0x020E, 0x020C, 0x0212, 0x0227, 0x0226, 0x0000,
    /* 0x48 - 0x4F */ 0x0000, 0x0237, 0x0238, 0x020F, 0x0233, 0x0213, 0x0234, 0x0000,
    /* 0x50 - 0x57 */ 0x0000, 0x0000, 0x0231, 0x0000, 0x022D, 0x0230, 0x0000, 0x0000,
    /* 0x58 - 0x5F */ 0x0239, 0x06E5, 0x0228, 0x022E, 0x0000, 0x022F, 0x0000, 0x0000,
    /* 0x60 - 0x67 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x022A, 0x0000,
    /* 0x68 - 0x6F */ 0x0000, 0x0259, 0x0000, 0x025C, 0x025F, 0x0000, 0x0000, 0x0000,
    /* 0x70 - 0x77 */ 0x0262, 0x0263, 0x025A, 0x025D, 0x025E, 0x0260, 0x0229, 0x0253,
    /* 0x78 - 0x7F */ 0x0244, 0x0257, 0x025B, 0x0256, 0x0255, 0x0261, 0x0247, 0x0000,
    /* 0x80 - 0x87 */ 0x0000, 0x0000, 0x0000, 0x0240, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x88 - 0x8F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x90 - 0x97 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x98 - 0x9F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA0 - 0xA7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA8 - 0xAF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB0 - 0xB7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB8 - 0xBF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC0 - 0xC7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC8 - 0xCF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD0 - 0xD7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD8 - 0xDF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE0 - 0xE7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE8 - 0xEF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF0 - 0xF7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF8 - 0xFF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  },

  // STATE 2
  {
    /* 0x00 - 0x07 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x08 - 0x0F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x10 - 0x17 */ 0x0000, 0x05E6, 0x000B, 0x0000, 0x05E4, 0x0000, 0x0000, 0x0000,
    /* 0x18 - 0x1F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x05E3,
    /* 0x20 - 0x27 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x05E7,
    /* 0x28 - 0x2F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0165,
    /* 0x30 - 0x37 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x38 - 0x3F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x40 - 0x47 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x48 - 0x4F */ 0x0000, 0x0000, 0x0154, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x50 - 0x57 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x58 - 0x5F */ 0x0000, 0x0000, 0x0158, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x60 - 0x67 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x68 - 0x6F */ 0x0000, 0x014D, 0x0000, 0x0150, 0x014A, 0x0000, 0x0000, 0x0000,
    /* 0x70 - 0x77 */ 0x0149, 0x014C, 0x0151, 0x0000, 0x014F, 0x0152, 0x0000, 0x0000,
    /* 0x78 - 0x7F */ 0x0000, 0x0000, 0x014E, 0x0000, 0x0000, 0x014B, 0x0000, 0x0000,
    /* 0x80 - 0x87 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x88 - 0x8F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x90 - 0x97 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x98 - 0x9F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA0 - 0xA7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA8 - 0xAF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB0 - 0xB7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB8 - 0xBF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC0 - 0xC7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC8 - 0xCF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD0 - 0xD7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD8 - 0xDF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE0 - 0xE7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE8 - 0xEF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF0 - 0xF7 */ 0x0003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF8 - 0xFF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  },

  // STATE 3
  {
    /* 0x00 - 0x07 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x08 - 0x0F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x10 - 0x17 */ 0x0000, 0x06E6, 0x0000, 0x0000, 0x06E4, 0x0000, 0x0000, 0x0000,
    /* 0x18 - 0x1F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x06E3,
    /* 0x20 - 0x27 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x06E7,
    /* 0x28 - 0x2F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0265,
    /* 0x30 - 0x37 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x38 - 0x3F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x40 - 0x47 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x48 - 0x4F */ 0x0000, 0x0000, 0x0254, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x50 - 0x57 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x58 - 0x5F */ 0x0000, 0x0000, 0x0258, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x60 - 0x67 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x68 - 0x6F */ 0x0000, 0x024D, 0x0000, 0x0250, 0x024A, 0x0000, 0x0000, 0x0000,
    /* 0x70 - 0x77 */ 0x0249, 0x024C, 0x0251, 0x0000, 0x024F, 0x0252, 0x0000, 0x0000,
    /* 0x78 - 0x7F */ 0x0000, 0x0000, 0x024E, 0x0000, 0x000D, 0x024B, 0x0000, 0x0000,
    /* 0x80 - 0x87 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x88 - 0x8F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x90 - 0x97 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0x98 - 0x9F */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA0 - 0xA7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xA8 - 0xAF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB0 - 0xB7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xB8 - 0xBF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC0 - 0xC7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xC8 - 0xCF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD0 - 0xD7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xD8 - 0xDF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE0 - 0xE7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xE8 - 0xEF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF0 - 0xF7 */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* 0xF8 - 0xFF */ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  },
};

uint8_t modifierKeys[8] = { 0 };

PS2Keyboard keyboard;

void setup() {
  modifierKeys[0x0] = (MODIFIERKEY_LEFT_CTRL   & 0xFF); // KEY_LCTRL
  modifierKeys[0x1] = (MODIFIERKEY_LEFT_SHIFT  & 0xFF); // KEY_LSHIFT
  modifierKeys[0x2] = (MODIFIERKEY_LEFT_ALT    & 0xFF); // KEY_LALT
  modifierKeys[0x3] = (MODIFIERKEY_LEFT_GUI    & 0xFF); // KEY_LGUI
  modifierKeys[0x4] = (MODIFIERKEY_RIGHT_GUI   & 0xFF); // KEY_RCTRL (swapped for RGUI)
  modifierKeys[0x5] = (MODIFIERKEY_RIGHT_SHIFT & 0xFF); // KEY_RSHIFT
  modifierKeys[0x6] = (MODIFIERKEY_RIGHT_ALT   & 0xFF); // KEY_RALT
  modifierKeys[0x7] = (MODIFIERKEY_RIGHT_CTRL  & 0xFF); // KEY_RGUI (swapped for RCTRL)

  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
}

void PrintHex8(uint8_t data) {
  if (data < 0x10) {
    Serial.print("0");
  }
  
  Serial.print(data, HEX); 

  Keyboard.set_modifier(0);
}

int lastStart = 0;
uint16_t state = 0;

uint8_t keysPressed[6] = { 0 };
uint8_t modifiersPressed = 0;

uint16_t nextState(uint16_t state, uint8_t code) {
    if (state < 4) {
        return keyFSM[state][code];
    }
    else switch (((state && 0xFF) << 8) | code) {
        case 0x0414: return 0x0005; break;
        case 0x0577: return 0x0006; break;
        case 0x06E1: return 0x0007; break;
        case 0x07F0: return 0x0008; break;
        case 0x0814: return 0x0009; break;
        case 0x09F0: return 0x000A; break;
        case 0x0A77: return 0x0148; break;
        case 0x0BE0: return 0x000C; break;
        case 0x0C7C: return 0x0146; break;
        case 0x0DE0: return 0x000E; break;
        case 0x0EF0: return 0x000F; break;
        case 0x0F12: return 0x0246; break;
        default: return 0x0000; break;
    }
}

void loop() {
  if (lastStart) {
    int now = millis();

    if ((now - lastStart) > 1000) {
      lastStart = 0;
      Serial.println("");

      // Let's assume that we're well and truly reset here.
      state = 0;
    }
  }
  
  if (keyboard.scanCodeAvailable()) {
    uint8_t code = keyboard.readScanCode();
    uint8_t usbCode = 0;

    Serial.print(state);
    Serial.print("-");
    PrintHex8(code);
    lastStart = millis();

    uint16_t next = nextState(state, code);
    bool sendNow = 0;

    // Is this a MAKE code?
    if (next & 0x0100) {
      usbCode = next & 0xFF;

      if (next & 0x0400) {
        // This is a modifier.
        modifiersPressed |= modifierKeys[usbCode & 0x0F];
      }

      for (int i = 0; i < 6; i++) {
        if (keysPressed[i] == 0) {
          // Add the new key.
          keysPressed[i] = usbCode;
          break;
        }
        else if (keysPressed[i] == usbCode) {
          // Already there.
          break;
        }
      }

      Serial.print(" MAKE ");
      PrintHex8(usbCode);
      Serial.println("");
      state = 0;

      sendNow = 1;
    }
    else if (next & 0x200) {
      // BREAK code.
      usbCode = next & 0xFF;

      if (next & 0x0400) {
        // This is a modifier.
        modifiersPressed &= ~(modifierKeys[usbCode & 0x0F]);
      }

      int i = 0;
      int j = 0;

      while (i < 6) {
        if (keysPressed[i] == 0) {
          break;
        }
        else if (keysPressed[i] != usbCode) {
          j++;
        }

        i++;

        if (i != j) {
          keysPressed[j] = keysPressed[i];
        }
      }

      while (j < 6) {
        keysPressed[j] = 0;
        j++;
      }

      Serial.print(" BREAK ");
      PrintHex8(usbCode);
      Serial.println("");
      state = 0;

      sendNow = 1;
    }
    else {
      state = next;
      Serial.print(">");
      Serial.print(state);
      Serial.print(" ");
    }

    if (sendNow) {
      Serial.print("SEND ");
      PrintHex8(modifiersPressed);
      Serial.print(":");

      for (int i = 0; i < 6; i++) {
        PrintHex8(keysPressed[i]);

        if (i < 5) {
          Serial.print(",");
        }
      }

      Serial.println("");

      Keyboard.set_modifier(modifiersPressed);

      for (int i = 0; i < 6; i++) {
        keyboard_keys[i] = keysPressed[i];
      }

      Keyboard.send_now();

      sendNow = 0;
    }
  }
}


