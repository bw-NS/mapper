#include "m2.h"
#include "graphics.h"
#include <iostream>
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include <vector>
#include "unordered_map"
#include <unordered_set>
#include <algorithm>
#include <math.h>
#include <iostream>
#include "m2_function_declaration.h"
#include <string>
#include "graphics.cpp"
#include "m3.h"
#include "m4.h"
#include <set>



//load different map according to user command

bool loadmap() {
    bool load_success = false;
    while (!load_success) {
        std::string city_name;
        std::cout << "City name in lower case (e.g. toronto): ";
        getline(std::cin, city_name);

        //different command cases
        if (city_name == "beijing") load_success = load_map("/cad2/ece297s/public/maps/beijing_china.streets.bin");
        else if (city_name == "cairo") load_success = load_map("/cad2/ece297s/public/maps/cairo_egypt.streets.bin");
        else if (city_name == "cape-town") load_success = load_map("/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin");
        else if (city_name == "golden-horseshoe") load_success = load_map("/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin");
        else if (city_name == "hamilton") load_success = load_map("/cad2/ece297s/public/maps/hamilton_canada.streets.bin");
        else if (city_name == "hong-kong") load_success = load_map("/cad2/ece297s/public/maps/hong-kong_china.streets.bin");
        else if (city_name == "iceland") load_success = load_map("/cad2/ece297s/public/maps/iceland.streets.bin");
        else if (city_name == "interlaken") load_success = load_map("/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin");
        else if (city_name == "london") load_success = load_map("/cad2/ece297s/public/maps/london_england.streets.bin");
        else if (city_name == "moscow") load_success = load_map("/cad2/ece297s/public/maps/moscow_russia.streets.bin");
        else if (city_name == "new-delhi") load_success = load_map("/cad2/ece297s/public/maps/new-delhi_india.streets.bin");
        else if (city_name == "new-york") load_success = load_map("/cad2/ece297s/public/maps/new-york_usa.streets.bin");
        else if (city_name == "rio-de-janeiro") load_success = load_map("/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin");
        else if (city_name == "saint-helena") load_success = load_map("/cad2/ece297s/public/maps/saint-helena.streets.bin");
        else if (city_name == "singapore") load_success = load_map("/cad2/ece297s/public/maps/singapore.streets.bin");
        else if (city_name == "sydney") load_success = load_map("/cad2/ece297s/public/maps/sydney_australia.streets.bin");
        else if (city_name == "tehran") load_success = load_map("/cad2/ece297s/public/maps/tehran_iran.streets.bin");
        else if (city_name == "tokyo") load_success = load_map("/cad2/ece297s/public/maps/tokyo_japan.streets.bin");
        else if (city_name == "toronto") load_success = load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
        else std::cout << "Error! Please type again." << std::endl;
    }
    return load_success;
}

//initial boundary from (minlon, minlat) to (maxlon, maxlat)

t_bound_box bounds() {
    double minlat = positionInt[0].lat();
    double maxlat = positionInt[0].lat();
    double minlon = positionInt[0].lon();
    double maxlon = positionInt[0].lon();

    //get max & min of latitude & longitude of intersection positions
    for (unsigned i = 1; i < positionInt.size(); i++) {
        if (positionInt[i].lat() < minlat) minlat = positionInt[i].lat();
        if (positionInt[i].lat() > maxlat) maxlat = positionInt[i].lat();
        if (positionInt[i].lon() < minlon) minlon = positionInt[i].lon();
        if (positionInt[i].lon() > maxlon) maxlon = positionInt[i].lon();
    }

    //get max & min of latitude & longitude of interests
    for (unsigned p = 0; p < positionPOI.size(); p++) {
        if (positionPOI[p].lat() < minlat) minlat = positionPOI[p].lat();
        if (positionPOI[p].lat() > maxlat) maxlat = positionPOI[p].lat();
        if (positionPOI[p].lon() < minlon) minlon = positionPOI[p].lon();
        if (positionPOI[p].lon() > maxlon) maxlon = positionPOI[p].lon();
    }

    AvgLat = (minlat + maxlat) * DEG_TO_RAD / 2;
    t_bound_box bound(minlon * cos(AvgLat), minlat, maxlon * cos(AvgLat), maxlat);

    return bound;
}

//---------------------------------------------------------------search bar feature implement-----------------------------------------------------------------------
//calculate the location of search bar1 in the window

t_bound_box search_bar1_location() {

    t_bound_box currentWorld = get_visible_world();
    //the height is (the height of window) / 15
    //the width is (the width of window) * (2/5))
    float sb_width = (currentWorld.get_width()) / 8 * 3;
    float sb_height = -(currentWorld.get_height()) / 35;

    //the search bar locates on the top of the window
    t_point sb1_bottom_left;
    sb1_bottom_left.x = currentWorld.left();
    sb1_bottom_left.y = currentWorld.top() + sb_height;
    t_point sb1_upper_right;
    sb1_upper_right.x = sb1_bottom_left.x + sb_width;
    sb1_upper_right.y = currentWorld.top();

    t_bound_box search_bar1(sb1_bottom_left, sb1_upper_right);
    return search_bar1;
}

//calculate the location of search bar2 in the window

t_bound_box search_bar2_location() {
    t_bound_box currentWorld = get_visible_world();
    //the height is (the height of window) / 15
    //the width is (the width of window) * (2/5))
    float sb_width = (currentWorld.get_width()) / 8 * 3;
    float sb_height = -(currentWorld.get_height()) / 35;

    //the search bar locates on the top of the window
    t_point sb2_bottom_left;
    sb2_bottom_left.x = currentWorld.left();
    sb2_bottom_left.y = currentWorld.top() + sb_height * 2;
    t_point sb2_upper_right;
    sb2_upper_right.x = sb2_bottom_left.x + sb_width;
    sb2_upper_right.y = currentWorld.top() + sb_height;

    t_bound_box search_bar2(sb2_bottom_left, sb2_upper_right);
    return search_bar2;
}

//calculate the location of suggestion window 1 of search bar1 in the window

t_bound_box search_bar1_option1_location() {
    t_bound_box currentWorld = get_visible_world();

    //the height is (the height of window) / 25
    //the width is (the width of window) * (2/5)
    t_bound_box searchBar2 = search_bar2_location();
    float option_height = -(currentWorld.get_height()) / 35;

    //the suggestion windows is right below the search bar
    t_point sb2_bottom_left = searchBar2.bottom_left();
    sb2_bottom_left.offset(0, option_height);

    t_point sb2_upper_right = searchBar2.top_right();
    sb2_upper_right.offset(0, -searchBar2.get_height());

    t_bound_box search_bar2(sb2_bottom_left, sb2_upper_right);
    return search_bar2;
}

//calculate the location of suggestion window 1 of search bar1 in the window

t_bound_box search_bar1_option2_location() {

    //the height is (the height of window) / 25
    //the width is (the width of window) * (2/5)
    t_bound_box optionBar2 = search_bar2_option1_location();
    float option_height = -optionBar2.get_height();

    //the suggestion windows is right below the search bar
    t_point sb2_bottom_left = optionBar2.bottom_left();
    sb2_bottom_left.offset(0, option_height);
    t_point sb2_upper_right = optionBar2.top_right();
    sb2_upper_right.offset(0, option_height);

    t_bound_box search_bar2(sb2_bottom_left, sb2_upper_right);
    return search_bar2;
}

//calculate the location of suggestion window 1 of search bar1 in the window

t_bound_box search_bar2_option1_location() {

    return search_bar1_option1_location();
}

//calculate the location of suggestion window 1 of search bar1 in the window

t_bound_box search_bar2_option2_location() {

    return search_bar1_option2_location();

}

//get the string before the first "&"

string cap_first_string(string itsc_info) {
    return itsc_info.substr(0, itsc_info.find("&") - 1);
}

//get the string after the first "&"

string cap_second_string(string itsc_info) {
    string temp = itsc_info.substr(itsc_info.find("&") + 2);
    if (temp.find("&") != string::npos) {
        return temp.substr(0, temp.find("&") - 1);
    }
    return temp;
}

//back end function for navigate button

void Navigate_button_action(void drawscreen()) {
    navigate_path();
}

//show the highlighted road and navigation window

