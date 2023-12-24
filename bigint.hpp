#include <iostream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
const uint64_t maxdec = 999999999999999999ULL; // The max 18 digits number

/**
 * @brief Here I use vector<uint64_t> number to store the big integer. Every element in vector(number) is a (at most)
 * 18 digits decimal number.Higher digit will be stored in the further back in the vector.
 * @example 111,222,333,444,555,666,777,888,999,000 will be stored in vector<uint64_t> number like this:
 *          number[0]=555,666,777,888,999,000
 *          number[1]=111,222,333,444
 * @details The reason of 18 digits. 1.UINT64_MAX is a 20 digits number. So uint64_t can only guarantee represent all 19
 * digits number. 2.Considering the implement of multiplication. A 19 digits number times a 1 digit number may cause
 * overflow. So 18 digits number make sure there will be no overflow.
 *          The reason of high digit in back vector: this will be convenient for operation with carry or borrow.
 */
class bigint
{
public:
    bigint();
    bigint(const int64_t &);
    /**
     * @brief Construct a new bigint object with string
     *
     * @attention Except the first char of the string can be '+' or '-',all other chars in the string must be number.
     * @example "+123","-123","123" are all valid. But "*123","12*3","123*" are all invalid.
     *
     * @attention Redundant 0s will be automatically deleted
     * @example "00001" will be stored as 1;
     */
    bigint(string);
    bigint(vector<uint64_t> vec, bool pos); // This constructor is more for convenience of implement of operation overloading
    bigint(const bigint &other);            // Eliminate implicitly-declared warning
    bool getsign() const;
    vector<uint64_t> getnumber() const;
    /**
     * @brief Add the 2 big integer from low digit. i.e. number1[0]+number2[0], then number1[1]+number2[1] and so on.
     * @attention   1.Every time we add number1[i]+number2[i], we need to consider carry.
     *              2.We only need to implement non-negative number plus non-negative number, all another scenarios can
     *                be converted to non-negative number plus non-negative number or non-negative number minus
     *                non-negative number.
     * @return bigint&
     */
    bigint &operator+=(const bigint &);
    bigint &operator-=(const bigint &); // Same idea as +=. Details changed as explained inside this overloading;
    /**
     * @brief This function is implemented by the following steps:
     *        1. Split every digit of multiplier to a vector<uint32_t> digits. For example, multiplier=123,456
     *           The vector digits should be {1,2,3,4,5,6}
     *        2. Invoke the timessingal below
     *        3. Add the result of every time
     *        4. Process the sign
     */
    bigint &operator*=(const bigint &);
    bigint &operator=(const bigint &);

private:
    vector<uint64_t> number;
    bool positive = true;
    /**
     * @brief This function is more for convenience of implement of operation *=. So user can't get access to this function
     * @param coefficient this parameter should be a 1-digit non-positive number, i.e. 0-9
     * @param power
     * @details After calling this function, the big integer a will become a*coefficient*10^power
     */
    void timessingal(const uint32_t &, uint64_t);
};
bool operator>(const bigint &big1, const bigint &big2);
bool operator>=(const bigint &big1, const bigint &big2);
bool operator<(const bigint &big1, const bigint &big2);
bool operator<=(const bigint &big1, const bigint &big2);
bool operator==(const bigint &big1, const bigint &big2);
bool operator!=(const bigint &big1, const bigint &big2);
bigint operator-(const bigint &big1);
bigint operator+(bigint big1, const bigint &big2);
bigint operator-(bigint big1, const bigint &big2);
bigint operator*(bigint big1, const bigint &big2);
ostream &operator<<(ostream &out, const bigint &big);

bigint::bigint()
{
    number.push_back(0);
}

bigint::bigint(const int64_t &a)
{
    uint64_t temp = static_cast<uint64_t>(a);
    number.push_back(temp);
}

