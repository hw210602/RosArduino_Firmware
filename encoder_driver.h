#define LEFT__ENC_PIN_A 20
#define LEFT__ENC_PIN_B 21
#define RIGHT_ENC_PIN_A 18
#define RIGHT_ENC_PIN_B 19

long readEncoder(int i);
long updateEncoder(int i);
void resetEncoder(int i);
void resetEncoders();