// Define the pins for each lane's traffic lights
const int Lane1[] = {11, 12, 13}; // Green, Yellow, Red
const int Lane2[] = {8, 9, 10};   // Green, Yellow, Red
const int Lane3[] = {7, 6, 5};    // Green, Yellow, Red
const int Lane4[] = {4, 3, 2};    // Green, Yellow, Red

// Vehicle densities for each lane
int densities[4] = {0, 0, 0, 0};

// Timing constants
const unsigned long maxGreenTime = 10000; // Max green light time
const unsigned long minGreenTime = 1000;  // Min green light time
const unsigned long yellowTime = 2000;    // Yellow light duration

// Array to store calculated green times for each lane
unsigned long greenTimes[4];

// Function prototypes
void redLight(int lane);
void yellowLight(int lane1, int lane2);
void greenLight(int lane);
void calculateGreenTimes();
void manageTrafficLights();
const int* getLane(int index);

void setup() {
  // Initialize all traffic light pins as OUTPUT and set them to Red
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(getLane(i)[j], OUTPUT);
      digitalWrite(getLane(i)[2], HIGH); // Set all lanes to Red initially
    }
  }

  // Initialize serial communication
  Serial.begin(9600);

  // Debug statement
  Serial.println("Arduino initialized. Waiting for density data...");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming data
    String densityString = Serial.readStringUntil('\n');
    Serial.println("Received: " + densityString);  // Debug statement

    // Parse the density data
    int index = 0;
    int start = 0;
    int end;

    // Clear previous density values
    for (int i = 0; i < 4; i++) {
      densities[i] = 0;
    }

    while (index < 4) {
      end = densityString.indexOf(' ', start);
      if (end == -1) { 
        densities[index] = densityString.substring(start).toInt();
        break;
      }
      densities[index] = densityString.substring(start, end).toInt();
      start = end + 1;
      index++;
    }

    // Debug statement to print parsed densities
    Serial.print("Parsed Densities: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(densities[i]);
      Serial.print(" ");
    }
    Serial.println();

    // Manage traffic lights based on the new densities
    manageTrafficLights();
  }
}

const int* getLane(int index) {
  switch (index) {
    case 0:
      return Lane1;
    case 1:
      return Lane2;
    case 2:
      return Lane3;
    case 3:
      return Lane4;
    default:
      return NULL;
  }
}

void redLight(int lane) {
  const int* lanePins = getLane(lane);
  digitalWrite(lanePins[0], LOW);  // Green OFF
  digitalWrite(lanePins[1], LOW);  // Yellow OFF
  digitalWrite(lanePins[2], HIGH); // Red ON
}

void yellowLight(int lane1, int lane2) {
  const int* lane1Pins = getLane(lane1);
  const int* lane2Pins = getLane(lane2);

  // Set both lanes to Yellow
  digitalWrite(lane1Pins[0], LOW);  // Green OFF
  digitalWrite(lane1Pins[1], HIGH); // Yellow ON
  digitalWrite(lane1Pins[2], LOW);  // Red OFF

  digitalWrite(lane2Pins[0], LOW);  // Green OFF
  digitalWrite(lane2Pins[1], HIGH); // Yellow ON
  digitalWrite(lane2Pins[2], LOW);  // Red OFF

  delay(yellowTime);

  // After Yellow duration, set lane1 to Red and lane2 to Green
  redLight(lane1);
  greenLight(lane2);
}

void greenLight(int lane) {
  const int* lanePins = getLane(lane);
  digitalWrite(lanePins[0], HIGH); // Green ON
  digitalWrite(lanePins[1], LOW);  // Yellow OFF
  digitalWrite(lanePins[2], LOW);  // Red OFF
}

void calculateGreenTimes() {
  int totalDensity = 0;
  for (int i = 0; i < 4; i++) {
    totalDensity += densities[i];
  }

  for (int i = 0; i < 4; i++) {
    greenTimes[i] = (densities[i] > 0)
                        ? map(densities[i], 0, totalDensity, minGreenTime, maxGreenTime)
                        : minGreenTime;
    greenTimes[i] = constrain(greenTimes[i], minGreenTime, maxGreenTime);
  }
}

void manageTrafficLights() {
  calculateGreenTimes();

  // Sort lanes by density in descending order and store indices
  int sortedLanes[4] = {0, 1, 2, 3};
  for (int i = 0; i < 3; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (densities[sortedLanes[i]] < densities[sortedLanes[j]]) {
        int temp = sortedLanes[i];
        sortedLanes[i] = sortedLanes[j];
        sortedLanes[j] = temp;
      }
    }
  }

  // Manage traffic lights based on sorted densities
  for (int i = 0; i < 4; i++) {
    int currentLane = sortedLanes[i];
    int nextLane = sortedLanes[(i + 1) % 4];

    greenLight(currentLane);
    delay(greenTimes[currentLane]);

    yellowLight(currentLane, nextLane);
  }
}