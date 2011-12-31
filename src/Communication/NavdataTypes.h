/*
 * NavdataTypes.h
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#ifndef NAVDATATYPES_H_
#define NAVDATATYPES_H_

typedef struct navdata_header_t {
	unsigned int droneState;
	unsigned int sequenceNumber;
	unsigned int visionFlag;
} navdata_header;

typedef struct navdata_option_t {
	unsigned short tag;
	unsigned short size;
	char data[];
} navdata_option;

typedef struct navdata_checksum_t {
	unsigned short id;
	unsigned short size;
	int cks_data;
} navdata_checksum;

typedef struct navdata_demo_t {
	unsigned short 	tag;
	unsigned short 	size;
	unsigned int 	ctrl_state;
	unsigned int 	battery;
	float 			pitch;
	float			roll;
	float			yaw;
	float			altitude;
	float			vx;
	float			vy;
	float			vz;
	unsigned int	num_frames; // Not used -> To integrate in video stage.

	//Camera parameters compute by detection
	float			detection_camera_rot[9];
	float			detection_camera_trans[3];
	unsigned int	detection_tag_index;
	unsigned int	detection_camera_type;

	// Camera parameters compute by drone
	float			drone_camera_rot[9];
	float			drone_camera_trans[3];
} navdata_demo;

typedef struct navdata_t {
	navdata_header header;
	unsigned short tag;
	unsigned short size;
	navdata_demo demo;
} navdata;

#endif /* NAVDATATYPES_H_ */