void navigate_path() {
    drawclearscreen();
    std::string str1 = searchBar1Text;
    std::string str2 = searchBar2Text;
    draw_the_route.clear();

    string POI_name;
    string cap1;
    string cap2;

    bool str1_intersection = false;
    bool str2_intersection = false;

    //check if the two input string is intersection
    if (str1.find('&') != std::string::npos)
        str1_intersection = true;
    if (str2.find('&') != std::string::npos)
        str2_intersection = true;

    //initialize the variable
    current_page = 1;
    navigation_info_init = true;

    if (!clickMode) {// not click two intersections to find path
        if (str1_intersection && str2_intersection) {//search from intersection to intersection

            //get the two intersection id
            cap1 = cap_first_string(str1);
            cap2 = cap_second_string(str1);
            itsc_id1 = find_intersection_ids_from_street_names(cap1, cap2)[0];

            cap1 = cap_first_string(str2);
            cap2 = cap_second_string(str2);
            itsc_id2 = find_intersection_ids_from_street_names(cap1, cap2)[0];

            if (itsc_id1 == itsc_id2) {
                std::string info1 = "Right-click to reset.";
                std::string info2 = "No path found!";
                std::vector<std::string> infoSet;
                infoSet.push_back(info1);
                infoSet.push_back(info2);
                draw_info_bar(infoSet);
                return;
            }
            //for draw navigation window
            wanted_id1 = itsc_id1;
            wanted_id2 = itsc_id2;

            //get the path (vector of segment id)
            draw_the_route = find_path_between_intersections(itsc_id1, itsc_id2, 1);

            //drawing
            highlight_path(draw_the_route);
            previousRoute.clear();
            previousRoute = draw_the_route;

            //check if the route exist, then change the boolean variable condition
            if (draw_the_route.size() != 0) {
                is_navigating = true;
                close_nav = false;
                drawscreen();

                draw_navigation_message(draw_the_route);
                navigateDone = true;
            }

            //search from intersection to POI
        } else if (!str1_intersection && str2_intersection) {
            if (POIname.find(str1) != POIname.end()) {

                //get the two string(street name) from user input
                cap1 = cap_first_string(str2);
                cap2 = cap_second_string(str2);
                itsc_id1 = find_intersection_ids_from_street_names(cap1, cap2)[0];

                //find the path
                draw_the_route = find_path_to_point_of_interest(itsc_id1, str1, 1);
                if (draw_the_route.size() == 0) {
                    std::string info1 = "Right-click to reset.";
                    std::string info2 = "No path found!";
                    std::vector<std::string> infoSet;
                    infoSet.push_back(info1);
                    infoSet.push_back(info2);
                    draw_info_bar(infoSet);
                    return;
                } else if (draw_the_route.size() == 1) {
                    if (itsc_id1 == getStreetSegmentInfo(draw_the_route[0]).to)
                        itsc_id2 = getStreetSegmentInfo(draw_the_route[0]).from;
                    else itsc_id2 = getStreetSegmentInfo(draw_the_route[0]).to;
                } else {
                    unsigned last_seg = draw_the_route[draw_the_route.size() - 1];
                    unsigned second_last_seg = draw_the_route[draw_the_route.size() - 2];
                    vector<unsigned> second_last_from_to = find_from_to(second_last_seg, last_seg);
                    if (second_last_from_to[1] == getStreetSegmentInfo(last_seg).from)
                        itsc_id2 = getStreetSegmentInfo(last_seg).to;
                    else
                        itsc_id2 = getStreetSegmentInfo(last_seg).from;
                }
                //drawing
                highlight_path(draw_the_route);
                previousRoute.clear();
                previousRoute = draw_the_route;
                if (draw_the_route.size() != 0) {
                    is_navigating = true;
                    close_nav = false;
                    drawscreen();
                    draw_navigation_message(draw_the_route);
                    navigateDone = true;
                }
            }
            //When both bars content is empty, click navigate button to click two
            //intersections and find path between them
        } else if ((str1 == "" && str2 == "") || (str1 == " " && str2 == " ")) {
            std::string info = "Click two intersections to find path.";
            std::vector<std::string> infoSet;
            infoSet.push_back(info);
            draw_info_bar(infoSet);
            clickMode = true;
            //user does not operate correctly, show hint messgae
        } else {
            std::string info2 = "Error: please check the help instruction!";
            std::string info1 = "Right-click to reset.";
            std::vector<std::string> infoSet;
            infoSet.push_back(info1);
            infoSet.push_back(info2);
            draw_info_bar(infoSet);
        }
    } else {//when click two intersections to find path
        itsc_id1 = clickID1;
        itsc_id2 = clickID2;
        if (itsc_id1 == -1 || itsc_id2 == -1) return;
        draw_the_route = find_path_between_intersections(itsc_id1, itsc_id2, 1);
        if (draw_the_route.size() != 0) {//find path, show the route
            is_navigating = true;
            close_nav = false;
            drawscreen();
            draw_navigation_message(draw_the_route);
            navigateDone = true;
        }            //no path found
        else {
            std::string info1 = "Right-click to reset.";
            std::string info2 = "No path found!";
            std::vector<std::string> infoSet;
            infoSet.push_back(info1);
            infoSet.push_back(info2);
            draw_info_bar(infoSet);
        }
        highlight_path(draw_the_route);
        previousRoute.clear();
        previousRoute = draw_the_route;
    }
    return;
}

//check whether mouse click in the specific range

bool check_valid_range(t_point bottom_left, t_point top_right) {
    float x_min = bottom_left.x;
    float x_max = top_right.x;
    float y_max = top_right.y;
    float y_min = bottom_left.y;
    if (press_location.x >= x_min && press_location.x <= x_max) {
        if (press_location.y >= y_min && press_location.y <= y_max)
            return true;
    }
    return false;
}

//draw the search bar in the window

void draw_search_bar() {
    t_bound_box sb1 = search_bar1_location();
    t_bound_box sb2 = search_bar2_location();

    //draw rec
    setcolor(WHITE);
    fillrect(sb1);
    fillrect(sb2);

    //draw border
    setlinewidth(1);
    setcolor(BLACK);
    drawrect(sb1);
    drawrect(sb2);

    //draw text
    setcolor(BLACK);
    settextrotation(0);
    drawtext_in(sb1, "Street 1/POI name: " + searchBar1Text);
    drawtext_in(sb2, "Street 2: " + searchBar2Text);

}

//draw information bar

void draw_info_bar(std::vector<std::string> lines) {
    int previousSize = getfontsize();
    t_bound_box currentWorld = get_visible_world();
    float info_bar_width = currentWorld.get_width() / 3;
    float info_bar_height = currentWorld.get_height() / 25;
    unsigned boxNum = lines.size();
    if (boxNum > 6) boxNum = 6; //draw at most six boxes
    t_point bottomLeft(currentWorld.left() + 2 * info_bar_width, currentWorld.bottom());
    t_bound_box infoBar(bottomLeft, info_bar_width, info_bar_height);
    //draw boxes
    for (unsigned i = 0; i < boxNum; i++) {
        if (i != 0)
            infoBar.offset(0.0, info_bar_height); //move each box to the top of previous one
        //draw box
        setcolor(WHITE);
        fillrect(infoBar);
        //draw border
        setcolor(BLACK);
        setlinewidth(1);
        drawrect(infoBar);
        //draw text
        settextrotation(0);
        setfontsize(10);
        drawtext_in(infoBar, lines[i]);
    }
    //store information and reset line width
    previousInfoSet = lines;
    setfontsize(previousSize);
}

//do the functionality of find button

void find_button_action(void drawscreen()) {

    drawclearscreen();

    std::string str1 = searchBar1Text;
    std::string str2 = searchBar2Text;

    //input the POI name in search bar1 can search the location
    //can only search the POI with one location
    if (str1 != "" && str2 == "") {//search POI
        if (POIname.find(str1) == POIname.end()) {
            std::string info = "Please click help icon on top right corner.";
            std::vector<std::string> infoSet;
            infoSet.push_back(info);
            draw_info_bar(infoSet);
        } else {
            std::vector<unsigned> POI_result = POIname[str1];

            searchBar1Text = "";
            searchBar2Text = "";

            //if the POI name is unique, then zoom in to that place in the map
            if (POI_result.size() == 1) {
                unsigned POI_ID = POI_result[0];
                LatLon temp = positionPOI[POI_ID];
                float center_x = temp.lon() * cos(AvgLat);
                float center_y = temp.lat();
                set_visible_world(center_x - 0.00005, center_y - 0.00005, center_x + 0.00005, center_y + 0.00005);
                zoom_level = VERY_CLOSE_LEVEL;

                //adjust the zoom level to a proper one
                if (zoom_level < RE_FOCUS_LEVEL) {
                    while (zoom_level != RE_FOCUS_LEVEL) {
                        zoom_in(drawscreen);
                    }
                }

                if (zoom_level > RE_FOCUS_LEVEL) {
                    while (zoom_level != RE_FOCUS_LEVEL) {
                        zoom_out(drawscreen);
                    }
                }
            }

            //de-highlight previous intersection
            drawscreen();

            //highlight the found POI
            std::vector<std::string> infoSet;
            std::string info;
            for (unsigned i = 0; i < (POI_result).size(); i++) {
                LatLon POI_temp = positionPOI[POI_result[i]];
                if ((POI_result).size() != 0) previousHightPOI = POI_result[0];
                //change positionPOI color (highlight)
                t_bound_box currentWorld = get_visible_world();
                setcolor(PURPLE);
                if (currentWorld.intersects(POI_temp.lon() * cos(AvgLat), POI_temp.lat()))
                    fillarc(POI_temp.lon() * cos(AvgLat), POI_temp.lat(), 0.00005, 0, 360);

                //display POI information
                std::string name = str1;
                auto lat = std::to_string(getPointOfInterestPosition(POI_result[i]).lat());
                auto lon = std::to_string(getPointOfInterestPosition(POI_result[i]).lon());
                std::string position = "(" + lat + ", " + lon + ")";
                infoSet.push_back(position);
                infoSet.push_back(name);
            }
            draw_info_bar(infoSet);
        }
    } else if (str1 != "" && str2 != "") {//do a normal search
        std::vector<unsigned> result = find_intersection_ids_from_street_names(str1, str2);
        searchBar1Text = "";
        searchBar2Text = "";

        //can't find the intersection
        if (result.size() == 0) {
            std::string info = "Sorry, there's no intersection between these two streets.";
            std::vector<std::string> infoSet;
            infoSet.push_back(info);
            draw_info_bar(infoSet);

            //find out the intersection
        } else {
            //zoom in to the intersection
            unsigned intersectionID = result[0];
            LatLon temp = positionInt[intersectionID];
            float center_x = temp.lon() * cos(AvgLat);
            float center_y = temp.lat();
            set_visible_world(center_x - 0.00003, center_y - 0.00003, center_x + 0.00003, center_y + 0.00003);
            zoom_level = VERY_CLOSE_LEVEL;

            if (zoom_level < RE_FOCUS_LEVEL) {
                while (zoom_level != RE_FOCUS_LEVEL) {
                    zoom_in(drawscreen);
                }

            } else if (zoom_level > RE_FOCUS_LEVEL) {
                while (zoom_level != RE_FOCUS_LEVEL) {
                    zoom_out(drawscreen);
                }
            }

            //de-highlight previous intersection
            drawscreen();

            //change intersection color (highlight)
            t_bound_box currentWorld = get_visible_world();
            setcolor(255, 60, 60);
            if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
                //draw_surface(load_png_from_file("libstreetmap/resources/inter.png"),temp.lon() * cos(AvgLat)-currentWorld.get_width()*0.01, temp.lat()+currentWorld.get_height()*0.01);
                fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.00003, 0, 360);
            previousHightIntersectionID = intersectionID;

            //display intersection information
            show_intersection_name(intersectionID);

        }
    } else if (str1 == "" && str2 == "") {
        std::string info = "Please click help icon on top right corner.";
        std::vector<std::string> infoSet;
        infoSet.push_back(info);
        draw_info_bar(infoSet);
    } else {//highlight center of the given street

    }
}

//do the functionality of suggestion window

