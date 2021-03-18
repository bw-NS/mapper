/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_helper_H.h
 * Author: haojinze
 *
 * Created on April 2, 2018, 3:40 PM
 */

#ifndef M4_HELPER_H_H
#define M4_HELPER_H_H

#include "m4.h"
#include "m3.h"
#include "m3_function_declaration.h"
#include "cmath"
#include <vector>

bool change_valid(unsigned from, unsigned to, std::vector<unsigned> &pickList, 
    std::vector<unsigned> &dropList, std::vector<unsigned> &deliverySequence);
std::vector<unsigned> calculate_path(unsigned depotFrom, unsigned depotTo, 
    std::vector<unsigned> &deliverySequence, double turn_penalty, bool segmentValid);

#endif /* M4_HELPER_H_H */

