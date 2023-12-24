# bigint Class

The `bigint` class is designed to represent large integers using a vector of `uint64_t` numbers. 

## Overview

- The class uses a `vector<uint64_t>` named `number` to store the big integer. Each element in the vector represents a decimal number with a maximum of 18 digits. Higher digits are stored further back in the vector.
- The `positive` flag is used to determine the sign of the integer (positive or negative).
- The class supports basic operations : "+","-","*","+=","-=","*=","=","==","!=",">",">=","<","<=","<<"
- Constructors allow the creation of a `bigint` object from a 64-bit integer, a string, or another `bigint` object.

## Constructors

1. `bigint()`: Default constructor creates a `bigint` object with a value of 0.
2. `bigint(const int64_t &)`: Constructs a `bigint` object from a 64-bit integer.
3. `bigint(string)`: Constructs a `bigint` object from a string representation. The string can start with '+' or '-'.
4. `bigint(vector<uint64_t> vec, bool pos)`: More for convenience of implementation. Constructs a `bigint` object from a vector of `uint64_t` and a sign flag.
5. `bigint(const bigint &other)`: This is a copy constructor whose purpose is to eliminate implicitly-declared warning.

## Member Functions

1. `getsign() const`: Returns the sign of the `bigint` (true for positive, false for negative).
2. `getnumber() const`: Returns the vector of `uint64_t` representing the big integer.

## Operator Overloads

1. `operator>(const bigint &, const bigint &)`: Checks if the first `bigint` is greater than the second.
2. `operator>=(const bigint &, const bigint &)`: Checks if the first `bigint` is greater than or equal to the second.
3. `operator<(const bigint &, const bigint &)`: Checks if the first `bigint` is less than the second.
4. `operator<=(const bigint &, const bigint &)`: Checks if the first `bigint` is less than or equal to the second.
5. `operator==(const bigint &, const bigint &)`: Checks if two `bigint` objects are equal.
6. `operator!=(const bigint &, const bigint &)`: Checks if two `bigint` objects are not equal.
7. `operator-(const bigint &)`: Unary negation. Returns the negation of a `bigint`.
8. `operator+(bigint, const bigint &)`: Addition. Returns a new `bigint` representing the sum.
9. `operator-(bigint, const bigint &)`: Subtraction. Returns a new `bigint` representing the difference.
10. `operator*(bigint, const bigint &)`: Multiplication. Returns a new `bigint` representing the product.
11. `ostream &operator<<(ostream &, const bigint &)`: Output stream operator. Prints a `bigint` to the output stream.

## Operation Details

- Addition and subtraction are implemented by adding or subtracting corresponding digits with consideration for carry or borrow.
- Multiplication is implemented by splitting the multiplier into digits and invoking a single-digit multiplication operation.

## Important Notes

- Redundant zeros in the representation are automatically removed.
- The class ensures that the digits of the big integer are within the 18-digit limit to prevent overflow. The reasons behind it can be found in the comment inside the code.
- The design of the class allows for convenient implementation of arithmetic operations on large integers.

## Example Usage

- You can find a more detailed demo on demo.cpp.
- Here is a brief example.
```cpp
#include "bigint.h" 
using namespace std;
int main() {
    // Example usage of bigint class
    bigint num1("1234567890123456789");
    bigint num2(9876543210);
    
    // Addition
    bigint sum = num1 + num2;
    cout << "Sum: " << sum << endl;

    // Subtraction
    bigint diff = num1 - num2;
    cout << "Difference: " << diff << endl;

    // Multiplication
    bigint product = num1 * num2;
    cout << "Product: " << product << endl;

    return 0;
}
