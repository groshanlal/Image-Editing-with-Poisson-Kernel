#include "PoissonSolver.hpp"
#include <Eigen/Sparse>
#include <vector>
#include <iostream>

PoissonSolver::PoissonSolver(std::set<Point> domain, Size sizeImage, cv::Mat domainMask) {
    Eigen::SparseMatrix<double, Eigen::ColMajor> A;
    this->domain = domain;
    this->sizeImage = sizeImage;
    this->domainMask = domainMask;
    std::cout << "Number of pixels in domain: " << domain.size() << std::endl;
}

unsigned int PoissonSolver::numNeighbors(Point p) {
    unsigned int numNeighbors = 4;
    if (p.x == sizeImage.width - 1 || p.x == 0) {
        numNeighbors--;
    }
    if (p.y == sizeImage.height - 1 || p.y == 0) {
        numNeighbors--;
    }
    return numNeighbors;
}

std::set<Point> PoissonSolver::neighbors(Point p) {
    std::set<Point> neighbors;
    if (p.x != 0) {
        neighbors.insert(Point(p.x - 1, p.y));
    }
    if (p.y != 0) {
        neighbors.insert(Point(p.x, p.y - 1));
    }
    if (p.x != sizeImage.width - 1) {
        neighbors.insert(Point(p.x + 1, p.y));
    }
    if (p.y != sizeImage.height - 1) {
        neighbors.insert(Point(p.x, p.y + 1));
    }
    return neighbors;
}

void PoissonSolver::compute() {
    unsigned int n = (unsigned int) domain.size();
    Eigen::SparseMatrix<double, Eigen::ColMajor> A(n, n);
    A.reserve(4 * n);
    std::vector<Eigen::Triplet<double>> coefficients;
    coefficients.reserve(4 * n);
    {
        int i = 0;
        int index;
        for (std::set<Point>::iterator p = domain.begin(); p != domain.end(); p++, i++) {
            coefficients.push_back(Eigen::Triplet<double>(i, i, numNeighbors(*p)));
            std::set<Point> pNeighbors = neighbors(*p);
            for (std::set<Point>::iterator q = pNeighbors.begin(); q != pNeighbors.end(); q++) {
                index = domainMask.at<int>(q->y, q->x);
                if (index != -1) {
                    coefficients.push_back(Eigen::Triplet<double>(i, index, -1));
                }
            }
        }
    }
    A.setFromTriplets(coefficients.begin(), coefficients.end());
    solver.compute(A);
}

Eigen::MatrixXd PoissonSolver::solve(std::function<double(Point)> dirichlet, std::function<double(Point, Point)> guidance) {
    unsigned int n = (unsigned int) domain.size();
    Eigen::MatrixXd b(n, 1), x(n, 1);
    b.setZero();
    x.setZero();
    {
        int i = 0;
        int index;
        for (std::set<Point>::iterator p = domain.begin(); p != domain.end(); p++, i++) {
            std::set<Point> pNeighbors = neighbors(*p);
            for (std::set<Point>::iterator q = pNeighbors.begin(); q != pNeighbors.end(); q++) {
                index = domainMask.at<int>(q->y, q->x);
                if (index == -1) {
                    b(i, 0) += dirichlet(*q);
                }
            }
            for (std::set<Point>::iterator q = pNeighbors.begin(); q != pNeighbors.end(); q++) {
                b(i, 0) += guidance(*p, *q);
            }
        }
    }
    x = solver.solve(b);
    return x;
}
