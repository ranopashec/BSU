#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

const int POINTS = 100;
const double a = -3, b = 3;
const double PI = 3.14159265358979323846;

double f1(double x) { return x * x * cos(2 * x); }
double f2(double x) { return 1.0 / (1 + 5 * x * x); }


vector<double> newtonDividedDifferences(const vector<double>& x, const vector<double>& y)
{
    int n = x.size();
    vector<double> coef(y);
    for (int j = 1; j < n; j++)
        for (int i = n - 1; i >= j; i--)
            coef[i] = (coef[i] - coef[i - 1]) / (x[i] - x[i - j]);
    return coef;
}

double newtonInterpolation(double x, const vector<double>& nodes, const vector<double>& coef)
{
    double result = coef.back();
    for (int i = coef.size() - 2; i >= 0; i--)
        result = result * (x - nodes[i]) + coef[i];
    return result;
}

vector<double> equidistantNodes(int n)
{
    vector<double> nodes(n + 1);
    for (int i = 0; i <= n; i++)
        nodes[i] = a + i * (b - a) / n;
    return nodes;
}

vector<double> chebyshevNodes(int n)
{
    vector<double> nodes(n + 1);
    for (int i = 0; i <= n; i++)
        nodes[i] = (a + b) / 2.0 + (b - a) / 2.0 * cos((2.0 * i + 1) * PI / (2.0 * (n + 1)));
    return nodes;
}

void writeToFile(const string& filename, const vector<double>& nodes, const vector<double>& coef, double (*f)(double))
{
    ofstream file(filename);
    for (int i = 0; i <= POINTS; i++)
    {
        double xi = a + i * (b - a) / POINTS;
        double yi = newtonInterpolation(xi, nodes, coef);
        file << xi << " " << yi << endl;
    }
    file.close();
}

double computeError(const vector<double>& nodes, const vector<double>& coef, double (*f)(double))
{
    double maxError = 0;
    for (int i = 0; i <= POINTS; i++)
    {
        double xi = a + i * (b - a) / POINTS;
        double error = fabs(newtonInterpolation(xi, nodes, coef) - f(xi));
        if (error > maxError) maxError = error;
    }
    return maxError;
}

int main()
{
    
    vector<int> ns = { 3, 10, 20, 30 };

    ofstream file_f1("f1.txt");
    ofstream file_f2("f2.txt");
    for (int i = 0; i <= POINTS; i++)
    {
        double xi = a + i * (b - a) / POINTS;
        file_f1 << xi << " " << f1(xi) << endl;
        file_f2 << xi << " " << f2(xi) << endl;
    }
    file_f1.close();
    file_f2.close();

    cout << "======================================================================\n";
    cout << "error for f1(x) = x²cos(2x)\n";
    cout << "n  | max error (ravnoudaleniye)  | max error (Chebishev)\n";
    cout << "---|-----------------------------|-----------------------------\n";

    for (int n : ns)
    {
        vector<double> x_eq = equidistantNodes(n);
        vector<double> x_ch = chebyshevNodes(n);

        vector<double> y1_eq(n + 1), y1_ch(n + 1);
        for (int i = 0; i <= n; i++)
        {
            y1_eq[i] = f1(x_eq[i]);
            y1_ch[i] = f1(x_ch[i]);
        }

        vector<double> coef1_eq = newtonDividedDifferences(x_eq, y1_eq);
        vector<double> coef1_ch = newtonDividedDifferences(x_ch, y1_ch);

        writeToFile("P1_" + to_string(n) + ".txt", x_eq, coef1_eq, f1);
        writeToFile("C1_" + to_string(n) + ".txt", x_ch, coef1_ch, f1);

        cout << setw(2) << n << " | "
            << setw(27) << fixed << setprecision(12) << computeError(x_eq, coef1_eq, f1) << " | "
            << setw(27) << computeError(x_ch, coef1_ch, f1) << endl;
    }

    cout << "\n======================================================================\n";
    cout << "error for f2(x) = 1 / (1 + 5x²)\n";
    cout << "n  | max error (ravnoudaleniye)  | max error (Chebishev)\n";
    cout << "---|-----------------------------|-----------------------------\n";

    for (int n : ns)
    {
        vector<double> x_eq = equidistantNodes(n);
        vector<double> x_ch = chebyshevNodes(n);

        vector<double> y2_eq(n + 1), y2_ch(n + 1);
        for (int i = 0; i <= n; i++)
        {
            y2_eq[i] = f2(x_eq[i]);
            y2_ch[i] = f2(x_ch[i]);
        }

        vector<double> coef2_eq = newtonDividedDifferences(x_eq, y2_eq);
        vector<double> coef2_ch = newtonDividedDifferences(x_ch, y2_ch);

        writeToFile("P2_" + to_string(n) + ".txt", x_eq, coef2_eq, f2);
        writeToFile("C2_" + to_string(n) + ".txt", x_ch, coef2_ch, f2);

        cout << setw(2) << n << " | "
            << setw(27) << computeError(x_eq, coef2_eq, f2) << " | "
            << setw(27) << computeError(x_ch, coef2_ch, f2) << endl;
    }

    return 0;
}
