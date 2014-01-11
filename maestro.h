/* 
 * File:   maestro.h
 * Author: mark
 *
 * Created on 11 January 2014, 17:25
 */

#ifndef MAESTRO_H
#define	MAESTRO_H

#include <vector>
#include <stdint.h>

#define MAESTRO_DEFAULT_DEVICE "/dev/ttyACM0"

class MAESTRO;
class CHANNEL;

class MAESTRO {
private:
	~MAESTRO();
	const int fd;
	std::vector<CHANNEL> channels;
	friend CHANNEL;
public:
	MAESTRO(const char * const device, const uint8_t channels);
	MAESTRO(const uint8_t channels);
	const CHANNEL& operator [] (const uint8_t channel) const
	{
		return channels[channel];
	}
};

class CHANNEL {
private:
	const MAESTRO& owner;
	const uint8_t index;
	friend MAESTRO;
	CHANNEL(const MAESTRO& owner, const uint8_t index);
public:
	/* Get the raw position of a motor (quarter-microseconds) */
	uint16_t getQU() const;
	/* Set the raw position of a motor in 0.25us units */
	void setQU(const uint16_t target) const;
	/* Set the motor speed in 0.25us/10ms units */
	void setSpeedQU10M(const uint16_t target) const;
	/* Set the motor speed in 0.25us / d(seconds) units */
	void setSpeedQU_t(const float dqu, const float dt) const;
	/* Set the motor acceleration control in 0.25us/10ms/80ms units */
	void setAccelerationQU10M80M(const uint8_t value) const;
	//	float angle() const;
	//	void move(float theta, float omega = 0) const;
	//	void moveIn(float theta, float time) const;
};

#endif	/* MAESTRO_H */

