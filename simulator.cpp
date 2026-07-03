#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <bitset>
#include <ctime>
using namespace std;

int charToVal(char c) {
    if (c >= '0' && c <= '9') return (int)c - '0';
    else if (c >= 'A' && c <= 'Z') return (int)c - 'A' + 10;
    else if (c >= 'a' && c <= 'z') return (int)c - 'a' + 10;
    return -1;
}

char valToChar(int num) {
    if (num >= 0 && num <= 9) return (char)(num + '0');
    else return (char)(num - 10 + 'A');
}

bool isValidNumber(const string& num, int base) {
    if (base < 2 || base > 20) return false;
    int dotCount = 0;
    for (char c : num) {
        if (c =='.') {
            dotCount++;
            if (dotCount > 1) return false;
            continue;
        }
        int val = charToVal(c);
        if (val < 0 || val >= base) return false;
    }
    return true;
}

string enforce32Bits(string b) {
    if (b.length() >= 32) return b.substr(b.length() - 32);
    return string(32 - b.length(), '0') + b;
}

double toDecimal(const string& num, int base) {
    size_t dotPos = num.find('.');
    string intPart = (dotPos == string::npos) ? num : num.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : num.substr(dotPos + 1);
    double decimalValue = 0.0;
    long long intVal = 0;
    long long power = 1;
    for (int i = intPart.length() - 1; i >= 0; i--) {
        intVal += charToVal(intPart[i]) * power;
        power *= base;
    }
    decimalValue += intVal;
    double fracVal = 0.0;
    double fracPower = 1.0 / base;
    for (size_t i = 0; i < fracPart.length(); i++) {
        fracVal += charToVal(fracPart[i]) * fracPower;
        fracPower /= base;
    }
    decimalValue += fracVal;
    return decimalValue;
}

string fromDecimal(double num, int base) {
    long long intPart = (long long)num;
    double fracPart = num - intPart;
    string intStr = "";
    if (intPart == 0) intStr = "0";
    while (intPart > 0) {
        intStr += valToChar(intPart % base);
        intPart /= base;
    }
    reverse(intStr.begin(), intStr.end());
    string fracStr = "";
    int precision = 6; 
    while (fracPart > 0 && precision-- > 0) {
        fracPart *= base;
        int bit = (int)fracPart;
        fracStr += valToChar(bit);
        fracPart -= bit;
    }
    if (fracStr.empty()) return intStr;
    return intStr + "." + fracStr;
}

void processCSVConversions(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    if (!in.is_open()) {
        cerr << "Error: Could not open " << inputFile << endl;
        return;
    }
    string line;
    if (getline(in, line)) out << "number,base1,base2,output1,output2,match\n";
    while (getline(in, line)) {
        stringstream ss(line);
        string numberStr, base1Str, base2Str;
        getline(ss, numberStr, ',');
        getline(ss, base1Str, ',');
        getline(ss, base2Str, ',');
        int base1 = stoi(base1Str);
        int base2 = stoi(base2Str);
        if (!isValidNumber(numberStr, base1)|| base2 < 2 || base2 > 20) {
            out << numberStr << "," << base1 << "," << base2 << ",INVALID,INVALID,False\n";
            continue;
        }
        double decVal1 = toDecimal(numberStr, base1);
        string output1 = fromDecimal(decVal1, base2);
        double decVal2 = toDecimal(output1, base2);
        string output2 = fromDecimal(decVal2, base1);
        bool match = abs(decVal1 - decVal2) < 1e-5; 
        string matchStr = match ? "True" : "False";
        out << numberStr << "," << base1 << "," << base2 << "," 
            << output1 << "," << output2 << "," << matchStr << "\n";
    }
    cout << "Processed base conversions successfully! Check " << outputFile << endl;
    in.close();
    out.close();
}

int64_t parseBinaryStr(string b, int mode) {
    b = enforce32Bits(b);
    uint32_t raw = bitset<32>(b).to_ulong();
    switch (mode) {
        case 1: return (int64_t)raw;
        case 2: 
            if (b[0] == '1') return -1 * (int64_t)(raw & 0x7FFFFFFF); 
            return (int64_t)raw;
        case 3:
            if (b[0] == '1') return -1 * (int64_t)(~raw & 0x7FFFFFFF); 
            return (int64_t)raw;
        case 4: return (int64_t)((int32_t)raw); 
    }
    return 0;
}

