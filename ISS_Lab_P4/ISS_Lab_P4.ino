#include "arduino_secrets.h"

// Hannah Soria
// IIS Lab Capactive Sensing
// 9/15/26

// variables
bool print = false;
const int LED_PIN1 = 12;
const int LED_PIN2 = 14;
const int LED_PIN3 = 27;

const int TOUCH_PIN1 = 4;
const int TOUCH_PIN2 = 33;
const int TOUCH_PIN3 = 32;

// track the state and direction of swipe
int swipedState = 0;

// start positions
int untouched1, untouched2, untouched3;

// threshold to track a touch
int threshold1, threshold2, threshold3;

// track time
unsigned long start_time, curr_time, game_start_time;

// track swipes
int leftSwipe = 0;
int rightSwipe = 0;

// randomize right and left command
int swipes[10] = {};

// function that returns a calibration of the sensors
int calibrate(const int pin) {
  int total = 0;

  for(int i = 0; i < 30; i++){
    total += touchRead(pin);
  }
  int result = total / 30;
  return result;
}

// check what state the touch gestures are in
// checking if there is a right or left swipe
// start of swipe is red, a complete swipe is green
void checkState() {
  // retrieve the value of the touch
  int sensed1 = touchRead(TOUCH_PIN1);
  int sensed2 = touchRead(TOUCH_PIN2);
  int sensed3 = touchRead(TOUCH_PIN3);

  curr_time = millis();

  // track that the swipes happen fast enough
  if((swipedState > 0) && (curr_time - start_time > 250)){
    swipedState = 0;
  }

  if((swipedState == 2 || swipedState == 4) && (curr_time - start_time > 500)){
    swipedState = 0;
  }

  //if state is 0
  if (swipedState == 0){
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);

    // start of a right swipe
    if (sensed1 < threshold1 ){
      start_time = millis();
      swipedState = 1;
      digitalWrite(LED_PIN1, HIGH);

    } else if (sensed3 < threshold3) {
      start_time = millis();
      swipedState = 3;
      digitalWrite(LED_PIN1, HIGH);
      if (curr_time - start_time > 500){
        digitalWrite(LED_PIN1, LOW);
      }
    }

  // going right check touch 2
  } else if (swipedState == 1){
    if (sensed2 < threshold2) {
      swipedState = 2;
      digitalWrite(LED_PIN2, HIGH);
    } 

  // going right check touch 3
  } else if (swipedState == 2){
    if (sensed3 < threshold3 ) {
      digitalWrite(LED_PIN3, HIGH);
      delay(100);
      rightSwipe = 1;
      digitalWrite(LED_PIN1, LOW);
      digitalWrite(LED_PIN2, LOW);
      digitalWrite(LED_PIN3, LOW);
      swipedState = 0;
    } 

  // going left check touch 2
  } else if (swipedState == 3){
    if (sensed2 < threshold2){
      swipedState = 4;
      digitalWrite(LED_PIN2, HIGH);
    } 

  // going left check touch 3
  } else if (swipedState == 4){
    if (sensed1 < threshold1 ){
      digitalWrite(LED_PIN3, HIGH);
      delay(100);
      leftSwipe = 1;
      digitalWrite(LED_PIN1, LOW);
      digitalWrite(LED_PIN2, LOW);
      digitalWrite(LED_PIN3, LOW);
      swipedState = 0;
    } 
  } 
}

// run the dance swipe game
// game gives ten right or left arrows, random every time
// when gesture matches next gesture is provided
// after ten correct gestures the total time (score) is given
void game() {
  // randomize right and left command
  for (int i = 0; i < 10; i ++){
    swipes[i] = random(0,2);
  }

  // prepping player to start
  Serial.println("Game starting in");
  delay(1000);
  Serial.println("3...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("1...");
  delay(1000);
  Serial.println("0...");
  delay(1000);

  game_start_time = millis();

  // for each of the ren gestures
  for (int i = 0; i < 10; i++){
    //reset swipes
    rightSwipe = 0;
    leftSwipe = 0;
    swipedState = 0;

    // 0 = right swipe
    if (swipes[i] == 0) {
      Serial.println("--->");

      // while it is correct
      while (rightSwipe == 0){
        checkState();
        delay(1);

        // incorrect is a miss
        if (leftSwipe == 1) {
          Serial.println("â MISS!");
          leftSwipe = 0;
        }
      }

      // correct
      Serial.println("ð PERFECT!");

    //left swipe
    } else {
      Serial.println("<---");

      // while the gesture is correct
      while (leftSwipe == 0){
        checkState();
        delay(1);

        // if swipe is wrong it is a miss
        if (rightSwipe == 1) {
          Serial.println("â MISS!");
          rightSwipe = 0;
        }
      }
      // correct swipe
      Serial.println("ðº PERFECT!");
    }
  }

  // ending information and instructions
  unsigned long game_time = millis() - game_start_time;
  Serial.println();
  Serial.println("GAME OVER");
  Serial.print("TIME: ");
  Serial.print(game_time);
  Serial.print(" ms");
  Serial.println();
  Serial.println("PRESS 'p' TO PLAY AGAIN");

  // reset
  rightSwipe = 0;
  leftSwipe = 0;
  swipedState = 0;
}

// connect outputs, calibrate, set threshold, start
void setup() {
  // set led pin to output
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);

  // retrieve the starting values of each sensor; calibrate
  untouched1 = calibrate(TOUCH_PIN1);
  untouched2 = calibrate(TOUCH_PIN2);
  untouched3 = calibrate(TOUCH_PIN3);

  // set the threshold for each sensor (hardcoded)
  threshold1 = untouched1 - 100;
  threshold2 = untouched2 - 100;
  threshold3 = untouched3 - 100;

  Serial.begin(115200);
  Serial.println("PRESS 'p' THEN 'return' TO PLAY");
}

// in the loop wait for p then return then start the game
void loop() {
  if (print == false){
    Serial.println("PRESS 'p' THEN 'return' TO PLAY");
    print = true;
  }
  if (Serial.available() > 0){
    char key = Serial.read();

    if (key == 'p' || key == 'P') {
      game();
    }
  }
}
