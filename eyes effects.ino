//Sayan Electronics(LIKE-SUBSCRIBE-SHARE)
//Fluid ROBOT Eye Motion 
//Connect Vcc-3.3V, Gnd-Gnd, SCL-D22, SDA-D21 (0.96 inch OLED - ESP32 DEV KIT V1)
//CODE_NO: 01
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Eye position and size
int leftEyeX = 40;       // X position of left eye
int rightEyeX = 80;      // X position of right eye
int eyeY = 18;           // Y position of both eyes (slightly above the center)
int eyeWidth = 25;       // Eye width
int eyeHeight = 30;      // Eye height

// Target positions (for smooth movement)
int targetLeftEyeX = leftEyeX;
int targetRightEyeX = rightEyeX;
int moveSpeed = 3;  // Control the speed of eye movement

// Blinking and animation variables
int blinkState = 0;      // 0 = eyes open, 1 = eyes closed (blinking)
int blinkDelay = 700;   // Blink delay (2 seconds)
unsigned long lastBlinkTime = 0;
unsigned long moveTime = 0;

// Expression states (0 = normal, 1 = happy, 2 = sad, 3 = angry)
int expression = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(800);
}

void loop() {
  unsigned long currentTime = millis();

  // Blinking logic (timed)
  if (currentTime - lastBlinkTime > blinkDelay && blinkState == 0) {
    blinkState = 1;   // Blink starts
    lastBlinkTime = currentTime;
  } 
  else if (currentTime - lastBlinkTime > 400 && blinkState == 1) {  // Blink fast
    blinkState = 0;   // Blink ends
    lastBlinkTime = currentTime;
  }

  // Random movements logic (move eyes after random intervals)
  if (currentTime - moveTime > random(2000, 5000) && blinkState == 0) {
    int eyeMovement = random(0, 3);   // Random move left, right, or center
    if (eyeMovement == 1) {
      targetLeftEyeX = 30;      // Move eyes closer
      targetRightEyeX = 60;
    } else if (eyeMovement == 2) {
      targetLeftEyeX = 50;      // Move eyes apart
      targetRightEyeX = 80;
    } else {
      targetLeftEyeX = 40;      // Default position (center)
      targetRightEyeX = 70;
    }
    moveTime = currentTime;
  }

  // Smooth transition logic for eye positions (interpolating for fluid motion)
  if (leftEyeX != targetLeftEyeX) {
    leftEyeX += (targetLeftEyeX - leftEyeX) / moveSpeed;  // Smooth movement
  }

  if (rightEyeX != targetRightEyeX) {
    rightEyeX += (targetRightEyeX - rightEyeX) / moveSpeed; // Smooth movement
  }

  // Clear the display
  display.clearDisplay();

  // Draw the left eye
  if (blinkState == 0) {
    drawExpression(leftEyeX, eyeY, eyeWidth, eyeHeight, expression);
  } else {
    // Blink - make the eyes close
    display.fillRect(leftEyeX, eyeY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE); // Small line for blink
  }

  // Draw the right eye
  if (blinkState == 0) {
    drawExpression(rightEyeX, eyeY, eyeWidth, eyeHeight, expression);
  } else {
    // Blink - make the eyes close
    display.fillRect(rightEyeX, eyeY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE); // Small line for blink
  }

  // Update the screen with new eye positions or expressions
  display.display();

  // Small delay to prevent screen flickering too much
  delay(50);

  // Randomly change the expression every few seconds for variety
  if (currentTime - moveTime > random(2000, 5000)) {
    expression = 1; // random(0, 6);  // Change expression to a random value (0 to 3)
    moveTime = currentTime;
  }
}

// Function to draw expression on eye shape
void drawExpression(int eyeX, int eyeY, int eyeWidth, int eyeHeight, int exp) {
  display.fillRoundRect(eyeX, eyeY, eyeWidth, eyeHeight, 5, WHITE);  // Default round rectangle for eye

  // Add expression inside the eye
  switch (exp) {
    case 0: // Normal
      // No additional shapes inside the eye (neutral expression)
      break;
      
    case 1: // Happy
      display.fillRect(eyeX , eyeY + 18 , eyeWidth - 10, 4, WHITE);  // Smiling curve in the middle
      break;

    
  }
}
