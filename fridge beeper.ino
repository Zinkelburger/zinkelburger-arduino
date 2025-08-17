/*
  Final Fridge Door Alarm

  This program uses a non-blocking timer (millis()) to monitor a
  magnetic reed switch. If the switch is open (door open) for more
  than 30 seconds, a buzzer will start beeping every half-second
  until the door is closed again.

  This is the final, recommended version of the code.

  Wiring:
  - Buzzer Wire 1 -> Pin 9
  - Buzzer Wire 2 -> GND
  - Reed Switch Wire 1 -> Pin 2
  - Reed Switch Wire 2 -> GND
*/

// Define the pins for our components.
const int buzzerPin = 9;
const int switchPin = 2;

// --- Timer and Delay Settings ---
// The main delay before the alarm triggers (30 seconds = 30,000 milliseconds).
const long alarmTriggerDelay = 30000L; 

// The interval for the beeping sound (1 second = 1000ms).
const long beepInterval = 1000L;

// --- State Tracking Variables ---
// Stores the time (from millis()) when the door was first opened.
unsigned long doorOpenTime = 0;

// Stores the time when the last beep occurred.
unsigned long lastBeepTime = 0;

// Tracks if the door is currently open or closed.
boolean doorIsOpen = false;

// Tracks the state of the buzzer (is it currently on or off?).
boolean buzzerIsOn = false;


void setup() {
  // Set the buzzer pin to be an OUTPUT.
  pinMode(buzzerPin, OUTPUT);

  // Set the switch pin to be an INPUT_PULLUP.
  // This means: LOW = door closed, HIGH = door open.
  pinMode(switchPin, INPUT_PULLUP);
}

void loop() {
  // Get the current time from the Arduino's internal "stopwatch".
  unsigned long currentTime = millis();
  
  // Read the current state of the reed switch.
  int switchState = digitalRead(switchPin);

  // --- LOGIC FOR WHEN THE DOOR IS OPEN ---
  if (switchState == HIGH) {
    // This 'if' statement runs only ONCE when the door is first opened.
    if (doorIsOpen == false) {
      doorIsOpen = true;
      // Record the exact time the door was opened.
      doorOpenTime = currentTime;
    }

    // Check if the door has been open longer than our trigger delay.
    if (currentTime - doorOpenTime > alarmTriggerDelay) {
      // It's time to start beeping!
      // This checks if it's time to toggle the beep sound.
      if (currentTime - lastBeepTime > beepInterval) {
        lastBeepTime = currentTime; // Reset the beep timer.
        
        // If the buzzer is on, turn it off. If it's off, turn it on.
        if (buzzerIsOn) {
          noTone(buzzerPin);
          buzzerIsOn = false;
        } else {
          tone(buzzerPin, 1500); // Beep at 1500 Hz
          buzzerIsOn = true;
        }
      }
    }
  } 
  // --- LOGIC FOR WHEN THE DOOR IS CLOSED ---
  else { 
    // If the door is closed, reset everything.
    doorIsOpen = false;
    
    // Make sure the buzzer is off.
    if (buzzerIsOn) {
      noTone(buzzerPin);
      buzzerIsOn = false;
    }
  }
}
