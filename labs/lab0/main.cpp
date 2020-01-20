// Created by Tristan on 1/17/2020.
// CSCI 441 Spring 2020
// David Millman
// 1/21/2020

#include <iostream>
#include <string>

class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3() : x(0), y(0), z(0) {
	//initializes a Vector3 set to 0,0,0
	std::cout << "in Vector3 default constructor" << std::endl;
    }

    // Constructor
    Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {
        // nothing to do here as we've already initialized x, y, and z above
        std::cout << "in Vector3 constructor" << std::endl;
    }

    // Destructor - called when an object goes out of scope or is destroyed
    ~Vector3() {
        // this is where you would release resources such as memory or file descriptors
        // in this case we don't need to do anything
        std::cout << "in Vector3 destructor" << std::endl;
    }

};
//Part 9: changed args to const Vector3&
Vector3 add(const Vector3& v, const Vector3& v2) {
    //Returns a Vector3 that is made from the summation of the components of v and v2
    return Vector3((v.x+v2.x),(v.y+v2.y),(v.z+v2.z));
}
//Part 9: changed args to const Vector3&
Vector3 operator+(const Vector3& v, const Vector3& v2) {
	float x = v.x + v2.x;
	float y = v.y + v2.y;
	float z = v.z + v2.z;
	return Vector3(x, y, z);
}
//overloads the << operator to print out a Vector3 object
std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
	stream << v.x << ", " << v.y << ", " << v.z;
	return stream;
}

int main(int argc, char** argv) {
    /*Part 4*/
    //std::string name;
    //std::cout << "Hello world " << argv[0] << " " << 1234 << " " << std::endl;
    //std::cout << "Please enter your name: ";
    //std::cin >> name;
    //std::cout << "Hello "<< name << std::endl;

    /*Part 5*/
    //Vector3 a(1,2,3);
    //Vector3 b(4,5,6);
    //Vector3 c = add(a, b);
    //std::cout << "Vector3 c is: [" << c.x << ", " << c.y << ", " << c.z << "] " << std::endl;

    /*Part 6*/
    Vector3 a(1,2,3);
    Vector3 b(4,5,6);
    std::cout << a+b <<std::endl;

    /*Part 7*/
    //Vector3 s(0,0,0);
    //s.y = 5;
    //std::cout << s << std::endl;

    //Vector3* h = new Vector3(0,0,0);
    //h->y = 5;
    //std::cout << *h << std::endl;
    //delete h;

    /*Part 8*/
    //Vector3 v_array[10];
    //Vector3* ptr = v_array;

    //Vector3* array_of_vector3s = new Vector3[10];
    //for (int i = 0; i < 10; i++) {
    //	array_of_vector3s[i].y = 5;
    //	std::cout << array_of_vector3s[i] << std::endl;
    //}
    //delete [] array_of_vector3s;
}
