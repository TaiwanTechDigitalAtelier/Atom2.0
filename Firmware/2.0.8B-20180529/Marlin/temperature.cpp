/*
  temperature.c - temperature control
  Part of Marlin
  
 Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 This firmware is a mashup between Sprinter and grbl.
  (https://github.com/kliment/Sprinter)
  (https://github.com/simen/grbl/tree)
 
 It has preliminary support for Matthew Roberts advance algorithm 
    http://reprap.org/pipermail/reprap-dev/2011-May/003323.html

 */


#include "Marlin.h"
#include "ultralcd.h"
#include "temperature.h"
#include "watchdog.h"

//===========================================================================
//=============================public variables============================
//===========================================================================
int target_temperature =  0;
int target_temperature_bed = 0;
int current_temperature_raw = 0;
int last_temperature_raw = 0;
unsigned long last_update_temperature = 0;
float current_temperature = 0.0;
int current_temperature_bed_raw = 0;
float current_temperature_bed = 0.0;
#ifdef PIDTEMP
  float Kp=DEFAULT_Kp;
  float Ki=(DEFAULT_Ki*PID_dT);
  float Kd=(DEFAULT_Kd/PID_dT);
  #ifdef PID_ADD_EXTRUSION_RATE
    float Kc=DEFAULT_Kc;
  #endif
#endif //PIDTEMP

#ifdef PIDTEMPBED
  float bedKp=DEFAULT_bedKp;
  float bedKi=(DEFAULT_bedKi*PID_dT);
  float bedKd=(DEFAULT_bedKd/PID_dT);
#endif //PIDTEMPBED
  
#ifdef FAN_SOFT_PWM
  unsigned char fanSpeedSoftPwm;
#endif
  
//===========================================================================
//=============================private variables============================
//===========================================================================
static volatile bool temp_meas_ready = false;

#ifdef PIDTEMP
  //static cannot be external:
  static float temp_iState = { 0 };
  static float temp_dState = { 0 };
  static float pTerm;
  static float iTerm;
  static float dTerm;
  //int output;
  static float pid_input;
  static float pid_output;
  static float pid_error;
  static float temp_iState_min;
  static float temp_iState_max;
  static bool pid_reset;
#endif //PIDTEMP
#ifdef PIDTEMPBED
  //static cannot be external:
  static float temp_iState_bed = { 0 };
  static float temp_dState_bed = { 0 };
  static float pTerm_bed;
  static float iTerm_bed;
  static float dTerm_bed;
  //int output;
  static float pid_error_bed;
  static float temp_iState_min_bed;
  static float temp_iState_max_bed;
#else //PIDTEMPBED
	static unsigned long  previous_millis_bed_heater;
#endif //PIDTEMPBED
  static unsigned char soft_pwm;
  static unsigned char soft_pwm_bed;
#ifdef FAN_SOFT_PWM
  static unsigned char soft_pwm_fan;
#endif
#if (defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN > -1) || \
    (defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN > -1) || \
    (defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN > -1)
  static unsigned long extruder_autofan_last_check;
#endif  

// Init min and max temp with extreme values to prevent false errors during startup
static int minttemp_raw = HEATER_0_RAW_LO_TEMP;
static int maxttemp_raw = HEATER_0_RAW_HI_TEMP;
static int minttemp = 0;
static int maxttemp = 16383;
//static int bed_minttemp_raw = HEATER_BED_RAW_LO_TEMP; /* No bed mintemp error implemented?!? */
#ifdef BED_MAXTEMP
static int bed_maxttemp_raw = HEATER_BED_RAW_HI_TEMP;
#endif

static void *heater_ttbl_map = (void *)HEATER_0_TEMPTABLE ;
static uint8_t heater_ttbllen_map = HEATER_0_TEMPTABLE_LEN ;

static float analog2temp(int raw);
static float analog2tempBed(int raw);
static void updateTemperaturesFromRawValues();

#ifdef WATCH_TEMP_PERIOD
int watch_start_temp[EXTRUDERS] = ARRAY_BY_EXTRUDERS(0,0,0);
unsigned long watchmillis[EXTRUDERS] = ARRAY_BY_EXTRUDERS(0,0,0);
#endif //WATCH_TEMP_PERIOD

#ifndef SOFT_PWM_SCALE
#define SOFT_PWM_SCALE 0
#endif

//===========================================================================
//=============================   functions      ============================
//===========================================================================

