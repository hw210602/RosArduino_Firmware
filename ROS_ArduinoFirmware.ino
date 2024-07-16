

#define BAUDRATE 57600
#define MAX_PWM 255
#define PID_RATE 30
#define AUTO_STOP_INTERVAL 1000

#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include "pid_controller.h"

const int PID_INTERVAL = 1000 / PID_RATE;
unsigned long nextPID = PID_INTERVAL;
long lastMotorCommand = AUTO_STOP_INTERVAL;

int arg = 0;
int indesk = 0;

char chr;
char cmd;

char argv1[16];
char argv2[16];

long arg1;
long arg2;

void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  indesk = 0;
}

int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch (cmd) {
    case GET_BAUDRATE:
      Serial2.println(BAUDRATE);
      break;
    case READ_ENCODERS:
      Serial2.println(String(readEncoder(LEFT_)) + " " + String(readEncoder(RIGHT)));
      break;
    case UPDATE_ENCODERS:
      Serial2.println(String(updateEncoder(LEFT_)) + " " + String(updateEncoder(RIGHT)));
      break;
    case RESET_ENCODERS:
      resetEncoders();
      resetPID();
      break;
    case MOTOR_SPEEDS:
      lastMotorCommand = millis();
      if (arg1 == 0 && arg2 == 0) {
        setMotorSpeeds(0, 0);
        resetPID();
        moving = 0;
      } else moving = 1;
      m1_PID.TargetTicksPerFrame = arg1;
      m2_PID.TargetTicksPerFrame = arg2;
      break;
    case MOTOR_RAW_PWM:
      lastMotorCommand = millis();
      resetPID();
      moving = 0;
      setMotorSpeeds(arg1, arg2);
      break;
    case UPDATE_PID:
      while ((str = strtok_r(p, ":", &p)) != '\0') {
        pid_args[i] = atoi(str);
        i++;
      }
      Kp = pid_args[0];
      Kd = pid_args[1];
      Ki = pid_args[2];
      Ko = pid_args[3];
      break;
    default:
      Serial2.println("Invalid Command");
      break;
  }
}

void setup() {
  Serial2.begin(BAUDRATE);
  /* ============ Set Pin Motor ============ */
  pinMode(RIGHT_MOTOR_ENABLE__, OUTPUT);
  pinMode(RIGHT_FORWARD__MOTOR, OUTPUT);
  pinMode(RIGHT_BACKWARD_MOTOR, OUTPUT);
  pinMode(LEFT__MOTOR_ENABLE__, OUTPUT);
  pinMode(LEFT__FORWARD__MOTOR, OUTPUT);
  pinMode(LEFT__BACKWARD_MOTOR, OUTPUT);
  /* ============ Set Pin Encoder ============ */
  pinMode(LEFT__ENC_PIN_A, INPUT_PULLUP);
  pinMode(LEFT__ENC_PIN_B, INPUT_PULLUP);
  pinMode(RIGHT_ENC_PIN_A, INPUT_PULLUP);
  pinMode(RIGHT_ENC_PIN_B, INPUT_PULLUP);
  /* ============ Set Interrupt ============ */
  attachInterrupt(digitalPinToInterrupt(LEFT__ENC_PIN_A), left__updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT__ENC_PIN_B), left__updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_A), right_updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_B), right_updateEncoder, CHANGE);
  resetPID();
}

void loop() {

  // Serial2.println(String(readEncoder(LEFT_)) + " " + String(readEncoder(RIGHT)));
  // Serial2.println(String(updateEncoder(LEFT_)) + " " + String(updateEncoder(RIGHT)));

  while (Serial2.available() > 0) {
    chr = Serial2.read();
    if (chr == 13) {
      if (arg == 1) argv1[indesk] = NULL;
      else if (arg == 2) argv2[indesk] = NULL;
      runCommand();
      resetCommand();
    } else if (chr == ' ') {
      if (arg == 0) arg = 1;
      else if (arg == 1) {
        argv1[indesk] = NULL;
        arg = 2;
        indesk = 0;
      }
      continue;
    } else {
      if (arg == 0) {
        cmd = chr;
      } else if (arg == 1) {
        argv1[indesk] = chr;
        indesk++;
      } else if (arg == 2) {
        argv2[indesk] = chr;
        indesk++;
      }
    }
  }
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}
