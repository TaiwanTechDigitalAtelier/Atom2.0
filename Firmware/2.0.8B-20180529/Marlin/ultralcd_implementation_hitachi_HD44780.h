#ifndef ULTRA_LCD_IMPLEMENTATION_HITACHI_HD44780_H
#define ULTRA_LCD_IMPLEMENTATION_HITACHI_HD44780_H

/**
* Implementation of the LCD display routines for a hitachi HD44780 display. These are common LCD character displays.
* When selecting the rusian language, a slightly different LCD implementation is used to handle UTF8 characters.
**/

#ifndef REPRAPWORLD_KEYPAD
extern volatile uint8_t buttons;  //the last checked buttons in a bit array.
#else
extern volatile uint16_t buttons;  //an extended version of the last checked buttons in a bit array.
#endif

unsigned long progressTimeout = 0;
uint8_t progressType = 0;
uint8_t extruder_count = 0;

////////////////////////////////////
// Setup button and encode mappings for each panel (into 'buttons' variable
//
// This is just to map common functions (across different panels) onto the same 
// macro name. The mapping is independent of whether the button is directly connected or 
// via a shift/i2c register.

#ifdef ULTIPANEL
// All Ultipanels might have an encoder - so this is always be mapped onto first two bits
#define BLEN_B 1
#define BLEN_A 0

#define EN_B (1<<BLEN_B) // The two encoder pins are connected through BTN_EN1 and BTN_EN2
#define EN_A (1<<BLEN_A)

#if defined(BTN_ENC) && BTN_ENC > -1
  // encoder click is directly connected
  #define BLEN_C 2 
  #define EN_C (1<<BLEN_C) 
#endif 
  
//
// Setup other button mappings of each panel
//
#if defined(LCD_I2C_VIKI)
  #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)
  
  // button and encoder bit positions within 'buttons'
  #define B_LE (BUTTON_LEFT<<B_I2C_BTN_OFFSET)    // The remaining normalized buttons are all read via I2C
  #define B_UP (BUTTON_UP<<B_I2C_BTN_OFFSET)
  #define B_MI (BUTTON_SELECT<<B_I2C_BTN_OFFSET)
  #define B_DW (BUTTON_DOWN<<B_I2C_BTN_OFFSET)
  #define B_RI (BUTTON_RIGHT<<B_I2C_BTN_OFFSET)

  #if defined(BTN_ENC) && BTN_ENC > -1 
    // the pause/stop/restart button is connected to BTN_ENC when used
    #define B_ST (EN_C)                            // Map the pause/stop/resume button into its normalized functional name 
    #define LCD_CLICKED (buttons&(B_MI|B_RI|B_ST)) // pause/stop button also acts as click until we implement proper pause/stop.
  #else
    #define LCD_CLICKED (buttons&(B_MI|B_RI))
  #endif  

  // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
  #define LCD_HAS_SLOW_BUTTONS

#elif defined(LCD_I2C_PANELOLU2)
  // encoder click can be read through I2C if not directly connected
  #if BTN_ENC <= 0 
    #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)
  
    #define B_MI (PANELOLU2_ENCODER_C<<B_I2C_BTN_OFFSET) // requires LiquidTWI2 library v1.2.3 or later

    #define LCD_CLICKED (buttons&B_MI)

    // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
    #define LCD_HAS_SLOW_BUTTONS
  #else
    #define LCD_CLICKED (buttons&EN_C)  
  #endif

