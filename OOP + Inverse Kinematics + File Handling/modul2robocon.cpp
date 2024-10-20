#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <string>
#include <unordered_map>
#include <algorithm>

#define PI 3.14159265359
#define RAD PI / 180.0

using namespace std;

// ANSI color codes
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BLUE    "\033[34m"

// Kelas Motor
class Motor {
public:
    float theta; // Sudut roda
    int speed;   // Kecepatan motor

    Motor(float theta) : theta(theta), speed(0) {} // Constructor

    void calculateSpeed(int input_speed[3]) {
        speed = cos(theta * RAD) * input_speed[0] + sin(theta * RAD) * input_speed[1] + input_speed[2];
    }

    void displaySpeed(int motorIndex) const {
        cout << "Motor " << motorIndex << " (Theta = " << theta << " derajat):" << endl;
        cout << MAGENTA << "V_" << motorIndex << " = cos(" << theta << ") * X + sin(" << theta << ") * Y + Z" << RESET << endl;
        cout << "Kecepatan motor " << motorIndex << " : " << speed << endl << endl;
    }
};

// Kelas untuk menghitung kinematics
class Kinematics {
private:
    Motor motors[4]; // Array dari objek Motor
public:
    Kinematics() : motors{Motor(45.0), Motor(135.0), Motor(225.0), Motor(315.0)} {}

    void calculateMotorSpeed(int input_speed[3]) {
        for (int i = 0; i < 4; i++) {
            motors[i].calculateSpeed(input_speed);
        }
    }

    void displayMotorSpeeds() const {
        cout << GREEN << "Hasil perhitungan kecepatan motor:" << RESET << endl;
        for (int i = 0; i < 4; i++) {
            motors[i].displaySpeed(i);
        }
    }

    bool saveDataToFile(int input_speed[3], const string& format) const {
        string filename = (format == "json") ? "motor_output.json" : "motor_output.txt";
        ofstream output_file(filename);
        
        // Memeriksa apakah file berhasil dibuka
        if (!output_file.is_open()) {
            cout << "Error: Tidak bisa membuka file untuk menyimpan data!" << endl;
            return false; // Keluar dari fungsi jika tidak dapat membuka file
        }

        if (format == "plain") {
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
                output_file << "Kecepatan motor " << i << " : " << motors[i].speed << "\n";
                output_file << "------------------------------\n";
            }
        } else if (format == "json") {
            // Menyimpan dalam format JSON
            output_file << "{\n";
            output_file << "  \"Input Kecepatan\": {\n";
            output_file << "    \"X\": " << input_speed[0] << ",\n";
            output_file << "    \"Y\": " << input_speed[1] << ",\n";
            output_file << "    \"Z\": " << input_speed[2] << "\n";
            output_file << "  },\n";
            output_file << "  \"Hasil Kecepatan Motor\": [\n";
            
            for (int i = 0; i < 4; i++) {
                output_file << "    {\n";
                output_file << "      \"Motor\": " << i << ",\n";
                output_file << "      \"Theta\": " << (45.0 + i * 90) << ",\n";
                output_file << "      \"Kecepatan\": " << motors[i].speed << "\n";
                output_file << "    }" << (i < 3 ? "," : "") << "\n"; // Tambah koma kecuali pada item terakhir
            }
            
            output_file << "  ]\n";
            output_file << "}\n";
        } else {
            cout << "Format tidak dikenali!" << endl;
        }

        output_file.close();
        return true; // Berhasil menyimpan data
    }

    void readDataFromFile(const string& filename) const {
        ifstream input_file(filename);
        string line;
        
        if (input_file.is_open()) {
            cout << "\n==============================\n";
            cout << "         Kinematics Output     \n";
            cout << "==============================\n\n";
            
            while (getline(input_file, line)) {
                // Menampilkan baris dengan warna
                if (line.find("Motor") != string::npos) {
                    cout << GREEN << line << RESET << endl; // Warna hijau untuk baris motor
                } else if (line.find("V_") != string::npos) {
                    cout << MAGENTA << line << RESET << endl; // Warna magenta untuk baris kecepatan
                } else {
                    cout << line << endl; // Baris biasa
                }
            }
            input_file.close();
        } else {
            cout << "Tidak bisa membuka file!" << endl;
        }
    }
};