bigint::bigint(string s)
{
    if (s.length() == 0)
        throw invalid_argument("Empty input is not acceptable!");
    if (s[0] == '+')   // If the first char in the string is '+' or '-'
        s.erase(0, 1); // we record the sign and then delete the char for further detection
    else if (s[0] == '-')
    {
        positive = false;
        s.erase(0, 1);
    }
    for (const char &c : s) // We have deleted the sign that maybe exist. So all char should be digit
        if (!isdigit(c))
            throw invalid_argument("This is not a valid number!");

    for (uint64_t i = s.length() - 1; i < s.length(); i -= 18) // split string to substrings of 18bits.
    {                                                          // The reasons can be found above
        uint64_t temp;                                         // Because i and s.length are all unsigned, loop
        if (i < 18)                                            // termination condition is i < s.length(). i.e. i<0
        {
            temp = stoull(s.substr(0, i + 1));
            number.push_back(temp);
            break;
        }
        else
            temp = stoull(s.substr(i - 17, 18));
        number.push_back(temp);
    }
    while (number.size() > 1) // Delete the redundant 0s.
    {
        if (number.back() == 0)
            number.pop_back();
        else
            break;
    }
}

bigint::bigint(vector<uint64_t> vec, bool pos) : number(vec), positive(pos){};

bigint::bigint(const bigint &other) : number(other.getnumber()), positive(other.getsign()){};

bool bigint::getsign() const
{
    return positive;
}

vector<uint64_t> bigint::getnumber() const
{
    return number;
}

bigint &bigint::operator+=(const bigint &addbig)
{
    vector<uint64_t> maxnumber = addbig.getnumber(), minnumber = number, resnumber;
    uint64_t maxlength = maxnumber.size(), minlength = minnumber.size();
    bool add_sign = addbig.getsign(), carry = false;
    if (maxnumber.size() == 0)
        maxnumber.push_back(0);
    if (minnumber.size() == 0)
        minnumber.push_back(0); // In case we have empty vector
    if (positive && !add_sign)  // positive + negative, convert to positive - positive
    {
        bigint oppoadd = -addbig;
        *this -= oppoadd;
        return *this;
    }
    if (!positive && add_sign) // negative + positive, convert to positive - positive
    {
        bigint oppothis(0);
        oppothis = -(*this);
        *this = addbig - oppothis;
        return *this;
    }
    // negative + negative will automaticaly convert to -(positive + positive). Because the positive has already been false
    // All other scenarios have been proceed. The left scenario is positive + positive
    if (*this > addbig && add_sign) // Distinguish the bigger integer and smaller one. Store the bigger one in maxnumber;
    {
        swap(maxnumber, minnumber);
        swap(maxlength, minlength);
    }
    if (*this < addbig && !add_sign) // maxnumber and minnumber are absolute value
    {
        swap(maxnumber, minnumber);
        swap(maxlength, minlength);
    }
    for (size_t i = 0; i < minlength; i++)
    {
        uint64_t res = minnumber[i] + maxnumber[i];
        if (carry)
            res += 1;
        if (res > maxdec) // Need carry. Note the result of 2 18-digits number addition is at max 19-digits, which
        {                 // never cause overflow(of int64_t data)
            carry = true;
            res -= (maxdec + 1); // Remove the 19th digit.
        }
        else
            carry = false;
        resnumber.push_back(res);
    }
    for (uint64_t i = minlength; i < maxlength; i++) // low digit addition has finished.
    {                                                // Now consider if the carry bit cause further more carry in high digit
        uint64_t res = maxnumber[i];
        if (carry)
        {
            res += 1;
            if (res > maxdec)
                carry = true;
            else
                carry = false;
        }
        resnumber.push_back(res);
    }
    if (carry)                  // If there is still a carry bit, we need to push 1 more bit to our vector
    {                           // For example, vector number is of size 1, number[0]=000,000,000,000,000,000. Carry is still true at this time
        resnumber.push_back(1); // We push back 1 to number. Now number[1]=1, number[0] doesn't change
    }
    while (resnumber.back() == 0)
        resnumber.pop_back(); // Pop out the extra 0
    number = resnumber;
    return *this;
}

