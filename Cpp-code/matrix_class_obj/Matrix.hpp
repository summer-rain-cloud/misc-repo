//
//  Matrix.hpp
//
//  Created by Yu Xia on 2/7/23.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <iostream>
#include <stdio.h>

class Matrix{
public:
    Matrix(); // default set to zero|zero
    Matrix(const Matrix& other); // copy constructor
    Matrix(unsigned int rows, unsigned int columns, double fill); // set to row|column and all value is fill
    
    // data manipulation functions
        // .h defined
    unsigned int num_rows() const{return rows;}
    unsigned int num_cols() const{return columns;}
        // .cpp defined
    double* get_row(unsigned int x_row) const;
    double* get_column(unsigned int y_col) const;
    bool get(int row, int column, double& stow) const; // get function that stores the value in stow
    bool set(int row, int column, double input); // set function that sets value of input in row|column
    void multiply_by_coefficient(double coeff); // multiply matrix by the parameter
    bool swap_row(unsigned int source, unsigned int target); // swaps the rows of source and target
    void transpose(); // flips the matrix in object
    bool add(const Matrix& other); // adds the right to the left matrix
    bool subtract(const Matrix& other); // subtracts the right from the left matrix
    Matrix* quarter(); // returns four matrix objects that contains an array of a quadrant
    void resize(unsigned int row, unsigned int columns, double fill); // resize the array
    
    // operator overloads
    Matrix& operator=(const Matrix& other); // copy?
    bool operator==(Matrix& other); // comparator
    bool operator!=(Matrix& other); // comparator
    
    // de-constructor
    
    ~Matrix(){
        //std::cout << "cleared " << rows << columns << sizeof(*array2D) << "\n";
        if(array2D != NULL){
            unsigned int size_ = sizeof(array2D) / sizeof(array2D[0]);
            for(unsigned int i = 0; i < size_; i++){
                delete [] array2D[i];
                //std::cout << (array2D[i] == NULL) << "\n";
            }
            delete [] array2D;
        }
        else{delete [] array2D;}
        //std::cout << (array2D == NULL) << "\n";
    }
    
private:
    void clear(); // set rows and columns to zero and de-allocate memory
    double** array2D;
    unsigned int rows, columns;
    double placeholder;
};

// non-member functions
std::ostream& operator<< (std::ostream& output, const Matrix& reference);

#endif /* Matrix_hpp */