void PID_autotune(float temp, int extruder, int ncycles)
{
  float input = 0.0;
  int cycles=0;
  bool heating = true;

  unsigned long temp_millis = millis();
  unsigned long t1=temp_millis;
  unsigned long t2=temp_millis;
  long t_high = 0;
  long t_low = 0;

  long bias, d;
  float Ku, Tu;
  float Kp, Ki, Kd;
  float max = 0, min = 10000;

  if ((extruder > EXTRUDERS)
  #if (TEMP_BED_PIN <= -1)
       ||(extruder < 0)
  #endif
       ){
          SERIAL_ECHOLN("PID Autotune failed. Bad extruder number.");
          return;
        }
	
  SERIAL_ECHOLN("PID Autotune start");
  
  disable_heater(); // switch off all heaters.

  if (extruder<0)
  {
     soft_pwm_bed = (MAX_BED_POWER)/2;
     bias = d = (MAX_BED_POWER)/2;
   }
   else
   {
     soft_pwm = (PID_MAX)/2;
     bias = d = (PID_MAX)/2;
  }




 for(;;) {

    if(temp_meas_ready == true) { // temp sample ready
      updateTemperaturesFromRawValues();

      input = (extruder<0)?current_temperature_bed:current_temperature;

      max=max(max,input);
      min=min(min,input);
      if(heating == true && input > temp) {
        if(millis() - t2 > 5000) { 
          heating=false;
          if (extruder<0)
            soft_pwm_bed = (bias - d) >> 1;
          else
            soft_pwm = (bias - d) >> 1;
          t1=millis();
          t_high=t1 - t2;
          max=temp;
        }
      }
      if(heating == false && input < temp) {
        if(millis() - t1 > 5000) {
          heating=true;
          t2=millis();
          t_low=t2 - t1;
          if(cycles > 0) {
            bias += (d*(t_high - t_low))/(t_low + t_high);
            bias = constrain(bias, 20 ,(extruder<0?(MAX_BED_POWER):(PID_MAX))-20);
            if(bias > (extruder<0?(MAX_BED_POWER):(PID_MAX))/2) d = (extruder<0?(MAX_BED_POWER):(PID_MAX)) - 1 - bias;
            else d = bias;

            SERIAL_PROTOCOL_CPGM(" bias: "); SERIAL_PROTOCOL(bias);
            SERIAL_PROTOCOL_CPGM(" d: "); SERIAL_PROTOCOL(d);
            SERIAL_PROTOCOL_CPGM(" min: "); SERIAL_PROTOCOL(min);
            SERIAL_PROTOCOL_CPGM(" max: "); SERIAL_PROTOCOLLN(max);
            if(cycles > 2) {
              Ku = (4.0*d)/(3.14159*(max-min)/2.0);
              Tu = ((float)(t_low + t_high)/1000.0);
              SERIAL_PROTOCOL_CPGM(" Ku: "); SERIAL_PROTOCOL(Ku);
              SERIAL_PROTOCOL_CPGM(" Tu: "); SERIAL_PROTOCOLLN(Tu);
              Kp = 0.6*Ku;
              Ki = 2*Kp/Tu;
              Kd = Kp*Tu/8;
              SERIAL_PROTOCOLLN_CPGM(" Clasic PID ");
              SERIAL_PROTOCOL_CPGM(" Kp: "); SERIAL_PROTOCOLLN(Kp);
              SERIAL_PROTOCOL_CPGM(" Ki: "); SERIAL_PROTOCOLLN(Ki);
              SERIAL_PROTOCOL_CPGM(" Kd: "); SERIAL_PROTOCOLLN(Kd);
              /*
              Kp = 0.33*Ku;
              Ki = Kp/Tu;
              Kd = Kp*Tu/3;
              SERIAL_PROTOCOLLN_CPGM(" Some overshoot ")
              SERIAL_PROTOCOL_CPGM(" Kp: "); SERIAL_PROTOCOLLN(Kp);
              SERIAL_PROTOCOL_CPGM(" Ki: "); SERIAL_PROTOCOLLN(Ki);
              SERIAL_PROTOCOL_CPGM(" Kd: "); SERIAL_PROTOCOLLN(Kd);
              Kp = 0.2*Ku;
              Ki = 2*Kp/Tu;
              Kd = Kp*Tu/3;
              SERIAL_PROTOCOLLN_CPGM(" No overshoot ")
              SERIAL_PROTOCOL_CPGM(" Kp: "); SERIAL_PROTOCOLLN(Kp);
              SERIAL_PROTOCOL_CPGM(" Ki: "); SERIAL_PROTOCOLLN(Ki);
              SERIAL_PROTOCOL_CPGM(" Kd: "); SERIAL_PROTOCOLLN(Kd);
              */
            }
          }
          if (extruder<0)
            soft_pwm_bed = (bias + d) >> 1;
          else
            soft_pwm = (bias + d) >> 1;
          cycles++;
          min=temp;
        }
      } 
    }
    if(input > (temp + 20)) {
      SERIAL_PROTOCOLLN_CPGM("PID Autotune failed! Temperature too high");
      return;
    }
    if(millis() - temp_millis > 2000) {
      int p;
      if (extruder<0){
        p=soft_pwm_bed;       
        SERIAL_PROTOCOL_CPGM("ok B:");
      }else{
        p=soft_pwm;       
        SERIAL_PROTOCOL_CPGM("ok T:");
      }
			
      SERIAL_PROTOCOL(input);   
      SERIAL_PROTOCOL_CPGM(" @:");
      SERIAL_PROTOCOLLN(p);       

      temp_millis = millis();
    }
    if(((millis() - t1) + (millis() - t2)) > (10L*60L*1000L*2L)) {
      SERIAL_PROTOCOLLN_CPGM("PID Autotune failed! timeout");
      return;
    }
    if(cycles > ncycles) {
      SERIAL_PROTOCOLLN_CPGM("PID Autotune finished! Put the Kp, Ki and Kd constants into Configuration.h");
      return;
    }
    lcd_update();
  }
}

