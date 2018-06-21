
#include <iostream>
#include <Eigen/Dense>

void useEigen()
{
    Eigen::MatrixXd m(2, 2);  
    Eigen::Vector3d vec3d;  
    Eigen::Vector4d vec4d(1.0, 2.0, 3.0, 4.0);  
   
    Eigen::MatrixXd matrixXd;  
    Eigen::Matrix3d matrix3d;  
    
    m(0, 0) = 1;  
    m(0, 1) = 2;  
    m(1, 0) = m(0, 0) + 3;   
    m(1, 1) = m(0, 0) * m(0, 1);  
    std::cout << m << std::endl << std::endl;  
  
    m << -1.5, 2.4,  
        6.7, 2.0;  
    std::cout << m << std::endl << std::endl; 
}