std::vector<std::string> findAllSimilarStreet(std::string str) {
    std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<unsigned>> allStreet = streetHT;
    std::unordered_map<std::string, std::vector<unsigned>> allPOI = POIname;

    //loop through all street name to find the result that contains the input string
    for (std::pair<std::string, std::vector<unsigned>> kv : allStreet) {
        if (kv.first.find(str) != string::npos) {
            result.push_back(kv.first);
        }
        //once two results are found, then return (only two suggestion will be shown)
        if (result.size() >= 2) return result;
    }

    //if two results have been found, then no need to search for more result
    if (result.size() < 2) {
        //loop through all intersection name to find the result that contains the input string
        for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
            string name = getIntersectionName(i);
            if (name.find(str) != string::npos) {
                result.push_back(name);
            }

            if (result.size() >= 2) return result;
        }
    }

    //if two results have been found, then no need to search for more result
    if (result.size() < 2) {
        //loop through POI name to find the result that contains the input string
        for (std::pair<std::string, std::vector<unsigned>> kv : allPOI) {
            if (kv.first.find(str) != string::npos) {
                result.push_back(kv.first);
            }
            if (result.size() >= 2) return result;
        }

    }

    return result;
}

//draw the suggestion window

void drawAutoFill() {
    //there exist suggestion that can be displayed for search bar1
    if (searchBar1AutoFill) {
        std::string str = searchBar1Text;
        std::vector<std::string> result = findAllSimilarStreet(str);

        t_bound_box op1 = search_bar1_option1_location();
        t_bound_box op2 = search_bar1_option2_location();
        //draw background of auto-suggestion bar
        setcolor(255, 255, 204);
        fillrect(op1);
        fillrect(op2);

        setlinewidth(1);
        setcolor(0, 0, 0);
        drawrect(op1);
        drawrect(op2);
        settextrotation(0);

        //draw the auto fill options
        if (result.size() > 1) {
            option1 = result[0];
            option2 = result[1];

            setcolor(BLACK);
            drawtext_in(op1, "Suggestion 1: " + option1);
            drawtext_in(op2, "Suggestion 2: " + option2);

        } else if (result.size() == 1) {
            option1 = result[0];
            option2 = "Not Found";

            setcolor(BLACK);
            drawtext_in(op1, "Suggestion 1: " + option1);
            drawtext_in(op2, "Suggestion 2: " + option2);

        } else {
            option1 = "Not Found";
            option2 = "Not Found";

            setcolor(BLACK);
            drawtext_in(op1, "Suggestion 1: " + option1);
            drawtext_in(op2, "Suggestion 2: " + option2);
        };

        //there exists suggestion that can be displayed for search bar2
    } else if (searchBar2AutoFill) {
        std::string str = searchBar2Text;
        std::vector<std::string> result = findAllSimilarStreet(str);

        t_bound_box op1 = search_bar2_option1_location();
        t_bound_box op2 = search_bar2_option2_location();
        //draw rec
        setcolor(255, 255, 204);
        fillrect(op1);
        fillrect(op2);

        setcolor(0, 0, 0);
        drawrect(op1);
        drawrect(op2);
        settextrotation(0);

        if (result.size() > 1) {//draw the auto fill options
            option1 = result[0];
            option2 = result[1];

            setcolor(BLACK);
            drawtext_in(op1, "Suggestion 1: " + option1);
            drawtext_in(op2, "Suggestion 2: " + option2);

        } else if (result.size() == 1) {
            option1 = result[0];
            option2 = "Not Found";

            setcolor(BLACK);
            drawtext_in(op1, "Suggestion 1: " + option1);
            drawtext_in(op2, "Suggestion 2: " + option2);

        } else {
            option1 = "Not Found";
            option2 = "Not Found";

            setcolor(BLACK);
            drawtext_in(op1, "Suggestion 1: " + option1);
            drawtext_in(op2, "Suggestion 2: " + option2);
        };

    } else {
        return;
    }
}

//allow users to click on the suggestion and auto complete

void autoComplete() {
    //check if users are inputting in one of the search bar
    if (searchBar1AutoFill) {
        //check if the users press on one of the auto suggestion
        //if the suggestion is "not found", then error message will print
        //if not, the string will automatically fill in the search bar
        if (selectSb1Op1) {
            selectSb1Op1 = false;
            if (option1 == "Not Found") {
                std::string info = "Can't Complete 'Not Found'!";
                std::vector<std::string> infoSet;
                infoSet.push_back(info);
                draw_info_bar(infoSet);
                return;
            }
            searchBar1Text = option1;
            draw_search_bar();
            drawscreen();
            searchBar1Editing = false;
            return;
        }
        if (selectSb1Op2) {
            selectSb1Op2 = false;
            if (option2 == "Not Found") {
                std::string info = "Can't Complete 'Not Found'!";
                std::vector<std::string> infoSet;
                infoSet.push_back(info);
                draw_info_bar(infoSet);
                return;
            }
            searchBar1Text = option2;
            draw_search_bar();
            drawscreen();
            searchBar1Editing = false;
            return;
        }
    }
    //allow users to click on the suggestion and auto complete
    if (searchBar2AutoFill) {
        //check if the users press on one of the auto suggestion
        //if the suggestion is "not found", then error message will print
        //if not, the string will automatically fill in the search bar
        if (selectSb2Op1) {
            selectSb2Op1 = false;
            if (option1 == "Not Found") {
                std::string info = "Can't Complete 'Not Found'!";
                std::vector<std::string> infoSet;
                infoSet.push_back(info);
                draw_info_bar(infoSet);
                return;
            }
            searchBar2Text = option1;
            draw_search_bar();
            drawscreen();
            searchBar2Editing = false;
            return;
        }
        if (selectSb2Op2) {
            selectSb2Op2 = false;
            if (option2 == "Not Found") {
                std::string info = "Can't Complete 'Not Found'!";
                std::vector<std::string> infoSet;
                infoSet.push_back(info);
                draw_info_bar(infoSet);
                return;
            }
            searchBar2Text = option2;
            draw_search_bar();
            drawscreen();
            searchBar2Editing = false;
            return;
        }
    }
}


//--------------------------------------------------------reaction functions to mouse and keyboard-------------------------------------------------------------------------

void act_on_button_press(float x, float y, t_event_buttonPressed event) {
    if (event.button == 1) {
        if (navigateDone) {//reset search bar content after navigation is done
            searchBar1Text = "";
            searchBar2Text = "";
            navigateDone = false;
        }
        if (!clickMode) {//not click two intersections to find path
            resetMark = true;
            tempRoute = previousRoute;
            previousInfoSet.clear();
            previousRoute.clear();
            t_bound_box sb1 = search_bar1_location();
            t_bound_box sb1_op1 = search_bar1_option1_location();
            t_bound_box sb1_op2 = search_bar1_option2_location();
            t_bound_box sb2_op1 = search_bar2_option1_location();
            t_bound_box sb2_op2 = search_bar2_option2_location();
            press_location.x = x;
            press_location.y = y;

            //if click on search bar 1
            if (check_valid_range(sb1.bottom_left(), sb1.top_right())) { //pressed in search bar 1 area
                searchBar1Editing = true;
                searchBar2Editing = false;

                drawscreen();
                searchBar2AutoFill = false;
                if (searchBar1Text.length() >= 2) {
                    searchBar1AutoFill = true;
                    searchBar2AutoFill = false;
                    drawAutoFill();
                }

                //click on search bar 1 options
            } else if (check_valid_range(sb1_op1.bottom_left(), sb1_op1.top_right()) && searchBar1Editing) {
                selectSb1Op1 = true;
                autoComplete();
            } else if (check_valid_range(sb1_op2.bottom_left(), sb1_op2.top_right()) && searchBar1Editing) {
                selectSb1Op2 = true;
                autoComplete();
                //click on search bar 2
            } else if (check_valid_range(search_bar2_location().bottom_left(), search_bar2_location().top_right())) { //pressed in search bar 2 area
                searchBar1Editing = false;
                searchBar2Editing = true;

                drawscreen();
                searchBar1AutoFill = false;
                if (searchBar2Text.length() >= 2) {
                    searchBar1AutoFill = false;
                    searchBar2AutoFill = true;
                    drawAutoFill();
                }

                //click on search bar 2
            } else if (check_valid_range(sb2_op1.bottom_left(), sb2_op1.top_right()) && searchBar2Editing) {
                selectSb2Op1 = true;
                autoComplete();
            } else if (check_valid_range(sb2_op2.bottom_left(), sb2_op2.top_right()) && searchBar2Editing) {
                selectSb2Op2 = true;
                autoComplete();

                //click on help icon
            } else if (check_valid_range(help_button_location().bottom_left(), help_button_location().top_right())) {
                previousRoute.clear();
                previousRoute = tempRoute;
                draw_help_content();

                //click on navigation instruction window
            } else if ((total_page > 1) && (check_valid_range(next_page_icon().bottom_left(), next_page_icon().top_right()) || check_valid_range(last_page_icon().bottom_left(), last_page_icon().top_right()))) {
                previousRoute.clear();
                previousRoute = tempRoute;
                if (check_valid_range(next_page_icon().bottom_left(), next_page_icon().top_right())) {
                    if (current_page < total_page) {
                        current_page++;
                        fill_navigation_step_text();
                        drawscreen();
                    }
                } else {
                    if (current_page > 1) {
                        current_page--;
                        fill_navigation_step_text();
                        drawscreen();
                    }
                }

                //click on navigation icon
            } else if (check_valid_range(navigation_icon().bottom_left(), navigation_icon().top_right()) && is_navigating) {
                //draw_the_route = find_path_between_intersections(wanted_id1, wanted_id2, 1);
                //highlight_path(draw_the_route);
                previousRoute.clear();
                previousRoute = draw_the_route;
                if (!close_nav) {
                    close_nav = true;
                    previousRoute.clear();
                    previousRoute = tempRoute;
                    drawscreen();
                } else if (close_nav) {
                    close_nav = false;
                    previousRoute.clear();
                    previousRoute = tempRoute;
                    drawscreen();
                    if (total_page > 1) {
                        next_page_icon();
                        last_page_icon();
                    }
                }

            } else {
                searchBar1Editing = false;
                searchBar2Editing = false;
                searchBar1AutoFill = false;
                searchBar2AutoFill = false;

                is_navigating = false;
                total_page = 0;

                //click to highlight intersection and show the name of intersection
                LatLon currentPosition(y, x / cos(AvgLat));
                unsigned intersectionID;
                intersectionID = find_closest_intersection(currentPosition);
                //de-highlight previous intersection
                drawscreen();

                //change intersection color (highlight)
                t_bound_box currentWorld = get_visible_world();
                LatLon temp = positionInt[intersectionID];
                setcolor(255, 60, 60);
                if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
                    //draw_surface(load_png_from_file("libstreetmap/resources/inter.png"),temp.lon() * cos(AvgLat)-currentWorld.get_width()*0.01, temp.lat()+currentWorld.get_height()*0.01);
                    fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.00003, 0, 360);

                //display intersection information
                show_intersection_name(intersectionID);
                if (intersectionID != previousHightIntersectionID && previousHightIntersectionID != -1) {
                    LatLon pretemp = positionInt[previousHightIntersectionID];
                    setcolor(150, 225, 255);
                    if (currentWorld.intersects(pretemp.lon() * cos(AvgLat), pretemp.lat()))
                        fillarc(pretemp.lon() * cos(AvgLat), pretemp.lat(), 0.00003, 0, 360);
                }
                previousHightIntersectionID = intersectionID;
            }

            //click two intersections to find path
        } else {
            //click to highlight intersection and show the name of intersection
            LatLon currentPosition(y, x / cos(AvgLat));
            unsigned intersectionID;
            intersectionID = find_closest_intersection(currentPosition);
            t_bound_box currentWorld = get_visible_world();
            LatLon temp = positionInt[intersectionID];
            setcolor(255, 60, 60);
            if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
                fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.00003, 0, 360);
            show_intersection_name(intersectionID);
            //count click times
            clickCounter++;
            if (clickCounter % 2 == 0) {//second time click
                //clickModeFind = true;
                clickID2 = intersectionID;
                searchBar2Text = getIntersectionName(clickID2);
                if (clickID1 == clickID2) {
                    std::string info1 = "Right-click to reset.";
                    std::string info2 = "No path found!";
                    std::vector<std::string> infoSet;
                    infoSet.push_back(info1);
                    infoSet.push_back(info2);
                    draw_info_bar(infoSet);
                    return;
                }
                navigate_path();
                clickMode = false;
            } else {//first time click
                clickID1 = intersectionID;
                searchBar1Text = getIntersectionName(clickID1);
                previousRoute.clear();
            }
        }

        //mouse right-click, reset highlighted points and path
    } else if (event.button == 3) {
        resetMark = false;
        clickID1 = -1;
        clickID2 = -1;
        searchBar1Text = "";
        searchBar2Text = "";
        previousInfoSet.clear();
        drawclearscreen();
    } else resetMark = false;
}

