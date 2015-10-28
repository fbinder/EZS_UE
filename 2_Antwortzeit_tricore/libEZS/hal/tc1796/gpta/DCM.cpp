#include "DCM.h"

template<> const dcmRegPointer_t DCM<0>::pDcmRegs[4] = {
    {
        &(((GPTA0_t *) GPTA0_BASE)->DCM[0].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[0].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[0].CAV),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[0].COV),
        &(((GPTA0_t *) GPTA0_BASE)->SRSC0),
        &(((GPTA0_t *) GPTA0_BASE)->SRSS0),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->DCM[1].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[1].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[1].CAV),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[1].COV),
        &(((GPTA0_t *) GPTA0_BASE)->SRSC0),
        &(((GPTA0_t *) GPTA0_BASE)->SRSS0),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->DCM[2].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[2].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[2].CAV),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[2].COV),
        &(((GPTA0_t *) GPTA0_BASE)->SRSC0),
        &(((GPTA0_t *) GPTA0_BASE)->SRSS0),
    },
    {
        &(((GPTA0_t *) GPTA0_BASE)->DCM[3].CTR),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[3].TIM),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[3].CAV),
        &(((GPTA0_t *) GPTA0_BASE)->DCM[3].COV),
        &(((GPTA0_t *) GPTA0_BASE)->SRSC0),
        &(((GPTA0_t *) GPTA0_BASE)->SRSS0),
    },
};

template<> const dcmRegPointer_t DCM<1>::pDcmRegs[4] = {
    {
        &(((GPTA1_t *) GPTA1_BASE)->DCM[0].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[0].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[0].CAV),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[0].COV),
        &(((GPTA1_t *) GPTA1_BASE)->SRSC0),
        &(((GPTA1_t *) GPTA1_BASE)->SRSS0),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->DCM[1].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[1].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[1].CAV),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[1].COV),
        &(((GPTA1_t *) GPTA1_BASE)->SRSC0),
        &(((GPTA1_t *) GPTA1_BASE)->SRSS0),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->DCM[2].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[2].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[2].CAV),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[2].COV),
        &(((GPTA1_t *) GPTA1_BASE)->SRSC0),
        &(((GPTA1_t *) GPTA1_BASE)->SRSS0),
    },
    {
        &(((GPTA1_t *) GPTA1_BASE)->DCM[3].CTR),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[3].TIM),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[3].CAV),
        &(((GPTA1_t *) GPTA1_BASE)->DCM[3].COV),
        &(((GPTA1_t *) GPTA1_BASE)->SRSC0),
        &(((GPTA1_t *) GPTA1_BASE)->SRSS0),
    },
};
