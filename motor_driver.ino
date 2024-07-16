/* ================== Motor Setup ================== */
/* ================================================= */

void setMotorSpeed(int i, int spd) {
  unsigned char reverse = 0;
  if (spd < 0) {
    spd = -spd;
    reverse = 1;
  }
  if (spd > MAX_PWM) spd = MAX_PWM;
  if (i == LEFT_) {
    if (reverse == 0) {
      analogWrite(LEFT__MOTOR_ENABLE__, spd);
      digitalWrite(LEFT__FORWARD__MOTOR, !reverse);
      digitalWrite(LEFT__BACKWARD_MOTOR, reverse);
    } else if (reverse == 1) {
      analogWrite(LEFT__MOTOR_ENABLE__, spd);
      digitalWrite(LEFT__FORWARD__MOTOR, !reverse);
      digitalWrite(LEFT__BACKWARD_MOTOR, reverse);
    }
  } else {
    if (reverse == 0) {
      analogWrite(RIGHT_MOTOR_ENABLE__, spd);
      digitalWrite(RIGHT_FORWARD__MOTOR, !reverse);
      digitalWrite(RIGHT_BACKWARD_MOTOR, reverse);
    } else if (reverse == 1) {
      analogWrite(RIGHT_MOTOR_ENABLE__, spd);
      digitalWrite(RIGHT_FORWARD__MOTOR, !reverse);
      digitalWrite(RIGHT_BACKWARD_MOTOR, reverse);
    }
  }
}

void setMotorSpeeds(int m1, int m2) {
  setMotorSpeed(LEFT_, m1);
  setMotorSpeed(RIGHT, m2);
}