void act_on_mouse_move(float x, float y) {
}

void act_on_key_press(char c, int keysym) {


    if (searchBar1Editing) {
        if (keysym == XK_BackSpace) {
            if (searchBar1Text != "") {
                searchBar1Text.pop_back();
            }
        } else if (keysym == XK_Caps_Lock) {
        } else if (keysym == XK_Shift_L) {
        } else if (keysym == XK_Shift_R) {
        } else {
            searchBar1Text += c;

        }
        draw_search_bar();


        searchBar1AutoFill = true;
        drawAutoFill();

    }

    if (searchBar2Editing) {
        if (keysym == XK_BackSpace) {
            if (searchBar2Text != "") {
                searchBar2Text.pop_back();
            }
        } else if (keysym == XK_Caps_Lock) {
        } else if (keysym == XK_Shift_L) {
        } else if (keysym == XK_Shift_R) {
        } else {
            searchBar2Text += c;
        }
        draw_search_bar();

        searchBar2AutoFill = true;
        drawAutoFill();

    }


}

//--------------------------------------------------------------------draw screen and map-------------------------------------------------------------------------

void drawscreen() {
    clearscreen();
    drawFeature();
    drawRoad();
    drawIntersect();
    drawPOI();
    drawNames();
    draw_search_bar();
    draw_help_button();
    draw_info_bar(previousInfoSet);
    LatLon temp;
    LatLon tempPOI;
    t_bound_box currentWorld = get_visible_world();


    //draw last highlighted intersection
    if (previousHightIntersectionID != -1) {
        temp = positionInt[previousHightIntersectionID];
        positionInt[previousHightIntersectionID];
        setcolor(255, 60, 60);
        if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
            fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.00003, 0, 360);
    }

    //draw last highlighted POI
    if (previousHightPOI != -1) {
        setcolor(PURPLE);
        tempPOI = positionPOI[previousHightPOI];
        if (currentWorld.intersects(tempPOI.lon() * cos(AvgLat), tempPOI.lat()))
            fillarc(tempPOI.lon() * cos(AvgLat), tempPOI.lat(), 0.00005, 0, 360);
    }
    highlight_path(previousRoute);

    //draw navigating window
    if (is_navigating && !close_nav) {
        draw_navigation_window();
        LatLon pretemp = positionInt[previousHightIntersectionID];
        setcolor(150, 225, 255);
        if (currentWorld.intersects(pretemp.lon() * cos(AvgLat), pretemp.lat()))
            fillarc(pretemp.lon() * cos(AvgLat), pretemp.lat(), 0.00003, 0, 360);
    }

    //draw navigating icon
    if (is_navigating && close_nav) {
        navigation_icon();
        LatLon pretemp = positionInt[previousHightIntersectionID];
        setcolor(150, 225, 255);
        if (currentWorld.intersects(pretemp.lon() * cos(AvgLat), pretemp.lat()))
            fillarc(pretemp.lon() * cos(AvgLat), pretemp.lat(), 0.00003, 0, 360);
    }

    std::vector<unsigned> depots;
    depots.push_back(1000);
    depots.push_back(2000);

    std::vector<DeliveryInfo> deliveries;
    DeliveryInfo temp1(100, 150);
    deliveries.push_back(temp1);

    temp1.pickUp = 200;
    temp1.dropOff = 250;
    deliveries.push_back(temp1);

    temp1.pickUp = 300;
    temp1.dropOff = 350;
    deliveries.push_back(temp1);

    temp1.pickUp = 400;
    temp1.dropOff = 550;
    deliveries.push_back(temp1);

    highlight_path(traveling_courier(deliveries, depots, 0.0));



    for (unsigned i = 0; i < deliveries.size(); i++) {
        temp = positionInt[deliveries[i].pickUp];

        setcolor(0, 255, 0); //green for pickup
        if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
            //draw_surface(load_png_from_file("libstreetmap/resources/inter.png"),temp.lon() * cos(AvgLat)-currentWorld.get_width()*0.01, temp.lat()+currentWorld.get_height()*0.01);
            fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.003, 0, 360);
        
        
        temp = positionInt[deliveries[i].dropOff];

        setcolor(255, 204, 0);//yellow for dropoff
        if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
            //draw_surface(load_png_from_file("libstreetmap/resources/inter.png"),temp.lon() * cos(AvgLat)-currentWorld.get_width()*0.01, temp.lat()+currentWorld.get_height()*0.01);
            fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.003, 0, 360);
    }
    
    for (unsigned i = 0; i < depots.size(); i++) {
        temp = positionInt[depots[i]];

        setcolor(0, 51, 102); //blue for depot
        if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
            //draw_surface(load_png_from_file("libstreetmap/resources/inter.png"),temp.lon() * cos(AvgLat)-currentWorld.get_width()*0.01, temp.lat()+currentWorld.get_height()*0.01);
            fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.003, 0, 360);
        
    }
}

//clear all windows and information

void drawclearscreen() {
    previousHightIntersectionID = -1;
    previousHightPOI = -1;
    is_navigating = false;
    close_nav = true;
    previousRoute.clear();
    clearscreen();
    drawFeature();
    drawRoad();
    drawIntersect();
    drawPOI();
    drawNames();
    draw_search_bar();
    draw_help_button();
}

void draw_map() {
    bool load_success = loadmap();

    //check if load map successfully
    if (!load_success) {
        std::cout << "Load map failed." << std::endl;
        return;
    }

    t_color background(232, 232, 232);
    init_graphics("City map", background);
    t_bound_box bound = bounds();
    set_visible_world(bound);
    set_keypress_input(true);
    set_mouse_move_input(true);
    create_button("Window", "Find", find_button_action);
    create_button("Find", "Navigate", Navigate_button_action);
    create_button("Navigate", "Show POI", show_point_of_interest);
    create_button("Show POI", "Change Map", change_map);
    event_loop(act_on_button_press, act_on_mouse_move, act_on_key_press, drawscreen);

    close_graphics();
}

//--------------------------------------------------------------------drawing functions---------------------------------------------------------------------------

//function to draw the roads

