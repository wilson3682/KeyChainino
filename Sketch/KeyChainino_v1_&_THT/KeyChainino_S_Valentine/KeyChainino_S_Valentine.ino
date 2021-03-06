/*************************************************************************
   HAPPY VALENTINE'S DAY - FOR KEYCHAININO www.keychainino.com

   created by Alessandro Matera
 * ************************************************************************
*/

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define MATRIX_ROW 5
#define MATRIX_COL 6
#define PIN_NUMBER 7

#define BUTTON_A 6 //pin 6 - PCINT6
#define BUTTON_B 8 //pin 8 - INT0

const byte pins[PIN_NUMBER] = {0, 1, 2, 3, 7, 9, 10}; //the number of the pin used for the LEDs in ordered

const byte connectionMatrix[MATRIX_ROW][MATRIX_COL][2] = { //the matrix that shows the LEDs pin connections. First Value is the Anode, second is the Catode
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}},
  {{0, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}},
  {{0, 2}, {1, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}},
  {{0, 3}, {1, 3}, {2, 3}, {4, 3}, {5, 3}, {6, 3}},
  {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {5, 4}, {6, 4}}
};

bool matrixState[MATRIX_ROW][MATRIX_COL] = { //the matrix that will be always used to turn ON or OFF the LEDs
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};

//KeyChainino Face stored in FLASH in order to reduce RAM size
const PROGMEM bool KeyChaininoFace[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 1},
  {0, 1, 1, 1, 1, 0}
};

//heart matrix stored in FLASH in order to reduce RAM size
const PROGMEM bool heart[MATRIX_ROW][MATRIX_COL] = {
  {1, 1, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 0, 0}
};

//CHARTERS used to make the word to be scrolling. stored in FLASH in order to reduce RAM size

const PROGMEM bool A[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0}
};

const PROGMEM bool B[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 0}
};

const PROGMEM bool C[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 0, 1, 1, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0}
};

const PROGMEM bool D[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 0}
};

const PROGMEM bool E[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0}
};

const PROGMEM bool F[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0}
};

const PROGMEM bool G[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 1, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0}
};

const PROGMEM bool H[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0}
};

const PROGMEM bool I[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0}
};

const PROGMEM bool L[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0}
};

const PROGMEM bool M[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 0, 0, 0, 1},
  {0, 1, 1, 0, 1, 1},
  {0, 1, 0, 1, 0, 1},
  {0, 1, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 1}
};

const PROGMEM bool N[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 0, 1, 0},
  {0, 1, 0, 1, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0}
};

const PROGMEM bool O[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0}
};

const PROGMEM bool P[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0}
};

const PROGMEM bool Q[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 1, 0, 0},
  {0, 0, 1, 0, 1, 0}
};

const PROGMEM bool R[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 0},
  {0, 1, 0, 1, 0, 0},
  {0, 1, 0, 0, 1, 0}
};

const PROGMEM bool S[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 0}
};

const PROGMEM bool T[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 1, 1},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0}
};

const PROGMEM bool U[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0}
};

const PROGMEM bool V[MATRIX_ROW][MATRIX_COL] = {
  {1, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 1, 0},
  {0, 1, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 0}
};

const PROGMEM bool Z[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 1, 1, 1, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 0}
};

const PROGMEM bool Y[MATRIX_ROW][MATRIX_COL] = {
  {0, 1, 0, 0, 0, 1},
  {0, 0, 1, 0, 1, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 0}
};

const PROGMEM bool J[MATRIX_ROW][MATRIX_COL] = {
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {1, 0, 0, 1, 0, 0},
  {0, 1, 1, 0, 0, 0}
};

const PROGMEM bool K[MATRIX_ROW][MATRIX_COL]  = {
  {0, 0, 1, 0, 1, 0},
  {0, 0, 1, 1, 0, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 0, 1, 1, 0, 0},
  {0, 0, 1, 0, 1, 0}
};

const PROGMEM bool X[MATRIX_ROW][MATRIX_COL]  = {
  {1, 0, 0, 0, 1, 0},
  {0, 1, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 0},
  {0, 1, 0, 1, 0, 0},
  {1, 0, 0, 0, 1, 0}
};


const PROGMEM bool W[MATRIX_ROW][MATRIX_COL]  = {
  {1, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 1, 0},
  {1, 0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0, 0}
};

