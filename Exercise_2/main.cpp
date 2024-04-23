#include <iostream>
#include <fstream>
#include <sstream>

#include <iomanip>

using namespace std;

/// \brief ImporData reads the input data from the data file
/// \param inputFilePath: path name of the input file
/// \param S: the resulting initial wealth
/// \param n: the resulting number of assets
/// \param w: the resulting vector of the weights of assets in the portfolio
/// \param r: the resulting vector of the rates of return of assets
/// \return the result of the reading: true is success, false is error
bool ImportData(const string& inputFilePath,
                double& S,
                size_t& n,
                double*& w,
                double*& r);

/// \brief ComputeRateOfReturn computes the rate of return of the portfolio and the final amount of wealth
/// \param S: the initial wealth
/// \param n: the number of assets
/// \param w: the vector of the weights of assets in the portfolio
/// \param r: the vector of the rates of return of assets
/// \param rateOfReturn: the resulting rate of return of the portfolio
/// \param V: the resulting final wealth
void ComputeRateOfReturn(const double& S,
                         const size_t& n,
                         const double* const& w,
                         const double* const& r,
                         double& rateOfReturn,
                         double& V);

/// \brief ExportData prints data on an output stream
/// \param out: object of type ostream
/// \param S: the initial wealth
/// \param n: the number of assets
/// \param w: the vector of the weights of assets in the portfolio
/// \param r: the vector of the rates of return of assets
/// \param rateOfReturn: the rate of return of the portfolio
/// \param V: the final wealth
void ExportData(ostream& out,
                const double& S,
                const size_t& n,
                const double* const& w,
                const double* const& r,
                const double& rateOfReturn,
                const double& V);

/// \brief Export a vector in a string
/// \param n: size of the vector
/// \param v: vector
/// \return the resulting string
string ArrayToString(const size_t& n,
                     const double* const& v)
{

    ostringstream toString;
    toString << "[ ";
    for (unsigned int i = 0; i < n; i++)
        toString<< v[i]<< " ";
    toString << "]";

    return toString.str();

}

int main()
{
    string inputFileName = "./data.csv";
    double S = 0.0;
    size_t n = 0;
    double* w = nullptr;
    double* r = nullptr;

    if (!ImportData(inputFileName, S, n, w, r))
    {
        cerr<< "Something goes wrong with import"<< endl;
        return -1;
    }

    // Compute the rate of return of the portfolio and the final wealth V
    double rateOfReturn;
    double V;
    ComputeRateOfReturn(S, n, w, r, rateOfReturn, V);


    // Export data on the standard output
    ExportData(cout, S, n, w, r, rateOfReturn, V);


    // Write data to a file
    string outputFileName = "./result.txt";
    ofstream file;
    file.open(outputFileName);

    if (!file.fail())
    {
        ExportData(file, S, n, w, r, rateOfReturn, V);
    }

    file.close();

    delete[] w;
    delete[] r;

    return 0;
}

bool ImportData(const string& inputFilePath,
                double& S,
                size_t& n,
                double*& w,
                double*& r)
{
    // Open File
    ifstream file;
    file.open(inputFilePath);

    if (file.fail())
    {
        cerr<< "file open failed"<< endl;
        return false;
    }

    // Get total sum invested
    string line;
    getline(file, line);

    istringstream convertStot;
    convertStot.str(line);
    string word;
    getline(convertStot,word, ';');
    getline(convertStot,word, ';');
    S = stof(word);

    //Get number of assets
    getline(file, line);

    istringstream convertNtot;
    convertNtot.str(line);
    getline(convertNtot,word, ';');
    getline(convertNtot,word, ';');
    n = stof(word);

    //Get single rates of return and fractions of the sum invested
    getline(file, line);
    r = new double[n];
    w = new double[n];

    for (unsigned int i = 0; i < n; i++)
    {
        getline(file, line);
        istringstream str(line);
        getline(str,word, ';');
        w[i] = stof(word);
        getline(str,word, ';');
        r[i] = stof(word);
        //i++
    }

    //Close File
    file.close();
    return true;
}


void ComputeRateOfReturn(const double& S,
                         const size_t& n,
                         const double* const& w,
                         const double* const& r,
                         double& rateOfReturn,
                         double& V)
{
    rateOfReturn = 0;
    for (unsigned int i = 0; i < n; i++)
        rateOfReturn += r[i] * w[i];
    V = (1+rateOfReturn)*S;
}


void ExportData(ostream& out,
                const double& S,
                const size_t& n,
                const double* const& w,
                const double* const& r,
                const double& rateOfReturn,
                const double& V)
{
    out<< "S = " << fixed <<setprecision(2) << S << ", n = " << n << endl;
    out<< "w = [";
    for (unsigned int i = 0; i < n; i++)
        out<< " " << w[i];
    out<< " ]" << endl;
    out<< "r = [";
    for (unsigned int i = 0; i < n; i++)
        out<< " " << r[i];
    out<< " ]" << endl;
    out<< "Rate of return of the portfolio: " <<  fixed <<setprecision(4) <<rateOfReturn << endl;
    out<< "V: " << fixed <<setprecision(2) << V << endl;
}
