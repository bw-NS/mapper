/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m3_function_declaration.h
 * Author: haojinze
 *
 * Created on March 19, 2018, 12:59 AM
 */

#ifndef M3_FUNCTION_DECLARATION_H
#define M3_FUNCTION_DECLARATION_H

#include "StreetsDatabaseAPI.h"
#include <vector>
#include <algorithm>
#include "unordered_map"
#include <queue>
#include "m1_function_declaration.h"
#include "node.h"

std::vector<unsigned> available_intersections(unsigned current_int, const std::vector<unsigned>& visited_list);
std::vector<unsigned> get_available_seg_id(unsigned current, std::vector<unsigned>& avail_int);
std::unordered_map<unsigned, unsigned> find_cloest_int_to_POI(std::vector<unsigned>POI_name_vec);
std::vector<unsigned> available_segs_for_node_seg(unsigned intersection_id,const std::unordered_map<unsigned,unsigned>& visited_list);
std::vector<unsigned> available_segs(unsigned intersection_id,const std::unordered_map<unsigned,node>& visited_list);
std::vector<unsigned> available_intersection(unsigned current_int, const std::unordered_map<unsigned,unsigned>& visited_list);

#endif /* M3_FUNCTION_DECLARATION_H */