void drawRoad() {
    t_bound_box currentWorld = get_visible_world();
    t_color road_color(255, 250, 250);
    setcolor(road_color);

    for (unsigned i = 0; i < streetSeg.size(); i++) {
        for (unsigned j = 0; j < streetSeg[i].size(); j++) {
            StreetSegmentInfo temp = getStreetSegmentInfo(streetSeg[i][j]);
            unsigned curve_point_number = temp.curvePointCount;

            double xf = positionInt[temp.from].lon() * cos(AvgLat);
            double yf = positionInt[temp.from].lat();

            double xt = positionInt[temp.to].lon() * cos(AvgLat);
            double yt = positionInt[temp.to].lat();

            if ((currentWorld.intersects(xf, yf) || currentWorld.intersects(xt, yt))) {
                if (curve_point_number == 0) {
                    if (temp.speedLimit >= EXPRESSWAY_SPEED) {
                        if (zoom_level >= ROAD_ZOOM_LEVEL) setlinewidth(zoom_level - 1 + ROAD_DIFFERENCE);
                        else setlinewidth(2);
                        setcolor(255, 225, 0);
                        drawline(xf, yf, xt, yt);
                    } else if (temp.speedLimit >= MAJOR_SPEED) {
                        if (zoom_level >= ROAD_ZOOM_LEVEL) setlinewidth(zoom_level - 1);
                        else setlinewidth(1);
                        setcolor(255, 255, 240);
                        if (zoom_level >= 3)
                            drawline(xf, yf, xt, yt);
                    } else {
                        if (zoom_level >= ROAD_ZOOM_LEVEL) setlinewidth(zoom_level - 1 - ROAD_DIFFERENCE);
                        else setlinewidth(0);
                        setcolor(WHITE);
                        if (zoom_level >= 8)drawline(xf, yf, xt, yt);
                    }
                } else {
                    bool draw = false;
                    if ((currentWorld.intersects(xf, yf) || currentWorld.intersects(xt, yt))) draw = 1;
                    else {
                        for (unsigned p = 0; p < curve_point_number&&!draw; p++) {
                            double x = getStreetSegmentCurvePoint(streetSeg[i][j], p).lon() * cos(AvgLat);
                            double y = getStreetSegmentCurvePoint(streetSeg[i][j], p).lat();
                            if (currentWorld.intersects(x, y)) draw = 1;
                        }
                    }
                    if (draw) {
                        if (temp.speedLimit >= EXPRESSWAY_SPEED) {
                            if (zoom_level >= ROAD_ZOOM_LEVEL) setlinewidth(zoom_level + ROAD_DIFFERENCE);
                            else setlinewidth(2);
                            setcolor(255, 225, 0);

                            LatLon first = getStreetSegmentCurvePoint(streetSeg[i][j], 0);
                            drawline(xf, yf, first.lon() * cos(AvgLat), first.lat());
                            for (unsigned p = 0; p < curve_point_number - 1; p++) {
                                LatLon Cfrom = getStreetSegmentCurvePoint(streetSeg[i][j], p);
                                LatLon Cto = getStreetSegmentCurvePoint(streetSeg[i][j], p + 1);
                                drawline(Cfrom.lon() * cos(AvgLat), Cfrom.lat(), Cto.lon() * cos(AvgLat), Cto.lat());
                            }
                            LatLon last = getStreetSegmentCurvePoint(streetSeg[i][j], curve_point_number - 1);
                            drawline(last.lon() * cos(AvgLat), last.lat(), xt, yt);

                        } else if (temp.speedLimit >= MAJOR_SPEED) {
                            if (zoom_level >= ROAD_ZOOM_LEVEL) setlinewidth(zoom_level);
                            else setlinewidth(1);
                            setcolor(255, 255, 240);
                            if (zoom_level >= 3) {
                                LatLon first = getStreetSegmentCurvePoint(streetSeg[i][j], 0);
                                drawline(xf, yf, first.lon() * cos(AvgLat), first.lat());
                                for (unsigned p = 0; p < curve_point_number - 1; p++) {
                                    LatLon Cfrom = getStreetSegmentCurvePoint(streetSeg[i][j], p);
                                    LatLon Cto = getStreetSegmentCurvePoint(streetSeg[i][j], p + 1);
                                    drawline(Cfrom.lon() * cos(AvgLat), Cfrom.lat(), Cto.lon() * cos(AvgLat), Cto.lat());
                                }
                                LatLon last = getStreetSegmentCurvePoint(streetSeg[i][j], curve_point_number - 1);
                                drawline(last.lon() * cos(AvgLat), last.lat(), xt, yt);
                            }

                        } else {
                            if (zoom_level >= ROAD_ZOOM_LEVEL) setlinewidth(zoom_level - ROAD_DIFFERENCE);
                            else setlinewidth(0);
                            setcolor(WHITE);
                            if (zoom_level >= 4) {
                                LatLon first = getStreetSegmentCurvePoint(streetSeg[i][j], 0);
                                drawline(xf, yf, first.lon() * cos(AvgLat), first.lat());
                                for (unsigned p = 0; p < curve_point_number - 1; p++) {
                                    LatLon Cfrom = getStreetSegmentCurvePoint(streetSeg[i][j], p);
                                    LatLon Cto = getStreetSegmentCurvePoint(streetSeg[i][j], p + 1);
                                    drawline(Cfrom.lon() * cos(AvgLat), Cfrom.lat(), Cto.lon() * cos(AvgLat), Cto.lat());
                                }
                                LatLon last = getStreetSegmentCurvePoint(streetSeg[i][j], curve_point_number - 1);
                                drawline(last.lon() * cos(AvgLat), last.lat(), xt, yt);
                            }
                        }


                    }

                }

            }

        }

    }
}


//--------------------------drawing Features & help functions-------------------------

//function to draw open features 

void openFeature(unsigned i) {
    int number_of_point = getFeaturePointCount(i);

    for (int j = 0; j < number_of_point - 1; j++) {
        LatLon from = getFeaturePoint(i, j);
        LatLon to = getFeaturePoint(i, j + 1);
        drawline(from.lon() * cos(AvgLat), from.lat(), to.lon() * cos(AvgLat), to.lat());
    }
}

//function to draw close features 

void closeFeature(unsigned i) {
    unsigned number_of_point = getFeaturePointCount(i);

    t_point *latlonArray = new t_point[number_of_point];
    //t_point latlonArray[number_of_point];
    for (unsigned j = 0; j < number_of_point; j++) {
        LatLon temp = getFeaturePoint(i, j);
        t_point points(temp.lon() * cos(AvgLat), temp.lat());
        latlonArray[j] = points;
    }
    fillpoly(latlonArray, number_of_point);
    delete [] latlonArray;
}

//function to judge if a feature is closed

bool Feature_close(unsigned FeatureID) {
    bool close = false;
    LatLon Start, End;
    int number_of_point = getFeaturePointCount(FeatureID);

    Start = getFeaturePoint(FeatureID, 0);
    End = getFeaturePoint(FeatureID, number_of_point - 1);

    if (Start.lat() == End.lat() && Start.lon() == End.lon()) close = true;
    return close;
}

t_color color_of_type(FeatureType type);

t_color color_of_type(FeatureType type) {
    t_color color_chosen;
    if (type == Park || type == Greenspace || type == Golfcourse || type == Island) {
        t_color temp(200, 238, 185);
        color_chosen = temp;
    } else if (type == Unknown || type == Beach || type == Building) {
        t_color temp(253, 245, 226);
        color_chosen = temp;
    } else {
        t_color temp(170, 218, 255);
        color_chosen = temp;
    }
    return color_chosen;
}

void drawFeature() {
    //draw closed feature first in the order of decending
    int size_of_area = area_of_index.size();
    for (int i = size_of_area - 1; i >= 0; i--) {
        long double area = area_of_index[i].first;
        unsigned feature_index = area_of_index[i].second;
        FeatureType type = feature_type[feature_index];
        t_color color = color_of_type(type);
        setcolor(color);
        if (area >= 0.00001)closeFeature(feature_index);
        else if (zoom_level >= 3 && zoom_level < 5 && area >= 0.0000001)closeFeature(feature_index);
        else if (zoom_level >= 5)closeFeature(feature_index);
    }
    //draw open feature next
    for (unsigned i = 0; i < openfeatures.size(); i++) {
        unsigned feature_index = openfeatures[i];
        FeatureType type = feature_type[feature_index];
        t_color color = color_of_type(type);
        setcolor(color);
        if (zoom_level >= 3) openFeature(feature_index);
    }

}

//-------------------------------intersection & POI------------------------------

void drawIntersect() {
    t_bound_box currentWorld = get_visible_world();
    for (unsigned i = 0; i < positionInt.size(); i++) {
        LatLon temp = positionInt[i];
        setcolor(150, 225, 255);
        if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat()))
            fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.00003, 0, 360);
    }
}

void drawPOI() {
    t_bound_box currentWorld = get_visible_world();
    if (showPOI) {
        for (unsigned i = 0; i < positionPOI.size(); i++) {
            LatLon temp = positionPOI[i];
            setcolor(ORANGE);
            if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat())) fillarc(temp.lon() * cos(AvgLat), temp.lat(), 0.0001, 0, 360);
        }
    }
}

//--------------------------display names---------------------------------

void drawNames() {
    if (zoom_level >= DETAILED_LEVEL) {//only show names in detailed level
        drawStreetNames();
        //drawFeatureNames();
        drawPOINames();
    }
}

