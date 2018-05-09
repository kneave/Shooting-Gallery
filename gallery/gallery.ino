//  NERF Shooting Gallery, Keegan Neave 2018

#include <QTRSensors.h>
#include <FastLED.h>
#include <Adafruit_PWMServoDriver.h>

//  Game states
enum GAME_STATE {
  Playing,
  MaxScore,
  Attract,
  Setting,
  Resetting
};

enum SETTING_LIMITS {
  Green,
  Yellow,
  Blue,
  Red
};

GAME_STATE game_state = Setting;
SETTING_LIMITS setting_limits = Green;
int score = 0;
int scores[5] = {5, 10, 25, 10, 5};

//  Timing Variables
long counter_led_millis = 0;
bool counter_led_on;

//  Buttons
#define RESET_BUTTON  41
#define SET_BUTTON    43
#define UP_BUTTON     45
#define DOWN_BUTTON   47
#define RESET_LED     37
bool reset_led_on;
long reset_button_pressed = 0;
long set_button_pressed = 0;
long up_button_pressed = 0;
long down_button_pressed = 0;

int button_pressed_debounce = 250;

// Servo variables

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//  These are backwards because of the way the motor is mounted
#define COUNTER_MIN   448
#define COUNTER_MAX   180

#define TARGET_DOWN   200 
#define TARGET_UP     495 

//  Sensor variables
#define LeftFiveSen      0
#define LeftTenSen       1
#define TwentyFiveSen    2
#define RightTenSen      3
#define RightFiveSen     4

bool prev_values[5];
int day_sensor_levels[5] = {60, 30, 50, 70, 5};
int night_sensor_levels[5] = {100, 100, 100, 180, 100};
int sensor_levels[5] = {};
bool daytime = true;


#define NUM_SENSORS             5  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             42  // emitter is controlled by digital pin 2

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {8, 9, 10, 11, 12}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

//  LED variables
#define LED_PIN     32
#define NUM_LEDS    6
#define BRIGHTNESS  95
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define CounterLED       0
#define LeftFiveLED      1
#define LeftTenLED       2
#define TwentyFiveLED    3
#define RightTenLED      4
#define RightFiveLED     5

long counter_led_prev = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
bool attract_leds_set = false;

#define UPDATES_PER_SECOND 50

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
  //  Serial setup
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission
  Serial.println("Gallery Comms Open...");
  Serial.println("Configuring buttons...");

  //  Button setup
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(SET_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(RESET_LED, OUTPUT);
  reset_led_on = false;
  
  //  LED setup
  Serial.println("Setting up LEDs...");
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  counter_led_on = true;
    
  //  Servo setup
  Serial.println("Setting up servos...");
  pwm.begin();
  pwm.setPWMFreq(60);

  ResetAllTargets();
   
  Serial.println("Gallery To Attract Mode...");
  game_state = Attract;

  SetAllLEDs(CRGB::Purple);

  for(int i = 0; i < 5; i++)
  {
    prev_values[i] = false;
  }

  daytime = true;
  SwitchSensorLevels(day_sensor_levels);
  
  Serial.println("Gallery Online.");
}

