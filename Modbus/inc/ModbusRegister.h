#ifndef MODBUSREGISTER_H_
#define MODBUSREGISTER_H_

#include "ModbusLeader.h"

class ModbusRegister {
public:
	ModbusRegister(ModbusMaster *master, int address, bool holdingRegister = true);
	ModbusRegister(const ModbusRegister &)  = delete;
	virtual ~ModbusRegister();
	int read();
	void write(int value);
private:
	ModbusMaster *m;
	int addr;
	bool hr;
};

#endif /* MODBUSREGISTER_H_ */
