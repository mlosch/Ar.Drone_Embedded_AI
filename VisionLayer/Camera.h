/*
 * Camera.h
 *
 *  Created on: 14.01.2012
 *      Author: max
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "../Common/Types.h"
#include <string>

class Camera {
public:
	Camera(std::string device, uint16_t width, uint16_t height);
	virtual ~Camera();
};

#endif /* CAMERA_H_ */
