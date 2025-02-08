#include <Servo.h>
#include <stdint.h>

#define MIN_BALL_RAMP_TOP_SERVO_START_POS 120
#define MIN_BALL_RAMP_TOP_SERVO_END_POS 55
#define MIN_BALL_RAMP_TOP_SERVO_PIN 3

#define MIN_BALL_RAMP_BOTTOM_SERVO_START_POS 60
#define MIN_BALL_RAMP_BOTTOM_SERVO_END_POS 120
#define MIN_BALL_RAMP_BOTTOM_SERVO_PIN 5

#define MIN_BALL_RELEASE_SERVO_START_POS 120
#define MIN_BALL_RELEASE_SERVO_END_POS 60
#define MIN_BALL_RELEASE_SERVO_PIN 6

#define HR_BALL_RAMP_TOP_SERVO_START_POS 60
#define HR_BALL_RAMP_TOP_SERVO_END_POS 130
#define HR_BALL_RAMP_TOP_SERVO_PIN 9

#define HR_BALL_RAMP_BOTTOM_SERVO_START_POS 50
#define HR_BALL_RAMP_BOTTOM_SERVO_END_POS 120
#define HR_BALL_RAMP_BOTTOM_SERVO_PIN 10

#define HR_BALL_RELEASE_SERVO_START_POS 150
#define HR_BALL_RELEASE_SERVO_END_POS 55
#define HR_BALL_RELEASE_SERVO_PIN 11

#define MIN_BALL_RAMP_SERVO_START_POS 55
#define MIN_BALL_RAMP_SERVO_END_POS 150
#define MIN_BALL_RAMP_SERVO_PIN 12

#define HR_BALL_RAMP_SERVO_START_POS 78
#define HR_BALL_RAMP_SERVO_END_POS 138
#define HR_BALL_RAMP_SERVO_PIN 13

#define RESET_BUTTON_PIN A2 // Define the reset button pin
#define DEBOUNCE_DELAY 50   // Debounce time in milliseconds

#define NO_OF_HR_BALLS 4
#define NO_OF_MIN_BALLS 6

typedef enum servoName_T {
    MIN_BALL_RAMP_TOP_SERVO,
    MIN_BALL_RAMP_BOTTOM_SERVO,
    MIN_BALL_RELEASE_SERVO,
    HR_BALL_RAMP_TOP_SERVO,
    HR_BALL_RAMP_BOTTOM_SERVO,
    HR_BALL_RELEASE_SERVO,
    MIN_BALL_RAMP_SERVO,
    HR_BALL_RAMP_SERVO,
    SERVO_MAX // for dimension
} servoName_e;

typedef struct servoCtrl_T {
    Servo *servoFp;
    int pinNumber;
    int startPos;
    int endPos;
} servoCtrl_s;

Servo servos[SERVO_MAX];