void drawStreetNames() {
    if (zoom_level >= DETAILED_LEVEL) {
        double top, bottom, right, left;
        t_bound_box currentWorld = get_visible_world();
        int angle;
        setcolor(BLACK);
        //go to each segments of street
        for (unsigned i = 0; i < streetSeg.size(); i++) {
            for (unsigned j = 0; j < streetSeg[i].size(); j++) {
                StreetSegmentInfo temp = getStreetSegmentInfo(streetSeg[i][j]);
                double xf = positionInt[temp.from].lon() * cos(AvgLat);
                double yf = positionInt[temp.from].lat();
                double xt = positionInt[temp.to].lon() * cos(AvgLat);
                double yt = positionInt[temp.to].lat();
                unsigned num_curve = temp.curvePointCount;

                //case the street segment has no curved point
                if (num_curve == 0) {
                    if (currentWorld.intersects(xf, yf) || currentWorld.intersects(xt, yt)) {
                        std::string tempName = getStreetName(temp.streetID);
                        angle = atan((yf - yt) / (xf - xt)) / DEG_TO_RAD;
                        if (xf > xt) {
                            right = xf;
                            left = xt;
                        } else {
                            right = xt;
                            left = xf;
                        }
                        if (yf > yt) {
                            top = yf;
                            bottom = yt;
                        } else {
                            top = yt;
                            bottom = yf;
                        }
                        t_bound_box streetBox(left, bottom, right, top);
                        settextrotation(angle);
                        setfontsize(zoom_level - 1);
                        t_point textCenter = streetBox.get_center();
                        if (temp.oneWay) tempName = tempName + "->";
                        if (tempName != "<unknown>")
                            drawtext(textCenter, tempName, streetBox);
                        //reset rotation
                        settextrotation(0);
                    }

                    //case the street segment has curved points
                } else {
                    bool draw = 0;
                    if (currentWorld.intersects(xf, yf) || currentWorld.intersects(xt, yt)) draw = 1;
                    else {
                        for (unsigned p = 0; p < num_curve&&!draw; p++) {
                            double x = getStreetSegmentCurvePoint(streetSeg[i][j], p).lon() * cos(AvgLat);
                            double y = getStreetSegmentCurvePoint(streetSeg[i][j], p).lat();
                            if (currentWorld.intersects(x, y)) draw = 1;
                        }
                    }
                    if (draw == 1) {
                        std::string tempName1 = getStreetName(temp.streetID);

                        double x = getStreetSegmentCurvePoint(streetSeg[i][j], 0).lon() * cos(AvgLat);
                        double y = getStreetSegmentCurvePoint(streetSeg[i][j], 0).lat();

                        t_point textCenter1((xf + x) / 2, (yf + y) / 2);
                        angle = atan((yf - y) / (xf - x)) / DEG_TO_RAD;

                        if (xf > x) {
                            right = xf;
                            left = x;
                        } else {
                            right = x;
                            left = xf;
                        }
                        if (yf > y) {
                            top = yf;
                            bottom = y;
                        } else {
                            top = y;
                            bottom = yf;
                        }
                        t_bound_box streetBox1(left, bottom, right, top);
                        settextrotation(angle);
                        setfontsize(zoom_level - 1);
                        if (temp.oneWay) tempName1 = tempName1 + "->";
                        if (tempName1 != "<unknown>")
                            drawtext(textCenter1, tempName1, streetBox1);

                        //reset rotation
                        settextrotation(0);

                        for (unsigned p = 0; p < num_curve - 1; p++) {
                            std::string tempName2 = getStreetName(temp.streetID);
                            double x1 = getStreetSegmentCurvePoint(streetSeg[i][j], p).lon() * cos(AvgLat);
                            double y1 = getStreetSegmentCurvePoint(streetSeg[i][j], p).lat();
                            double x2 = getStreetSegmentCurvePoint(streetSeg[i][j], p + 1).lon() * cos(AvgLat);
                            double y2 = getStreetSegmentCurvePoint(streetSeg[i][j], p + 1).lat();
                            t_point textCenter2((x1 + x2) / 2, (y1 + y2) / 2);
                            angle = atan((y1 - y2) / (x1 - x2)) / DEG_TO_RAD;


                            if (x1 > x2) {
                                right = x1;
                                left = x2;
                            } else {
                                right = x2;
                                left = x1;
                            }
                            if (y1 > y2) {
                                top = y1;
                                bottom = y2;
                            } else {
                                top = y2;
                                bottom = y1;
                            }
                            t_bound_box streetBox2(left, bottom, right, top);
                            settextrotation(angle);
                            setfontsize(zoom_level - 1);
                            if (temp.oneWay) tempName2 = tempName2 + "->";
                            if (tempName2 != "<unknown>")
                                drawtext(textCenter2, tempName2, streetBox2);

                            //reset rotation
                            settextrotation(0);

                        }
                        double x0 = getStreetSegmentCurvePoint(streetSeg[i][j], num_curve - 1).lon() * cos(AvgLat);
                        double y0 = getStreetSegmentCurvePoint(streetSeg[i][j], num_curve - 1).lat();

                        t_point textCenter3((xt + x0) / 2, (yt + y0) / 2);
                        angle = atan((yt - y0) / (xt - x0)) / DEG_TO_RAD;

                        //double length1 = find_distance_between_two_points(positionInt[temp.to], getStreetSegmentCurvePoint(streetSeg[i][j], num_curve - 1));

                        if (x0 > xt) {
                            right = x0;
                            left = xt;
                        } else {
                            right = xt;
                            left = x0;
                        }
                        if (y0 > yt) {
                            top = y0;
                            bottom = yt;
                        } else {
                            top = yt;
                            bottom = y0;
                        }
                        std::string tempName3 = getStreetName(temp.streetID);
                        t_bound_box streetBox3(left, bottom, right, top);
                        settextrotation(angle);
                        setfontsize(zoom_level - 1);
                        if (temp.oneWay) tempName3 = tempName3 + "->";
                        if (tempName1 != "<unknown>")
                            drawtext(textCenter3, tempName3, streetBox3);

                        //reset rotation
                        settextrotation(0);
                    }
                }
            }
        }
    }
}

int find_feature_longest_edge_angle(unsigned i) {
    int num_of_points = getFeaturePointCount(i);
    LatLon temp1 = getFeaturePoint(i, 0);
    LatLon temp2 = getFeaturePoint(i, 1);
    int longest_index = 0;
    double max_length = (temp1.lon() * cos(AvgLat) - temp2.lon() * cos(AvgLat))*(temp1.lon() * cos(AvgLat) - temp2.lon() * cos(AvgLat))+(temp1.lat() - temp2.lat())*(temp1.lat() - temp2.lat());
    for (int j = 1; j < num_of_points - 1; j++) {
        LatLon temp3 = getFeaturePoint(i, j);
        LatLon temp4 = getFeaturePoint(i, j + 1);
        double length = (temp3.lon() * cos(AvgLat) - temp4.lon() * cos(AvgLat))*(temp3.lon() * cos(AvgLat) - temp4.lon() * cos(AvgLat))+(temp3.lat() - temp4.lat())*(temp3.lat() - temp4.lat());
        if (length > max_length) {
            max_length = length;
            longest_index = j;
        }

    }
    int angle = atan((getFeaturePoint(i, longest_index).lat() - getFeaturePoint(i, longest_index + 1).lat()) / ((getFeaturePoint(i, longest_index).lon() * cos(AvgLat) - getFeaturePoint(i, longest_index + 1).lon() * cos(AvgLat)))) / DEG_TO_RAD;
    return angle;
}

void drawFeatureNames() {
    t_bound_box currentWorld = get_visible_world();
    setcolor(BLACK);
    int angle = 0;
    for (unsigned i = 0; i < getNumberOfFeatures(); i++) {
        bool draw = false;
        if (getFeatureName(i) == "<noname>");
        else {
            for (unsigned j = 0; j < getFeaturePointCount(i)&&!draw; j++) {
                LatLon temp = getFeaturePoint(i, j);
                if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat())) draw = true;
            }
            if (draw) {
                if (Feature_close(i) && zoom_level >= DETAILED_LEVEL) {
                    setfontsize(zoom_level - 1);
                    t_point center = find_center(i);
                    //         angle=find_feature_longest_edge_angle(i);
                    //        settextrotation(angle);
                    drawtext(center, getFeatureName(i), 0.01, 0.01);

                } else if (!Feature_close(i) && zoom_level >= MORE_DETAILED_LEVEL) {
                    for (unsigned j = 0; j < getFeaturePointCount(i) - 1; j++) {
                        LatLon from = getFeaturePoint(i, j);
                        LatLon to = getFeaturePoint(i, j + 1);
                        t_point center((from.lon() * cos(AvgLat) + to.lon() * cos(AvgLat)) / 2, (from.lat() + to.lat()) / 2);
                        setfontsize(zoom_level - 1);
                        angle = atan((from.lat() - to.lat()) / (from.lon() * cos(AvgLat) - to.lon() * cos(AvgLat))) / DEG_TO_RAD;
                        settextrotation(angle);
                        drawtext(center, getFeatureName(i), 0.01, 0.01);
                    }
                }
            }
        }
        settextrotation(0);
    }
}

//function to find center of a feature

t_point find_center(unsigned i) {
    unsigned numOfPoints = getFeaturePointCount(i);
    double x = 0.0, y = 0.0;
    for (unsigned j = 0; j < numOfPoints; j++) {
        LatLon temp = getFeaturePoint(i, j);
        x += temp.lon() * cos(AvgLat);
        y += temp.lat();
    }
    t_point center(x / numOfPoints, y / numOfPoints);
    return center;
}

void drawPOINames() {
    t_bound_box currentWorld = get_visible_world();
    setcolor(BLACK);
    setfontsize(8);
    if (zoom_level >= MORE_DETAILED_LEVEL && showPOI)
        for (unsigned i = 0; i < positionPOI.size(); i++) {
            LatLon temp = positionPOI[i];
            if (currentWorld.intersects(temp.lon() * cos(AvgLat), temp.lat())) {
                std::string POInames = getPointOfInterestName(i);

                drawtext(temp.lon() * cos(AvgLat), temp.lat(), POInames, 0.01, 0.01);
            }
        }
}

//function to show & hide POI

void show_point_of_interest(void drawscreen()) {
    if (!showPOI) showPOI = true;
    else showPOI = false;
    drawscreen();
}

//function to highlight path

void highlight_path(std::vector<unsigned> segmentSet) {
    t_bound_box currentWorld = get_visible_world();
    t_color highlight_color(255, 60, 60);
    setcolor(highlight_color);
    t_point dest;
    t_point begin;
    int previousWidth = gl_state.currentlinewidth;

    if (segmentSet.size() == 0) {
        return;
    }

    for (unsigned i = 0; i < segmentSet.size(); i++) {
        StreetSegmentInfo temp = getStreetSegmentInfo(segmentSet[i]);
        unsigned curve_point_number = temp.curvePointCount;

        double xf = positionInt[temp.from].lon() * cos(AvgLat);
        double yf = positionInt[temp.from].lat();

        double xt = positionInt[temp.to].lon() * cos(AvgLat);
        double yt = positionInt[temp.to].lat();

        begin.x = getIntersectionPosition(itsc_id1).lon() * cos(AvgLat) - currentWorld.get_width()*0.01;
        begin.y = getIntersectionPosition(itsc_id1).lat() + currentWorld.get_height()*0.03;
        dest.x = getIntersectionPosition(itsc_id2).lon() * cos(AvgLat) - currentWorld.get_width()*0.01;
        dest.y = getIntersectionPosition(itsc_id2).lat() + currentWorld.get_height()*0.03;

        setlinewidth(10);
        if ((currentWorld.intersects(xf, yf) || currentWorld.intersects(xt, yt))) {
            if (curve_point_number == 0) {
                drawline(xf, yf, xt, yt);
            } else {
                bool draw = false;
                if ((currentWorld.intersects(xf, yf) || currentWorld.intersects(xt, yt))) draw = 1;
                else {
                    for (unsigned p = 0; p < curve_point_number&&!draw; p++) {
                        double x = getStreetSegmentCurvePoint(segmentSet[i], p).lon() * cos(AvgLat);
                        double y = getStreetSegmentCurvePoint(segmentSet[i], p).lat();
                        if (currentWorld.intersects(x, y)) draw = 1;
                    }
                }
                if (draw) {
                    LatLon first = getStreetSegmentCurvePoint(segmentSet[i], 0);
                    drawline(xf, yf, first.lon() * cos(AvgLat), first.lat());
                    for (unsigned p = 0; p < curve_point_number - 1; p++) {
                        LatLon Cfrom = getStreetSegmentCurvePoint(segmentSet[i], p);
                        LatLon Cto = getStreetSegmentCurvePoint(segmentSet[i], p + 1);
                        drawline(Cfrom.lon() * cos(AvgLat), Cfrom.lat(), Cto.lon() * cos(AvgLat), Cto.lat());
                    }
                    LatLon last = getStreetSegmentCurvePoint(segmentSet[i], curve_point_number - 1);
                    drawline(last.lon() * cos(AvgLat), last.lat(), xt, yt);
                }
            }
        }
    }
    draw_surface(load_png_from_file("libstreetmap/resources/inter.png"), begin);
    draw_surface(load_png_from_file("libstreetmap/resources/dest.png"), dest);
    setlinewidth(previousWidth);
}