void loop() 
{    

  DebugSensors();

  if(digitalRead(SET_BUTTON) == 0 & SetButtonBounce())
  {
    set_button_pressed = millis();
    ResetAllTargets();
  }

  switch(game_state)
  {
    case Playing:
      CheckTargets(true);
      UpdateCounter();
      
      if(digitalRead(RESET_BUTTON) == 0 & ResetButtonBounce())
      {
        reset_button_pressed = millis();
        game_state = Attract;
      }
      break;

    case MaxScore:
    if(digitalRead(RESET_BUTTON) == 0 & ResetButtonBounce())
      {
        reset_button_pressed = millis();
        game_state = Attract;
      }
      
      if(millis() - counter_led_prev > 250)
      {
        counter_led_prev = millis();
        if(counter_led_on)
        {
          counter_led_on = false;
          leds[CounterLED] = CRGB::Black;
        }
        else
        {
          counter_led_on = true;
          leds[CounterLED] = CRGB::Red;
        }
        FastLED.show();
      }
      break;
    case Attract:
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
      
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */
      
      FillLEDsFromPaletteColors( startIndex);
      FastLED.show();
      FastLED.delay(2000 / UPDATES_PER_SECOND);
    
//      if(digitalRead(SET_BUTTON) == 0 & SetButtonBounce())
//      {
//        game_state = Setting;
//        setting_limits = Green;
//        set_button_pressed = millis();
//      }

      CheckTargets(false);
      
      if(digitalRead(RESET_BUTTON) == 0 & ResetButtonBounce())
      {
        reset_button_pressed = millis();
        game_state = Resetting;
      }

      if(digitalRead(DOWN_BUTTON) == 0 & DownButtonBounce())
      {
        down_button_pressed = millis();        
        daytime = !daytime;

        if(daytime == true)
        {
          SwitchSensorLevels(day_sensor_levels);
          FlashCounter(CRGB::Yellow);
        }
        else
        {  
          SwitchSensorLevels(night_sensor_levels); 
          FlashCounter(CRGB::Purple);
        } 
      }
     break;
     
    case Setting:
      //  Set button pressed, switch to next limit
      if(digitalRead(SET_BUTTON) == 0 & SetButtonBounce())
      {
        set_button_pressed = millis();
        if(setting_limits == Red)
        {
          game_state = Attract;   
          Serial.println("Red setting done, reset to Attract...");
          loop();
        }
        else
        {
          IncrementSettingLimit();
        }
      }
    
      if(millis() - counter_led_millis >= 500)
        {      
          counter_led_millis = millis();
            
          if(counter_led_on == true)
          {
            leds[CounterLED] = CRGB::Black;       
          }
          else
          {
            switch(setting_limits)
            {
              case Green:
                leds[CounterLED] = CRGB::Green; 
                break;  
              case Yellow:
                leds[CounterLED] = CRGB::Yellow; 
                break;  
              case Blue:
                leds[CounterLED] = CRGB::Blue; 
                break;  
              case Red:
                leds[CounterLED] = CRGB::Red; 
                break;  
            }      
          }
    
          counter_led_on = !counter_led_on;
          FastLED.show();
        }
      break;
      
    case Resetting:
      score = 0;
      UpdateCounter();
      ResetAllTargets();
      FastLED.show();
      game_state = Playing;
      Serial.println("Game entering play mode...");
      break;      
  }
  
//  NOTE:  The resistor I used makes the LED too weak to see so commented out for now
  ToggleResetLed();
}

//  Gameplay Methods

void SwitchSensorLevels(int newLimits[])
{
  String str = "New levels: {";
  for(int i = 0; i < 5; i++)
  {
    sensor_levels[i] = newLimits[i];
    str += String(sensor_levels[i]) + "\t";
  }

  str += '}';
  Serial.println(str);  
}

void CheckTargets(bool TrackScore)
{
  qtra.read(sensorValues);

  if(score < 100){
    HasScored(LeftFiveSen);
    HasScored(LeftTenSen);
    HasScored(TwentyFiveSen); 
    HasScored(RightFiveSen);
    HasScored(RightTenSen);
  } 
  else
  {
    if(TrackScore)
    {
      game_state = MaxScore;
      SetAllLEDs(CRGB::Red); 
    }
    else
    {
      score = 0;
    }
  }
  FastLED.show();
}

void ResetTarget(int target)
{
  //  Let the targets come to rest
  delay(500);
  pwm.setPWM(target + 1, 0, TARGET_UP);
  delay(500);
  pwm.setPWM(target + 1, 0, TARGET_DOWN);
  delay(500);
}

void HasScored(int currentSen)
{
  bool currentSenValue = sensorValues[currentSen] > sensor_levels[currentSen] ? true : false;
    
  if(currentSenValue != prev_values[currentSen])
  {    
    prev_values[currentSen] = currentSenValue;
    
    if(currentSenValue == true)
    {
      //  ie, just knocked down 
      //  Need array of ints with scores
      score = score + scores[currentSen] < 100 ? score + scores[currentSen] : 100;
      leds[currentSen + 1] = CRGB::Red;
      FastLED.show();
          
      Serial.print("Target hit: ");
      Serial.print(scores[currentSen]);
      Serial.print("\tSensor Value:\t");
      Serial.print(sensorValues[currentSen]);
      Serial.print("\tSensor Limit:\t");
      Serial.println(sensor_levels[currentSen]);

      ResetTarget(currentSen);  
    }
    else
    {
      //  Just reset
      leds[currentSen + 1] = CRGB::Green;
    }
  }
}

