/*====================================================================
* Function: Basic abstraction for TriCore TC1796 IS
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: IS.h 3159 2010-09-13 11:02:47Z siandeis $
*====================================================================*/
#ifndef IS_H_INCLUDED
#define IS_H_INCLUDED


/** Calculate node number from DCM number */
#define IS_GET_NODE_DCM(x) (x)
/** Calculate node number from PLL number */
#define IS_GET_NODE_PLL(x) (4)
/** Calculate node number from GT number */
#define IS_GET_NODE_GT(x) (5)
/** Calculate node number from LTC number */
#define IS_GET_NODE_LTC(x) (22 + (x >> 2))
/** Calculate node number from GTC number */
#define IS_GET_NODE_GTC(x) (6 + (x >> 1))


enum isNodeTOS_t
{
    IRQ_HANDLED_BY_CPU = 0,
    IRQ_HANDLED_BY_PCP = 1
};


#define TD_IS template<uint8_t _GPTA_ID>
#define PL_IS IS<_GPTA_ID>

TD_IS class IS
{
    public:
        /**
         * \brief Enable and configure a service request node
         * \param node_number Number of the service request node (0-37). This can be
         *        determined by using the IS_GET_NODE_(DCM,PLL,GT,LTC,GTC) macros.
         * \param tos Type of service (IRQ_HANDLED_BY_CPU or IRQ_HANDLED_BY_PCP)
         * \param srpn Priority (Vector) number
         * \warning Please consult the Manual to find out about the interrupt sharing
         *          mechanisms used in the Tricore GPTAs.
         */
        static void enableNode(uint8_t node_number, isNodeTOS_t tos, uint8_t srpn);

        /**
         * \brief Disable a service request node
         * \param node_number Number of the service request node (0-37). This can be
         *        determined by using the IS_GET_NODE_(DCM,PLL,GT,LTC,GTC) macros.
         */
        static void disableNode(uint8_t node_number);
};


#ifdef _OS_PXROS

TD_IS void PL_IS::enableNode(uint8_t node_number, isNodeTOS_t tos, uint8_t srpn)
{

#ifdef _OS_PXROS
    GPTAn_SRCm_t_nonv src;

    src.reg = 0;
    src.bits.TOS = tos;
    src.bits.SRPN = srpn;
    src.bits.SRE = 1;

    GPTA0_t* pGPTA0 = (GPTA0_t *) GPTA0_BASE;
    GPTA1_t* pGPTA1 = (GPTA1_t *) GPTA1_BASE;

    switch(_GPTA_ID)
    {
        case 0:
            *((GPTAn_SRCm_t_nonv*)((unsigned long) &(pGPTA0->SRC00) - 4 * node_number)) = src;
            break;

        case 1:
            *((GPTAn_SRCm_t_nonv*)((unsigned long) &(pGPTA1->SRC00) - 4 * node_number)) = src;
            break;

        default:
            osPanic();
    }
#endif // _OS_CIAO
}


TD_IS void PL_IS::disableNode(uint8_t node_number)
{
    GPTA0_t* pGPTA0 = (GPTA0_t *) GPTA0_BASE;
    GPTA1_t* pGPTA1 = (GPTA1_t *) GPTA1_BASE;

    switch(_GPTA_ID)
    {
        case 0:
            ((GPTAn_SRCm_t*)((unsigned long) &(pGPTA0->SRC00) - 4 * node_number))->bits.SRE = 0;
            break;

        case 1:
            ((GPTAn_SRCm_t*)((unsigned long) &(pGPTA1->SRC00) - 4 * node_number))->bits.SRE = 0;
            break;

        default:
            osPanic();
    }
}

#endif // _OS_CIAO_DISABLE

#endif // IS_H_INCLUDED