void updatePID()
{
#ifdef PIDTEMP
  temp_iState_max = PID_INTEGRAL_DRIVE_MAX / Ki;  
#endif
#ifdef PIDTEMPBED
  temp_iState_max_bed = PID_INTEGRAL_DRIVE_MAX / bedKi;  
#endif
}
  
int getHeaterPower(int heater) {
	if (heater<0)
		return soft_pwm_bed;
  return soft_pwm;
}

#if (defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN > -1) || \
    (defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN > -1) || \
    (defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN > -1)

  #if defined(FAN_PIN) && FAN_PIN > -1
    #if EXTRUDER_0_AUTO_FAN_PIN == FAN_PIN 
       #error "You cannot set EXTRUDER_0_AUTO_FAN_PIN equal to FAN_PIN"
    #endif
    #if EXTRUDER_1_AUTO_FAN_PIN == FAN_PIN 
       #error "You cannot set EXTRUDER_1_AUTO_FAN_PIN equal to FAN_PIN"
    #endif
    #if EXTRUDER_2_AUTO_FAN_PIN == FAN_PIN 
       #error "You cannot set EXTRUDER_2_AUTO_FAN_PIN equal to FAN_PIN"
    #endif
  #endif 

void setExtruderAutoFanState(int pin, bool state)
{
  unsigned char newFanSpeed = (state != 0) ? EXTRUDER_AUTO_FAN_SPEED : 0;
  // this idiom allows both digital and PWM fan outputs (see M42 handling).
  pinMode(pin, OUTPUT);
  digitalWrite(pin, newFanSpeed);
  analogWrite(pin, newFanSpeed);
}