#elif defined(REPRAPWORLD_KEYPAD)
    // define register bit values, don't change it
    #define BLEN_REPRAPWORLD_KEYPAD_F3 0
    #define BLEN_REPRAPWORLD_KEYPAD_F2 1
    #define BLEN_REPRAPWORLD_KEYPAD_F1 2
    #define BLEN_REPRAPWORLD_KEYPAD_UP 3
    #define BLEN_REPRAPWORLD_KEYPAD_RIGHT 4
    #define BLEN_REPRAPWORLD_KEYPAD_MIDDLE 5
    #define BLEN_REPRAPWORLD_KEYPAD_DOWN 6
    #define BLEN_REPRAPWORLD_KEYPAD_LEFT 7
    
    #define REPRAPWORLD_BTN_OFFSET 3 // bit offset into buttons for shift register values

    #define EN_REPRAPWORLD_KEYPAD_F3 (1<<(BLEN_REPRAPWORLD_KEYPAD_F3+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_F2 (1<<(BLEN_REPRAPWORLD_KEYPAD_F2+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_F1 (1<<(BLEN_REPRAPWORLD_KEYPAD_F1+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_UP (1<<(BLEN_REPRAPWORLD_KEYPAD_UP+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_RIGHT (1<<(BLEN_REPRAPWORLD_KEYPAD_RIGHT+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_MIDDLE (1<<(BLEN_REPRAPWORLD_KEYPAD_MIDDLE+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_DOWN (1<<(BLEN_REPRAPWORLD_KEYPAD_DOWN+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_LEFT (1<<(BLEN_REPRAPWORLD_KEYPAD_LEFT+REPRAPWORLD_BTN_OFFSET))

    #define LCD_CLICKED ((buttons&EN_C) || (buttons&EN_REPRAPWORLD_KEYPAD_F1))
    #define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN (buttons&EN_REPRAPWORLD_KEYPAD_DOWN)
    #define REPRAPWORLD_KEYPAD_MOVE_Y_UP (buttons&EN_REPRAPWORLD_KEYPAD_UP)
    #define REPRAPWORLD_KEYPAD_MOVE_HOME (buttons&EN_REPRAPWORLD_KEYPAD_MIDDLE)

#elif defined(NEWPANEL)
  #define LCD_CLICKED (buttons&EN_C)
  
#else // old style ULTIPANEL
  //bits in the shift register that carry the buttons for:
  // left up center down right red(stop)
  #define BL_LE 7
  #define BL_UP 6
  #define BL_MI 5
  #define BL_DW 4
  #define BL_RI 3
  #define BL_ST 2

  //automatic, do not change
  #define B_LE (1<<BL_LE)
  #define B_UP (1<<BL_UP)
  #define B_MI (1<<BL_MI)
  #define B_DW (1<<BL_DW)
  #define B_RI (1<<BL_RI)
  #define B_ST (1<<BL_ST)
  
  #define LCD_CLICKED (buttons&(B_MI|B_ST))
#endif

////////////////////////
// Setup Rotary Encoder Bit Values (for two pin encoders to indicate movement)
// These values are independent of which pins are used for EN_A and EN_B indications
// The rotary encoder part is also independent to the chipset used for the LCD
#if defined(EN_A) && defined(EN_B)
  #ifndef ULTIMAKERCONTROLLER
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1
  #else
    #define encrot0 0
    #define encrot1 1
    #define encrot2 3
    #define encrot3 2
  #endif
#endif 

#endif //ULTIPANEL

////////////////////////////////////
// Create LCD class instance and chipset-specific information
#if defined(LCD_I2C_TYPE_PCF8575)
  // note: these are register mapped pins on the PCF8575 controller not Arduino pins
  #define LCD_I2C_PIN_BL  3
  #define LCD_I2C_PIN_EN  2
  #define LCD_I2C_PIN_RW  1
  #define LCD_I2C_PIN_RS  0
  #define LCD_I2C_PIN_D4  4
  #define LCD_I2C_PIN_D5  5
  #define LCD_I2C_PIN_D6  6
  #define LCD_I2C_PIN_D7  7

  #include <Wire.h>
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  #define LCD LiquidCrystal_I2C
  LCD lcd(LCD_I2C_ADDRESS,LCD_I2C_PIN_EN,LCD_I2C_PIN_RW,LCD_I2C_PIN_RS,LCD_I2C_PIN_D4,LCD_I2C_PIN_D5,LCD_I2C_PIN_D6,LCD_I2C_PIN_D7);
  
#elif defined(LCD_I2C_TYPE_MCP23017)
  //for the LED indicators (which maybe mapped to different things in lcd_implementation_update_indicators())
  #define LED_A 0x04 //100
  #define LED_B 0x02 //010
  #define LED_C 0x01 //001

  #define LCD

  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2
  LCD lcd(LCD_I2C_ADDRESS);
  
#elif defined(LCD_I2C_TYPE_MCP23008)
  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2
  LCD lcd(LCD_I2C_ADDRESS);  

#elif defined(LCD_I2C_TYPE_PCA8574)
    #include <LiquidCrystal_I2C.h>
    #define LCD_CLASS LiquidCrystal_I2C
    LCD lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
  
