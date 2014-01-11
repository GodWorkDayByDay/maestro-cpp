/*
 *  The Maestro's serial mode MUST be set to "USB Dual Port"
 */

#include "maestro.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>

using namespace std;

/* Open a Maestro device with the specified number of channels */
MAESTRO::MAESTRO(const char * const device, const uint8_t channels) : fd(open(device, O_RDWR | O_NOCTTY))
{
	if (fd == -1) {
		throw "Failed to open Maestro device";
	}
	for (uint8_t i = 0; i < channels; i++) {
		this->channels.push_back(CHANNEL(*this, i));
	}
}

/* Open the default Maestro device with the specified number of channels */
MAESTRO::MAESTRO(const uint8_t channels) : MAESTRO(MAESTRO_DEFAULT_DEVICE, channels)
{
}

/* Close the maestro device */
MAESTRO::~MAESTRO()
{
	close(fd);
}

/* Create a channel wrapper */
CHANNEL::CHANNEL(const MAESTRO& owner, const uint8_t index) : owner(owner), index(index)
{
}

uint16_t CHANNEL::getQU() const
{
	uint8_t request[] = {0x90, index};
	if (write(owner.fd, request, sizeof(request)) == -1) {
		throw "Failed to get position (write failed)";
	}
	uint16_t response;
	if (read(owner.fd, (void*) &response, sizeof(response)) != sizeof(response)) {
		throw "Failed to get position (read failed)";
	}
	return response;
}

void CHANNEL::setQU(const uint16_t target) const
{
	uint8_t request[] = {0x84, index, (uint8_t) (target & 0x7f), (uint8_t) (target >> 7 & 0x7f)};
	if (write(owner.fd, request, sizeof(request)) == -1) {
		throw "Failed to set position (write failed)";
	}
}

void CHANNEL::setSpeedQU10M(const uint16_t value) const
{
	uint8_t request[] = {0x84, index, (uint8_t) (value & 0x7f), (uint8_t) (value >> 7 & 0x7f)};
	if (write(owner.fd, request, sizeof(request)) == -1) {
		throw "Failed to set speed (write failed)";
	}
}

void CHANNEL::setSpeedQU_t(const float dqu, const float dt) const
{
	float ddt = dqu / (dt * 100.0);
	uint16_t speed = (ddt > 0x3fff) ? 0x3fff : (ddt < 0) ? 0 : (uint16_t) ddt;
	setSpeedQU10M(speed);
}

void CHANNEL::setAccelerationQU10M80M(const uint8_t value) const
{
	uint8_t request[] = {0x89, index, (uint8_t) (value & 0x7f), (uint8_t) (value >> 7 & 0x7f)};
	if (write(owner.fd, request, sizeof(request)) == -1) {
		throw "Failed to set acceleration (write failed)";
	}
}
