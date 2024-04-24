#include <Arduino.h>
#include <mydebug.h>
#include <myfunction.h>
// #include <AsyncTCP.h>

// void onReceiveDebug(void *arg, AsyncClient *client, void *data, size_t len) {
// 	// DEBUGLOG("Receive %d car %.*s\n", len, len, (char*)data);
// 	switch (((char *)data)[0]) {
// 		case '0':
// 			break;
// 	}
// }

void ReadSerial() {
	static char cmd;
	if (Serial.available()) {
		Serial.readBytes(&cmd, 1);
		switch (cmd) {
			case '0':
				DEBUGLOG("Reset\n");
				WiFi.disconnect(false, true);
				ESP.restart();
				break;
			case 'r':
				DEBUGLOG("Restart\n");
				ESP.restart();
				break;
			case '?':
				DEBUGLOG("BSSID : %s\nSSID : %s\nPWD :%s\nIP : %s\nAuto : %s\n",
						 WiFi.BSSIDstr().c_str(),
						 WiFi.SSID().c_str(),
						 WiFi.psk().c_str(),
						 WiFi.localIP().toString().c_str(),
						 WiFi.getAutoReconnect()?"On":"Off");
				break;
			case 'w':
				myWPSStart();
				break;
			case 's':
				myWPSStop();
				break;
			case 't':
				xTaskCreate(myWPSStartTask, "WPSTask", 10000, NULL, 1, NULL);
				break;
			case 'd':
				DEBUGLOG("Event Handler %lu\n", myWPSWiFiEvent_ID);
				break;
		}
	}
}

void setup() {
	DEBUGINIT();
	// DEBUGSETDATAHANDLER(onReceiveDebug);
	loadIP();
	myWifiBeginWPS();
}

void loop() {
	static unsigned long memMillis;
	ReadSerial();

	if ((millis() - memMillis) > 1000) {
		memMillis = millis();
		// DEBUGLOG("%d / %d \n",millis(), memMillis);
		DEBUGLOG("%s", WiFi.isConnected() ? "." : "?");
	}
	delay(100);
}
