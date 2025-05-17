//
//  Matrix.cpp
//
//  Created by Yu Xia on 2/7/23.
//

#include <iomanip>
#include <string>
#include "Matrix.h"

//constructors
Matrix::Matrix(){
    rows = columns = 0;
    placeholder = 0;
    array2D = NULL;
    //std::cout << "made " << rows << columns << sizeof(*array2D) <<"\n";
}
Matrix::Matrix(const Matrix& other){
    rows = other.num_rows();
    columns = other.num_cols();
    if(rows == 0 || columns ==0){array2D = NULL;}
    else{
        array2D = new double*[rows];
        for(unsigned int i = 0; i < rows; i++){
            array2D[i] = new double[columns];
            for(unsigned int j = 0; j < columns; j++){
                array2D[i][j] = other.array2D[i][j];
            }
        }
    }
    //std::cout << "made " << rows << columns << sizeof(*array2D) << "\n";
}
Matrix::Matrix(unsigned int x_row, unsigned int y_col, double fill){
    rows = x_row; columns = y_col; placeholder = fill;
    if(rows == 0 || columns == 0){array2D = NULL;}
    else{
        array2D = new double*[rows];
        for(unsigned int i = 0; i < rows; i++){
            array2D[i] = new double[columns];
            for(unsigned int j = 0; j < columns; j++){
                array2D[i][j] = placeholder;
            }
        }
    }
    //std::cout << "made " << rows << columns << sizeof(*array2D) << "\n";
}
//member functions
double* Matrix::get_row(unsigned int x_row) const{ // returns pointer to the row specified
    if(x_row > this->num_rows() || x_row < 0){
        return NULL;
    }
    return array2D[x_row];}
double* Matrix::get_column(unsigned int y_col) const{ // returns a new array of column values specified
    if(y_col > this->num_cols() || y_col < 0){
        return NULL;
    }
    double* temp = new double[this->num_cols()];
    for(unsigned int i = 0; i < this->num_rows(); i++){
        temp[i] = this->array2D[i][y_col];
    }
    return temp;
}
bool Matrix::get(int x_row, int y_col, double& stow) const{ // single get at the row col specified
    if(!(x_row > -1 && x_row < int(this->num_rows()))){return false;}
    if(!(y_col > -1 && y_col < int(this->num_cols()))){return false;}
    stow = this->array2D[x_row][y_col];
    return true;
}
bool Matrix::set(int x_row, int y_col, double input){ // single set at the row col specified
    if(!(x_row > -1 && x_row < int(this->num_rows()))){return false;}
    if(!(y_col > -1 && y_col < int(this->num_cols()))){return false;}
    this->array2D[x_row][y_col] = input;
    return true;
}
void Matrix::multiply_by_coefficient(double coeff){ // multiplication by a constant
    for(unsigned int i = 0; i < this->rows; i++){
        for(unsigned int j = 0; j < this->columns; j++){
            this->array2D[i][j] *= coeff;
        }
    }
}
bool Matrix::swap_row(unsigned int source, unsigned int target){ // swaps rows
    if(source > (this->rows-1) || target > (this->rows-1)){return false;}
    double t_stow = 0;
    for(unsigned int i = 0; i < this->columns; i++){
        t_stow = this->array2D[source][i];
        this->array2D[source][i] = this->array2D[target][i];
        this->array2D[target][i] = t_stow;
    }
    return true;
}
void Matrix::transpose(){ // swaps the array dimensions and also their values
    unsigned int new_col = this->rows;
    unsigned int new_row = this->columns;
    double** t_stow = new double*[new_row];
    for(unsigned int i = 0; i < new_row; i++){
        t_stow[i] = new double[new_col];
    }
    for(unsigned int j = 0; j < new_row; j++){
        for(unsigned int k = 0; k < new_col; k++){
            t_stow[j][k] = this->array2D[k][j];
        }
    }
    this->clear();
    this->rows = new_row; this->columns = new_col;
    this->array2D = t_stow;
}
bool Matrix::add(const Matrix& other){ // adds the values of two matrices of same dimension
    if(this->rows != other.num_rows() || this->columns != other.num_cols()){return false;}
    for(unsigned int i = 0; i < this->rows; i++){
        for(unsigned int j = 0; j < this->columns; j++){
            this->array2D[i][j] += other.array2D[i][j];
        }
    }
    return true;
}
bool Matrix::subtract(const Matrix& other){ // substracts the values of two matrices of same dimensions
    if(this->rows != other.num_rows() || this->columns != other.num_cols()){return false;}
    for(unsigned int i = 0; i < this->rows; i++){
        for(unsigned int j = 0; j < this->columns; j++){
            this->array2D[i][j] -= other.array2D[i][j];
        }
    }
    return true;
}
Matrix* Matrix::quarter(){ // divides a matrix into four quarters and returns them in an array as matrix obj
    Matrix* t_stow = new Matrix[4];
    unsigned int t_row, t_col;
    int x, y;
    placeholder = 0;
    if(this->rows % 2 > 0){t_row = int(this->rows)/2 + 1; x = 1;}
    else{t_row = this->rows/2; x = 0;}
    if(this->columns % 2 > 0){t_col = int(this->columns)/2 + 1; y = 1;}
    else{t_col = this->columns/2; y = 0;}
    for(unsigned int i = 0; i < 4; i++){
        t_stow[i].clear();
        t_stow[i].rows = t_row; t_stow[i].columns = t_col;
        t_stow[i].array2D = new double*[rows];
        for(unsigned int j = 0; j < t_stow[i].rows; j++){
            t_stow[i].array2D[j] = new double[columns];
            for(unsigned int k = 0; k < t_stow[i].columns; k++){
                t_stow[i].array2D[j][k] = placeholder;
            }
        }
    }
    for(unsigned int l = 0; l < t_row; l++){
        for(unsigned int m = 0; m < t_col; m++){
            t_stow[0].array2D[l][m] = this->array2D[l][m];
        }
    }
    for(unsigned int n = 0; n < t_row; n++){
        for(unsigned int o = 0; o < t_col; o++){
            t_stow[1].array2D[n][o] = this->array2D[n][o+t_col-y];
        }
    }
    for(unsigned int p = 0; p < t_row; p++){
        for(unsigned q = 0; q < t_col; q++){
            t_stow[2].array2D[p][q] = this->array2D[p+t_row-x][q];
        }
    }
    for(unsigned int r = 0; r < t_row; r++){
        for(unsigned int s = 0; s < t_col; s++){
            t_stow[3].array2D[r][s] = this->array2D[r+t_row-x][s+t_col-y];
        }
    }
    return t_stow;
}
void Matrix::resize(unsigned int row, unsigned int column, double fill){ // xtra credit
    unsigned int t_row = row;
    unsigned int t_col = column;
    double** t_stow = new double*[t_row];
    for(unsigned int i = 0; i < t_row; i++){
        t_stow[i] = new double[t_col];
        for(unsigned int j = 0; j < t_col; j++){
            t_stow[i][j] = fill;
        }
    }
    for(unsigned int k = 0; k < this->rows && k < t_row; k++){
        for(unsigned int l = 0; l < this->columns && l < t_col; l++){
            t_stow[k][l] = this->array2D[k][l];
        }
    }
    this->clear();
    this->rows = t_row;
    this->columns = t_col;
    this->array2D = t_stow;
}

