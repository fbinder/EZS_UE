/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file
 * \ingroup tc1796
 * \brief TC1796 specific interface for an analog device
 * \author Matthias Brischwein <matthias.brischwein@studium.uni-erlangen.de>
 * \addtogroup tc1796
 * @{
 */

#ifndef I2CDEVICEIMPL_H_
#define I2CDEVICEIMPL_H_

#include <stdint.h>

namespace hal { namespace tc1796 {

// Since the TC1796 has no I2C interface, this is just a dummy class
class I2CDeviceImpl {

public:
	void init(){ return; }

	uint32_t send(uint8_t address, uint8_t* data, uint32_t length){ return 0; };
	uint32_t receive(uint8_t address, uint8_t* data, uint32_t length){ return 0; };

private:
	I2CDeviceImpl(){};

};

// Why should I do this? Why is the way of I2C abstraction different from the way in the RS232 case?
extern I2CDeviceImpl i2cDeviceImpl;

}}

using namespace hal::tc1796;

#endif /* I2CDEVICEIMPL_H_ */
