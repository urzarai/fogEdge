// IoT Soil Monitoring System with Environmental Analysis
// Edge Computing Irrigation Control

// Pin Definitions
const int SOIL_MOISTURE_PIN = A0;   // Soil Moisture Sensor
const int TEMPERATURE_PIN = A1;     // Temperature Sensor
const int GAS_SENSOR_PIN = A2;      // Gas Sensor
const int WATER_PUMP_PIN = 7;       // Water Pump Control Pin

// Threshold Configuration
const int MOISTURE_THRESHOLD = 300;   // Soil moisture trigger point
const int GAS_THRESHOLD = 400;        // Gas level stress indicator
const float TEMP_THRESHOLD = 30.0;    // Temperature stress threshold

// Calibration Parameters
const float VOLTAGE_REF = 5.0;        // Reference voltage
const int ADC_RESOLUTION = 1023;      // 10-bit ADC resolution

// Logging and Debugging
const unsigned long SAMPLING_INTERVAL = 5000;  // 5 seconds between readings

// Data Logging Structure
struct SensorData {
  int soilMoisture;
  float temperature;
  int gasReading;
  bool irrigationStatus;
};

// Function Prototypes
float readTemperature();
int readSoilMoisture();
int readGasSensor();
bool shouldIrrigate(int moisture, float temp, int gas);
void controlIrrigation(bool irrigate);
void logSensorData(const SensorData& data);

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  
  // Set Pin Modes
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  
  // Initial Pump State - OFF
  digitalWrite(WATER_PUMP_PIN, LOW);
  
  // Welcome Message
  Serial.println("Edge Computing Irrigation Monitoring System");
  Serial.println("-------------------------------------------");
}

float readTemperature() {
  // Convert analog reading to temperature
  int tempReading = analogRead(TEMPERATURE_PIN);
  float voltage = tempReading * (VOLTAGE_REF / ADC_RESOLUTION);
  
  // Linear conversion for TMP sensor (typical for many temperature sensors)
  // Adjust slope and offset based on specific sensor characteristics
  float temperatureC = (voltage - 0.5) * 100.0;
  
  return temperatureC;
}

int readSoilMoisture() {
  // Read soil moisture sensor
  return analogRead(SOIL_MOISTURE_PIN);
}

int readGasSensor() {
  // Read environmental gas/stress sensor
  return analogRead(GAS_SENSOR_PIN);
}

bool shouldIrrigate(int moisture, float temp, int gas) {
  // Comprehensive irrigation decision logic
  bool lowMoisture = moisture < MOISTURE_THRESHOLD;
  bool highTemperature = temp > TEMP_THRESHOLD;
  bool highGasStress = gas > GAS_THRESHOLD;
  
  // Irrigation conditions:
  // 1. Low soil moisture is primary trigger
  // 2. High temperature increases irrigation likelihood
  // 3. Gas sensor indicates environmental stress
  return lowMoisture || (highTemperature && highGasStress);
}

void controlIrrigation(bool irrigate) {
  if (irrigate) {
    digitalWrite(WATER_PUMP_PIN, HIGH);  // Turn on water pump
    Serial.println("IRRIGATION ACTIVATED");
  } else {
    digitalWrite(WATER_PUMP_PIN, LOW);   // Turn off water pump
    Serial.println("No irrigation required");
  }
}

void logSensorData(const SensorData& data) {
  // Detailed sensor data logging
  Serial.println("\n--- Sensor Data Log ---");
  Serial.print("Soil Moisture: ");
  Serial.print(data.soilMoisture);
  Serial.print(" | Temperature: ");
  Serial.print(data.temperature);
  Serial.print("°C | Gas Reading: ");
  Serial.print(data.gasReading);
  Serial.print(" | Irrigation Status: ");
  Serial.println(data.irrigationStatus ? "REQUIRED" : "NOT NEEDED");
}

void loop() {
  // Read current sensor values
  int soilMoisture = readSoilMoisture();
  float temperature = readTemperature();
  int gasReading = readGasSensor();
  
  // Determine irrigation need
  bool irrigationNeeded = shouldIrrigate(soilMoisture, temperature, gasReading);
  
  // Create sensor data record
  SensorData currentData = {
    soilMoisture,
    temperature,
    gasReading,
    irrigationNeeded
  };
  
  // Log sensor data
  logSensorData(currentData);
  
  // Control irrigation based on conditions
  controlIrrigation(irrigationNeeded);
  
  // Wait before next reading
  delay(SAMPLING_INTERVAL);
}
