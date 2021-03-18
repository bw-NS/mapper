/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tester.cpp
 * Author: mengqi
 *
 * Created on February 4, 2018, 5:23 PM
 */

#include <cstdlib>
#include <iostream>
#include <unittest++/UnitTest++.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    int num = UnitTest::RunAllTests();

    return num;
}

