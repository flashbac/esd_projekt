/*
 * settings.h
 *
 *  Created on: 01.06.2014
 *      Author: dennis
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define SerialPortForServoGroupCommunication "/dev/ttyS0"
//#define SerialPortForServoGroupCommunication "/dev/ttyACM0"
//#define SerialPortForServoGroupCommunication "/dev/ttyUSB0"
//#define SerialPortForServoGroupCommunication "/dev/ttymxc3"
#define AvailabeServoGroups 1


struct CAMandServoGroup{
	int camID;
	int ServoGroup;
};

// init -> {{CamID,ServoGroupID},{CamID.ServoGroupID}...}
//CAMandServoGroup CAMtoServoGroupMapping[AvailabeServoGroups] = {{0,0}};



#endif /* SETTINGS_H_ */
