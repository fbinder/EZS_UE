#include "FPC.h"

template<> const fpcRegPointer_t FPC<0>::pFpcRegs[6] = {
    {
        &(((GPTA0_t *) GPTA0_BASE)->FPC[0].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->FPC[0].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->FPCSTAT),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->FPC[1].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->FPC[1].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->FPCSTAT),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->FPC[2].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->FPC[2].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->FPCSTAT),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->FPC[3].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->FPC[3].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->FPCSTAT),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->FPC[4].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->FPC[4].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->FPCSTAT),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->FPC[5].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->FPC[5].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->FPCSTAT),
    },
};

template<> const fpcRegPointer_t FPC<1>::pFpcRegs[6] = {
    {
        &(((GPTA1_t *) GPTA1_BASE)->FPC[0].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->FPC[0].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->FPCSTAT),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->FPC[1].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->FPC[1].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->FPCSTAT),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->FPC[2].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->FPC[2].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->FPCSTAT),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->FPC[3].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->FPC[3].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->FPCSTAT),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->FPC[4].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->FPC[4].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->FPCSTAT),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->FPC[5].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->FPC[5].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->FPCSTAT),
    },
};
