/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   intersection.cpp
 * Author: wubowen2
 * 
 * Created on April 2, 2018, 6:59 PM
 */

#include "intersection.h"

intersection::intersection() {
    
    pickup=0;
    dropoff=0;
    Copickup={};
    Codropoff={};
}

intersection::intersection(const intersection& orig) {
    
    pickup=orig.pickup;
    dropoff=orig.dropoff;
    Copickup=orig.Copickup;
    Codropoff=orig.Codropoff;
}

intersection::~intersection() {
}

intersection::intersection( bool p, bool dp, std::vector<unsigned> Cod, std::vector<unsigned> Cop){
    
    pickup=p;
    dropoff=dp;
    Copickup=Cop;
    Codropoff=Cod;
    
    
    
}

//note : these two judgments are not fully defined,  the limited use will cover the undefined part of the function
bool intersection::operator==(const intersection&  rhs){
    if(pickup==rhs.pickup&&dropoff==rhs.dropoff) return true;
    return false;
    
    
}

bool intersection::operator!=(const intersection& rhs){
    if(pickup!=rhs.pickup||dropoff!=rhs.dropoff) return true;
    else return false;
}

