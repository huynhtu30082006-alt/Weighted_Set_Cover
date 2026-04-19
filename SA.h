#pragma once
#include "GA.h"
#include "problem.h"
#include "solution.h"

class SimulatedAnnealing
{
public:
    static Solution run(const Problem& p);
};