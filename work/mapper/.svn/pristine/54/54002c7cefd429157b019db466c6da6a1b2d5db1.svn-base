/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node.cpp
 * Author: wubowen2
 * 
 * Created on March 20, 2018, 4:29 PM
 */

#include "node.h"

node::node() {
    intersectionID=0;
    from_segID=0;
    previous_node=0;
    weight=0;
    visited=0;
}

node::node(unsigned Int, unsigned seg, unsigned previous, double Weight){
    
    intersectionID=Int;
    from_segID=seg;
    previous_node=previous;
    weight=Weight;
    visited=0;
}

node::~node() {
}

bool node::operator==(const node&  rhs){
    if(intersectionID==rhs.intersectionID&&from_segID==rhs.from_segID&&previous_node==rhs.previous_node&&weight==rhs.weight&&visited==rhs.visited) return true;
    return false;
    
    
}

bool node::operator!=(const node& rhs){
    if(intersectionID!=rhs.intersectionID||from_segID!=rhs.from_segID||previous_node!=rhs.previous_node||weight!=rhs.weight||visited!=rhs.visited) return true;
    else return false;
}