Matrix& Matrix::operator=(const Matrix& other){ // copy constructor but overloaded for =
    this->clear();
    this->rows = other.num_rows();
    this->columns = other.num_cols();
    this->array2D = new double*[this->rows];
    for(unsigned int i = 0; i < this->rows; i++){
        this->array2D[i] = new double[this->columns];
        for(unsigned int j = 0; j < this->columns; j++){
            this->array2D[i][j] = other.array2D[i][j];
        }
    }
    return *this; 
}
bool Matrix::operator!=(Matrix& other){ // comparison overload
    if(this->rows != other.num_rows() || this->columns != other.num_cols()){return true;}
    for(unsigned int i = 0; i < this->rows; i++){
        for(unsigned int j = 0; j < this->columns; j++){
            if(this->array2D[i][j] != other.array2D[i][j]){
                return true;
            }
        }
    }
    return false;
}
bool Matrix::operator==(Matrix& other){ // comparison overload
    if(this->rows != other.num_rows() || this->columns != other.num_cols()){return false;}
    for(unsigned int i = 0; i < this->rows; i++){
        for(unsigned int j = 0; j < this->columns; j++){
            if(this->array2D[i][j] != other.array2D[i][j]){
                return false;
            }
        }
    }
    return true;
}
void Matrix::clear(){ // definition of an explicit deletion, is kept in private to prevent unwanted use
    if(array2D != NULL){
        unsigned int size_ = sizeof(array2D) / sizeof(array2D[0]);
        for(unsigned int i = 0; i < size_; i++){delete [] array2D[i];}
        delete [] array2D;
    }
    else{delete [] array2D;}
    this->rows = 0; this->columns = 0;
}
//non-member functions
std::ostream& operator<< (std::ostream& output, const Matrix& reference){ // print overload
    output << reference.num_rows() << " x " << reference.num_cols() << " matrix:\n";
    for(unsigned int i = 0; i < reference.num_rows(); i++){
        double* temp_row = reference.get_row(i);
        if(i == 0){output << "[";}
        else{output << " ";}
        for(unsigned int j = 0; j < reference.num_cols(); j++){
            output << std::setw(5) << std::right << std::setprecision(3) << temp_row[j];
            if(i == reference.num_rows()-1 && j == reference.num_cols()-1){output << "]";}
        }
        output << "\n";
    }
    if(reference.num_rows() == 0 || reference.num_cols() == 0){
        output << "[ ]\n";
    }
    return output;
}

