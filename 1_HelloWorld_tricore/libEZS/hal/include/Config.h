/*
 * SPIConfig.h
 *
 *  Created on: Aug 21, 2012
 *      Author: hoffmann
 */

#ifndef SPICONFIG_H_
#define SPICONFIG_H_

namespace hal {
namespace spi {

	enum Mode_t {
		Master = 1,
		Slave,
		NotConfigured,
	};

	typedef void(*tx_callback)(void*);
	typedef void(*rx_callback)(void*);

} // spi

namespace digitaldevice {

	enum IRQDIR_t {
		Falling = 1,
		Rising,
		Rising_Falling,
	};

	enum PINDIR_t {
		Input = 1,
		Output,
	};

	typedef void(*callback)(void*);

} // digitaldevice

namespace dma {

	typedef void(*callback)(void*);

} // dma

} // hal

#endif /* SPICONFIG_H_ */
