/*====================================================================
* Function: Basic abstraction for TriCore TC1796 GPIO
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: GPIO.h 3561 2011-02-24 14:41:39Z ulbrich $
*====================================================================*/
#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include <stdint.h>

#define GPIO_FUNCTION_INPUT_NOPULL      0
#define GPIO_FUNCTION_INPUT_PULLUP      2
#define GPIO_FUNCTION_INPUT_PULLDOWN    1
#define GPIO_FUNCTION_INPUT_DEFAULT     GPIO_FUNCTION_INPUT_PULLUP

#define GPIO_FUNCTION_OUTPUT        8
#define GPIO_FUNCTION_ALTERNATE1    9
#define GPIO_FUNCTION_ALTERNATE2    10
#define GPIO_FUNCTION_ALTERNATE3    11

/** This one needs to be |'ed with another output function */
#define GPIO_FUNCTION_OUTPUT_OPEN_DRAIN 4

#define GPIO_FUNCTION_GPTA0 GPIO_FUNCTION_ALTERNATE1
#define GPIO_FUNCTION_GPTA1 GPIO_FUNCTION_ALTERNATE2
#define GPIO_FUNCTION_LTCA2 GPIO_FUNCTION_ALTERNATE3
#define GPIO_FUNCTION_SSC0  GPIO_FUNCTION_ALTERNATE1
#define GPIO_FUNCTION_SSC1  GPIO_FUNCTION_ALTERNATE2
#define GPIO_FUNCTION_SSC01 GPIO_FUNCTION_ALTERNATE3
#define GPIO_FUNCTION_ASC0  GPIO_FUNCTION_ALTERNATE1
#define GPIO_FUNCTION_ASC1  GPIO_FUNCTION_ALTERNATE2
#define GPIO_FUNCTION_MSC   GPIO_FUNCTION_ALTERNATE1
#define GPIO_FUNCTION_MLI   GPIO_FUNCTION_ALTERNATE2
#define GPIO_FUNCTION_SSC   GPIO_FUNCTION_ALTERNATE1
#define GPIO_FUNCTION_ADC   GPIO_FUNCTION_ALTERNATE1

#define IO_PORT(x) ((           x <=  7) ? 2 : \
                    (x >=  8 && x <= 23) ? 3 : \
                    (x >= 24 && x <= 39) ? 4 : \
                    (x >= 40 && x <= 47) ? 8 : \
                    (x >= 48 && x <= 55) ? 9 : 42)

#define IO_PIN(x)  ((           x <=  7) ? (x+8) : \
                    (x >=  8 && x <= 23) ? (x-8) : \
                    (x >= 24 && x <= 39) ? (x-24) : \
                    (x >= 40 && x <= 47) ? (x-40) : \
                    (x >= 48 && x <= 55) ? (x-48) : 42)



class GPIO
{
    public:
        /**
         * \brief Initialise the GPIO pins
         *
         * This sets all pins to output mode and clears the output
         */
        static void init();

        /**
         * \brief Set/Clear port output bits
         * \param port Port
         * \param pin Pin
         * \param out 0: reset output, >0: set output
         */
        static void setOutput(uint8_t port, uint8_t pin, uint8_t out);

        /**
         * \brief Read port input bits
         * \param port Port
         * \param pin Pin
         * \return 0/1
         */
        static int getInput(uint8_t port, uint8_t pin);

        /**
         * \brief Read port output bits
         * \param port Port
         * \param pin Pin
         * \return 0/1
         */
        static int getOutput(uint8_t port, uint8_t pin);

        /**
         * \brief Set the function of a port pin
         * \param port Port number (you might want to use the IO_PORT macro)
         * \param pin Pin number (you might want to use the IO_PIN macro)
         * \param function Function code (use the GPIO_FUNCTION_* macros)
         * \return false if requested port pin is invalid, true otherwise
         */
        static bool setFunction(uint8_t port, uint8_t pin, uint8_t function);
};

#endif // GPIO_H_INCLUDED
