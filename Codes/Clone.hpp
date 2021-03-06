#ifndef Clone_hpp
#define Clone_hpp

#include "Point.hpp"
#include <Eigen/Dense>
#include <set>
#include <opencv2/core/core.hpp>

cv::Mat flatten(cv::Mat, std::set<Point>, cv::Mat);
cv::Mat cloneImportedGrad(cv::Mat, cv::Mat, std::set<Point>, cv::Mat, Point);
cv::Mat cloneMixedGrad(cv::Mat, cv::Mat, std::set<Point>, cv::Mat, Point);

#endif /* Clone_hpp */
