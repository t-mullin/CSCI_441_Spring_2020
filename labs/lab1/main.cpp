// Created by Tristan on 1/23/2020.
// CSCI 441 Spring 2020
// David Millman
// 1/28/2020

#include <iostream>
#include <string>
#include "bitmap_image.hpp"

class Vector5 {
public:
    float x;
    float y;
    float r;
    float b;
    float g;

    Vector5() : x(0), y(0), r(0), g(0), b(0) {
        //initializes a Vector5 set to 0,0,0
        //std::cout << "in Vector5 default constructor" << std::endl;
    }

    // Constructor
    Vector5(float xx, float yy, float rr, float gg, float bb) : x(xx), y(yy), r(rr), g(gg), b(bb) {
        // nothing to do here as we've already initialized x, y, r, g, and b above
        //std::cout << "in Vector5 constructor" << std::endl;
    }

    // Destructor - called when an object goes out of scope or is destroyed
    ~Vector5() {
        // this is where you would release resources such as memory or file descriptors
        // in this case we don't need to do anything
        //std::cout << "in Vector5 destructor" << std::endl;
    }

};

//overloads the << operator to print out a Vector3 object
std::ostream& operator<<(std::ostream& stream, const Vector5& v) {
    stream << v.x << ", " << v.y << ": " << v.r << ", " << v.g << ", " << v.b;
    return stream;
}

//checks to see if the point is in the triangle
bool inTriangle(float l_1, float l_2, float l_3) {
    if(l_1 >= 0 && l_1 <= 1) {
        if(l_2 >= 0 && l_2 <= 1) {
            if(l_3 >= 0 && l_3 <= 1) {
                return true;
            }
        }
    }
    return false;
}

//gets the a value between 0 and 255
int getValue(float l1, float l2, float l3, float v1, float v2, float v3) {
    return 255*((l1*v1)+(l2*v2)+(l3*v3));
}

int main(int argc, char** argv) {
    /* Part 1 and Part 3 User input*/
    Vector5* v_array = new Vector5[3];
    std::cout << "Enter 3 points (enter a point as x,y:r,g,b):" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cin >> v_array[i].x >> v_array[i].y >> v_array[i].r >> v_array[i].g >> v_array[i].b;
    }
    std::cout << "You entered:" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << v_array[i] << std::endl;
    }

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(640, 480);

    /* Part 1: White Box */
    /*
    int x_min = fminf(fminf(v_array[0].x, v_array[1].x), v_array[2].x);
    int x_max = fmaxf(fmaxf(v_array[0].x, v_array[1].x), v_array[2].x);
    int y_min = fminf(fminf(v_array[0].y, v_array[1].y), v_array[2].y);
    int y_max = fmaxf(fmaxf(v_array[0].y, v_array[1].y), v_array[2].y);

    rgb_t color = make_colour(255, 255, 255);
    for(int i = x_min; i <= x_max; i++) {
       for(int j = y_min; j <= y_max; j++) {
           image.set_pixel(i,j,color);
        }
    }
    */

    /* Part 2: White Triangle*/
    /*
    float lambda_1;
    float lambda_2;
    float lambda_3;
    float det = ((v_array[1].y - v_array[2].y)*(v_array[0].x - v_array[2].x))+((v_array[2].x - v_array[1].x)*(v_array[0].y - v_array[2].y));

    float x_min = fminf(fminf(v_array[0].x, v_array[1].x), v_array[2].x);
    float x_max = fmaxf(fmaxf(v_array[0].x, v_array[1].x), v_array[2].x);
    float y_min = fminf(fminf(v_array[0].y, v_array[1].y), v_array[2].y);
    float y_max = fmaxf(fmaxf(v_array[0].y, v_array[1].y), v_array[2].y);

    rgb_t color = make_colour(255, 255, 255);
    for(int i = x_min; i <= x_max; i++) {
        for(int j = y_min; j <= y_max; j++) {
            lambda_1 = (((v_array[1].y - v_array[2].y)*(i - v_array[2].x))+((v_array[2].x - v_array[1].x)*(j - v_array[2].y)))/det;
            lambda_2 = (((v_array[2].y - v_array[0].y)*(i - v_array[2].x))+((v_array[0].x - v_array[2].x)*(j - v_array[2].y)))/det;
            lambda_3 = 1 - lambda_1 - lambda_2;

            if(inTriangle(lambda_1, lambda_2, lambda_3)) {
                image.set_pixel(i,j,color);
            }
        }
    }
     */

    /* Part 3: Rainbow Triangle*/
    float lambda_1;
    float lambda_2;
    float lambda_3;
    //calculates the determinate
    float det = ((v_array[1].y - v_array[2].y)*(v_array[0].x - v_array[2].x))+((v_array[2].x - v_array[1].x)*(v_array[0].y - v_array[2].y));

    int red;
    int green;
    int blue;

    //setting up bounds by way of min/max values of x and y
    float x_min = fminf(fminf(v_array[0].x, v_array[1].x), v_array[2].x);
    float x_max = fmaxf(fmaxf(v_array[0].x, v_array[1].x), v_array[2].x);
    float y_min = fminf(fminf(v_array[0].y, v_array[1].y), v_array[2].y);
    float y_max = fmaxf(fmaxf(v_array[0].y, v_array[1].y), v_array[2].y);

    //loops through x values
    for(int i = x_min; i <= x_max; i++) {
        //loops through y values
        for(int j = y_min; j <= y_max; j++) {
            //calculates the values of lambda
            lambda_1 = (((v_array[1].y - v_array[2].y)*(i - v_array[2].x))+((v_array[2].x - v_array[1].x)*(j - v_array[2].y)))/det;
            lambda_2 = (((v_array[2].y - v_array[0].y)*(i - v_array[2].x))+((v_array[0].x - v_array[2].x)*(j - v_array[2].y)))/det;
            lambda_3 = 1 - lambda_1 - lambda_2;
            //checks if the value is in the triangle. do nothing if outside triangle
            if(inTriangle(lambda_1, lambda_2, lambda_3)) {
                //sets the individual rgb values
                red = getValue(lambda_1, lambda_2, lambda_3, v_array[0].r, v_array[1].r, v_array[2].r);
                green = getValue(lambda_1, lambda_2, lambda_3, v_array[0].g, v_array[1].g, v_array[2].g);
                blue = getValue(lambda_1, lambda_2, lambda_3, v_array[0].b, v_array[1].b, v_array[2].b);

                //makes the color
                rgb_t color = make_colour(red, green, blue);
                //sets the pixels color
                image.set_pixel(i,j,color);
            }
        }
    }

    image.save_image("triangle.bmp");
    std::cout << "Success" << std::endl;
    delete [] v_array;
}
