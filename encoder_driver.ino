volatile long left__enc_pos = 0L;
volatile long right_enc_pos = 0L;
volatile int left__encoderDirection_prev = 0L;
volatile int right_encoderDirection_prev = 0L;

void left__updateEncoder() {
  int change = 0;
  static byte encoderA, encoderB;
  byte encoderNewA = digitalRead(LEFT__ENC_PIN_A);
  byte encoderNewB = digitalRead(LEFT__ENC_PIN_B);
  if (encoderA == LOW && encoderNewA == HIGH) {
    change = (encoderB == LOW) ? 1 : -1;
  } else if (encoderA == HIGH && encoderNewA == LOW) {
    change = (encoderB == HIGH) ? 1 : -1;
  }
  left__enc_pos += change;
  encoderA = encoderNewA;
  encoderB = encoderNewB;
}

void right_updateEncoder() {
  int change = 0;
  static byte encoderA, encoderB;
  byte encoderNewA = digitalRead(RIGHT_ENC_PIN_A);
  byte encoderNewB = digitalRead(RIGHT_ENC_PIN_B);
  if (encoderA == LOW && encoderNewA == HIGH) {
    change = (encoderB == HIGH) ? 1 : -1;
  } else if (encoderA == HIGH && encoderNewA == LOW) {
    change = (encoderB == LOW) ? 1 : -1;
  }
  right_enc_pos += change;
  encoderA = encoderNewA;
  encoderB = encoderNewB;
}

long readEncoder(int i) {
  if (i == LEFT_) return left__enc_pos;
  else return right_enc_pos;
}

long updateEncoder(int i) {
  if (i == LEFT_) {
    volatile int left__D = left__enc_pos - left__encoderDirection_prev;
    left__encoderDirection_prev = left__enc_pos;
    return left__D;
  } else {
    volatile int right_D = right_enc_pos - right_encoderDirection_prev;
    right_encoderDirection_prev = right_enc_pos;
    return right_D;
  }
}

void resetEncoder(int i) {
  if (i == LEFT_) {
    left__enc_pos = 0L;
    return;
  } else {
    right_enc_pos = 0L;
    return;
  }
}

void resetEncoders() {
  resetEncoder(LEFT_);
  resetEncoder(RIGHT);
}