servoCtrl_s servoCtrl[SERVO_MAX] = {
    [MIN_BALL_RAMP_TOP_SERVO] = {
        .servoFp = &servos[MIN_BALL_RAMP_TOP_SERVO],
        .pinNumber = MIN_BALL_RAMP_TOP_SERVO_PIN,
        .startPos = MIN_BALL_RAMP_TOP_SERVO_START_POS,
        .endPos = MIN_BALL_RAMP_TOP_SERVO_END_POS
    },
    [MIN_BALL_RAMP_BOTTOM_SERVO] = {
        .servoFp = &servos[MIN_BALL_RAMP_BOTTOM_SERVO],
        .pinNumber = MIN_BALL_RAMP_BOTTOM_SERVO_PIN,
        .startPos = MIN_BALL_RAMP_BOTTOM_SERVO_START_POS,
        .endPos = MIN_BALL_RAMP_BOTTOM_SERVO_END_POS
    },
    [MIN_BALL_RELEASE_SERVO] = {
        .servoFp = &servos[MIN_BALL_RELEASE_SERVO],
        .pinNumber = MIN_BALL_RELEASE_SERVO_PIN,
        .startPos = MIN_BALL_RELEASE_SERVO_START_POS,
        .endPos = MIN_BALL_RELEASE_SERVO_END_POS
    },
    [HR_BALL_RAMP_TOP_SERVO] = {
        .servoFp = &servos[HR_BALL_RAMP_TOP_SERVO],
        .pinNumber = HR_BALL_RAMP_TOP_SERVO_PIN,
        .startPos = HR_BALL_RAMP_TOP_SERVO_START_POS,
        .endPos = HR_BALL_RAMP_TOP_SERVO_END_POS
    },
    [HR_BALL_RAMP_BOTTOM_SERVO] = {
        .servoFp = &servos[HR_BALL_RAMP_BOTTOM_SERVO],
        .pinNumber = HR_BALL_RAMP_BOTTOM_SERVO_PIN,
        .startPos = HR_BALL_RAMP_BOTTOM_SERVO_START_POS,
        .endPos = HR_BALL_RAMP_BOTTOM_SERVO_END_POS
    },
    [HR_BALL_RELEASE_SERVO] = {
        .servoFp = &servos[HR_BALL_RELEASE_SERVO],
        .pinNumber = HR_BALL_RELEASE_SERVO_PIN,
        .startPos = HR_BALL_RELEASE_SERVO_START_POS,
        .endPos = HR_BALL_RELEASE_SERVO_END_POS
    },
    [MIN_BALL_RAMP_SERVO] = {
        .servoFp = &servos[MIN_BALL_RAMP_SERVO],
        .pinNumber = MIN_BALL_RAMP_SERVO_PIN,
        .startPos = MIN_BALL_RAMP_SERVO_START_POS,
        .endPos = MIN_BALL_RAMP_SERVO_END_POS
    },
    [HR_BALL_RAMP_SERVO] = {
        .servoFp = &servos[HR_BALL_RAMP_SERVO],
        .pinNumber = HR_BALL_RAMP_SERVO_PIN,
        .startPos = HR_BALL_RAMP_SERVO_START_POS,
        .endPos = HR_BALL_RAMP_SERVO_END_POS
    }
};

// Time variables
unsigned long previousMillis = 0;
const long interval = 20000;
static unsigned int hours = 0; // Starting hours
static unsigned int minutes = 0; // Starting minute

// Button debounce variables
unsigned long lastDebounceTime = 0;
bool lastButtonState = HIGH;
bool buttonState = HIGH;

void setup() {
    //Serial.begin(9600);
    //delay(2000);

    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP); // Configure reset button pin as input with pullup

    // Put servo in starting position
    initServos();
    allBallsRelease();
}

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        incrementTime();
        //printTime();
    }

    checkResetButton();
}

void incrementTime(void) 
{
    if (!(minutes > (NO_OF_MIN_BALLS-1)))
    { 
        minutes++;
        launchMinuteBall();
    } 
    else 
    {
      if(!(hours > (NO_OF_HR_BALLS-1)))
      {
        minutes = 0;
        hours++;
        launchHourBall();
        allMinuteBallsRelease();
      }
      else
      {
        minutes = 0;
        hours = 0;
        allBallsRelease();
      }
    }
}

void printTime(void) 
{
    char timeString[6];
    snprintf(timeString, sizeof(timeString), "%02d:%02d", hours, minutes);
    Serial.println(timeString);
}

void setServoPosition(Servo *servo, int startPos, int endPos, int speedDelay) 
{
    if (startPos < endPos) {
        for (int pos = startPos; pos <= endPos; pos++) {
            servo->write(pos);
            delay(speedDelay);
        }
    } else {
        for (int pos = startPos; pos >= endPos; pos--) {
            servo->write(pos);
            delay(speedDelay);
        }
    }
}

void initServos(void) 
{
    for (uint8_t ii = 0; ii < SERVO_MAX; ii++) {
        servoCtrl[ii].servoFp->attach(servoCtrl[ii].pinNumber);
        servoCtrl[ii].servoFp->write(servoCtrl[ii].startPos);
    }
}

void launchMinuteBall(void) 
{
  minBallRelease();
  minBallLift();
  minBallRampRelease();
}

