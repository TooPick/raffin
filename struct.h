#ifndef point3_HPP
#define point3_HPP

#include <iostream>
using namespace std ;

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

/******************* point3 ******************/

class	point3{

public:

	double x;
	double y;
	double z;



	point3(); // (0,0,0) par defaut

	point3(const double, const double, const double);


	bool operator==(const point3 &)const;

	point3& operator=(const point3 &);

	point3 operator+(const point3 &)const;

	point3& operator*=(const double); //produit par un scalaire

	point3 operator*(const double)const; //idem

	point3& operator/=(const double); //division par un scalaire

	point3 operator/(const double)const; //idem


	void	Permutation(point3 *, point3 *);	 // Permutation de deux points

	friend ostream& operator<<(ostream&, point3);

	friend istream& operator>>(istream&, point3 &);

};
#endif
