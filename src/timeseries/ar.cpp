#include "timeseries/ar.hpp"
#include <chrono>

AR::AR(const std::vector<double> data) {
    this->data = data;
    this->count = data.size();

    // Calculate mean
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    double mean = sum / this->count;
    this->mean = mean;

    // Calculate variance
    double sq_sum = std::accumulate(data.begin(), data.end(), 0.0,
        [mean](double acc, double val) {
            return acc + (val - mean) * (val - mean);
        });
    this->variance = sq_sum / this->count;

    // Calculate autocorrelations
    this->autocorrelations.reserve(this->count);
    for (int lag = 0; lag < this->count; ++lag) {
        double diffSum = 0.0;
        for (int i = 0; i < this->count - lag; ++i) {
            diffSum += (data[i] - this->mean) * (data[i + lag] - this->mean);
        }

        double autocorrelation = diffSum / sq_sum;
        this->autocorrelations.push_back(autocorrelation);
    }
}

AR::~AR() {}

Eigen::VectorXd AR::solveYuleWalkerEquations(int k) {
    // See: http://www-stat.wharton.upenn.edu/~steele/Courses/956/Resource/YWSourceFiles/YW-Eshel.pdf
    
    // Construct kxk autocorrelation matrix and autocorrelation vector
    // See: https://stats.stackexchange.com/questions/129052/acf-and-pacf-formula/129374
    Eigen::MatrixXd autocorrelationM(k, k);
    Eigen::VectorXd autocorrelationV(k);
    for (int i = 0; i < k; ++i) {
        autocorrelationV(i) = this->autocorrelations[i+1];
        for (int j = 0; j < k; ++j) {
            autocorrelationM(i, j) = this->autocorrelations[std::abs(i - j)];
        }
    }

    std::cout << "AC Matrix (" << autocorrelationM.rows() << ", " << autocorrelationM.cols() << "):\n";
    std::cout << autocorrelationM << std::endl;

    std::cout << "AC Vector (" << autocorrelationV.rows() << ", " << autocorrelationV.cols() << "):\n";
    std::cout << autocorrelationV << std::endl; 

    // Solve, LU decomp is most stable apparently?
    Eigen::VectorXd phis = autocorrelationM.fullPivLu().solve(autocorrelationV);

    std::cout << "Phis (" << phis.rows() << ", " << phis.cols() << "):\n";
    std::cout << phis << std::endl;

    return phis;
}

void AR::train(int k) {
}

std::vector<double> AR::forecast(int steps) {
    return {};
}

std::vector<double> AR::getPhis() const {
    return this->phis;
}

std::vector<double> AR::getAutocorrelations() const {
    return this->autocorrelations;
}

int AR::plot() const {
    return 0;
}

std::string AR::toString() const {
    return "";
}