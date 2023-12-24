#include "bigint.hpp"
#include <fstream>
#include <random>
using namespace std;
/**
 * @brief Generate a test file.
 * @param x Generate x random numbers from 1 to 1000 digits.
 */
void generate_test_file(const uint64_t &x);
/**
 * @brief You can input your own number(Only read from file is acceptable.)
 *        I have provided a function: generate_test_file to automaticaly generate the test file.
 */
int main()
{
    //generate_test_file(10);
    ifstream test("test.txt"); // Read data from file. CHANGE THE NAME OF FILE HERE!
    if (!test.is_open())
    {
        cout << "There is no test.txt. You can generate one by calling generate_test_file()";
        return -1;
    }
    vector<bigint> testcase;
    bigint temp;
    string s;
    while (getline(test, s)) // Generate bigint according to every line of file, store all bigint in a vector
    {
        try
        {
            bigint big(s);
            testcase.push_back(big);
        }
        catch (const exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    for (uint32_t i = 1; i < testcase.size() - 1; i++)
    {
        cout << "If\n" << testcase[i] << "\n>" << testcase[i + 1] << "?\nThe answer:" << (testcase[i] > testcase[i + 1]) << "\n";
        cout << "-------------------------------------------\n";
        cout << "If\n" << testcase[i] << "\n>=" << testcase[i + 1] << "?\n The answer:" << (testcase[i] >= testcase[i + 1]) << "\n";
        cout << "-------------------------------------------\n";
        cout << "If\n" << testcase[i] << "\n<" << testcase[i - 1] << "?\n The answer:" << (testcase[i] < testcase[i - 1]) << "\n";
        cout << "-------------------------------------------\n";
        cout << "If\n" << testcase[i] << "\n<=" << testcase[i - 1] << "?\n The answer:" << (testcase[i] < testcase[i - 1]) << "\n";
        cout << "-------------------------------------------\n";
        cout << testcase[i] << "\n+=\n"<< testcase[i + 1] << ".\n The answer:\n";
        testcase[i] += testcase[i + 1];
        cout << testcase[i] << "\n";
        cout << "-------------------------------------------\n";
        cout << testcase[i] << "\n-=\n"<< testcase[i + 1] << ".\n The answer:\n";
        testcase[i] -= testcase[i + 1];
        cout << testcase[i] << "\n";
        cout << "-------------------------------------------\n";
        cout << testcase[i] << "\n+\n" << testcase[i - 1] << "\n=\n" << (testcase[i] + testcase[i - 1])<< "\n";
        cout << "-------------------------------------------\n";
        cout << testcase[i] << "\n-\n" << testcase[i - 1] << "\n=\n" << (testcase[i] - testcase[i - 1])<< "\n";
        cout << "-------------------------------------------\n";
        cout << testcase[i] << "\n*\n" << testcase[i - 1] << "\n=\n" << 
        (testcase[i] * testcase[i - 1])<< "\n";
        cout << "---------------Next Group-----------------\n";
    }
}

void generate_test_file(const uint64_t &x)
{
    ofstream test("test.txt");
    if (!test.is_open())
        throw invalid_argument("Fail to create file!");
    // Boundary detection testcase
    test << "0"
         << "\n"
         << "1"
         << "\n"
         << "-1"
         << "\n"
         << "001"
         << "\n";
    for (int32_t i = 1; i < 18; ++i)
        test << "9";
    test << "\n"
         << "-";
    for (int32_t i = 1; i < 18; ++i)
        test << "9";
    test << "\n"
         << "-";
    for (int32_t i = 1; i <= 18; ++i)
        test << "9";
    test << "\n"
         << "-";
    for (int32_t i = 1; i <= 18; ++i)
        test << "9";
    test << "\n"
         << "1";
    for (int32_t i = 1; i <= 18; ++i)
        test << "0";
    test << "\n"
         << "-1";
    for (int32_t i = 1; i <= 18; ++i)
        test << "0";
    test << "\n";
    // Random common situation detection
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint32_t> sign(0, 1);
    uniform_int_distribution<uint32_t> uid(0, 9);
    uniform_int_distribution<uint32_t> dig(1, 1000);
    uint64_t digits;
    for (uint64_t i = 0; i < x; i++)
    {
        if(sign(gen))
            test<<"-";
        digits = dig(gen);
        for (uint64_t j = 1; j <= digits; j++)
            test << uid(gen);
        test << "\n";
    }
    test.close();
}