#else
  // Standard directly connected LCD implementations
  #if LANGUAGE_CHOICE == 6
    #include "LiquidCrystalRus.h"
    #define LCD_CLASS LiquidCrystalRus
  #endif
  #include <LiquidCrystal.h>
  #define LCD LiquidCrystal
  LCD lcd(LCDC, LCDD, LCD2, LCD3, LCD4, LCD5, LCD6, LCD7, LCD8, LCD9);
#endif

/* Custom characters defined in the first 8 characters of the LCD */
#define LCD_STR_BEDTEMP     "\x00"
#define LCD_STR_THERMOMETER "\x01"
#define LCD_STR_DOT         "\x02"
#define LCD_STR_FEEDRATE    "\x03"
#define LCD_STR_CLOCK       "\x04"
#define LCD_STR_SDCARD      "\x05"
#define LCD_STR_ZHEIGHT     "\x06"
#define LCD_STR_CURRENTFILE "\x07"
#define LCD_STR_FOLDER      "\xDB"
#define LCD_STR_DEGREE		"\xDF"
#define LCD_STR_UPLEVEL		"\x5E"
#define LCD_STR_ARROW_RIGHT "\x7E"  /* from the default character set */

static void lcd_implementation_init()
{
    byte bedTemp[8] =
    {
		B10010,
		B10010,
		B01001,
		B01001,
		B10010,
		B00000,
		B11111,
		B00000
    }; //thanks Sonny Mounicou
	/*
    byte degree[8] =
    {
		B01100,
		B10010,
		B10010,
		B01100,
		B00000,
		B00000,
		B00000,
		B00000
    };
	*/
    byte thermometer[8] =
    {
		B01010,
		B01010,
		B01010,
		B01010,
		B11011,
		B01010,
		B00100,
		B00000
    };
    byte dot[8]={
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B10101,
		B00000,
		B00000,
    }; //thanks joris
    byte feedrate [8]={
		B01000,
		B00100,
		B10010,
		B11001,
		B10010,
		B00100,
		B01000,
		B00000
    }; //thanks Sonny Mounicou
    byte clock [8]={
		B01110,
		B00100,
		B01110,
		B10001,
		B10101,
		B10001,
		B01110,
		B00000
    }; //thanks Sonny Mounicou
	byte sdcard[8] = {
		B11100,
		B10010,
		B10001,
		B10001,
		B10001,
		B10001,
		B11111,
		B00000
	};
	byte zheight[8] = {
		B11111,
		B10001,
		B11101,
		B11011,
		B10111,
		B10001,
		B11111,
		B00000
	}; //current z height
	byte currentfile[8] = {
		B11111,
		B10001,
		B10111,
		B10001,
		B10111,
		B10110,
		B11100,
		B00000
	}; //current printing file
#if defined(LCDI2C_TYPE_PCF8575)
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  #ifdef LCD_I2C_PIN_BL
    lcd.setBacklightPin(LCD_I2C_PIN_BL,POSITIVE);
    lcd.setBacklight(HIGH);
  #endif
  
#elif defined(LCD_I2C_TYPE_MCP23017)
    lcd.setMCPType(LTI_TYPE_MCP23017);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    lcd.setBacklight(0); //set all the LEDs off to begin with
    
#elif defined(LCD_I2C_TYPE_MCP23008)
    lcd.setMCPType(LTI_TYPE_MCP23008);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);

#elif defined(LCD_I2C_TYPE_PCA8574)
      lcd.init();
      lcd.backlight();
    