bigint &bigint::operator-=(const bigint &minbig)
{
    vector<uint64_t> maxnumber = minbig.getnumber(), minnumber = number, resnumber;
    uint64_t maxlength = maxnumber.size(), minlength = minnumber.size();
    bool sub_sign = minbig.getsign(), borrow = false;
    if (positive && !sub_sign) // positive - negative, convert to positive + negative
    {
        bigint oppbig = -minbig;
        *this += oppbig;
        return *this;
    }
    if (!positive && sub_sign) // negative - positive, convert to -(positive + positive)
    {
        bigint oppbig = -(*this);
        *this += minbig;
        positive = false;
        return *this;
    }
    if (!positive && !sub_sign) // negative - negative, convert to positive - positive
    {
        bigint new_minuend = -minbig, new_subtrahend = -(*this);
        new_minuend -= new_subtrahend;
        (*this) = new_minuend;
        return *this;
    }
    // All other scenarios have been proceed. The left scenario is positive - positive
    if (*this > minbig) // alway use the bigger integer minus smaller integer
    {
        swap(maxnumber, minnumber);
        swap(maxlength, minlength);
    }
    else
        positive = false; // Still use the bigger number minus smaller number, just converse the sign
    for (uint64_t i = 0; i < minlength; i++)
    {
        uint64_t res;
        if (maxnumber[i] < minnumber[i] + borrow) // need to borrow
        {
            res = minnumber[i] + borrow - maxnumber[i]; // To simplify, consider 2 digits integer as an example: 12-8
            res = maxdec + 1 - res;                     // Here 2<8+0, so borrow is required. We first do 8-2=6,
            borrow = true;                              // then use 10-6=4.
        }
        else
        {
            res = maxnumber[i] - minnumber[i] - borrow;
            borrow = false;
        }
        resnumber.push_back(res);
    }
    for (uint64_t i = minlength; i < maxlength; i++) // Consider the left high digits that didn't involved in minus
    {
        uint64_t res = maxnumber[i];
        if (borrow)
        {
            if (res < 1)
            {
                res = maxdec;
                borrow = true;
            }
            else
            {
                res -= 1;
                borrow = false;
            }
        }
        resnumber.push_back(res);
    }
    // We always use the bigger integer minus the smaller one, so it is impossible the borrow is still true after
    // processing all digits
    if (resnumber.size() > 1)
    {
        while (resnumber.back() == 0)
            resnumber.pop_back(); // Pop out the extra 0
    }
    number = resnumber;
    return *this;
}

bigint &bigint::operator*=(const bigint &timebig)
{
    vector<uint64_t> timenum = timebig.getnumber(), resnum;
    vector<uint32_t> digits;
    bigint res(0), original = *this;
    string temp;
    for (uint64_t i = 0; i < timenum.size(); i++) // Execrate every digit of timenum to digit
    {
        temp = to_string(timenum[i]);
        if (i != timenum.size() - 1)
            temp.insert(temp.begin(), 18 - temp.length(), '0');
        for (uint64_t j = temp.length() - 1; j < temp.length(); j--)
        {
            int16_t pushnumber = temp[j] - '0';
            digits.push_back(static_cast<uint32_t>(pushnumber));
        }
    }
    for (uint64_t i = 0; i < digits.size(); i++)
    {
        (*this).timessingal(digits[i], i);
        res += (*this);
        (*this) = original;
    }
    *this = res;
    if (positive == timebig.getsign())
        positive = true;
    else
        positive = false;
    return *this;
}

bigint &bigint::operator=(const bigint &timebig)
{
    number = timebig.getnumber();
    positive = timebig.getsign();
    return *this;
}

void bigint::timessingal(const uint32_t &coeffi, uint64_t power)
{
    vector<uint64_t> resnum;
    uint64_t res = 0, carry = 0;
    for (uint64_t i = 0; i < number.size(); i++)
    {
        res = number[i] * coeffi;
        if (carry != 0)
            res += carry;
        if (res > maxdec) // overflow 18 digits, carry on
        {
            carry = res / (maxdec + 1);
            res %= (maxdec + 1);
        }
        else
            carry = 0;
        resnum.push_back(res);
    }
    if (carry != 0)
    {
        resnum.push_back(carry);
        carry = 0;
    }

    while (power != 0)
    {
        for (uint64_t i = 0; i < resnum.size(); i++)
        {
            resnum[i] *= 10;
            if (carry != 0)
                resnum[i] += carry;
            if (resnum[i] > maxdec)
            {
                carry = resnum[i] / (maxdec + 1);
                resnum[i] %= (maxdec + 1);
            }
            else
                carry = 0;
        }
        if (carry != 0)
        {
            resnum.push_back(carry);
            carry = 0;
        }
        power--;
    }
    number = resnum;
}

