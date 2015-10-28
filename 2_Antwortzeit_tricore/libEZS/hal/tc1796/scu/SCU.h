/*====================================================================
* Function: Base class for TriCore SCU configuration
*
* Description:
*
*
* Copyright I4Copter Project 2011
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Tobias, Peter
*====================================================================
* $Id: $
*====================================================================*/
#include <tc1796b/scu.h>

class SCU{
    SCU(){
    };
public:
    static void setupExtREQ(uint8_t eReq){
        if(eReq == 3){
          EICRn_t *EICR1 = reinterpret_cast<EICRn_t*>(EICR1_ADDR);
          //enable falling edge trigger
          EICR1->reg = 0x19000000;
//            EICR1->reg = 0x1A000000;
        }
    }

};