string encodeBinaryStr(int64_t val, int mode) {
    uint32_t raw = 0;
    const int64_t MAX_UNSIGN = 4294967295LL; 
    const int64_t MAX_SIGN = 2147483647LL; 
    const int64_t MIN_SIGN = -2147483647LL;
    const int64_t MIN_2SCOMP = -2147483648LL;
    switch (mode) {
        case 1: 
            if (val < 0) return "ERROR: Underflow";
            if (val > MAX_UNSIGN) return "ERROR: Overflow";
            raw = (uint32_t)val;
            break;
        case 2: 
            if (val < MIN_SIGN) return "ERROR: Underflow";
            if (val > MAX_SIGN) return "ERROR: Overflow";
            raw = (uint32_t)abs(val);
            if (val < 0) raw |= 0x80000000; 
            break;
        case 3: 
            if (val < MIN_SIGN) return "ERROR: Underflow";
            if (val > MAX_SIGN) return "ERROR: Overflow";
            raw = (uint32_t)abs(val);
            if (val < 0) raw = ~raw | 0x80000000; 
            break;
        case 4: 
            if (val < MIN_2SCOMP) return "ERROR: Underflow";
            if (val > MAX_SIGN) return "ERROR: Overflow";
            raw = (uint32_t)((int32_t)val);
            break;
    }
    return bitset<32>(raw).to_string();
}

void logHistory(const string& logEntry) {
    ofstream logFile("history.txt", ios::app); 
    if (logFile.is_open()) {
        time_t now = time(0);
        string timeStr(ctime(&now));
        timeStr.pop_back(); 
        logFile << "[" << timeStr << "] " << logEntry << "\n";
        logFile.close();
    } else {
        cerr << "Warning: Could not open history.txt to write log.\n";
    }
}

void processBinaryArithmetic() {
    cout << "\n--- Binary Arithmetic Module (32-bit) ---\n";
    cout << "Select Representation Mode:\n";
    cout << "1. Unsigned Binary\n2. Signed Magnitude\n3. One's Complement\n4. Two's Complement\nMode: ";
    int mode; cin >> mode;
    if (mode < 1 || mode > 4) { cout << "Invalid mode.\n"; return; }
    cout << "Select Operation (+, -, *, /): ";
    char op; cin >> op;
    cout << "Enter first binary number (up to 32 bits): ";
    string num1; cin >> num1;
    cout << "Enter second binary number (up to 32 bits): ";
    string num2; cin >> num2;
    if (!isValidNumber(num1, 2) || !isValidNumber(num2, 2)) {
        cout << "Error: Inputs must be valid binary strings (0s and 1s only).\n";
        return;
    }
    int64_t val1 = parseBinaryStr(num1, mode);
    int64_t val2 = parseBinaryStr(num2, mode);
    int64_t result = 0;
    switch (op) {
        case '+': result = val1 + val2; break;
        case '-': result = val1 - val2; break;
        case '*': result = val1 * val2; break;
        case '/': 
            if (val2 == 0) { cout << "Error: Division by Zero!\n"; return; }
            result = val1 / val2; 
            break;
        default: cout << "Invalid operation.\n"; return;
    }
    string finalBinaryStr = encodeBinaryStr(result, mode);
    cout << "\n============================================\n";
    cout << "Num 1 (padded): " << enforce32Bits(num1) << " (Value: " << val1 << ")\n";
    cout << "Num 2 (padded): " << enforce32Bits(num2) << " (Value: " << val2 << ")\n";
    cout << "Operation: " << op << "\n";
    if (finalBinaryStr.find("ERROR") != string::npos) {
        cout << "Result: " << finalBinaryStr << "\n";
    } else {
        cout << "Result (Bin):   " << finalBinaryStr << "\n";
        cout << "Result (Dec):   " << result << "\n";
    }
    cout << "============================================\n";
    stringstream logStream;
    string modeNames[] = {"", "Unsigned", "Signed Magnitude", "1s Complement", "2s Complement"};
    logStream << "Mode: " << modeNames[mode] << " | " << num1 << " " << op << " " << num2 << " | Output: " << finalBinaryStr;
    logHistory(logStream.str());
}

int main() {
    int choice;
    while (true) {
        cout << "\n============================================\n";
        cout << "   Digital Logic Design Simulator\n";
        cout << "============================================\n";
        cout << "1. Week 1: Base Conversion (from input.csv)\n";
        cout << "2. Week 2: Binary Arithmetic (Interactive)\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            break;
        }
        switch (choice) {
            case 1: processCSVConversions("input.csv", "output.csv"); break;
            case 2: processBinaryArithmetic(); break;
            case 3: cout << "Exiting Simulator. Goodbye!\n"; return 0;
            default: cout << "Invalid choice. Please select 1, 2, or 3.\n"; break;
        }
    }
    return 0;
}