//-------------------------------------------------------------------------help button-------------------------------------------------------------
//draw the help logo on the top left corner

void draw_help_button() {
    float hb_width = (get_visible_world().get_width()) * 0.05;
    float hb_height = (get_visible_world().get_height()) * 0.01;
    float hb_left = get_visible_world().right() - hb_width;
    float hb_upper = get_visible_world().top() - hb_height;
    t_point hb(hb_left, hb_upper);
    draw_surface(load_png_from_file("libstreetmap/src/test1.png"), hb);
}

//draw the help content when clicking the help logo

void draw_help_content() {
    float content_width = (get_visible_world().get_width()) * 0.22;
    float content_height = (get_visible_world().get_width()) * 0.09;
    float content_left = (get_visible_world().left()) + content_width;
    float content_top = (get_visible_world().top()) - content_height;
    t_point help_content(content_left, content_top);
    draw_surface(load_png_from_file("libstreetmap/src/help.png"), help_content);
    return;
}

//return the top_left corner location of help button

t_bound_box help_button_location() {
    float hbl_width = (get_visible_world().get_width()) * 0.05;
    float hbl_height = (get_visible_world().get_height()) * 0.06;

    float hb_width = (get_visible_world().get_width()) * 0.05;
    float hb_height = (get_visible_world().get_height()) * 0.01;

    float hbl_left = get_visible_world().right() - hb_width;
    float hbl_top = get_visible_world().top() - hb_height;
    float hbl_right = hbl_left + hbl_width;
    float hbl_bottom = hbl_top - hbl_height;

    t_bound_box hbl(hbl_left, hbl_bottom, hbl_right, hbl_top);

    setcolor(0, 0, 0);
    return hbl;
}

//------------------------------------------------------------------------change map------------------------------------------------------------------

void show_intersection_name(unsigned id) {
    std::string name = getIntersectionName(id);
    auto lat = std::to_string(getIntersectionPosition(id).lat());
    auto lon = std::to_string(getIntersectionPosition(id).lon());
    std::string position = "(" + lat + ", " + lon + ")";
    std::vector<std::string> infoSet;
    infoSet.push_back(position);
    infoSet.push_back(name);
    draw_info_bar(infoSet);
}

//enable users to change map by clicking "change map button";

void change_map(void drawscreen()) {
    close_graphics();
    close_map();
    resetClear();
    draw_map();
}

//clear all the data when closing the map

void resetClear() {
    searchBar1Editing = false;
    searchBar2Editing = false;
    searchBar1AutoFill = false;
    searchBar2AutoFill = false;
    selectSb1Op1 = false;
    selectSb1Op2 = false;
    selectSb2Op1 = false;
    selectSb2Op2 = false;
    resetMark = false;
    clickMode = false;
    //clickModeFind = false;

    clickID1 = -1;
    clickID2 = -1;
    clickCounter = 0;
    is_navigating = false;
    close_nav = true;
    navigateDone = false;
    previousRoute.clear();
    area_of_index.clear();
    zoom_level = 0;
}

//-------------------------------------------------------------------------navigation window--------------------------------------------------------
//helper function of calculating the angel between the street segment and vertical line
//return the angle between street segment and the vertical line (360degree base)

float segment_angle(unsigned seg_id, unsigned from) {
    unsigned itsc_from = from;
    unsigned itsc_to;

    //get the "from" info of two neighbour segment
    if (getStreetSegmentInfo(seg_id).to != from)
        itsc_to = getStreetSegmentInfo(seg_id).to;
    else
        itsc_to = getStreetSegmentInfo(seg_id).from;

    LatLon from_info = getIntersectionPosition(itsc_from);
    LatLon to_info = getIntersectionPosition(itsc_to);
    float y = to_info.lat() - from_info.lat();
    float x = to_info.lon() - from_info.lon();
    float degree = atan(x / y) * 180 / PIE;

    //process the angle to 360 degree base
    if (y <= 0 && x >= 0) degree += 180;
    if (y <= 0 && x <= 0) degree -= 180;
    if (degree <= 0) degree += 360;
    return degree;
}

//helper function of calculating the direction for navigate window
//get the turning direction by calculating the the angle between two street segment

string get_direction(float angle1, float angle2) {
    float difference = angle2 - angle1;
    if (difference < 0) difference += 360;
    float straight_threshold1 = 20;
    float straight_threshold2 = 340;
    float turning_left_threshold = 190;
    float turning_right_threshold = 170;
    if ((difference < straight_threshold1) || (difference > straight_threshold2))
        return "go straight";

    if (difference >= straight_threshold1 && difference < turning_right_threshold)
        return "turn right";

    if (difference > turning_left_threshold && difference <= straight_threshold2)
        return "turn left";

    return "make a Uturn";
}

//helper function of finding the from end and to end of a street segment

vector<unsigned> find_from_to(unsigned id1, unsigned id2) {
    unsigned itsc1_id1 = getStreetSegmentInfo(id1).from;
    unsigned itsc1_id2 = getStreetSegmentInfo(id1).to;
    unsigned itsc2_id1 = getStreetSegmentInfo(id2).from;
    unsigned itsc2_id2 = getStreetSegmentInfo(id2).to;
    vector<unsigned> from_to;

    //find the common intersection id, 
    //the common intersection is the "to" of first seg and "from" of next seg
    if ((itsc1_id1 == itsc2_id1) || (itsc1_id1 == itsc2_id2)) {
        from_to.push_back(itsc1_id2);
        from_to.push_back(itsc1_id1);
        return from_to;
    }
    if ((itsc1_id2 == itsc2_id1) || (itsc1_id2 == itsc2_id2)) {
        from_to.push_back(itsc1_id1);
        from_to.push_back(itsc1_id2);
        return from_to;
    }
    return from_to;
}

//initialize the structure to print the navigation message in text

void navigation_window(vector<unsigned> route) {
    unsigned current = draw_the_route[0]; //segment id
    unsigned next = draw_the_route[0];
    unsigned current_location = 0;
    int count = 0;
    navigate_window_per_page per_message;
    std::vector<navigate_window_per_page> per_page;
    total_info.clear();
    float length = 0;
    string current_name;
    string next_name;
    string direction = "go straight";

    //loop through each segment
    while (current_location < route.size()) {
        current = draw_the_route[current_location];
        //get the name of current segment and next segment
        current_name = getStreetName(getStreetSegmentInfo(current).streetID);
        next_name = getStreetName(getStreetSegmentInfo(next).streetID);

        //corner case1: there is no route
        if (route.size() == 0) {
            total_page = (total_info).size();
            return;
        }

        //corner case2: only one segment in this route
        if (route.size() == 1) {
            per_message.direction = "to the destination";
            per_message.distance = find_street_segment_length(current);
            per_message.name = (getStreetName(getStreetSegmentInfo(current).streetID));
            per_page.push_back(per_message);
            total_info.push_back(per_page);
            per_page.clear();
            per_page.resize(0);
            total_page = (total_info).size();
            return;
        }

        //corner case3: loop to the last segment in the route
        if (current_location == (route.size() - 1)) {
            per_message.direction = "to the destination";
            per_message.distance = length + find_street_segment_length(current);
            per_message.name = current_name;
            per_page.push_back(per_message);
            total_info.push_back(per_page);
            per_page.clear();
            per_page.resize(0);
            total_page = (total_info).size();
            return;

            //normal process
        } else {
            next = draw_the_route[current_location + 1];
            next_name = getStreetName(getStreetSegmentInfo(next).streetID);

            //the current segment and next segment don't belong to the same street
            if (current_name != next_name) {
                count++;
                //get the direction message when changing streets
                vector<unsigned> from_to = find_from_to(current, next);
                float seg1_angle = segment_angle(current, from_to[0]);
                float seg2_angle = segment_angle(next, from_to[1]);
                direction = get_direction(seg1_angle, seg2_angle);

                //put the content of one message 
                per_message.direction = direction;
                per_message.distance = length + find_street_segment_length(current);
                per_message.name = current_name;
                per_page.push_back(per_message);

                //the current segment and next segment belong to the same street
                //then keep calculate its length
            } else {
                length += find_street_segment_length(current);
            }

            //once there exists five message in one page, then store the following message to 
            //next page
            if (count == 5) {
                total_info.push_back(per_page);
                per_page.clear();
                per_page.resize(0);
                length = 0;
                count = 0;
            }
        }
        current_location++;
    }
    total_page = (total_info).size();
    return;
}

//draw the background of navigation window

void draw_navigation_window() {
    float navigation_width = (get_visible_world().get_width()) * 0.23;
    float navigation_height = (get_visible_world().get_height()) * 0.8;
    float navigation_left = (get_visible_world().left());
    float navigation_bottom = (get_visible_world().bottom());
    float navigation_right = navigation_left + navigation_width;
    float navigation_top = navigation_bottom + navigation_height;

    t_bound_box navigation_window(navigation_left, navigation_bottom, navigation_right, navigation_top);

    //draw the whole navigation window (background)
    setcolor(165, 170, 175, 100);
    fillrect(navigation_window);

    //draw the element of navigation window
    navigation_icon();
    instruction_to_close_navigation();

    //draw the text message
    if (!navigation_info_init) {
        fill_navigation_step_text();
        if (total_info.size() > 1) {
            next_page_icon();
            last_page_icon();
        }
    } else navigation_info_init = false;
}

