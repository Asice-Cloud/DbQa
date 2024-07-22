//
// Created by asice on 2024/7/22.
//

#ifndef DBQA_QBIT_H
#define DBQA_QBIT_H

#include <iostream>
#include <bitset>
#include <complex>
#include <numeric>
#include <random>
#include <ctime>
#include "matrix.h"

using namespace std::complex_literals;

#define MAX_QbitS 8

class Qbit {

private:
	unsigned	QbitNumber;
	Matrix		amplitudes;
	std::default_random_engine generator;

public:
	Qbit(unsigned = 0) noexcept;
	Qbit(const Qbit&) noexcept;

	virtual ~Qbit();

	std::complex<double>&		operator () (const unsigned&);
	const std::complex<double>&	operator () (const unsigned&) const;

	Qbit&	operator = (const Qbit&);
	Qbit	operator * (const Qbit&);
	Qbit&	operator *= (const Qbit&);

	bool operator == (const Qbit&);
	bool operator != (const Qbit&);

	Qbit	operator * (Matrix&);
	Qbit&	operator *= (Matrix&);

	bool operator == (const Matrix&);
	bool operator != (const Matrix&);

	void distribute(std::vector<int>);
	void concentrate(unsigned);
	void reset();

	Matrix				getAmplitudes();
	unsigned			getQbitNumber();
	std::vector<double>		getProbabilities();

	static Matrix	identity();
	static bool	identity(Qbit&);

	static Matrix	pauliX();
	static bool	pauliX(Qbit&);

	static Matrix	pauliY();
	static bool	pauliY(Qbit&);

	static Matrix	pauliZ();
	static bool	pauliZ(Qbit&);

	static Matrix	hadamard();
	static bool	hadamard(Qbit&);

	static Matrix	phaseShift(double);
	static bool	phaseShift(Qbit&, double);

	static Matrix	swap();
	static bool	swap(Qbit&);

	static Matrix	rotX(double);
	static bool	rotX(Qbit&, double);

	static Matrix	rotY(double);
	static bool	rotY(Qbit&, double);

	static Matrix	rotZ(double);
	static bool	rotZ(Qbit&, double);

	static Matrix	controlled(unsigned, std::string, std::string = "");
	static bool	controlled(Qbit&, unsigned, std::string, std::string = "");

	void measure();

	void printProbabilities();

	friend std::ostream& operator << (std::ostream&, Qbit&);
};

#endif //DBQA_QBIT_H
