/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   intersection.h
 * Author: wubowen2
 *
 * Created on April 2, 2018, 6:59 PM
 */

#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <vector>
class intersection {
public:
    
    
    bool pickup;
    bool dropoff;
    std::vector<unsigned> Codropoff;
    std::vector<unsigned> Copickup;
    intersection();
    intersection(const intersection& orig);
    intersection( bool p, bool dp,std::vector<unsigned> Cod,std::vector<unsigned> Cop );
    virtual ~intersection();
    bool operator==(const intersection&  rhs);
    bool operator!=(const intersection& rhs);
private:

};

#endif /* INTERSECTION_H */

