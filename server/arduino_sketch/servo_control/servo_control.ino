// Erstellt:  Dennis Rump & Rene Galow
//            26.05.2014
//            ESD Projekt

#include <Servo.h>
//#include <DueTimer.h>

#define SERVOS_MAX 1
#define PROTOKOLL_LENGTH 6  //ServoGrouipID(1Byte), X-Value(1Byte), Y-Value(1Byte), 0xff, 0x0, 0xff
#define STEP_WITDTH 1

struct servoGroup {
	Servo servoX;
	Servo servoY;
	unsigned char desiredX;
	unsigned char desiredY;
};
servoGroup servosGroups[SERVOS_MAX];

char inData[PROTOKOLL_LENGTH]; // Allocate some space for the string
unsigned char outData[10];
int dataIndex = 0; // Index into array; where to store the character

int pos = 0;    // variable to store the servo position

void servoAnnaehern() {
	for (int i = 0; i < SERVOS_MAX; i++) {
		if (servosGroups[i].desiredX > servosGroups[i].servoX.read())
			servosGroups[i].servoX.write(
					servosGroups[i].servoX.read() + STEP_WITDTH);
		if (servosGroups[i].desiredX < servosGroups[i].servoX.read())
			servosGroups[i].servoX.write(
					servosGroups[i].servoX.read() - STEP_WITDTH);

		if (servosGroups[i].desiredY > servosGroups[i].servoY.read())
			servosGroups[i].servoY.write(
					servosGroups[i].servoY.read() + STEP_WITDTH);
		if (servosGroups[i].desiredY < servosGroups[i].servoY.read())
			servosGroups[i].servoY.write(
					servosGroups[i].servoY.read() - STEP_WITDTH);
	}
}

//TC2 ch 0
void TC6_Handler()
{
        TC_GetStatus(TC2, 0);
        	for (int i = 0; i < SERVOS_MAX; i++) {
		if (servosGroups[i].desiredX > servosGroups[i].servoX.read())
			servosGroups[i].servoX.write(
					servosGroups[i].servoX.read() + STEP_WITDTH);
		if (servosGroups[i].desiredX < servosGroups[i].servoX.read())
			servosGroups[i].servoX.write(
					servosGroups[i].servoX.read() - STEP_WITDTH);

		if (servosGroups[i].desiredY > servosGroups[i].servoY.read())
			servosGroups[i].servoY.write(
					servosGroups[i].servoY.read() + STEP_WITDTH);
		if (servosGroups[i].desiredY < servosGroups[i].servoY.read())
			servosGroups[i].servoY.write(
					servosGroups[i].servoY.read() - STEP_WITDTH);
	}
}

void setup() {
	Serial.begin(115200);
	while (!Serial);

	servosGroups[0].servoX.attach(9); // attaches the servo on pin 9 to the servo object
	servosGroups[0].servoY.attach(10);

	for (int i = 0; i < SERVOS_MAX; i++) {
		servosGroups[i].servoX.write(90);  //mittelstellung
		servosGroups[i].servoY.write(90);  //mittelstellung
		servosGroups[i].desiredX = 90;  //mittelstellung
		servosGroups[i].desiredY = 90;  //mittelstellung
	}

	// isr alle 50ms
	//Timer.getAvailable().attachInterrupt(servoAnnaehern).start(50000);
        startTimer(TC2, 0, TC6_IRQn, 20); //TC1 channel 0, the IRQ for that channel and the desired frequency

	// geben wir dem arduino etwas zeit ;)
	delay(50);
}

void loop() {
	while (Serial.available() > 0) // Don't read unless
								   // there you know there is data
	{
		if (dataIndex < PROTOKOLL_LENGTH) // One less than the size of the array
		{
			inData[dataIndex] = Serial.read();
			dataIndex++; // Increment where to write next
		}
	}
	if ((dataIndex > 3) && (inData[PROTOKOLL_LENGTH - 3] == 0xff)
			&& (inData[PROTOKOLL_LENGTH - 3] == 0x0)
			&& (inData[PROTOKOLL_LENGTH - 3] == 0xff)) {
		// handelt es sich um eine gültige uebertragung?
		if (dataIndex == PROTOKOLL_LENGTH) {
			if (inData[0] < SERVOS_MAX) {
				if ((inData[1] < 180) && (inData[2] < 180)) {
					servosGroups[inData[0]].desiredX = inData[1];
					servosGroups[inData[0]].desiredY = inData[2];
					outData[0] = 0xfb;
					outData[1] = 0xff;
					outData[2] = 0x0;
					outData[3] = 0xff;
					Serial.write(outData, 4);
				}
			}
		}  // alive check
		else if ((dataIndex == 4) && (inData[0] == 'a')) {
			outData[0] = 'b';
			outData[1] = 0xff;
			outData[2] = 0x0;
			outData[3] = 0xff;
			Serial.write(outData, 4);
		}
	}
	// wieder auf anfang
	dataIndex = 0;
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
}
