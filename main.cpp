#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>
using namespace std;

vector <double> readFile(const string& file_path)
{
    vector<double> data;

    ifstream file(file_path);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << file_path << endl;
        return data;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        double value;
        while (iss >> value)
        {
            data.push_back(value);
        }
    }

    file.close();

    return data;
}

double mse(vector<double>& x1, vector<double>& x2, vector<double>& x3, vector<double>& y, double w0, double w1, double w2, double w3)
{
    size_t size = y.size();
    vector<double> diff(size);
    double mse = 0.0;
    for (size_t i = 0; i < 10; ++i)
    {
        double predY = w0 + w1 * x1[i] + w2 * x2[i] + w3 * x3[i];
        diff[i] = pow(predY - y[i], 2);
    }

    for (size_t i = 0; i < 10; i++)
    {
        mse += diff[i];
    }
    return mse / 10;
}

int main()
{
    vector<double> x1 = readFile("x1.txt");
    vector<double> x2 = readFile("x2.txt");
    vector<double> x3 = readFile("x3.txt");
    vector<double> y = readFile("y.txt");

    double alpha = 10;
    double decayRate = 0.999;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-1, 1);
    double w0 = dis(gen);
    double w1 = dis(gen);
    double w2 = dis(gen);
    double w3 = dis(gen);

    double minMSE = mse(x1, x2, x3, y, w0, w1, w2, w3);
    double MSE = 0.0;

    while(minMSE>0.001)
    {
        double new_w0 = w0 + alpha * dis(gen);
        double new_w1 = w1 + alpha * dis(gen);
        double new_w2 = w2 + alpha * dis(gen);
        double new_w3 = w3 + alpha * dis(gen);

        MSE = mse(x1, x2, x3, y, new_w0, new_w1, new_w2, new_w3);

        if (MSE < minMSE)
        {
            minMSE = MSE;
            w0 = new_w0;
            w1 = new_w1;
            w2 = new_w2;
            w3 = new_w3;
            alpha=10;
            cout << "Min MSE: " << setprecision(15) << minMSE << endl;
            continue;
        }
        alpha *= decayRate;
    }

    cout << "Optimal weights: " << w0 << ", " << w1 << ", " << w2 << ", " << w3 << endl;
    cout << "Final MSE: " << minMSE << endl;
    cout << "\nThe final equation: Y=" << w0 << "+" << w1 << "X1+" << w2 << "X2+" << w3 << "X3" << endl;

    double yPred = w0 + w1 * x1[0] + w2 * x2[0] + w3 * x3[0];
    cout << "Predictive y: " << yPred << ", real y: " << y[0] << endl;

    return 0;
}
