/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2_function_declaration.h
 * Author: haojinze
 *
 * Created on February 23, 2018, 8:19 PM
 */

#ifndef M2_FUNCTION_DECLARATION_H
#define M2_FUNCTION_DECLARATION_H

#include "OSMEntity.h"
#include "OSMNode.h"
#include "OSMWay.h"
#include "OSMRelation.h"
#include "OSMDatabaseAPI.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include <vector>
#include "unordered_map"
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <string>
#include <set>
#include "graphics.h"
#include <readline/readline.h>
#include <readline/history.h>

//constants
constexpr int VERY_CLOSE_LEVEL = 18;
constexpr int RE_FOCUS_LEVEL = 11;
constexpr int EXPRESSWAY_SPEED = 80;
constexpr int MAJOR_SPEED = 50;
constexpr int ROAD_ZOOM_LEVEL = 5;
constexpr int ROAD_DIFFERENCE = 3;
constexpr int DETAILED_LEVEL = 5;
constexpr int MORE_DETAILED_LEVEL = 10;
constexpr int SAMPLE_CONSTANT = 2;
constexpr double TEXT_TOLERANCE = 0.2;
float PIE = 3.14159265;

//global variable
t_point press_location;
bool searchBar1Editing = false;
bool searchBar2Editing = false;
bool searchBar1AutoFill = false;
bool searchBar2AutoFill = false;
bool selectSb1Op1 = false;
bool selectSb1Op2 = false;
bool selectSb2Op1 = false;
bool selectSb2Op2 = false;
bool resetMark = false;
int previousHightIntersectionID;
int previousHightPOI;
std::vector<std::string> previousInfoSet;
std::string searchBar1Text = "";
std::string searchBar2Text = ""; 
std::string option1 = "";
std::string option2 = "";
char key_input;

//for navigation
struct navigate_window_per_page {
    std::string name;
    float distance;
    std::string direction;
};
int itsc_id1;
int itsc_id2;
unsigned wanted_id1;
unsigned wanted_id2;
std::vector<std::vector<navigate_window_per_page>> total_info;
unsigned total_page;
unsigned current_page = 1;
bool navigation_info_init = true;
bool is_navigating = false;
bool close_nav;
std::vector<unsigned> draw_the_route;

//variable for changing map
bool clickMode = false;
bool navigateDone = false;
int clickCounter = 0;
int clickID1 = -1;
int clickID2 = -1;

extern std::unordered_map<std::string, std::vector<unsigned>> streetHT;
extern std::vector<std::vector<unsigned>> intersectionSeg;
extern std::vector<std::vector<unsigned>> streetSeg;
extern std::vector<LatLon> positionInt;
extern std::vector<LatLon> positionPOI;
extern std::vector<std::unordered_set<unsigned>> streetInt;
extern int zoom_level;

extern std::unordered_map<std::string, std::vector<unsigned>> POIname;
extern std::vector<std::pair<long double, unsigned>> area_of_index;
extern std::vector<unsigned> openfeatures;
extern std::vector<FeatureType> feature_type;
double AvgLat;


bool loadmap();
bool showPOI = false;
t_bound_box bounds();


//draw and functionality of search bar
t_bound_box search_bar1_location();
t_bound_box search_bar2_location();
t_bound_box search_bar1_option1_location();
t_bound_box search_bar1_option2_location();
t_bound_box search_bar2_option1_location();
t_bound_box search_bar2_option2_location();
bool check_valid_range(t_point bottom_left, t_point top_right);
void draw_search_bar();
void find_button_action(void drawscreen());
std::string cap_first_string (std::string itsc_info);
std::string cap_second_string (std::string itsc_info);
void Navigate_button_action(void drawscreen());
std::vector<std::string> findAllSimilarStreet(std::string str);
void drawAutoFill();
void autoComplete();

//reaction functions to mouse and keyboard
void act_on_button_press(float x, float y, t_event_buttonPressed event);
void act_on_mouse_move(float x, float y);
void act_on_key_press(char c, int keysym);

//functions used in draw screen
void drawRoad();
void drawPOI();
void drawFeature();
void drawIntersect();
void drawNames();
void draw_info_bar(std::vector<std::string> lines);

//draw screen and map
void drawscreen();

//helper functions of drawing feature
void openFeature(unsigned i);
void closeFeature(unsigned i);
bool Feature_close(unsigned FeatureID);
void show_point_of_interest(void drawscreen());

//display names & helper functions
void drawStreetNames();
void drawFeatureNames();
void drawPOINames();
t_point find_center(unsigned i);
int find_feature_longest_edge_angle(unsigned i);

//function to highlight path
void highlight_path(std::vector<unsigned> segmentSet);
std::vector<unsigned> previousRoute;
std::vector<unsigned> tempRoute;


//function to draw help button and content
void draw_help_button();
void draw_help_content();
t_bound_box help_button_location();

void show_intersection_name(unsigned id);



//functions for navigation window
void draw_navigation_window();
float segment_angle(unsigned seg_id, unsigned from);
std::vector<unsigned> find_from_to(unsigned id1, unsigned id2);
void navigation_window(std::vector<unsigned> route);
t_bound_box  navigation_icon();
t_bound_box instruction_to_close_navigation();
t_bound_box next_page_icon();
t_bound_box last_page_icon();
void draw_navigation_info();
void fill_navigation_step_text();
void draw_navigation_message(std::vector<unsigned> route);
void draw_direction_logo(std::string direction, t_point top_left);
float segment_angle(unsigned seg_id);
std::string get_direction(float angle1, float angle2); 
void navigate_path();

//functions for changing map
void change_map(void drawscreen());
void resetClear();
void drawclearscreen();
#endif /* M2_FUNCTION_DECLARATION_H */