#else
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
#endif

    lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
    lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
    lcd.createChar(LCD_STR_DOT[0], dot);
    //lcd.createChar(LCD_STR_FOLDER[0], folder);
    lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
    lcd.createChar(LCD_STR_CLOCK[0], clock);
    lcd.createChar(LCD_STR_SDCARD[0], sdcard);
    lcd.createChar(LCD_STR_ZHEIGHT[0], zheight);
    lcd.createChar(LCD_STR_CURRENTFILE[0], currentfile);
    lcd.clear();
}
static void lcd_implementation_clear()
{
    lcd.clear();
}
/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
static void lcd_printPGM(const char* str)
{
    char c;
    while((c = pgm_read_byte(str++)) != '\0')
    {
        lcd.write(c);
    }
}
/*
Possible status screens:
16x2   |0123456789012345|
       |000/000 B000/000|
       |Status line.....|

16x4   |0123456789012345|
       |000/000 B000/000|
       |SD100%    Z000.0|
       |F100%     T--:--|
       |Status line.....|

20x2   |01234567890123456789|
       |T000/000D B000/000D |
       |Status line.........|

20x4   |01234567890123456789|
       |T000/000D B000/000D |
       |X+000.0 Y+000.0 Z+000.0|
       |F100%  SD100% T--:--|
       |Status line.........|

20x4   |01234567890123456789|
       |T000/000D B000/000D |
       |T000/000D     Z000.0|
       |F100%  SD100% T--:--|
       |Status line.........|
*/

FORCE_INLINE void lcd_display_print_time()
{
	lcd.print(LCD_STR_CLOCK[0]);
	if (starttime != 0)
	{
		unsigned long difftime = currenttime - starttime;
		uint16_t time = difftime / 60000;
		lcd.print(itostr3(time / 60));
		lcd.print(':');
		lcd.print(itostr2(time % 60));
	}
	else 
	{
		lcd_printPGM(PSTR("000:00"));
	}
}

FORCE_INLINE void lcd_print_space(uint8_t &n)
{
	while (n--)
		lcd.print(' ');
}
FORCE_INLINE char extruder_char(char c, bool target)
{
	if (target)
	{
		if (extruder_count++ % 2 == 0)
		{
			return c;
		}
		else
		{
			return ' ';
		}
	}
	else
	{
		return c;
	}
}
static void lcd_implementation_status_screen()
{
  int tHotend = int(degHotend() + 0.5);
  int tTarget = int(degTargetHotend() + 0.5);

  lcd.setCursor(0, 0);
  lcd.print(LCD_STR_THERMOMETER[0]);
  lcd.print(itostr3(tHotend));
  lcd.print('/');
  lcd.print(itostr3(tTarget));
  lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

	lcd_printPGM(PSTR(LCD_STR_THERMOMETER));
	lcd_printPGM(PSTR(LCD_STR_THERMOMETER));
	lcd.print(' ');

	lcd.setCursor(13, 0);
	lcd.print(LCD_STR_ZHEIGHT[0]);
	lcd.print(ftostr32(current_position[Z_AXIS]));

#if defined(TEMP_SENSOR_BED) && TEMP_SENSOR_BED != 0
  //If we both have a 2nd extruder and a heated bed, show the heated bed temp on the 2nd line on the left, as the first line is filled with extruder temps
  tHotend=int(degBed() + 0.5);
	lcd.setCursor(0, 1);
	lcd.print(LCD_STR_BEDTEMP[0]);
	if (tHotend > 0)
	{
		tTarget = int(degTargetBed() + 0.5);
		lcd.print(itostr3(tHotend));
		lcd.print('/');
		lcd.print(itostr3(tTarget));
	}
	else
	{
		lcd.print("---/---");
	}
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
	if (extruder_inverting)
	{
		lcd.print(extruder_char('1', active_extruder == 1));
		lcd.print(extruder_char('0', active_extruder == 0));
	}
	else
	{
		lcd.print(extruder_char('0', active_extruder == 0));
		lcd.print(extruder_char('1', active_extruder == 1));
	}
	lcd.print(' ');
#endif

	lcd.setCursor(13, 1);
	if (card.start_print)
	{
		if (progressType)
		{
			lcd.print(LCD_STR_SDCARD[0]);
			lcd.print(itostr3(card.percentDone()));
			lcd.print('%');
			lcd.print(' ');
			lcd.print(' ');
		}
		else
		{
			currenttime = millis();
			lcd_display_print_time();
		}
	}
	else
	{
		lcd_display_print_time();
	}

	if (millis() - progressTimeout > 3000)
	{
		progressTimeout = millis();
		progressType = !progressType;
	}

  //Status message line on the last line
	uint8_t left = 12 - strlen(lcd_filename) - 1;
	lcd.setCursor(0, 2);
	lcd.print(LCD_STR_CURRENTFILE[0]);
	lcd.print(lcd_filename);
	lcd_print_space(left);

	lcd.setCursor(13, 2);
	lcd.print(LCD_STR_FEEDRATE[0]);
	lcd.print(itostr3(feedmultiply));
	lcd.print('%');

	left = LCD_WIDTH - strlen(lcd_status_message);
  lcd.setCursor(0, 3);
  lcd.print(lcd_status_message);
	lcd_print_space(left);
}

