#include <iostream>
#include <cmath>

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

class Motor {
public:
    float theta; // Sudut roda
    int speed;   // Kecepatan motor

    Motor(float theta) : theta(theta), speed(0) {} // Constructor

    void calculateSpeed(float wheel_matrix[3], int speed_vector[3]) {
        speed = 0;
        for (int j = 0; j < 3; j++) {
            speed += wheel_matrix[j] * speed_vector[j];
        }
    }

    void displaySpeed(int motorIndex) const {
        cout << "Motor " << motorIndex << " (Theta = " << theta << " derajat):" << endl;
        cout << MAGENTA << "V_" << motorIndex << " = cos(" << theta << ") * X + sin(" << theta << ") * Y + Z" << RESET << endl;
        cout << "Kecepatan motor " << motorIndex << " : " << speed << endl << endl;
    }
};

class OmniRobot {
private:
    Motor motors[4]; // Array dari objek Motor
    float wheel_matrix[4][3]; // Matriks yang merepresentasikan orientasi roda

public:
    OmniRobot() : motors{Motor(45.0), Motor(135.0), Motor(225.0), Motor(315.0)} {
        // Menginisialisasi matriks orientasi roda omni
        for (int i = 0; i < 4; i++) {
            wheel_matrix[i][0] = cos(motors[i].theta * RAD);
            wheel_matrix[i][1] = sin(motors[i].theta * RAD);
            wheel_matrix[i][2] = 1; // Faktor konstanta
        }
    }

    void calculateMotorSpeed(int speed_vector[3]) {
        for (int i = 0; i < 4; i++) {
            motors[i].calculateSpeed(wheel_matrix[i], speed_vector);
        }
    }

    void displayMotorSpeeds() const {
        cout << GREEN << "Hasil perhitungan kecepatan motor:" << RESET << endl;
        for (int i = 0; i < 4; i++) {
            motors[i].displaySpeed(i);
        }
    }

    void displayFormula() const {
        cout << BLUE << "Rumus Inverse Kinematics:" << RESET << endl;
        cout << "V_i = cos(theta_i) * x + sin(theta_i) * y + z" << endl;
        cout << "Di mana theta_i adalah sudut roda ke-i dalam derajat." << endl << endl;
    }
};

class UserInterface {
public:
    void welcomeMessage() const {
        cout << CYAN << "Selamat datang di KinematicsOmniBot!" << RESET << endl;
        cout << "Disini kamu bisa memasukkan kecepatan translasi X, Y, dan rotasi Z." << endl;
        cout << "Program ini akan memberikan hasil terkait kecepatan setiap motor robot." << endl << endl;
    }

    void getInput(int input_speed[3]) const {
        cout << "Masukkan kecepatan translasi X: ";
        cin >> input_speed[0];
        cout << "Masukkan kecepatan translasi Y: ";
        cin >> input_speed[1];
        cout << "Masukkan kecepatan rotasi Z: ";
        cin >> input_speed[2];
    }
};

int main() {
    UserInterface ui;
    OmniRobot robot;
    int input_speed[3];

    // Pesan sambutan
    ui.welcomeMessage();

    // Tampilkan rumus yang digunakan
    robot.displayFormula();

    // Input kecepatan translasi X, Y, dan rotasi Z
    ui.getInput(input_speed);

    // Hitung kecepatan motor berdasarkan input kecepatan
    robot.calculateMotorSpeed(input_speed);

    // Tampilkan hasil kecepatan motor dan sudut
    robot.displayMotorSpeeds();

    return 0;
}