void checkExtruderAutoFans()
{
  uint8_t fanState = 0;

  // which fan pins need to be turned on?      
  #if defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN > -1
    if (current_temperature[0] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
      fanState |= 1;
  #endif
  #if defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN > -1
    if (current_temperature[1] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
    {
      if (EXTRUDER_1_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN) 
        fanState |= 1;
      else
        fanState |= 2;
    }
  #endif
  #if defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN > -1
    if (current_temperature[2] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
    {
      if (EXTRUDER_2_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN) 
        fanState |= 1;
      else if (EXTRUDER_2_AUTO_FAN_PIN == EXTRUDER_1_AUTO_FAN_PIN) 
        fanState |= 2;
      else
        fanState |= 4;
    }
  #endif
  
  // update extruder auto fan states
  #if defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN > -1
    setExtruderAutoFanState(EXTRUDER_0_AUTO_FAN_PIN, (fanState & 1) != 0);
  #endif 
  #if defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN > -1
    if (EXTRUDER_1_AUTO_FAN_PIN != EXTRUDER_0_AUTO_FAN_PIN) 
      setExtruderAutoFanState(EXTRUDER_1_AUTO_FAN_PIN, (fanState & 2) != 0);
  #endif 
  #if defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN > -1
    if (EXTRUDER_2_AUTO_FAN_PIN != EXTRUDER_0_AUTO_FAN_PIN 
        && EXTRUDER_2_AUTO_FAN_PIN != EXTRUDER_1_AUTO_FAN_PIN)
      setExtruderAutoFanState(EXTRUDER_2_AUTO_FAN_PIN, (fanState & 4) != 0);
  #endif 
}

#endif // any extruder auto fan pins set
FORCE_INLINE void manage_extruder_heater()
{
#ifdef PIDTEMP
	pid_input = current_temperature;

#ifndef PID_OPENLOOP
	pid_error = target_temperature - pid_input;
	if (pid_error > PID_FUNCTIONAL_RANGE) {
		pid_output = BANG_MAX;
		pid_reset = true;
	}
	else if (pid_error < -PID_FUNCTIONAL_RANGE || target_temperature == 0) {
		pid_output = 0;
		pid_reset = true;
	}
	else {
		if (pid_reset == true) {
			temp_iState = 0.0;
			pid_reset = false;
		}
		pTerm = Kp * pid_error;
		temp_iState += pid_error;
		temp_iState = constrain(temp_iState, temp_iState_min, temp_iState_max);
		iTerm = Ki * temp_iState;

		//K1 defined in Configuration.h in the PID settings
#define K2 (1.0-K1)
		dTerm = (Kd * (pid_input - temp_dState))*K2 + (K1 * dTerm);
		pid_output = constrain(pTerm + iTerm - dTerm, 0, PID_MAX);
	}
	temp_dState = pid_input;
#else 
	pid_output = constrain(target_temperature[e], 0, PID_MAX);
#endif //PID_OPENLOOP
#ifdef PID_DEBUG
	SERIAL_ECHO_START(" PIDDEBUG ");
	SERIAL_ECHO(e);
	SERIAL_ECHO(": Input ");
	SERIAL_ECHO(pid_input);
	SERIAL_ECHO(" Output ");
	SERIAL_ECHO(pid_output);
	SERIAL_ECHO(" pTerm ");
	SERIAL_ECHO(pTerm[e]);
	SERIAL_ECHO(" iTerm ");
	SERIAL_ECHO(iTerm[e]);
	SERIAL_ECHO(" dTerm ");
	SERIAL_ECHOLN(dTerm[e]);
#endif //PID_DEBUG
#else /* PID off */
	pid_output = 0;
	if (current_temperature < target_temperature) {
		pid_output = PID_MAX;
	}
#endif

	// Check if temperature is within the correct range
	if ((current_temperature > minttemp) && (current_temperature < maxttemp))
	{
		soft_pwm = (int)pid_output >> 1;
	}
	else {
		soft_pwm = 0;
	}

#ifdef WATCH_TEMP_PERIOD
	if (watchmillis[e] && millis() - watchmillis[e] > WATCH_TEMP_PERIOD)
	{
		if (degHotend(e) < watch_start_temp[e] + WATCH_TEMP_INCREASE)
		{
			setTargetHotend(0, e);
			LCD_MESSAGE_CPGM("Heating failed");
			SERIAL_ECHO_START;
			SERIAL_ECHOLN("Heating failed");
		}
		else {
			watchmillis[e] = 0;
		}
	}
#endif

#if (defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN > -1) || \
      (defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN > -1) || \
      (defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN > -1)
	if (millis() - extruder_autofan_last_check > 2500)  // only need to check fan state very infrequently
	{
		checkExtruderAutoFans();
		extruder_autofan_last_check = millis();
	}
#endif       
}

FORCE_INLINE void manage_bed_heater()
{
#ifndef PIDTEMPBED
	if (millis() - previous_millis_bed_heater < BED_CHECK_INTERVAL)
		return;

	previous_millis_bed_heater = millis();
#endif

#if TEMP_SENSOR_BED != 0

#ifdef PIDTEMPBED
	pid_input = current_temperature_bed;

#ifndef PID_OPENLOOP
	pid_error_bed = target_temperature_bed - pid_input;
	pTerm_bed = bedKp * pid_error_bed;
	temp_iState_bed += pid_error_bed;
	temp_iState_bed = constrain(temp_iState_bed, temp_iState_min_bed, temp_iState_max_bed);
	iTerm_bed = bedKi * temp_iState_bed;

	//K1 defined in Configuration.h in the PID settings
#define K2 (1.0-K1)
	dTerm_bed = (bedKd * (pid_input - temp_dState_bed))*K2 + (K1 * dTerm_bed);
	temp_dState_bed = pid_input;

	pid_output = constrain(pTerm_bed + iTerm_bed - dTerm_bed, 0, MAX_BED_POWER);

#else 
	pid_output = constrain(target_temperature_bed, 0, MAX_BED_POWER);
#endif //PID_OPENLOOP

	if ((current_temperature_bed > BED_MINTEMP) && (current_temperature_bed < BED_MAXTEMP))
	{
		soft_pwm_bed = (int)pid_output >> 1;
	}
	else {
		soft_pwm_bed = 0;
	}

#elif !defined(BED_LIMIT_SWITCHING)
	// Check if temperature is within the correct range
	if ((current_temperature_bed > BED_MINTEMP) && (current_temperature_bed < BED_MAXTEMP))
	{
		if (current_temperature_bed >= target_temperature_bed)
		{
			soft_pwm_bed = 0;
		}
		else
		{
			soft_pwm_bed = MAX_BED_POWER >> 1;
		}
	}
	else
	{
		soft_pwm_bed = 0;
		WRITE(HEATER_BED_PIN, LOW);
	}
#else //#ifdef BED_LIMIT_SWITCHING
	// Check if temperature is within the correct band
	if ((current_temperature_bed > BED_MINTEMP) && (current_temperature_bed < BED_MAXTEMP))
	{
		if (current_temperature_bed > target_temperature_bed + BED_HYSTERESIS)
		{
			soft_pwm_bed = 0;
		}
		else if (current_temperature_bed <= target_temperature_bed - BED_HYSTERESIS)
		{
			soft_pwm_bed = MAX_BED_POWER >> 1;
		}
	}
	else
	{
		soft_pwm_bed = 0;
		WRITE(HEATER_BED_PIN, LOW);
	}
#endif
#endif
}

void manage_heater()
{
  if(temp_meas_ready != true)   //better readability
    return; 

  updateTemperaturesFromRawValues();
  manage_extruder_heater();
  manage_bed_heater();
}

#define PGM_RD_W(x)   (short)pgm_read_word(&x)
// Derived from RepRap FiveD extruder::getTemperature()
// For hot end temperature measurement.
static float analog2temp(int raw) {
  #ifdef HEATER_0_USES_MAX6675
    return 0.25 * raw;
  #endif

  if(heater_ttbl_map != NULL)
  {
    float celsius = 0;
    uint8_t i;
    short (*tt)[][2] = (short (*)[][2])(heater_ttbl_map);

    for (i=1; i<heater_ttbllen_map; i++)
    {
      if (PGM_RD_W((*tt)[i][0]) > raw)
      {
        celsius = PGM_RD_W((*tt)[i-1][1]) + 
          (raw - PGM_RD_W((*tt)[i-1][0])) * 
          (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i-1][1])) /
          (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i-1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == heater_ttbllen_map) celsius = PGM_RD_W((*tt)[i-1][1]);

    return celsius;
  }
  return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET;
}

// Derived from RepRap FiveD extruder::getTemperature()
// For bed temperature measurement.
static float analog2tempBed(int raw) {
  #ifdef BED_USES_THERMISTOR
    float celsius = 0;
    byte i;

    for (i=1; i<BEDTEMPTABLE_LEN; i++)
    {
      if (PGM_RD_W(BEDTEMPTABLE[i][0]) > raw)
      {
        celsius  = PGM_RD_W(BEDTEMPTABLE[i-1][1]) + 
          (raw - PGM_RD_W(BEDTEMPTABLE[i-1][0])) * 
          (float)(PGM_RD_W(BEDTEMPTABLE[i][1]) - PGM_RD_W(BEDTEMPTABLE[i-1][1])) /
          (float)(PGM_RD_W(BEDTEMPTABLE[i][0]) - PGM_RD_W(BEDTEMPTABLE[i-1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == BEDTEMPTABLE_LEN) celsius = PGM_RD_W(BEDTEMPTABLE[i-1][1]);

    return celsius;
  #elif defined BED_USES_AD595
    return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET;
  #else
    return 0;
  #endif
}

/* Called to get the raw values into the the actual temperatures. The raw values are created in interrupt context,
    and this function is called from normal context as it is too slow to run in interrupts and will block the stepper routine otherwise */
static void updateTemperaturesFromRawValues()
{
    current_temperature = analog2temp(current_temperature_raw);
    current_temperature_bed = analog2tempBed(current_temperature_bed_raw);
    //Reset the watchdog after we know we have a temperature measurement.
    watchdog_reset();

    CRITICAL_SECTION_START;
    temp_meas_ready = false;
    CRITICAL_SECTION_END;
}

void tp_init()
{
#if (MOTHERBOARD == 80) && ((TEMP_SENSOR_0==-1)||(TEMP_SENSOR_1==-1)||(TEMP_SENSOR_2==-1)||(TEMP_SENSOR_BED==-1))
  //disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
  MCUCR=(1<<JTD); 
  MCUCR=(1<<JTD);
#endif
  
  // Finish init of mult extruder arrays 
  // populate with the first value 
#ifdef PIDTEMP
  temp_iState_min = 0.0;
  temp_iState_max = PID_INTEGRAL_DRIVE_MAX / Ki;
#endif //PIDTEMP
#ifdef PIDTEMPBED
  temp_iState_min_bed = 0.0;
  temp_iState_max_bed = PID_INTEGRAL_DRIVE_MAX / bedKi;
#endif //PIDTEMPBED

  #if defined(HEATER_0_PIN) && (HEATER_0_PIN > -1) 
    SET_OUTPUT(HEATER_0_PIN);
  #endif  
  #if defined(HEATER_1_PIN) && (HEATER_1_PIN > -1) 
    SET_OUTPUT(HEATER_1_PIN);
  #endif  
  #if defined(HEATER_2_PIN) && (HEATER_2_PIN > -1) 
    SET_OUTPUT(HEATER_2_PIN);
  #endif  
  #if defined(HEATER_BED_PIN) && (HEATER_BED_PIN > -1) 
    SET_OUTPUT(HEATER_BED_PIN);
  #endif  
  #if defined(FAN_PIN) && (FAN_PIN > -1) 
    SET_OUTPUT(FAN_PIN);
    #ifdef FAST_PWM_FAN
    setPwmFrequency(FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
    #ifdef FAN_SOFT_PWM
    soft_pwm_fan = fanSpeedSoftPwm / 2;
    #endif
  #endif  

  #ifdef HEATER_0_USES_MAX6675
    #ifndef SDSUPPORT
      SET_OUTPUT(MAX_SCK_PIN);
      WRITE(MAX_SCK_PIN,0);
    
      SET_OUTPUT(MAX_MOSI_PIN);
      WRITE(MAX_MOSI_PIN,1);
    
      SET_INPUT(MAX_MISO_PIN);
      WRITE(MAX_MISO_PIN,1);
    #endif
    
    SET_OUTPUT(MAX6675_SS);
    WRITE(MAX6675_SS,1);
  #endif

  // Set analog inputs
  ADCSRA = 1<<ADEN | 1<<ADSC | 1<<ADIF | 0x07;
  DIDR0 = 0;
  #ifdef DIDR2
    DIDR2 = 0;
  #endif
  #if defined(TEMP_0_PIN) && (TEMP_0_PIN > -1)
    #if TEMP_0_PIN < 8
       DIDR0 |= 1 << TEMP_0_PIN; 
    #else
       DIDR2 |= 1<<(TEMP_0_PIN - 8); 
    #endif
  #endif
  #if defined(TEMP_1_PIN) && (TEMP_1_PIN > -1)
    #if TEMP_1_PIN < 8
       DIDR0 |= 1<<TEMP_1_PIN; 
    #else
       DIDR2 |= 1<<(TEMP_1_PIN - 8); 
    #endif
  #endif
  #if defined(TEMP_2_PIN) && (TEMP_2_PIN > -1)
    #if TEMP_2_PIN < 8
       DIDR0 |= 1 << TEMP_2_PIN; 
    #else
       DIDR2 |= 1<<(TEMP_2_PIN - 8); 
    #endif
  #endif
  #if defined(TEMP_BED_PIN) && (TEMP_BED_PIN > -1)
    #if TEMP_BED_PIN < 8
       DIDR0 |= 1<<TEMP_BED_PIN; 
    #else
       DIDR2 |= 1<<(TEMP_BED_PIN - 8); 
    #endif
  #endif
  
  // Use timer0 for temperature measurement
  // Interleave temperature interrupt with millies interrupt
  OCR0B = 128;
  TIMSK0 |= (1<<OCIE0B);  
  
  // Wait for temperature measurement to settle
  delay(250);

#ifdef HEATER_0_MINTEMP
  minttemp = HEATER_0_MINTEMP;
  while(analog2temp(minttemp_raw) < HEATER_0_MINTEMP) {
#if HEATER_0_RAW_LO_TEMP < HEATER_0_RAW_HI_TEMP
    minttemp_raw[0] += OVERSAMPLENR;
#else
    minttemp_raw -= OVERSAMPLENR;
#endif
  }
#endif //MINTEMP
#ifdef HEATER_0_MAXTEMP
  maxttemp = HEATER_0_MAXTEMP;
  while(analog2temp(maxttemp_raw) > HEATER_0_MAXTEMP) {
#if HEATER_0_RAW_LO_TEMP < HEATER_0_RAW_HI_TEMP
    maxttemp_raw -= OVERSAMPLENR;
#else
    maxttemp_raw += OVERSAMPLENR;
#endif
  }
#endif //MAXTEMP

#ifdef BED_MINTEMP
  /* No bed MINTEMP error implemented?!? */ /*
  while(analog2tempBed(bed_minttemp_raw) < BED_MINTEMP) {
#if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
    bed_minttemp_raw += OVERSAMPLENR;
#else
    bed_minttemp_raw -= OVERSAMPLENR;
#endif
  }
  */
#endif //BED_MINTEMP
#ifdef BED_MAXTEMP
  while(analog2tempBed(bed_maxttemp_raw) > BED_MAXTEMP) {
#if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
    bed_maxttemp_raw -= OVERSAMPLENR;
#else
    bed_maxttemp_raw += OVERSAMPLENR;
#endif
  }
#endif //BED_MAXTEMP
}

void setWatch() 
{  
#ifdef WATCH_TEMP_PERIOD
  for (int e = 0; e < EXTRUDERS; e++)
  {
    if(degHotend(e) < degTargetHotend(e) - (WATCH_TEMP_INCREASE * 2))
    {
      watch_start_temp[e] = degHotend(e);
      watchmillis[e] = millis();
    } 
  }
#endif 
}


void disable_heater()
{
  setTargetHotend(0);
  setTargetBed(0);
  #if defined(TEMP_0_PIN) && TEMP_0_PIN > -1
   target_temperature=0;
   soft_pwm=0;
   #if defined(HEATER_0_PIN) && HEATER_0_PIN > -1  
     WRITE(HEATER_0_PIN,LOW);
   #endif
  #endif
     
  #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
    target_temperature_bed=0;
    soft_pwm_bed=0;
    #if defined(HEATER_BED_PIN) && HEATER_BED_PIN > -1  
      WRITE(HEATER_BED_PIN,LOW);
    #endif
  #endif 
}

void max_temp_error(uint8_t e) {
  disable_heater();
  if(IsStopped() == false) {
    SERIAL_ERROR_START;
    SERIAL_ERRORLN((int)e);
    SERIAL_ERRORLN_CPGM(": Extruder switched off. MAXTEMP triggered !");
    LCD_ALERTMESSAGE_CPGM("Err: MAXTEMP");
  }
  #ifndef BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE
  Stop();
  #endif
}

void min_temp_error(uint8_t e) {
  disable_heater();
  if(IsStopped() == false) {
    SERIAL_ERROR_START;
    SERIAL_ERRORLN((int)e);
    SERIAL_ERRORLN_CPGM(": Extruder switched off. MINTEMP triggered !");
    LCD_ALERTMESSAGE_CPGM("Err: MINTEMP");
  }
  #ifndef BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE
  Stop();
  #endif
}

void bed_max_temp_error(void) {
#if HEATER_BED_PIN > -1
  WRITE(HEATER_BED_PIN, 0);
#endif
  if(IsStopped() == false) {
    SERIAL_ERROR_START;
    SERIAL_ERRORLN_CPGM("Temperature heated bed switched off. MAXTEMP triggered !!");
    LCD_ALERTMESSAGE_CPGM("Err: MAXTEMP BED");
  }
  #ifndef BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE
  Stop();
  #endif
}

#ifdef HEATER_0_USES_MAX6675
#define MAX6675_HEAT_INTERVAL 250
long max6675_previous_millis = -HEAT_INTERVAL;
int max6675_temp = 2000;

int read_max6675()
{
  if (millis() - max6675_previous_millis < MAX6675_HEAT_INTERVAL) 
    return max6675_temp;
  
  max6675_previous_millis = millis();
  max6675_temp = 0;
    
  #ifdef	PRR
    PRR &= ~(1<<PRSPI);
  #elif defined PRR0
    PRR0 &= ~(1<<PRSPI);
  #endif
  
  SPCR = (1<<MSTR) | (1<<SPE) | (1<<SPR0);
  
  // enable TT_MAX6675
  WRITE(MAX6675_SS, 0);
  
  // ensure 100ns delay - a bit extra is fine
  asm("nop");//50ns on 20Mhz, 62.5ns on 16Mhz
  asm("nop");//50ns on 20Mhz, 62.5ns on 16Mhz
  
  // read MSB
  SPDR = 0;
  for (;(SPSR & (1<<SPIF)) == 0;);
  max6675_temp = SPDR;
  max6675_temp <<= 8;
  
  // read LSB
  SPDR = 0;
  for (;(SPSR & (1<<SPIF)) == 0;);
  max6675_temp |= SPDR;
  
  // disable TT_MAX6675
  WRITE(MAX6675_SS, 1);

  if (max6675_temp & 4) 
  {
    // thermocouple open
    max6675_temp = 2000;
  }
  else 
  {
    max6675_temp = max6675_temp >> 3;
  }

  return max6675_temp;
}
#endif


// Timer 0 is shared with millies
ISR(TIMER0_COMPB_vect)
{
  //these variables are only accesible from the ISR, but static, so they don't lose their value
  static unsigned char temp_count = 0;
  static unsigned long raw_temp_0_value = 0;
  static unsigned long raw_temp_bed_value = 0;
  static unsigned char temp_state = 0;
  static unsigned char pwm_count = (1 << SOFT_PWM_SCALE);
  static unsigned char soft_pwm_0;
  #if HEATER_BED_PIN > -1
  static unsigned char soft_pwm_b;
  #endif
  
  if(pwm_count == 0){
    soft_pwm_0 = soft_pwm;
    if(soft_pwm_0 > 0) WRITE(HEATER_0_PIN,1);
    #if defined(HEATER_BED_PIN) && HEATER_BED_PIN > -1
    soft_pwm_b = soft_pwm_bed;
    if(soft_pwm_b > 0) WRITE(HEATER_BED_PIN,1);
    #endif
    #ifdef FAN_SOFT_PWM
    soft_pwm_fan = fanSpeedSoftPwm / 2;
    if(soft_pwm_fan > 0) WRITE(FAN_PIN,1);
    #endif
  }
  if(soft_pwm_0 <= pwm_count) WRITE(HEATER_0_PIN,0);
  #if defined(HEATER_BED_PIN) && HEATER_BED_PIN > -1
  if(soft_pwm_b <= pwm_count) WRITE(HEATER_BED_PIN,0);
  #endif
  #ifdef FAN_SOFT_PWM
  if(soft_pwm_fan <= pwm_count) WRITE(FAN_PIN,0);
  #endif
  
  pwm_count += (1 << SOFT_PWM_SCALE);
  pwm_count &= 0x7f;
  
  switch(temp_state) {
    case 0: // Prepare TEMP_0
      #if defined(TEMP_0_PIN) && (TEMP_0_PIN > -1)
        #if TEMP_0_PIN > 7
          ADCSRB = 1<<MUX5;
        #else
          ADCSRB = 0;
        #endif
        ADMUX = ((1 << REFS0) | (TEMP_0_PIN & 0x07));
        ADCSRA |= 1<<ADSC; // Start conversion
      #endif
      lcd_buttons_update();
      temp_state = 1;
      break;
    case 1: // Measure TEMP_0
      #if defined(TEMP_0_PIN) && (TEMP_0_PIN > -1)
        raw_temp_0_value += ADC;
      #endif
      #ifdef HEATER_0_USES_MAX6675 // TODO remove the blocking
        raw_temp_0_value = read_max6675();
      #endif
      temp_state = 2;
      break;
    case 2: // Prepare TEMP_BED
      #if defined(TEMP_BED_PIN) && (TEMP_BED_PIN > -1)
        #if TEMP_BED_PIN > 7
          ADCSRB = 1<<MUX5;
        #else
          ADCSRB = 0;
        #endif
        ADMUX = ((1 << REFS0) | (TEMP_BED_PIN & 0x07));
        ADCSRA |= 1<<ADSC; // Start conversion
      #endif
      lcd_buttons_update();
      temp_state = 3;
      break;
    case 3: // Measure TEMP_BED
      #if defined(TEMP_BED_PIN) && (TEMP_BED_PIN > -1)
        raw_temp_bed_value += ADC;
      #endif
      temp_state = 4;
      break;
    case 4: // Prepare TEMP_1
      #if defined(TEMP_1_PIN) && (TEMP_1_PIN > -1)
        #if TEMP_1_PIN > 7
          ADCSRB = 1<<MUX5;
        #else
          ADCSRB = 0;
        #endif
        ADMUX = ((1 << REFS0) | (TEMP_1_PIN & 0x07));
        ADCSRA |= 1<<ADSC; // Start conversion
      #endif
      lcd_buttons_update();
      temp_state = 5;
      break;
    case 5: // Measure TEMP_1
      #if defined(TEMP_1_PIN) && (TEMP_1_PIN > -1)
        raw_temp_1_value += ADC;
      #endif
      temp_state = 6;
      break;
    case 6: // Prepare TEMP_2
      #if defined(TEMP_2_PIN) && (TEMP_2_PIN > -1)
        #if TEMP_2_PIN > 7
          ADCSRB = 1<<MUX5;
        #else
          ADCSRB = 0;
        #endif
        ADMUX = ((1 << REFS0) | (TEMP_2_PIN & 0x07));
        ADCSRA |= 1<<ADSC; // Start conversion
      #endif
      lcd_buttons_update();
      temp_state = 7;
      break;
    case 7: // Measure TEMP_2
      #if defined(TEMP_2_PIN) && (TEMP_2_PIN > -1)
        raw_temp_2_value += ADC;
      #endif
      temp_state = 0;
      temp_count++;
      break;
  }
    
  if(temp_count >= 16) // 8 ms * 16 = 128ms.
  {
    if (!temp_meas_ready) //Only update the raw values if they have been read. Else we could be updating them during reading.
    {
      if (!last_update_temperature || millis() - last_update_temperature > 1000 || raw_temp_0_value < minttemp_raw)
      {
        last_update_temperature = millis();
        last_temperature_raw = current_temperature_raw;
        current_temperature_raw = raw_temp_0_value;
      }
      current_temperature_bed_raw = raw_temp_bed_value;
    }
    
    temp_meas_ready = true;
    temp_count = 0;
    raw_temp_0_value = 0;
    raw_temp_bed_value = 0;

#if HEATER_0_RAW_LO_TEMP > HEATER_0_RAW_HI_TEMP
    if(current_temperature_raw <= maxttemp_raw) {
#else
    if(current_temperature_raw >= maxttemp_raw) {
#endif
        max_temp_error(0);
    }
#if HEATER_0_RAW_LO_TEMP > HEATER_0_RAW_HI_TEMP
    if(current_temperature_raw >= minttemp_raw) {
#else
    if(current_temperature_raw <= minttemp_raw) {
#endif
        min_temp_error(0);
    }
  
  /* No bed MINTEMP error? */
#if defined(BED_MAXTEMP) && (TEMP_SENSOR_BED != 0)
# if HEATER_BED_RAW_LO_TEMP > HEATER_BED_RAW_HI_TEMP
    if(current_temperature_bed_raw <= bed_maxttemp_raw) {
#else
    if(current_temperature_bed_raw >= bed_maxttemp_raw) {
#endif
       target_temperature_bed = 0;
       bed_max_temp_error();
    }
#endif
  }  
}

#ifdef PIDTEMP
// Apply the scale factors to the PID values


float scalePID_i(float i)
{
	return i*PID_dT;
}

float unscalePID_i(float i)
{
	return i/PID_dT;
}

float scalePID_d(float d)
{
    return d/PID_dT;
}

float unscalePID_d(float d)
{
	return d*PID_dT;
}

#endif //PIDTEMP