//draw the icon of navigation window (click the icon can show/hide the navigation window)
//return a box of the range of clicking the navigation icon

t_bound_box navigation_icon() {
    float navigation_height = (get_visible_world().get_height()) * 0.8;
    float navigation_width = (get_visible_world().get_width());
    float navigation_left = (get_visible_world().left());
    float navigation_bottom = (get_visible_world().bottom());
    float navigation_top = navigation_bottom + navigation_height;
    t_point ni(navigation_left, navigation_top);
    draw_surface(load_png_from_file("libstreetmap/src/navigate.png"), ni);
    float navigation_down = (navigation_top) - (navigation_height * 0.13);
    float navigation_right = navigation_left + (navigation_width * 0.07);
    t_bound_box navigation(navigation_left, navigation_down, navigation_right, navigation_top);

    return navigation;
}

//return a box where the instruction to remind users of how to hide/show the navigation window

t_bound_box instruction_to_close_navigation() {
    float navigation_width = (get_visible_world().get_width()) * 0.23;
    float navigation_height = (get_visible_world().get_height()) * 0.8;
    float navigation_left = (get_visible_world().left());
    float navigation_right = navigation_left + navigation_width;
    float navigation_bottom = (get_visible_world().bottom());
    float navigation_top = navigation_bottom + navigation_height;

    float instruction_right = navigation_right;
    float instruction_left = instruction_right - navigation_width * 0.7;
    float instruction_top = navigation_top;
    float instruction_bottom = instruction_top - navigation_height * 0.10;

    t_bound_box itcn(instruction_left, instruction_bottom, instruction_right, instruction_top);

    setcolor(255, 255, 255, 50);
    fillrect(itcn);

    setlinewidth(1);
    setcolor(BLACK);
    drawtext_in(itcn, "Click icon to close the window");
    return itcn;
}

//draw the "next page" icon on the window
//return a box where users need to click when they want to go to next page

t_bound_box next_page_icon() {
    float lp_height = (get_visible_world().get_height());
    float lp_width = (get_visible_world().get_width());
    float lp_left = (get_visible_world().left()) + (lp_width * 0.17);
    float lp_bottom = (get_visible_world().bottom()) + (lp_height * 0.01);

    float lp_top = (get_visible_world().bottom()) + (lp_height * 0.04);
    float lp_right = lp_left + (lp_width * 0.02);
    t_point lp_left_top(lp_left, lp_top);
    draw_surface(load_png_from_file("libstreetmap/src/next.png"), lp_left_top);
    t_bound_box lp(lp_left, lp_bottom, lp_right, lp_top);

    return lp;
}

//draw the "last_page" icon
//return a box where users need to click when they want to go to last page

t_bound_box last_page_icon() {
    float lp_height = (get_visible_world().get_height());
    float lp_width = (get_visible_world().get_width());
    float lp_left = (get_visible_world().left()) + (lp_width * 0.03);
    float lp_bottom = (get_visible_world().bottom()) + (lp_height * 0.01);

    float lp_top = (get_visible_world().bottom()) + (lp_height * 0.04);
    float lp_right = lp_left + (lp_width * 0.02);
    t_point lp_left_top(lp_left, lp_top);
    draw_surface(load_png_from_file("libstreetmap/src/last.png"), lp_left_top);
    t_bound_box lp(lp_left, lp_bottom, lp_right, lp_top);

    return lp;
}

//draw 5 boxes where the navigation text will show

void draw_navigation_info() {
    float width = get_visible_world().get_width();
    float height = get_visible_world().get_height();
    float screen_left = get_visible_world().left();
    float screen_bottom = get_visible_world().bottom();
    float screen_right = screen_left + width * 0.205;
    float screen_top = screen_bottom + (height * 0.71);
    float difference = (screen_top - (screen_bottom + height * 0.054)) / 5;
    float offset = difference / 15;
    float box_left = screen_left + (width * 0.0145);
    float box_right = screen_right;

    //the location where the text will draw in
    t_bound_box first_info(box_left, screen_top - difference, box_right, screen_top - offset);
    t_bound_box second_info(box_left, (screen_top - 2 * difference), box_right, (screen_top - 1 * difference));
    t_bound_box third_info(box_left, (screen_top - 3 * difference), box_right, (screen_top - 2 * difference));
    t_bound_box four_info(box_left, (screen_top - 4 * difference), box_right, (screen_top - 3 * difference));
    t_bound_box five_info(box_left, (screen_top - 5 * difference), box_right, (screen_top - 4 * difference));

    //draw the background color of this five boxes
    setcolor(255, 255, 255);
    fillrect(first_info);
    fillrect(third_info);
    fillrect(five_info);
    fillrect(second_info);
    fillrect(four_info);

    //draw the frame of boxes
    setlinewidth(2);
    setcolor(0, 0, 0);
    drawrect(first_info);
    drawrect(third_info);
    drawrect(five_info);
    drawrect(second_info);
    drawrect(four_info);
    return;
}

//choose if draw the next_page and last_page icon (only when total page num larger than 1)

void draw_navigation_message(vector<unsigned> route) {

    navigation_window(route);

    //check if there are more than one page
    //if yes, then draw the next_page & last_page icon
    if (total_info.size() > 1) {
        next_page_icon();
        last_page_icon();
    }

    fill_navigation_step_text();
}

//print the navigation text to each box

void fill_navigation_step_text() {

    float width = get_visible_world().get_width();
    float height = get_visible_world().get_height();
    float screen_left = get_visible_world().left();
    float screen_bottom = get_visible_world().bottom();
    float screen_right = screen_left + width * 0.205;
    float screen_top = screen_bottom + (height * 0.71);
    float difference = (screen_top - (screen_bottom + height * 0.054)) / 5;


    float box_left = screen_left + (width * 0.0145);
    float box_right = screen_right;

    //define the location for the text
    t_bound_box first_1_text(box_left, (screen_top - 0.5 * difference), box_right, screen_top);
    t_bound_box first_2_text(box_left, screen_top - difference, box_right, (screen_top - 0.5 * difference));
    t_bound_box second_1_text(box_left, (screen_top - 1.5 * difference), box_right, (screen_top - 1 * difference));
    t_bound_box second_2_text(box_left, (screen_top - 2 * difference), box_right, (screen_top - 1.5 * difference));
    t_bound_box third_1_text(box_left, (screen_top - 2.5 * difference), box_right, (screen_top - 2 * difference));
    t_bound_box third_2_text(box_left, (screen_top - 3 * difference), box_right, (screen_top - 2.5 * difference));
    t_bound_box four_1_text(box_left, (screen_top - 3.5 * difference), box_right, (screen_top - 3 * difference));
    t_bound_box four_2_text(box_left, (screen_top - 4 * difference), box_right, (screen_top - 3.5 * difference));
    t_bound_box five_1_text(box_left, (screen_top - 4.5 * difference), box_right, (screen_top - 4 * difference));
    t_bound_box five_2_text(box_left, (screen_top - 5 * difference), box_right, (screen_top - 4.5 * difference));

    draw_navigation_info();
    vector<navigate_window_per_page> current_page_info = total_info[current_page - 1];

    //draw each message on one page (maximum 5)
    for (unsigned i = 1; i <= current_page_info.size(); i++) {
        setcolor(0, 0, 0);
        if (i == 1) {
            t_point top_left(box_left, (screen_top - 0.5 * difference));
            draw_direction_logo(((current_page_info[0]).direction), top_left);
            drawtext_in(first_1_text, (current_page_info[0]).name);
            drawtext_in(first_2_text, to_string((int) ((current_page_info[0]).distance)) + "m  " + (current_page_info[0]).direction);
        }
        if (i == 2) {
            t_point top_left(box_left, (screen_top - 1.5 * difference));
            draw_direction_logo(((current_page_info[1]).direction), top_left);
            drawtext_in(second_1_text, (current_page_info[1]).name);
            drawtext_in(second_2_text, to_string((int) ((current_page_info[1]).distance)) + "m  " + (current_page_info[1]).direction);
        }
        if (i == 3) {
            t_point top_left(box_left, (screen_top - 2.5 * difference));
            draw_direction_logo(((current_page_info[2]).direction), top_left);
            drawtext_in(third_1_text, (current_page_info[2]).name);
            drawtext_in(third_2_text, to_string((int) ((current_page_info[2]).distance)) + "m  " + (current_page_info[2]).direction);
        }
        if (i == 4) {
            t_point top_left(box_left, (screen_top - 3.5 * difference));
            draw_direction_logo(((current_page_info[3]).direction), top_left);
            drawtext_in(four_1_text, (current_page_info[3]).name);
            drawtext_in(four_2_text, to_string((int) ((current_page_info[3]).distance)) + "m  " + (current_page_info[3]).direction);
        }
        if (i == 5) {
            t_point top_left(box_left, (screen_top - 4.5 * difference));
            draw_direction_logo(((current_page_info[4]).direction), top_left);
            drawtext_in(five_1_text, (current_page_info[4]).name);
            drawtext_in(five_2_text, to_string((int) ((current_page_info[4]).distance)) + "m  " + (current_page_info[4]).direction);
        }
    }
}

//draw the direction icon beside each text

void draw_direction_logo(string direction, t_point top_left) {
    if (direction == "go straight") {
        draw_surface(load_png_from_file("libstreetmap/src/straight.png"), top_left);
    }
    if (direction == "turn left") {
        draw_surface(load_png_from_file("libstreetmap/src/left.png"), top_left);
    }
    if (direction == "turn right") {
        draw_surface(load_png_from_file("libstreetmap/src/right.png"), top_left);
    }
    if (direction == "make a U turn") {
        draw_surface(load_png_from_file("libstreetmap/src/Uturn.png"), top_left);
    }
    if (direction == "to the destination") {
        draw_surface(load_png_from_file("libstreetmap/src/Destination.png"), top_left);
    }
    return;
}
