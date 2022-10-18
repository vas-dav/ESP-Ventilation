<<<<<<< HEAD
=======


>>>>>>> bada194ebe7490c25ca7589366224d4ae0f80eab
#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include "Uart.h"

class SerialPort {
public:
	SerialPort();
	virtual ~SerialPort();
	int available();
	void begin(int speed = 9600);
	int read();
	int write(const char* buf, int len);
	int print(int val, int format);
	void flush();
private:
	static LpcUart *u;
};

#endif /* SERIALPORT_H_ */
