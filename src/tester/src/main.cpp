#include <Arduino.h>

const int Data_In_Pins[]    = {22, 23, 24, 25};
const int Internal_Register_Pins[] = {26, 27, 28, 29, 30, 31, 32, 33};
const int MARDEC = 34;
const int MR = 35;
const int CP = 38;
const int Not_Store = 39;
const int NUM_REG_PINS = sizeof(Internal_Register_Pins) / sizeof(Internal_Register_Pins[0]);

// colors

const char* GREEN  = "\033[32m";
const char* RED    = "\033[31m";
const char* RESET  = "\033[0m";



void setData(uint16_t data)
{
    digitalWrite(CP, LOW);
    for (int i = 0; i < 4; i++)
        digitalWrite(Data_In_Pins[i], (data >> i) & 1);
    digitalWrite(CP, HIGH);
}

uint16_t getData()
{
    uint16_t value = 0;
    for (int i = 0; i < NUM_REG_PINS; i++)
        value |= ((uint16_t)digitalRead(Internal_Register_Pins[i]) << i);
    return value;
}

void resetMAR()
{
    digitalWrite(MR, HIGH);
    delay(10);
    digitalWrite(MR, LOW);
}

void testMAR(uint16_t data, bool dec, uint16_t expected)
{
    digitalWrite(MARDEC, dec);
    uint16_t before = getData();

    setData(data);
    digitalWrite(Not_Store, LOW);

    uint16_t result = getData();

    // Buffer for formatted output
    char buf[160];

    snprintf(buf, sizeof(buf),
        "| MAR: 0x%02X | Data: 0x%04X | Op: %s | Expected: 0x%04X | Got: 0x%04X |",
        before,
        data,
        (dec == 0) ? "Inc" : "Dec",
        expected,
        result
    );

    Serial.print(buf);
    if (result == expected) {
        Serial.print(GREEN);
        Serial.print( " OK " );
        Serial.print(RESET);
        Serial.println("|");
    } else {
        Serial.print(RED);
        Serial.print( " FAIL " );
        Serial.print(RESET);
        Serial.println("|");
    }
}

void setup()
{
    Serial.begin(115200);
    delay(200);

    for (int i = 0; i < 8; i++)
    {
        pinMode(Internal_Register_Pins[i], INPUT);
    }
    for (int i = 0; i < 4; i++) {
        pinMode(Data_In_Pins[i], OUTPUT);
    }

    pinMode(MARDEC, OUTPUT);
    pinMode(MR, OUTPUT);
    pinMode(CP, OUTPUT);
    pinMode(Not_Store, OUTPUT);
    delay(1000);

    // Set pins to default state
    digitalWrite(CP, LOW);
    digitalWrite(MARDEC, LOW);
    digitalWrite(Not_Store, LOW);

   uint32_t maxSteps = ((1UL << NUM_REG_PINS) - 1) / 15;

    // increment tests
    resetMAR();
    for (int i = 0; i < maxSteps; i++) {
        testMAR(15, 0, getData() + 15);
    }

    // decrement tests
    for (int i = maxSteps; i > 0; i--) {
        testMAR(15, 1, getData() - 15);
    }


}

void loop(){

}