const PROGMEM bool QUOTE[MATRIX_ROW][MATRIX_COL]  = {
  {1, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};



ISR(TIM1_OVF_vect) {  // timer1 overflow interrupt service routine
  cli(); //disable interrupt
  TCNT1 = 65405;

  // THIS PART IS USED TO UPDATE THE CHARLIEPLEXING LEDS MATRIX
  // YOU CAN JUST DON'T CARE ABOUT THIS PART
  // BECAUSE YOU CAN CODE LIKE A STANDARD MATRIX BY MANIPULATING THE
  // VALUE OF THE matrixState MATRIX

  //check from matrixState which LED to turn ON or OFF
  for (byte i = 0; i < MATRIX_ROW; i++) {
    for (byte j = 0; j < MATRIX_COL; j++) {
      if (matrixState[i][j] == 1) { //turn on LED with 1 in matrixState
        pinMode(pins[connectionMatrix[i][j][0]], OUTPUT); //set positive pole to OUTPUT
        pinMode(pins[connectionMatrix[i][j][1]], OUTPUT); //set negative pole to OUTPUT
        digitalWrite(pins[connectionMatrix[i][j][0]], HIGH); //set positive pole to HIGH
        digitalWrite(pins[connectionMatrix[i][j][1]], LOW); //set negative pole to LOW
        delayMicroseconds(250); //250
        pinMode(pins[connectionMatrix[i][j][0]], INPUT); //set both positive pole and negative pole
        pinMode(pins[connectionMatrix[i][j][1]], INPUT); // to INPUT in order to turn OFF the LED
      }
      if (matrixState[i][j] == 0) { //turn off LED with 0 in matrixState
        pinMode(pins[connectionMatrix[i][j][0]], INPUT); //set both positive pole and negative pole
        pinMode(pins[connectionMatrix[i][j][1]], INPUT); // to INPUT in order to turn OFF the LED
      }
    }
  }
  sei(); //enable interrupt
}

ISR(PCINT0_vect) { //BUTTON A INTERRUPT
  //do nothing
}

ISR(INT0_vect) { //BUTTON B INTERRUPT
  //do nothing
}



void setup() {
  //configure LED pins
  for (byte i = 0; i < PIN_NUMBER; i++) {
    pinMode(pins[i], INPUT);
  }

  //configure Buttons pins
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  // initialize Timer1
  cli();         // disable global interrupts
  TCCR1A = 0;    // set entire TCCR1A register to 0
  TCCR1B = 0;    // set entire TCCR1A register to 0

  // enable Timer1 overflow interrupt:
  TIMSK1 |= (1 << TOIE1);

  // preload timer 65536 - (8000000 / 1024 / 60) = 60Hz
  TCNT1 = 65405;

  // set 1024 prescaler
  bitSet(TCCR1B, CS12);
  bitSet(TCCR1B, CS10);

  bitSet(GIMSK, PCIE0); //enable pingChange global interrupt

  //disabling all unnecessary peripherals to reduce power
  ADCSRA &= ~bit(ADEN); //disable ADC
  power_adc_disable(); // disable ADC converter
  power_usi_disable(); // disable USI
  // enable global interrupts:
  sei();

  showKeyChaininoFace(); //show KeyChainino smile face
  delay(500);
  clearMatrix(); //clear the Matrix

}

//WRITE HERE WHAT DO YOU WANT, BUT ONLY IN CAPITAL CHARTERS. DON'T USE SYMBOLS.
// ONLY THE SPACE CHARTER IS ALLOWED.
// IF YOU WANT OTHER SYMBOLS, WRITE YOUR OWN AND CREATE A NEW TABLE JUST LIKE THE
// OTHER CHARTERS
char phrase[] = "HAPPY VALENTINE'S DAY ";

void loop() {
  game(); //starting the game
}

void game() {
  for (char c = 0; phrase[c] != '\0'; c++) {
    for (int col = MATRIX_COL - 1; col >= 0; col--) { // we start to display the charter matrix from right to left
      for (byte i = 0; i < MATRIX_COL; i++) { //put the charter into the matrixState
        for (byte j = 0; j < MATRIX_ROW; j++) { //as usual
          if (i >= col) { //if the number of col(i) is higher than the scrolling col, we show the correct charter according to charterToShow var.
            writeCharter(phrase[c], i, j, col);
          } else { //else, if col (i) is less than col, we shift the matrixState
            matrixState[j][i] = matrixState[j][i + 1];
          }
        }
      }
      delay(60);
    }
  }

  for (int i = 0; i < 10; i++) {
    showHeart();
    delay(100);
    clearMatrix();
    delay(100);
  }
  showHeart();
  delay(500);
  endGame(); //go to the end of the game

}

void writeCharter(char charterToShow, byte i, byte j, byte col) {
  if (charterToShow == 'A') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(A[j][i - col]));
  }
  else if (charterToShow == 'B') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(B[j][i - col]));
  }
  else if (charterToShow == 'C') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(C[j][i - col]));
  }
  else if (charterToShow == 'D') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(D[j][i - col]));
  }
  else if (charterToShow == 'E') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(E[j][i - col]));
  }
  else if (charterToShow == 'F') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(F[j][i - col]));
  }
  else if (charterToShow == 'G') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(G[j][i - col]));
  }
  else if (charterToShow == 'H') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(H[j][i - col]));
  }
  else if (charterToShow == 'I') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(I[j][i - col]));
  }
  else if (charterToShow == 'L') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(L[j][i - col]));
  }
  else if (charterToShow == 'M') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(M[j][i - col]));
  }
  else if (charterToShow == 'N') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(N[j][i - col]));
  }
  else if (charterToShow == 'O') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(O[j][i - col]));
  }
  else if (charterToShow == 'P') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(P[j][i - col]));
  }
  else if (charterToShow == 'Q') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(Q[j][i - col]));
  }
  else if (charterToShow == 'R') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(R[j][i - col]));
  }
  else if (charterToShow == 'S') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(S[j][i - col]));
  }
  else if (charterToShow == 'T') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(T[j][i - col]));
  }
  else if (charterToShow == 'U') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(U[j][i - col]));
  }
  else if (charterToShow == 'V') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(V[j][i - col]));
  }
  else if (charterToShow == 'Z') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(Z[j][i - col]));
  }
  else if (charterToShow == 'K') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(K[j][i - col]));
  }
  else if (charterToShow == 'X') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(X[j][i - col]));
  }
  else if (charterToShow == 'Y') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(Y[j][i - col]));
  }
  else if (charterToShow == 'J') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(J[j][i - col]));
  }
  else if (charterToShow == 'W') {
    matrixState[j][i] = (bool*)pgm_read_byte(&(W[j][i - col]));
  }
  else if (charterToShow == ' ') { //SYMBOLS FOR SPACE
    matrixState[j][i] = 0;
  }
  else if (charterToShow == '\'') { //SYMBOLS FOR QUOTE
    matrixState[j][i] = (bool*)pgm_read_byte(&(QUOTE[j][i - col]));
  }

}

