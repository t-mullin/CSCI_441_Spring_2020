#ifndef _CSCI441_SHAPE_H_
#define _CSCI441_SHAPE_H_

#include <cstdlib>
#include <vector>

template <typename T, typename N, typename C>
void add_vertex(T& coords, const N& x, const N& y, const N& z,
        const C& r, const C& g, const C& b,
        const Vector4& n=Vector4(1,0,0), bool with_noise=false) {
    // adding color noise makes it easier to see before shading is implemented
    float noise = 1-with_noise*(rand()%150)/100.;
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
    coords.push_back(r*noise);
    coords.push_back(g*noise);
    coords.push_back(b*noise);

    Vector4 normal = n.normalized();
    coords.push_back(normal.x());
    coords.push_back(normal.y());
    coords.push_back(normal.z());
}

template <typename T, typename N, typename C, typename S>
//modified add_vertex that adds normals to vertex
void add_vertex(T& coords, const N& x, const N& y, const N& z,
                const C& r, const C& g, const C& b,
                const S& nX, const S& nY, const S& nZ, bool with_noise=false) {
    // adding color noise makes it easier to see before shading is implemented
    float noise = 1-with_noise*(rand()%150)/100.;
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
    coords.push_back(r*noise);
    coords.push_back(g*noise);
    coords.push_back(b*noise);
    coords.push_back(nX);
    coords.push_back(nY);
    coords.push_back(nZ);
}

//Generates the face normal using 3 vertices. Also normalizes them.
std::vector<float> generateNormals(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3) {
    std::vector<float> A;
    std::vector<float> B;
    std::vector<float> C;
    float length;
    std::vector<float> normal;

    A.push_back(vert2.at(0) - vert1.at(0));
    A.push_back(vert2.at(1) - vert1.at(1));
    A.push_back(vert2.at(2) - vert1.at(2));

    B.push_back(vert3.at(0) - vert1.at(0));
    B.push_back(vert3.at(1) - vert1.at(1));
    B.push_back(vert3.at(2) - vert1.at(2));

    C.push_back((A.at(1)*B.at(2)) - (A.at(2)*B.at(1)));
    C.push_back((A.at(2)*B.at(0)) - (A.at(0)*B.at(2)));
    C.push_back((A.at(0)*B.at(1)) - (A.at(1)*B.at(0)));

    length = sqrtf(pow(C.at(0), 2) + pow(C.at(1), 2) + pow(C.at(2), 2));

    //normalize the normal
    normal.push_back(C.at(0)/length);
    normal.push_back(C.at(1)/length);
    normal.push_back(C.at(2)/length);

    return normal;
}

class DiscoCube {
public:
    std::vector<float> coords;
    DiscoCube() : coords{
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,

        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0
    } {}

};

class Cylinder {
public:
    std::vector<float> coords;
    std::vector<float> vertex1;
    std::vector<float> vertex2;
    std::vector<float> vertex3;
    std::vector<float> normal;

    Cylinder(unsigned int n, float r, float g, float b) {
        double step_size = 2*M_PI / n;
        double c_x=0;
        double c_y=0;
        double h = .5;
        float radius = .5;

        for (int i = 0; i < n; ++i) {
            // vertex i
            double theta_i = i*step_size;
            double vi_x = radius*cos(theta_i);
            double vi_y = radius*sin(theta_i);

            // vertex i+1
            double theta_ip1 = ((i+1)%n)*step_size;
            double vip1_x = radius*cos(theta_ip1);
            double vip1_y = radius*sin(theta_ip1);

            vertex1.push_back(vi_x);
            vertex1.push_back(-h);
            vertex1.push_back(vi_y);
            vertex2.push_back(vi_x);
            vertex2.push_back(h);
            vertex2.push_back(vi_y);
            vertex3.push_back(vip1_x);
            vertex3.push_back(-h);
            vertex3.push_back(vip1_y);

            normal = generateNormals(vertex1, vertex2, vertex3);

            add_vertex(coords, vi_x, -h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vi_x, h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vip1_x, -h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));

            vertex1.clear();
            vertex2.clear();
            vertex3.clear();
            normal.clear();

            vertex1.push_back(vip1_x);
            vertex1.push_back(-h);
            vertex1.push_back(vip1_y);
            vertex2.push_back(vi_x);
            vertex2.push_back(h);
            vertex2.push_back(vi_y);
            vertex3.push_back(vip1_x);
            vertex3.push_back(h);
            vertex3.push_back(vip1_y);

            normal = generateNormals(vertex1, vertex2, vertex3);

            // add triangle vip1L, viH, vip1H
            add_vertex(coords, vip1_x, -h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vi_x, h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vip1_x, h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));

            vertex1.clear();
            vertex2.clear();
            vertex3.clear();
            normal.clear();

            vertex1.push_back(vip1_x);
            vertex1.push_back(h);
            vertex1.push_back(vip1_y);
            vertex2.push_back(vi_x);
            vertex2.push_back(h);
            vertex2.push_back(vi_y);
            vertex3.push_back(c_x);
            vertex3.push_back(h);
            vertex3.push_back(c_y);