bool operator>(const bigint &big1, const bigint &big2)
{
    vector<uint64_t> v1 = big1.getnumber(), v2 = big2.getnumber();
    bool s1 = big1.getsign(), s2 = big2.getsign();
    if (s1 && !s2)
        return true;
    if (!s1 && s2)
        return false;
    // Same sign
    if (v1.size() > v2.size())
        return s1;
    if (v1.size() < v2.size())
        return !s1;
    // v1.size()==v2.size()
    if (v1.size() == 0)
        return true; // big1 and big2 are all 0
    uint64_t n1 = v1.back(), n2 = v2.back();
    while (v1.size())
    {
        if (n1 > n2)
        {
            return s1;
        }

        else if (n1 < n2)
        {
            return !s1; // the result still rely on the sign of the 2 numbers.
        }
        v1.pop_back();
        v2.pop_back();
        n1 = v1.back();
        n2 = v2.back();
    }
    return false; // number1=number2. return false
}

bool operator>=(const bigint &big1, const bigint &big2)
{
    vector<uint64_t> v1 = big1.getnumber(), v2 = big2.getnumber();
    bool s1 = big1.getsign(), s2 = big2.getsign();
    if (s1 && !s2)
        return true;
    if (!s1 && s2)
        return false;
    // Same sign
    if (v1.size() > v2.size())
        return s1;
    if (v1.size() < v2.size())
        return !s1;
    // v1.size()==v2.size()
    if (v1.size() == 0)
        return true; // big1 and big2 are all 0
    uint64_t n1 = v1.back(), n2 = v2.back();
    while (v1.size())
    {
        if (n1 >= n2)
        {
            v1.pop_back();
            v2.pop_back();
        }

        else
        {
            return !s1; // the result still rely on the sign of the 2 numbers.
            v1.pop_back();
            v2.pop_back();
        }
    }
    return s1; // number1>number2. return the result based on the sign
}

bool operator<(const bigint &big1, const bigint &big2)
{
    return !(big1 >= big2);
}

bool operator<=(const bigint &big1, const bigint &big2)
{
    return !(big1 > big2);
}

bool operator==(const bigint &big1, const bigint &big2)
{
    vector<uint64_t> v1 = big1.getnumber(), v2 = big2.getnumber();
    bool s1 = big1.getsign(), s2 = big2.getsign();
    uint64_t n1 = v1.back(), n2 = v2.back();
    if (v1.size() != v2.size())
        return false;
    if (s1 != s2)
        return false;
    // v1.size()==v2.size() && v1 and v2 have the same sign
    while (v1.size())
    {
        if (n1 == n2)
        {
            v1.pop_back();
            v2.pop_back();
        }

        else
        {
            return false; // the result still rely on the sign of the 2 numbers.
            v1.pop_back();
            v2.pop_back();
        }
    }
    return true; // number1>number2. return the result based on the sign
}

bool operator!=(const bigint &big1, const bigint &big2)
{
    return !(big1 == big2);
}

bigint operator-(const bigint &big1)
{
    bigint res((big1.getnumber()), (!(big1.getsign())));
    return res;
}

ostream &operator<<(ostream &out, const bigint &big)
{
    vector<uint64_t> number = big.getnumber();
    string res;
    bool needpad = (number.size() > 1) ? true : false;
    if (number.size() == 0)
    {
        out << "0";
        return out;
    }
    if (!needpad && number[0] == 0)
    {
        out << "0";
        return out;
    }
    while (number.back() == 0)
        number.pop_back();
    if (!big.getsign()) // if the sign is negative, and the number is not 0, cout the '-'
        res = "-";
    res += to_string(number.back());
    number.pop_back();
    while (number.size() > 0)
    {
        string connum = to_string(number.back()); // Concatenate number
        if (needpad && connum.length() < 18)
            connum.insert(connum.begin(), 18 - connum.length(), '0'); // If it is less than 18 digits, pad with zeros
        res += connum;
        number.pop_back();
    }
    out << res;
    return out;
}

bigint operator+(bigint big1, const bigint &big2)
{
    big1 += big2;
    return big1;
}

bigint operator-(bigint big1, const bigint &big2)
{
    return big1 -= big2;
}

bigint operator*(bigint big1, const bigint &big2)
{
    return big1 *= big2;
}