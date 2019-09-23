#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

template<unsigned int k>
class Point {
    public:

        /* Constructor */
        Point(int c[k]) {
            for (int i = 0; i < k; i++) {
                this->coordinates[i] = c[i];
            }
        };

        /* Copy constructor & operator  */
        Point(const Point<k>& other) { (*this) = other; };
        Point& operator = (const Point<k>& other) {
            for (int i = 0; i < k; i++) {
                this->coordinates[i] = other.coordinates[i];
            }
        };

        /* Move constructor & operator */
        Point(Point<k>&& other) = default;
        Point& operator = (Point<k>&& other) = default;

        /* Functions */
        void reset(int c[k]) {
            for (int i = 0; i < k; i++) {
                this->coordinates[i] = c[i];
            }
        };

        unsigned int distanceTo(const Point& p) const {     
            unsigned int distance = 0;
            for (int i = 0; i < k; i++) {
                distance += (this->coordinates[i] - p.coordinates[i]) * (this->coordinates[i] - p.coordinates[i]);
            }

            return distance;
        };

        /* Operator Overloading */
        friend ostream& operator << (ostream& os, const Point& p){
            os << "(" << p.coordinates[0];
            for (int i = 1; i < k; i++) {
                os << ", " << p.coordinates[i];
            }
            os << ")";

            return os;
        }

        bool operator == (const Point& p) const {
            for (int i = 0; i < k; i++) {
                if (this->coordinates[i] != p.coordinates[i]) {
                    return false;
                }
            }

            return true;
        }

        bool operator != (const Point& p) const {
            return !(this->operator==(p));
        }

    // private:
        int coordinates[k];

};