void minBallRelease(void)
{
    servoCtrl[MIN_BALL_RELEASE_SERVO].servoFp->write(servoCtrl[MIN_BALL_RELEASE_SERVO].endPos);
    delay(1000);
    servoCtrl[MIN_BALL_RELEASE_SERVO].servoFp->write(servoCtrl[MIN_BALL_RELEASE_SERVO].startPos);
    delay(1000);
}

void minBallLift(void)
{
    setServoPosition(servoCtrl[MIN_BALL_RAMP_SERVO].servoFp, servoCtrl[MIN_BALL_RAMP_SERVO].startPos, servoCtrl[MIN_BALL_RAMP_SERVO].endPos, 20);
    delay(1000);
    setServoPosition(servoCtrl[MIN_BALL_RAMP_SERVO].servoFp, servoCtrl[MIN_BALL_RAMP_SERVO].endPos, servoCtrl[MIN_BALL_RAMP_SERVO].startPos, 20);
    delay(1000);
}

void minBallRampRelease(void)
{
    servoCtrl[MIN_BALL_RAMP_TOP_SERVO].servoFp->write(servoCtrl[MIN_BALL_RAMP_TOP_SERVO].endPos);
    delay(1000);
    servoCtrl[MIN_BALL_RAMP_TOP_SERVO].servoFp->write(servoCtrl[MIN_BALL_RAMP_TOP_SERVO].startPos);
    delay(1000);
}


void launchHourBall(void) 
{
  hrBallRelease();
  hrBallLift();
  hrBallRampRelease();
}

void hrBallRelease(void)
{
    servoCtrl[HR_BALL_RELEASE_SERVO].servoFp->write(servoCtrl[HR_BALL_RELEASE_SERVO].endPos);
    delay(1000);
    servoCtrl[HR_BALL_RELEASE_SERVO].servoFp->write(servoCtrl[HR_BALL_RELEASE_SERVO].startPos);
    delay(1000);
}

void hrBallLift(void)
{
    setServoPosition(servoCtrl[HR_BALL_RAMP_SERVO].servoFp, servoCtrl[HR_BALL_RAMP_SERVO].startPos, servoCtrl[HR_BALL_RAMP_SERVO].endPos, 12);
    delay(1000);
    setServoPosition(servoCtrl[HR_BALL_RAMP_SERVO].servoFp, servoCtrl[HR_BALL_RAMP_SERVO].endPos, servoCtrl[HR_BALL_RAMP_SERVO].startPos, 12);
    delay(1000);
}

void hrBallRampRelease(void)
{
    servoCtrl[HR_BALL_RAMP_TOP_SERVO].servoFp->write(servoCtrl[HR_BALL_RAMP_TOP_SERVO].endPos);
    delay(1000);
    servoCtrl[HR_BALL_RAMP_TOP_SERVO].servoFp->write(servoCtrl[HR_BALL_RAMP_TOP_SERVO].startPos);
    delay(1000);
}



void allMinuteBallsRelease(void) 
{
    servoCtrl[MIN_BALL_RAMP_BOTTOM_SERVO].servoFp->write(servoCtrl[MIN_BALL_RAMP_BOTTOM_SERVO].endPos);
    delay(1000);
    servoCtrl[MIN_BALL_RAMP_BOTTOM_SERVO].servoFp->write(servoCtrl[MIN_BALL_RAMP_BOTTOM_SERVO].startPos);
    delay(1000);
}

void allHourBallsRelease(void) 
{
    servoCtrl[HR_BALL_RAMP_BOTTOM_SERVO].servoFp->write(servoCtrl[HR_BALL_RAMP_BOTTOM_SERVO].endPos);
    delay(1000);
    servoCtrl[HR_BALL_RAMP_BOTTOM_SERVO].servoFp->write(servoCtrl[HR_BALL_RAMP_BOTTOM_SERVO].startPos);
    delay(1000);
}

void allBallsRelease(void)
{
  allHourBallsRelease();
  allMinuteBallsRelease();
}

void checkResetButton(void) 
{
    int reading = digitalRead(RESET_BUTTON_PIN);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading == LOW && buttonState == HIGH) {
            resetClock();
            allBallsRelease();
        }
        buttonState = reading;
    }

    lastButtonState = reading;
}

void resetClock(void) 
{
    hours = 0;
    minutes = 0;
}
