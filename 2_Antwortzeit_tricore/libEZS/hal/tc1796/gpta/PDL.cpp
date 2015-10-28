#include "PDL.h"

template<> GPTAn_PDLCTR_t* const PDL<0>::m_pControlReg[2] = { &((GPTA0_t *)GPTA0_BASE)->PDLCTR, &((GPTA0_t *)GPTA0_BASE)->PDLCTR };
template<> GPTAn_PDLCTR_t* const PDL<1>::m_pControlReg[2] = { &((GPTA1_t *)GPTA1_BASE)->PDLCTR, &((GPTA1_t *)GPTA1_BASE)->PDLCTR };