void endGame() {
  clearMatrix(); //clear all the LEDs
  showKeyChaininoFace(); //show KeyChaininoFace
  delay(500);
  goSleep(); //sleep to reduce power
  clearMatrix(); //clear all the LEDs
  showKeyChaininoFace(); //show KeyChaininoFace
  delay(500);
}

void clearMatrix() {
  //clear the matrix by inserting 0 to the matrixState
  for (byte i = 0; i < MATRIX_ROW; i++) {
    for (byte j = 0; j < MATRIX_COL; j++) {
      clearMatrixStateBit(i, j);
    }
  }
}

void fullMatrix() {
  //turn on all LEDs in the matrix by inserting 1 to the matrixState
  for (byte i = 0; i < MATRIX_ROW; i++) {
    for (byte j = 0; j < MATRIX_COL; j++) {
      setMatrixStateBit(i, j);
    }
  }
}

//here we set or clear a single bit on the matrixState. We use this funciton in order
//to really set or clear the matrix's bit when an interrupt occours. To do that we disable the
//interrupt -> set or clear the bit -> enable interrupt

void setMatrixStateBit(byte i, byte j) {
  cli();
  matrixState[i][j] = 1;
  sei();
}
void clearMatrixStateBit(byte i, byte j) {
  cli();
  matrixState[i][j] = 0;
  sei();
}

void showKeyChaininoFace() {
  for (byte i = 0; i < MATRIX_ROW; i++) {
    for (byte j = 0; j < MATRIX_COL; j++) {
      matrixState[i][j] = (bool*)pgm_read_byte(&(KeyChaininoFace[i][j])); //here we read the matrix from FLASH
    }
  }
}

void showHeart() {
  for (byte i = 0; i < MATRIX_ROW; i++) {
    for (byte j = 0; j < MATRIX_COL; j++) {
      matrixState[i][j] = (bool*)pgm_read_byte(&(heart[i][j])); //here we read the matrix from FLASH
    }
  }
}


void goSleep() {
  //going sleep to reduce power consuming

  //enable interrupt buttons to allow wakeup from button interrupts
  bitSet(GIMSK, INT0); //enable interrupt pin 8 - button B - INT0
  bitSet(PCMSK0, PCINT6); //enable interrupt pin 6 - button A - PCINT6
  power_timer0_disable(); //disable Timer 0
  power_timer1_disable(); //disable Timer 1
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  //disable interrupt buttons after sleep
  bitClear(GIMSK, INT0); //disable interrupt pin 8 - button B - INT0
  bitClear(PCMSK0, PCINT6); //disable interrupt pin 6 - button A - PCINT6
  power_timer0_enable(); //enable Timer 0
  power_timer1_enable(); //enable Timer 1
}