// Kelas untuk antarmuka pengguna
class UserInterface {
public:
    void welcomeMessage() const {
        cout << CYAN << "Selamat datang di KinematicsOmniBot!" << RESET << endl;
        cout << "Disini kamu bisa memasukkan kecepatan translasi X, Y, dan rotasi Z." << endl;
        cout << "Program ini akan memberikan hasil terkait kecepatan setiap motor robot." << endl << endl;
    }

    void getInput(int input_speed[3]) const {
        for (int i = 0; i < 3; i++) {
            while (true) {
                cout << "Masukkan kecepatan translasi " << (i == 0 ? "X" : (i == 1 ? "Y" : "Z")) << ": ";
                if (cin >> input_speed[i]) break;
                cout << "Input tidak valid. Harap masukkan angka." << endl;
                cin.clear(); // Bersihkan flag kesalahan
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang input yang tidak valid
            }
        }
    }

    void displayFormula() const {
        cout << BLUE << "Rumus Inverse Kinematics:" << RESET << endl;
        cout << "V_i = cos(theta_i) * x + sin(theta_i) * y + z" << endl;
        cout << "Di mana theta_i adalah sudut roda ke-i dalam derajat." << endl;
    }
};

int main() {
    UserInterface ui;
    Kinematics kinematics; // Instance untuk pengelolaan kinematics
    int input_speed[3];
    char repeat;

    do {
        // Pesan sambutan
        ui.welcomeMessage();

        // Tampilkan rumus yang digunakan
        ui.displayFormula();

        // Tanyakan apakah pengguna ingin menghitung inverse kinematics
        char choice;
        while (true) {
            cout << "Apakah Anda ingin menghitung inverse kinematics? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N') {
                break; // Keluar dari loop jika input valid
            } else {
                cout << "Input tidak valid! Silakan masukkan 'y' atau 'n'." << endl;
            }
        }

        if (choice == 'y' || choice == 'Y') {
            // Input kecepatan translasi X, Y, dan rotasi Z
            ui.getInput(input_speed);

            // Hitung kecepatan motor berdasarkan input kecepatan
            kinematics.calculateMotorSpeed(input_speed);

            // Tanyakan format penyimpanan
            string format;
            while (true) {
                cout << "Apakah Anda ingin menyimpan output sebagai Plain Text atau JSON? (plain/json): ";
                cin >> format;

                if (format == "plain" || format == "json") {
                    break; // Keluar dari loop jika format valid
                } else {
                    cout << "Format tidak dikenali! Silakan masukkan 'plain' atau 'json'." << endl;
                }
            }

            // Meminta nama file untuk menyimpan data
            string filename;
            cout << "Masukkan nama file (default: motor_output): ";
            cin.ignore(); // Bersihkan input buffer
            getline(cin, filename);

            if (filename.empty()) {
                filename = (format == "json") ? "motor_output.json" : "motor_output.txt"; // Gunakan nama default jika tidak ada input
            } else {
                filename += (format == "json") ? ".json" : ".txt"; // Tambahkan ekstensi sesuai format
            }

            // Simpan input dan hasil ke file
            kinematics.saveDataToFile(input_speed, format);

            // Tampilkan hasil kecepatan motor
            kinematics.displayMotorSpeeds();
        } else {
            // Jika tidak ingin menghitung, tawarkan opsi untuk membaca file
            cout << "Apakah Anda ingin melihat hasil yang disimpan di file? (y/n): ";
            cin >> choice;

            while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cout << "Input tidak valid! Silakan masukkan 'y' atau 'n'." << endl;
                cout << "Apakah Anda ingin melihat hasil yang disimpan di file? (y/n): ";
                cin >> choice;
            }

            if (choice == 'y' || choice == 'Y') {
                // Meminta nama file dari pengguna
                string filename;
                cout << "Masukkan nama file yang ingin dibaca (default: motor_output.txt): ";
                cin.ignore(); // Bersihkan input buffer
                getline(cin, filename);

                if (filename.empty()) {
                    filename = "motor_output.txt"; // Gunakan nama default jika tidak ada input
                }

                kinematics.readDataFromFile(filename);
            }
        }

        // Tanyakan apakah pengguna ingin menghitung lagi
        cout << "Apakah Anda ingin menghitung lagi? (y/n): ";
        cin >> repeat;

        // Bersihkan input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    } while (repeat == 'y' || repeat == 'Y');

    return 0;
}
