#include <Button.h>
#include <Servo.h>

const int SERVO_PIN = 11;
const int BUTTON_UP = 8;
const int BUTTON_DOWN = 7;

const int MAX_DEG = 170;
const int MIN_DEG = 20;
//const int TICK_DELAY = 24000;
const int TICK_DELAY = 200;

const int ACTION_UP = 1;
const int ACTION_NEUTRAL = 0;
const int ACTION_DOWN = -1;


Button buttonUp(BUTTON_UP);
Button buttonDown(BUTTON_DOWN);
Servo servo;

int _buttonUpState = 0;
int _buttonDownState = 0;
int _currentServoPos = 0;
int _currentAction = 0;
long _nextTick = 0;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(MIN_DEG);
  _currentServoPos = servo.read();
}

void loop() {
  buttonLoop();
}

void buttonLoop() {
  long now = millis();

  int currButtonUpState = buttonUp.readState();
  int currButtonDownState = buttonDown.readState();

  if (currButtonUpState != _buttonUpState) {
    _currentAction = ACTION_UP;
    _buttonUpState = currButtonUpState;
    _nextTick = now;
    Serial.print("Rotate up pressed\n");
  }

  if (currButtonDownState != _buttonDownState) {
    _currentAction = ACTION_DOWN;
    _buttonDownState = currButtonDownState;
    _nextTick = now;
    Serial.print("Rotate down pressed\n");
  }

  if (_currentAction != ACTION_NEUTRAL &&
      now >= _nextTick) {
    handleAction(_currentAction);
    _nextTick = now + TICK_DELAY;
  }
}

void handleAction(int action) {
  int newServoPos = _currentServoPos;
  switch (action) {
  case ACTION_UP:
    newServoPos++;
    break;
  case ACTION_DOWN:
    newServoPos--;
    break;
  default:
    // do nothing
    break;
  }

  Serial.print(newServoPos);
  Serial.print('\n');
  if (MIN_DEG <= newServoPos && newServoPos <= MAX_DEG) {
    servo.write(newServoPos);
    _currentServoPos = newServoPos;
  } else {
    _currentAction = ACTION_NEUTRAL;
    Serial.print("Rotate done\n");

  }
}