            normal = generateNormals(vertex1, vertex2, vertex3);

            // add high triangle vi, vip1, 0
            Vector4 nh(0, 1, 0);
            add_vertex(coords, vip1_x, h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vi_x, h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, c_x, h, c_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));

            vertex1.clear();
            vertex2.clear();
            vertex3.clear();
            normal.clear();

            vertex1.push_back(vip1_x);
            vertex1.push_back(-h);
            vertex1.push_back(vip1_y);
            vertex2.push_back(c_x);
            vertex2.push_back(-h);
            vertex2.push_back(c_y);
            vertex3.push_back(vi_x);
            vertex3.push_back(-h);
            vertex3.push_back(vi_y);

            normal = generateNormals(vertex1, vertex2, vertex3);

            // // add low triangle vi, vip1, 0
            Vector4 nl(0, -1, 0);
            add_vertex(coords, vip1_x, -h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, c_x, -h, c_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vi_x, -h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));

            vertex1.clear();
            vertex2.clear();
            vertex3.clear();
            normal.clear();
        }
    }
};


class Cone {
public:
    std::vector<float> coords;
    std::vector<float> vertex1;
    std::vector<float> vertex2;
    std::vector<float> vertex3;
    std::vector<float> normal;
    Cone(unsigned int n, float r, float g, float b) {

        double step_size = 2*M_PI / n;
        double c_x=0;
        double c_y=0;
        double h = .5;
        float radius = .5;

        for (int i = 0; i < n; ++i) {
            // vertex i
            double theta_i = i*step_size;
            double vi_x = radius*cos(theta_i);
            double vi_y = radius*sin(theta_i);

            // vertex i+1
            double theta_ip1 = ((i+1)%n)*step_size;
            double vip1_x = radius*cos(theta_ip1);
            double vip1_y = radius*sin(theta_ip1);

            vertex1.push_back(vi_x);
            vertex1.push_back(-h);
            vertex1.push_back(vi_y);
            vertex2.push_back(c_x);
            vertex2.push_back(h);
            vertex2.push_back(c_y);
            vertex3.push_back(vip1_x);
            vertex3.push_back(-h);
            vertex3.push_back(vip1_y);

            normal = generateNormals(vertex1, vertex2, vertex3);

            // add triangle viL, viH, vip1L
            add_vertex(coords, vi_x, -h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, c_x, h, c_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vip1_x, -h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));

            vertex1.clear();
            vertex2.clear();
            vertex3.clear();
            normal.clear();

            vertex1.push_back(vip1_x);
            vertex1.push_back(-h);
            vertex1.push_back(vip1_y);
            vertex2.push_back(c_x);
            vertex2.push_back(-h);
            vertex2.push_back(c_y);
            vertex3.push_back(vi_x);
            vertex3.push_back(-h);
            vertex3.push_back(vi_y);

            normal = generateNormals(vertex1, vertex2, vertex3);

            // // add low triangle vi, vip1, 0
            add_vertex(coords, vip1_x, -h, vip1_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, c_x, -h, c_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));
            add_vertex(coords, vi_x, -h, vi_y, r, g, b, normal.at(0), normal.at(1), normal.at(2));

            vertex1.clear();
            vertex2.clear();
            vertex3.clear();
            normal.clear();
        }
    }
};

class Sphere {
    double x(float r, float phi, float theta){
        return r*cos(theta)*sin(phi);
    }

    double y(float r, float phi, float theta){
        return r*sin(theta)*sin(phi);
    }

    double z(float r, float phi, float theta){
        return r*cos(phi);
    }

public:
    std::vector<float> coords;
    std::vector<float> vertex1;
    std::vector<float> vertex2;
    std::vector<float> vertex3;
    std::vector<float> normal;
    Sphere(unsigned int n, float radius, float r, float g, float b) {
        int n_steps = (n%2==0) ? n : n+1;
        double step_size = 2*M_PI / n_steps;

        for (int i = 0; i < n_steps/2.0; ++i) {
            for (int j = 0; j < n_steps; ++j) {
                double phi_i = i*step_size;
                double phi_ip1 = ((i+1)%n_steps)*step_size;
                double theta_j = j*step_size;
                double theta_jp1 = ((j+1)%n_steps)*step_size;

                // vertex i,j
                double vij_x = x(radius, phi_i, theta_j);
                double vij_y = y(radius, phi_i, theta_j);
                double vij_z = z(radius, phi_i, theta_j);

                // vertex i+1,j
                double vip1j_x = x(radius, phi_ip1, theta_j);
                double vip1j_y = y(radius, phi_ip1, theta_j);
                double vip1j_z = z(radius, phi_ip1, theta_j);

                // vertex i,j+1
                double vijp1_x = x(radius, phi_i, theta_jp1);
                double vijp1_y = y(radius, phi_i, theta_jp1);
                double vijp1_z = z(radius, phi_i, theta_jp1);

                // vertex i+1,j+1
                double vip1jp1_x = x(radius, phi_ip1, theta_jp1);
                double vip1jp1_y = y(radius, phi_ip1, theta_jp1);
                double vip1jp1_z = z(radius, phi_ip1, theta_jp1);

                vertex1.push_back(vij_x);
                vertex1.push_back(vij_y);
                vertex1.push_back(vij_z);
                vertex2.push_back(vip1j_x);
                vertex2.push_back(vip1j_y);
                vertex2.push_back(vip1j_z);
                vertex3.push_back(vijp1_x);
                vertex3.push_back(vijp1_y);
                vertex3.push_back(vijp1_z);

                normal = generateNormals(vertex1, vertex2, vertex3);

                // add triangle
                add_vertex(coords, vij_x, vij_y, vij_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vip1j_x, vip1j_y, vip1j_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vijp1_x, vijp1_y, vijp1_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));

                vertex1.clear();
                vertex2.clear();
                vertex3.clear();
                normal.clear();

                vertex1.push_back(vijp1_x);
                vertex1.push_back(vijp1_y);
                vertex1.push_back(vijp1_z);
                vertex2.push_back(vip1jp1_x);
                vertex2.push_back(vip1jp1_y);
                vertex2.push_back(vip1jp1_z);
                vertex3.push_back(vip1j_x);
                vertex3.push_back(vip1j_y);
                vertex3.push_back(vip1j_z);

                normal = generateNormals(vertex2, vertex1, vertex3);

                // add triangle
                add_vertex(coords, vijp1_x, vijp1_y, vijp1_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vip1jp1_x, vip1jp1_y, vip1jp1_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vip1j_x, vip1j_y, vip1j_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));

                vertex1.clear();
                vertex2.clear();
                vertex3.clear();
                normal.clear();
            }
        }
    }
};

