//
// Created by asice on 2024/7/22.
//

#ifndef DBQA_GROVER_H
#define DBQA_GROVER_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <set>
#include "Qbit.h"


void singleGrover(unsigned, unsigned = 0);

void multipleGrover(unsigned, std::set<unsigned> = {});
#endif //DBQA_GROVER_H
