//
// Created by asice on 2024/7/22.
//
#include "Qbit.h"

Qbit::Qbit(unsigned Qbits) noexcept : QbitNumber(Qbits) {
	generator.seed((unsigned)time(NULL));
	if (Qbits != 0)
		amplitudes = Matrix(1, (unsigned)pow(2, Qbits), sqrt(1.0 / pow(2, Qbits)));
}

Qbit::Qbit(const Qbit& q) noexcept {
	this->QbitNumber = q.QbitNumber;
	this->amplitudes = q.amplitudes;
	this->generator = q.generator;
}

Qbit::~Qbit() {}

std::complex<double>& Qbit::operator () (const unsigned& index) {
	return this->amplitudes(0, index);
}

const std::complex<double>& Qbit::operator () (const unsigned& index) const {
	return this->amplitudes(0, index);
}

Qbit& Qbit::operator = (const Qbit& q) {
	if (this == &q || this->amplitudes == q.amplitudes)
		return *this;
	this->amplitudes = q.amplitudes;
	this->QbitNumber = q.QbitNumber;
	return *this;
}

Qbit Qbit::operator * (const Qbit& q) {
	if (pow(2, this->QbitNumber) * pow(2, q.QbitNumber) > MAX_QbitS) {
		std::cout << "Qbits limit exceeded\n";
		return *this;
	}
	Qbit res(this->QbitNumber + q.QbitNumber);
	for (unsigned i = 0; i < pow(2, this->QbitNumber); i++)
		for (unsigned j = 0; j < pow(2, q.QbitNumber); j++)
			res.amplitudes(0, i * q.QbitNumber + j) = this->amplitudes(0, i) * q.amplitudes(0, j);
	return res;
}

Qbit& Qbit::operator *= (const Qbit& q) {
	Qbit res = (*this) * q;
	(*this) = res;
	return *this;
}

bool Qbit::operator == (const Qbit& q) {
	return this->amplitudes == q.amplitudes;
}

bool Qbit::operator != (const Qbit& q) {
	return this->amplitudes != q.amplitudes;
}

Qbit Qbit::operator * (Matrix& m) {
	if (this->amplitudes.getCols() != m.getRows())
		return *this;
	Qbit res(this->QbitNumber);
	Matrix ampl(this->amplitudes.getRows(), m.getCols(), 0.0 + 0.0i);
	for (unsigned i = 0; i < this->amplitudes.getRows(); i++)
		for (unsigned j = 0; j < m.getCols(); j++)
			for (unsigned k = 0; k < m.getRows(); k++)
				ampl(i, j) += this->amplitudes(i, k) * m(k, j);
	res.amplitudes = ampl;
	return res;
}

Qbit& Qbit::operator *= (Matrix& m) {
	Qbit res = (*this) * m;
	(*this) = res;
	return *this;
}

bool Qbit::operator == (const Matrix& m) {
	return this->amplitudes == m;
}

bool Qbit::operator != (const Matrix& m) {
	return this->amplitudes != m;
}

void Qbit::distribute(std::vector<int> indices) {
	int count = 0;
	for (int i : indices)
		count += (int)(i > -(int)pow(2, this->QbitNumber) && i < (int)pow(2, this->QbitNumber));
	std::cout << count << "\n";
	for (unsigned i = 0; i < this->amplitudes.getCols(); i++)
		this->amplitudes(0, i) = 0;
	for (int i : indices)
		if (i > -(int)pow(2, this->QbitNumber) && i < (int)pow(2, this->QbitNumber))
			this->amplitudes(0, i) = (i < 0) ? sqrt(1.0 / count) : -sqrt(1.0 / count);
	return;
}

void Qbit::concentrate(unsigned index) {
	for (unsigned i = 0; i < this->amplitudes.getMat()[0].size(); i++)
		this->amplitudes.getMat()[0][i % this->amplitudes.getMat()[0].size()] = 0 + 1 * (i == index);
	return;
}

