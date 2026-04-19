#pragma once
#include "SA.h"
#include "problem.h"
#include "solution.h"

class GeneticAlgorithm
{
public:
    static Solution run(const Problem& p);
};