class Torus {
    double x(float c, float a, float phi, float theta) {
        return (c+a*cos(theta))*cos(phi);
    }

    double y(float c, float a, float phi, float theta) {
        return (c+a*cos(theta))*sin(phi);
    }

    double z(float c, float a, float phi, float theta) {
        return a*sin(theta);
    }

public:
    std::vector<float> coords;
    std::vector<float> vertex1;
    std::vector<float> vertex2;
    std::vector<float> vertex3;
    std::vector<float> normal;

    Torus(unsigned int n, float c, float a, float r, float g, float b) {

        double step_size = 2*M_PI / n;
        double c_x=0;
        double c_y=0;
        double h = .5;
        float radius = .5;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                double phi_i = i*step_size;
                double phi_ip1 = ((i+1)%n)*step_size;
                double theta_j = j*step_size;
                double theta_jp1 = ((j+1)%n)*step_size;

                // vertex i,j
                double vij_x = x(c, a, phi_i, theta_j);
                double vij_y = y(c, a, phi_i, theta_j);
                double vij_z = z(c, a, phi_i, theta_j);

                // vertex i+1,j
                double vip1j_x = x(c, a, phi_ip1, theta_j);
                double vip1j_y = y(c, a, phi_ip1, theta_j);
                double vip1j_z = z(c, a, phi_ip1, theta_j);

                // vertex i,j+1
                double vijp1_x = x(c, a, phi_i, theta_jp1);
                double vijp1_y = y(c, a, phi_i, theta_jp1);
                double vijp1_z = z(c, a, phi_i, theta_jp1);

                // vertex i+1,j+1
                double vip1jp1_x = x(c, a, phi_ip1, theta_jp1);
                double vip1jp1_y = y(c, a, phi_ip1, theta_jp1);
                double vip1jp1_z = z(c, a, phi_ip1, theta_jp1);

                vertex1.push_back(vij_x);
                vertex1.push_back(vij_y);
                vertex1.push_back(vij_z);
                vertex2.push_back(vip1j_x);
                vertex2.push_back(vip1j_y);
                vertex2.push_back(vip1j_z);
                vertex3.push_back(vijp1_x);
                vertex3.push_back(vijp1_y);
                vertex3.push_back(vijp1_z);

                normal = generateNormals(vertex1, vertex2, vertex3);

                // add triangle
                add_vertex(coords, vij_x, vij_y, vij_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vip1j_x, vip1j_y, vip1j_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vijp1_x, vijp1_y, vijp1_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));

                vertex1.clear();
                vertex2.clear();
                vertex3.clear();
                normal.clear();

                vertex1.push_back(vijp1_x);
                vertex1.push_back(vijp1_y);
                vertex1.push_back(vijp1_z);
                vertex2.push_back(vip1jp1_x);
                vertex2.push_back(vip1jp1_y);
                vertex2.push_back(vip1jp1_z);
                vertex3.push_back(vip1j_x);
                vertex3.push_back(vip1j_y);
                vertex3.push_back(vip1j_z);

                normal = generateNormals(vertex2, vertex1, vertex3);

                // add triangle
                add_vertex(coords, vijp1_x, vijp1_y, vijp1_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vip1jp1_x, vip1jp1_y, vip1jp1_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));
                add_vertex(coords, vip1j_x, vip1j_y, vip1j_z, r, g, b, normal.at(0), normal.at(1), normal.at(2));

                vertex1.clear();
                vertex2.clear();
                vertex3.clear();
                normal.clear();
            }
        }
    }

};

#endif