void Qbit::reset() {
	this->amplitudes = Matrix(1, (unsigned)pow(2, this->QbitNumber), sqrt(1.0 / pow(2, this->QbitNumber)));
	return;
}

Matrix Qbit::getAmplitudes() {
	return this->amplitudes;
}

unsigned Qbit::getQbitNumber() {
	return this->QbitNumber;
}

std::vector<double> Qbit::getProbabilities() {
	std::vector<double> res;
	std::vector<std::complex<double>> ampl = (this->amplitudes).getMat()[0];
	for (std::complex<double> i : ampl)
		res.push_back(norm(i));
	return res;
}

Matrix Qbit::identity() {
	return Matrix::identity(2);
}

bool Qbit::identity(Qbit& q) {
	Matrix I = Matrix::identity((unsigned)pow(2, q.QbitNumber));
	q *= I;
	return true;
}

Matrix Qbit::pauliX() {
	Matrix X(2, 2);
	X(0, 1) = X(1, 0) = 1;
	return X;
}

bool Qbit::pauliX(Qbit& q) {
	if (q.QbitNumber != 1)
		return false;
	Matrix X = pauliX();
	q *= X;
	return true;
}

Matrix Qbit::pauliY() {
	Matrix Y(2, 2);
	Y(0, 1) = std::complex<double>(0, -1);
	Y(1, 0) = std::complex<double>(0, 1);
	return Y;
}

bool Qbit::pauliY(Qbit& q) {
	if (q.QbitNumber != 1)
		return false;
	Matrix Y = pauliY();
	q *= Y;
	return true;
}

Matrix Qbit::pauliZ() {
	Matrix Z(2, 2);
	Z(0, 0) = 1;
	Z(1, 1) = -1;
	return Z;
}

bool Qbit::pauliZ(Qbit& q) {
	if (q.QbitNumber != 1)
		return false;
	Matrix Z = pauliZ();
	q *= Z;
	return true;
}

Matrix Qbit::hadamard() {
	Matrix H(2, 2, 1);
	H(1, 1) = -1;
	H *= std::complex<double>(1 / sqrt(2), 0);
	return H;
}

bool Qbit::hadamard(Qbit& q) {
	if (q.QbitNumber != 1)
		return false;
	Matrix H = hadamard();
	q *= H;
	return true;
}

Matrix Qbit::phaseShift(double angle) {
	Matrix P(2, 2);
	P(0, 0) = 1;
	P(1, 1) = exp(std::complex<double>(0, 1) * angle);
	return P;
}

bool Qbit::phaseShift(Qbit& q, double angle) {
	if (q.QbitNumber != 1)
		return false;
	Matrix P = phaseShift(angle);
	q *= P;
	return true;
}

Matrix Qbit::swap() {
	Matrix S(4, 4);
	S(0, 0) = S(1, 2) = S(2, 1) = S(3, 3) = 1;
	return S;
}

bool Qbit::swap(Qbit& q) {
	if (q.QbitNumber != 2)
		return false;
	Matrix S = swap();
	q *= S;
	return true;
}

Matrix Qbit::rotX(double angle) {
	Matrix R(2, 2);
	R(0, 0) = R(1, 1) = cos(angle / 2);
	R(0, 1) = R(1, 0) = std::complex<double>(0, -1) * sin(angle / 2);
	return R;
}

bool Qbit::rotX(Qbit& q, double angle) {
	if (q.QbitNumber != 1)
		return false;
	Matrix R = rotX(angle);
	q *= R;
	return true;
}

Matrix Qbit::rotY(double angle) {
	Matrix R(2, 2);
	R(0, 0) = R(1, 1) = cos(angle / 2);
	R(0, 1) = -sin(angle / 2);
	R(1, 0) = sin(angle / 2);
	return R;
}