static void lcd_implementation_drawmenu_generic(uint8_t row, const char* pstr, char pre_char, char post_char)
{
    char c;
    //Use all characters in narrow LCDs
  #if LCD_WIDTH < 20
      uint8_t n = LCD_WIDTH - 1;
    #else
      uint8_t n = LCD_WIDTH - 1 - 1;
  #endif
    lcd.setCursor(0, row);
    lcd.print(pre_char);
    while( ((c = pgm_read_byte(pstr)) != '\0') && (n>0) )
    {
        lcd.print(c);
        pstr++;
        n--;
    }
    lcd_print_space(n);
    lcd.print(post_char);
}
static void lcd_implementation_drawmenu_value(uint8_t row, const char* pstr, const char* value, char sign, char pre_char)
{
    char c;
    //Use all characters in narrow LCDs
#if LCD_WIDTH < 20
    uint8_t n = LCD_WIDTH - 1 - 1;
#else
    uint8_t n = LCD_WIDTH - 2 - 1;
#endif
    lcd.setCursor(0, row);
    lcd.print(pre_char);
    while (((c = pgm_read_byte(pstr)) != '\0') && (n>0))
    {
        lcd.print(c);
        pstr++;
        n--;
    }
    lcd.print('(');
    while (((c = *value++) != '\0') && (n>0))
    {
        lcd.print(c);
        pstr++;
        n--;
    }
    if (sign)
    {
        lcd.print(sign);
        n--;
    }
    lcd.print(')');
    lcd_print_space(n);
}
static void lcd_implementation_drawmenu_setting_edit_generic(uint8_t row, const char* pstr, char pre_char, char* data)
{
    char c;
    //Use all characters in narrow LCDs
  #if LCD_WIDTH < 20
    uint8_t n = LCD_WIDTH - 1 - 1 - strlen(data);
  #else
    uint8_t n = LCD_WIDTH - 1 - 2 - strlen(data);
  #endif
    lcd.setCursor(0, row);
    lcd.print(pre_char);
    while( ((c = pgm_read_byte(pstr)) != '\0') && (n>0) )
    {
        lcd.print(c);
        pstr++;
        n--;
    }
    lcd.print(':');
    lcd_print_space(n);
    lcd.print(data);
}
static void lcd_implementation_drawmenu_setting_edit_generic_P(uint8_t row, const char* pstr, char pre_char, const char* data)
{
    char c;
    //Use all characters in narrow LCDs
  #if LCD_WIDTH < 20
      uint8_t n = LCD_WIDTH - 1 - 1 - strlen_P(data);
    #else
      uint8_t n = LCD_WIDTH - 1 - 2 - strlen_P(data);
  #endif
    lcd.setCursor(0, row);
    lcd.print(pre_char);
    while (((c = pgm_read_byte(pstr)) != '\0') && (n>0))
    {
        lcd.print(c);
        pstr++;
        n--;
    }
    lcd.print(':');
    lcd_print_space(n);
    lcd_printPGM(data);
}
#define lcd_implementation_drawmenu_setting_edit_int3_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_int3(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool_selected(row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_bool(row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

#define lcd_implementation_drawmenu_setting_edit_ex_int3_selected(row, pstr, pstr2, data, minValue, maxValue, text) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', text[*(data)])
#define lcd_implementation_drawmenu_setting_edit_ex_int3(row, pstr, pstr2, data, minValue, maxValue, text) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', text[*(data)])
#define lcd_implementation_drawmenu_setting_edit_ex_bool_selected(row, pstr, pstr2, data, text, invert) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', text[*(data) ^ invert])
#define lcd_implementation_drawmenu_setting_edit_ex_bool(row, pstr, pstr2, data, text, invert) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', text[*(data) ^ invert])

//Add version for callback functions
#define lcd_implementation_drawmenu_setting_edit_callback_int3_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_int3(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32ns_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32ns(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32ns(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32ns(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_bool_selected(row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_callback_bool(row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))


