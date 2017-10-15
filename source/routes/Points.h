/*
 * Points.h
 *
 *  Created on: 15 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_ROUTES_POINTS_H_
#define SOURCE_ROUTES_POINTS_H_

#include "utils.h"

class Point2D {
public:
    Point2D();
    Point2D(float lat, float lon);
    Point2D & operator=(const Point2D &point);
    Point2D & operator=(const Point2D *point);
    //void toString();
    //int isValid();
    float dist(Point2D *autre_p)  {
        return distance_between(_lat, _lon, autre_p->_lat, autre_p->_lon);
    }
public:
    float _lat;
    float _lon;
};

class Point : public Point2D {
public:
    Point();
    Point(float lat, float lon, float alt, float rtime);
    Point & operator=(const Point &point);
    Point & operator=(const Point *point);
    void toString();
    int isValid();
    float dist(Point *autre_p)  {
        return distance_between(_lat, _lon, autre_p->_lat, autre_p->_lon);
    }
    float dist(Point autre_p)  {
        return distance_between(_lat, _lon, autre_p._lat, autre_p._lon);
    }
    float dist(float lat_, float lon_)  {
    	return distance_between(_lat, _lon, lat_, lon_);
    }

public:
    float _alt;
    float _rtime;
};


#endif /* SOURCE_ROUTES_POINTS_H_ */
