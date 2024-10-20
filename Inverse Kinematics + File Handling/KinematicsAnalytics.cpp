#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#define PI 3.14159265359
#define RAD PI / 180.0

using namespace std;

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Fungsi untuk menyimpan data ke file
void saveDataToFile(int input_speed[3], int motor_speeds[4]) {
    ofstream output_file("motor_output.txt");
    
    // Menyimpan header
    output_file << "==============================\n";
    output_file << "         Kinematics Output     \n";
    output_file << "==============================\n\n";
    
    output_file << "Input Kecepatan:\n";
    output_file << "X: " << input_speed[0] << ", Y: " << input_speed[1] << ", Z: " << input_speed[2] << "\n";
    output_file << "\nHasil Kecepatan Motor:\n";
    output_file << "------------------------------\n";
    
    for (int i = 0; i < 4; i++) {
        output_file << "Motor " << i << " (Theta = " << (45.0 + i * 90) << " derajat):\n";
        output_file << "V_" << i << " = cos(" << (45.0 + i * 90) << ") * X + sin(" << (45.0 + i * 90) << ") * Y + Z\n";
        output_file << "Kecepatan motor " << i << " : " << motor_speeds[i] << "\n";
        output_file << "------------------------------\n";
    }
    
    output_file.close();
}

// Fungsi untuk membaca data dari file
void readDataFromFile() {
    ifstream input_file("motor_output.txt");
    string line;
    
    if (input_file.is_open()) {
        cout << "\n==============================\n";
        cout << "         Kinematics Output     \n";
        cout << "==============================\n\n";
        
        while (getline(input_file, line)) {
            cout << line << endl;
        }
        input_file.close();
    } else {
        cout << "Tidak bisa membuka file!" << endl;
    }
}

// Fungsi untuk menghitung kecepatan motor
void calculateMotorSpeed(int input_speed[3], int motor_speeds[4]) {
    motor_speeds[0] = cos(45.0 * RAD) * input_speed[0] + sin(45.0 * RAD) * input_speed[1] + input_speed[2];
    motor_speeds[1] = cos(135.0 * RAD) * input_speed[0] + sin(135.0 * RAD) * input_speed[1] + input_speed[2];
    motor_speeds[2] = cos(225.0 * RAD) * input_speed[0] + sin(225.0 * RAD) * input_speed[1] + input_speed[2];
    motor_speeds[3] = cos(315.0 * RAD) * input_speed[0] + sin(315.0 * RAD) * input_speed[1] + input_speed[2];
}

// Fungsi untuk menampilkan hasil kecepatan motor
void displayMotorSpeeds(int motor_speeds[4]) {
    cout << GREEN << "Hasil perhitungan kecepatan motor:" << RESET << endl;
    for (int i = 0; i < 4; i++) {
        cout << "Motor " << i << " (Theta = " << (45.0 + i * 90) << " derajat):" << endl;
        cout << MAGENTA << "V_" << i << " = cos(" << (45.0 + i * 90) << ") * X + sin(" << (45.0 + i * 90) << ") * Y + Z" << RESET << endl;
        cout << "Kecepatan motor " << i << " : " << motor_speeds[i] << endl << endl;
    }
}

// Fungsi untuk menampilkan rumus kinematics
void displayFormula() {
    cout << BLUE << "Rumus Inverse Kinematics:" << RESET << endl;
    cout << "V_i = cos(theta_i) * x + sin(theta_i) * y + z" << endl;
    cout << "Di mana theta_i adalah sudut roda ke-i dalam derajat." << endl << endl;
}

int main() {
    int input_speed[3];
    int motor_speeds[4];

    // Pesan sambutan
    cout << CYAN << "Selamat datang di KinematicsOmniBot!" << RESET << endl;
    cout << "Disini kamu bisa memasukkan kecepatan translasi X, Y, dan rotasi Z." << endl;
    cout << "Program ini akan memberikan hasil terkait kecepatan setiap motor robot." << endl << endl;

    // Tampilkan rumus yang digunakan
    displayFormula();

    // Input kecepatan translasi X, Y, dan rotasi Z
    cout << "Masukkan kecepatan translasi X: ";
    cin >> input_speed[0];
    cout << "Masukkan kecepatan translasi Y: ";
    cin >> input_speed[1];
    cout << "Masukkan kecepatan rotasi Z: ";
    cin >> input_speed[2];

    // Hitung kecepatan motor berdasarkan input kecepatan
    calculateMotorSpeed(input_speed, motor_speeds);

    // Simpan input dan hasil ke file
    saveDataToFile(input_speed, motor_speeds);

    // Tampilkan hasil kecepatan motor
    displayMotorSpeeds(motor_speeds);

    // Tanyakan apakah pengguna ingin membaca file
    char choice;
    cout << "Apakah Anda ingin melihat hasil yang disimpan di file? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        readDataFromFile();
    }

    return 0;
}