void lcd_implementation_drawedit(const char* pstr, char* value)
{
    lcd.setCursor(1, 1);
    lcd_printPGM(pstr);
    lcd.print(':');
   #if LCD_WIDTH < 20
      lcd.setCursor(LCD_WIDTH - strlen(value), 1);
    #else
      lcd.setCursor(LCD_WIDTH -1 - strlen(value), 1);
   #endif
    lcd.print(value);
}
void lcd_implementation_draweditM(const char* pstr) 
{
	int len = strlen(pstr);
	lcd.setCursor((20 - len) / 2, 1);
	uint8_t n = LCD_WIDTH - 1;
	char c;
	while (((c = *pstr) != '\0') && (n>0))
	{
		lcd.print(c);
		pstr++;
		n--;
	}
}
static void lcd_implementation_drawmenu_sdfile_selected(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    UNUSED(pstr);
    char c;
    uint8_t n = LCD_WIDTH - 1;
    lcd.setCursor(0, row);
    lcd.print('>');
    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-1] = '\0';
    }
    while (((c = *filename) != '\0') && (n>0))
    {
        lcd.print(c);
        filename++;
        n--;
    }
    lcd_print_space(n);
}
static void lcd_implementation_drawmenu_sdfile(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    UNUSED(pstr);
    char c;
    uint8_t n = LCD_WIDTH - 1;
    lcd.setCursor(0, row);
    lcd.print(' ');
    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-1] = '\0';
    }
    while (((c = *filename) != '\0') && (n>0))
    {
        lcd.print(c);
        filename++;
        n--;
    }
    lcd_print_space(n);
}
static void lcd_implementation_drawmenu_sddirectory_selected(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    UNUSED(pstr);
    char c;
    uint8_t n = LCD_WIDTH - 2;
    lcd.setCursor(0, row);
    lcd.print('>');
    lcd.print(LCD_STR_FOLDER[0]);
    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-2] = '\0';
    }
    while (((c = *filename) != '\0') && (n>0))
    {
        lcd.print(c);
        filename++;
        n--;
    }
    lcd_print_space(n);
}
static void lcd_implementation_drawmenu_sddirectory(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    UNUSED(pstr);
    char c;
    uint8_t n = LCD_WIDTH - 2;
    lcd.setCursor(0, row);
    lcd.print(' ');
    lcd.print(LCD_STR_FOLDER[0]);
    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-2] = '\0';
    }
    while (((c = *filename) != '\0') && (n>0))
    {
        lcd.print(c);
        filename++;
        n--;
    }
    lcd_print_space(n);
}
#define lcd_implementation_drawmenu_back_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, LCD_STR_UPLEVEL[0], ' ')
#define lcd_implementation_drawmenu_back(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_submenu(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode_selected(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_gcode(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')
#define lcd_implementation_drawmenu_function_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')

static void lcd_implementation_quick_feedback()
{
#ifdef LCD_USE_I2C_BUZZER
    lcd.buzz(60,1000/6);
#elif defined(BEEPER) && BEEPER > -1
    //lcd_buzz(100, 110);
#endif
}

#ifdef LCD_HAS_STATUS_INDICATORS
static void lcd_implementation_update_indicators()
{
  #if defined(LCD_I2C_PANELOLU2) || defined(LCD_I2C_VIKI)
    //set the LEDS - referred to as backlights by the LiquidTWI2 library 
    static uint8_t ledsprev = 0;
    uint8_t leds = 0;
    if (target_temperature_bed > 0) leds |= LED_A;
    if (target_temperature[0] > 0) leds |= LED_B;
    if (fanSpeed) leds |= LED_C;
    #if EXTRUDERS > 1  
      if (target_temperature[1] > 0) leds |= LED_C;
    #endif
    if (leds != ledsprev) {
      lcd.setBacklight(leds);
      ledsprev = leds;
    }
  #endif
}
#endif

#ifdef LCD_HAS_SLOW_BUTTONS
static uint8_t lcd_implementation_read_slow_buttons()
{
  #ifdef LCD_I2C_TYPE_MCP23017
    // Reading these buttons this is likely to be too slow to call inside interrupt context
    // so they are called during normal lcd_update
    return lcd.readButtons() << B_I2C_BTN_OFFSET; 
  #endif
}
#endif

#endif//ULTRA_LCD_IMPLEMENTATION_HITACHI_HD44780_H
