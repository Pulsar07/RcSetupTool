#ifndef ServoController_h
#define ServoController_h

#include <WString.h>

#define CONFIG_VERSION "RSV2"
#define CONFIG_VERSION_L 5
#define CONFIG_SSID_L 16
#define CONFIG_PASSW_L 64
#define CONFIG_SERVO_PRESET_L 5
#define CONFIG_SERVO_PRESET_MAX 3 

#define MIN_IDX 0
#define MAX_IDX 1

typedef enum {
  xAxis,
  yAxis,
  zAxis,
} referenceAxis_t;

typedef enum {
  P001,
  P010,
  P050,
  P100,
} precision_t;

typedef enum {
  ARC,
  CHORD,
  VERTICAL_DISTANCE,
} amplitude_calc_method_t;

typedef enum {
  jeti,   
  futaba,
  hott,    
  spektrum,
  taranis, 
  multiplex,
  custom
} rc_vendor_t;

// Types 'byte' und 'word' doesn't work!
typedef struct {
  char version[CONFIG_VERSION_L];
  char wlanSsid[CONFIG_SSID_L];
  char wlanPasswd[CONFIG_PASSW_L];
  char apSsid[CONFIG_SSID_L];
  char apPasswd[CONFIG_PASSW_L];
  boolean apIsActive;
  referenceAxis_t axis;
  int angleInversion;
  precision_t anglePrecision;
  precision_t amplitudePrecision;
  int amplitudeInversion;
  amplitude_calc_method_t amplitudeCalcMethod;
  int16_t xAccelOffset;
  int16_t yAccelOffset;
  int16_t zAccelOffset;
  int16_t xGyroOffset;
  int16_t yGyroOffset;
  int16_t zGyroOffset;
  int16_t calibrationOffsetHigh;
  int16_t calibrationOffsetLow;
  boolean calibrationOffsetEnabled;
  boolean servoInversion;
  rc_vendor_t servoRangeByVendor;
  int servoPresets[CONFIG_SERVO_PRESET_L];
  int servoPulseWidthPairFullRange[2];
  int servoPulseWidthPair100Percent[2];
} configData_t;

#endif