void UpdateCounter()
{
  //  Counter is 0..100 so set a fraction thereof
  //  Showing my working rather than just magic numbers
  //  #define COUNTER_MIN  450
  //  #define COUNTER_MAX  180
  //  diff = 450 - 180 = 270
  //  1 point = 2
  int point_servo_offset = round(2.7 * score);
  int point_servo_value = COUNTER_MIN - point_servo_offset;
  pwm.setPWM(0, 0, point_servo_value);

  //  If score less than five, stay green

  if(score >= 5) leds[CounterLED] = CRGB::White;
  if(score >= 35) leds[CounterLED] = CRGB::Yellow;      
  if(score >= 50) leds[CounterLED] = CRGB::Blue;    
  if(score >= 70) leds[CounterLED] = CRGB::Red;    

  FastLED.show();
}

void PrintScore()
{
    Serial.print("Score: ");
    Serial.println(score);
}

void ResetAllTargets()
{
  pwm.setPWM(0, 0, COUNTER_MIN);
  leds[CounterLED] = CRGB::Green;
  FastLED.show();
  delay(500);

  for(int i = 1; i < 6; i++)
  {
    leds[i] = CRGB::Red;
    FastLED.show();
    pwm.setPWM(i, 0, TARGET_UP);
    delay(500);
    pwm.setPWM(i, 0, TARGET_DOWN);
    
    leds[i] = CRGB::Green;
    FastLED.show();
  }
}

void FlashCounter(CRGB colour)
{
  for(int i = 0; i < 5; i++)
  {
    leds[CounterLED] = colour;
    FastLED.show();
    delay(500);
    leds[CounterLED] = CRGB::Black;
    FastLED.show();
    delay(500);
  }
}

//  Enum Methods

void IncrementSettingLimit()
{
  switch(setting_limits)
  {
    case Green:
      setting_limits = Yellow;
      break;    
    case Yellow:
      setting_limits = Blue;
      break;    
    case Blue:
      setting_limits = Red;
      break;    
    case Red:
      setting_limits = Green;
      break;    
  }
}

//  Button methods

bool SetButtonBounce()
{
  return (millis() - set_button_pressed > button_pressed_debounce);
}

bool ResetButtonBounce()
{
  return (millis() - reset_button_pressed > button_pressed_debounce);
}
bool UpButtonBounce()
{
  return (millis() - up_button_pressed > button_pressed_debounce);
}
bool DownButtonBounce()
{
  return (millis() - down_button_pressed > button_pressed_debounce);
}

void ToggleResetLed()
{
  if(reset_led_on)
  {
    digitalWrite(RESET_LED, LOW);
    reset_led_on = false;
  }
  else
  {
    digitalWrite(RESET_LED, HIGH);
    reset_led_on = true;
  }
}

void DebugButtons()
{
  Serial.print("Reset: ");
  Serial.print(digitalRead(RESET_BUTTON));
  Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  Serial.print("Set: ");
  Serial.print(digitalRead(SET_BUTTON));
  Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  Serial.print("Up: ");
  Serial.print(digitalRead(UP_BUTTON));
  Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  Serial.print("Down: ");
  Serial.print(digitalRead(DOWN_BUTTON));
  Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  Serial.println();
}

//  Sensor Methods

void DebugSensors()
{
    // read raw sensor values
  qtra.read(sensorValues);
  
  // print the sensor values as numbers from 0 to 1023, where 0 means maximum reflectance and
  // 1023 means minimum reflectance
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }
  Serial.println();
  
  delay(250);
}

//  LED Methods

void SetAllLEDs(CRGB colour)
{
  leds[0] = colour;
  leds[1] = colour;
  leds[2] = colour;
  leds[3] = colour;
  leds[4] = colour;
  leds[5] = colour;  

  FastLED.show();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