bool Qbit::rotY(Qbit& q, double angle) {
	if (q.QbitNumber != 1)
		return false;
	Matrix R = rotY(angle);
	q *= R;
	return true;
}

Matrix Qbit::rotZ(double angle) {
	Matrix R(2, 2);
	R(0, 0) = exp(std::complex<double>(0, -1) * (angle / 2));
	R(1, 1) = exp(std::complex<double>(0, 1) * (angle / 2));
	return R;
}

bool Qbit::rotZ(Qbit& q, double angle) {
	if (q.QbitNumber != 1)
		return false;
	Matrix R = rotZ(angle);
	q *= R;
	return true;
}

Matrix Qbit::controlled(unsigned Qbits, std::string gate, std::string angle) {
	if (gate == "identity")
		return Matrix((unsigned)pow(2, Qbits), (unsigned)pow(2, Qbits), 1);
	Matrix C((unsigned)pow(2, Qbits), (unsigned)pow(2, Qbits));
	for (unsigned i = 0; i < (unsigned)pow(2, Qbits) - 2; i++)
		C(i, i) = 1;
	Matrix lastQbit;
	if (gate == "pauliX")
		lastQbit = Qbit::pauliX();
	else if (gate == "pauliY")
		lastQbit = Qbit::pauliY();
	else if (gate == "pauliZ")
		lastQbit = Qbit::pauliZ();
	else if (gate == "hadamard")
		lastQbit = Qbit::hadamard();
	else if (gate == "swap")
		lastQbit = Qbit::swap();
	else if (gate == "phaseShift")
		lastQbit = Qbit::phaseShift(std::stod(angle));
	else if (gate == "rotX")
		lastQbit = Qbit::rotX(std::stod(angle));
	else if (gate == "rotY")
		lastQbit = Qbit::rotY(std::stod(angle));
	else if (gate == "rotZ")
		lastQbit = Qbit::rotZ(std::stod(angle));
	else
		return Matrix::identity((unsigned)pow(2, Qbits));

	for (unsigned i = (unsigned)pow(2, Qbits) - 1; i > (unsigned)pow(2, Qbits) - 3 - 2 * (gate == "swap"); i--)
		for (unsigned j = (unsigned)pow(2, Qbits) - 1; j > (unsigned)pow(2, Qbits) - 3 - 2 * (gate == "swap"); j--)
			C(i, j) = lastQbit(i % lastQbit.getRows(), j % lastQbit.getCols());
	return C;
}

bool Qbit::controlled(Qbit& q, unsigned Qbits, std::string gate, std::string angle) {
	if (q.QbitNumber != Qbits)
		return false;
	Matrix C = controlled(Qbits, gate, angle);
	q *= C;
	return true;
}

void Qbit::measure() {
	std::vector<double> prob = this->getProbabilities();
	std::discrete_distribution<int> distribution(prob.begin(), prob.end());
	std::cout << "|" << std::bitset<MAX_QbitS>(distribution(this->generator))
			.to_string()
			.substr(MAX_QbitS - this->QbitNumber) << ">\n";
	return;
}

void Qbit::printProbabilities() {
	std::vector<double> prob = this->getProbabilities();
	for (double i : prob)
		std::cout << i << ' ';
	std::cout << "\n";
	return;
}

std::ostream& operator << (std::ostream& out, Qbit& q) {
	if (q.QbitNumber != 0) {
		for (size_t i = 0; i < q.amplitudes.getMat()[0].size(); i++) {
			if (q.amplitudes.getMat()[0][i].imag() == 0 && q.amplitudes.getMat()[0][i].real() == 0)
				continue;
			if (i)
				printf(" + ");
			out << q.amplitudes.getMat()[0][i];
			out << "|" << std::bitset<MAX_QbitS>(i)
					.to_string()
					.substr(MAX_QbitS - q.QbitNumber) << ">";
		}
		out << "\n";
	}
	else
		out << "Empty Qbit register\n";
	out << "\n";
	return out;
}