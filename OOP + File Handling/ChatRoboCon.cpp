#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#ifdef _WIN32
    #include <windows.h>  // Library untuk fungsi Sleep di Windows
#else
    #include <unistd.h>   // Library untuk fungsi sleep di sistem Unix
#endif

// Kelas untuk memuat pertanyaan dan jawaban dari file serta menyimpan pertanyaan baru ke file
class QuestionLoader {
public:
    // Method untuk memuat pertanyaan dan jawaban yang sudah dipelajari dari file
    static void loadQuestionsFromFile(const std::string& filename, std::unordered_map<std::string, std::string>& learned_questions_and_answers) {
        std::ifstream file(filename);
        // Jika file tidak ada, tampilkan pesan bahwa file belum ada
        if (!file) {
            std::cout << "File belum ada atau tidak dapat dibuka. Mulai dari awal." << std::endl;
            return;
        }
        std::string question, answer;
        // Membaca setiap baris pertanyaan dan jawaban dari file
        while (std::getline(file, question) && std::getline(file, answer)) {
            if (!question.empty() && !answer.empty()) {
                learned_questions_and_answers[toLowerCase(question)] = answer;  // Simpan ke dalam map dengan key lowercase
            }
        }
        file.close();  // Tutup file setelah selesai membaca
    }

    // Method untuk menyimpan pertanyaan dan jawaban yang baru dipelajari ke file
    static void saveQuestionToFile(const std::string& filename, const std::string& question, const std::string& answer) {
        std::ofstream file(filename, std::ios::app);  // Buka file dan tambahkan data di akhir
        if (file.is_open()) {
            file << question << std::endl;  // Simpan pertanyaan
            file << answer << std::endl;    // Simpan jawaban
            file.close();  // Tutup file setelah selesai menulis
        }
    }

    // Helper function untuk mengubah string menjadi huruf kecil (lowercase)
    static std::string toLowerCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);  // Ubah setiap karakter menjadi lowercase
        return result;
    }

    // Helper function untuk memeriksa apakah input mengandung kata kunci tertentu
    static bool containsKeyword(const std::string& input, const std::string& question) {
        return input.find(question) != std::string::npos;  // Cek apakah kata kunci ditemukan dalam input
    }
};

// Kelas untuk mencocokkan beberapa kata kunci dalam satu input
class KeywordMatcher {
public:
    // Method untuk memeriksa apakah input mengandung salah satu dari sekumpulan kata kunci
    static bool containsKeyword(const std::string& text, const std::vector<std::string>& keywords) {
        for (const auto& keyword : keywords) {
            if (text.find(keyword) != std::string::npos) {  // Jika ditemukan kata kunci, return true
                return true;
            }
        }
        return false;  // Jika tidak ditemukan kata kunci, return false
    }
};

// Kelas utama bot yang menangani pertanyaan pengguna dan jawaban
class QuestionAnsweringBot {
private:
    std::string name;  // Nama bot
    std::unordered_map<std::string, std::string> learned_questions_and_answers;  // Map untuk menyimpan pertanyaan dan jawaban yang dipelajari
    std::string filename;  // Nama file tempat menyimpan pertanyaan dan jawaban

public:
    // Konstruktor bot, diinisialisasi dengan nama bot dan file penyimpanan
    QuestionAnsweringBot(const std::string& bot_name, const std::string& file) : name(bot_name), filename(file) {
        std::cout << "Halo, namaku adalah " << name << "!" << std::endl;
        std::cout << "Tanya pertanyaan. Ketik 'exit' untuk keluar." << std::endl;
        QuestionLoader::loadQuestionsFromFile(filename, learned_questions_and_answers);  // Muat pertanyaan yang dipelajari dari file
    }

    // Method untuk menjawab pertanyaan pengguna
    std::string getAnswer(const std::string& question) {
        std::string q_lower = QuestionLoader::toLowerCase(question);  // Ubah input pengguna menjadi lowercase

        // Cek apakah pertanyaan sudah dipelajari sebelumnya
        for (const auto& entry : learned_questions_and_answers) {
            if (QuestionLoader::containsKeyword(q_lower, entry.first)) {
                return entry.second;  // Jika ditemukan kecocokan, berikan jawaban yang sesuai
            }
        }

        // Cek kecocokan berdasarkan keyword yang sudah ditentukan
        if (KeywordMatcher::containsKeyword(q_lower, {"nama mu", "kamu siapa"})) {
            return "Nama saya " + name + ".";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"apa kabar", "kabar"})) {
            return "Saya baik, terima kasih! Bagaimana denganmu?";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"oop", "object oriented"})) {
            return "OOP adalah Object Oriented Programming, sebuah paradigma pemrograman.";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"presiden indonesia", "siapa presiden"})) {
            return "Presiden Indonesia saat ini adalah Joko Widodo.";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"ibukota indonesia", "ibukota"})) {
            return "Ibukota Indonesia adalah Jakarta.";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"berikan saya lelucon", "lelucon"})) {
            return "Mengapa sepeda tidak bisa berdiri sendiri? Karena dia hanya punya dua roda!";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"siapa penemu robot", "penemu robot"})) {
            return "Penemu robot adalah Al-Jazari.";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"apa warna langit", "warna langit"})) {
            return "Warna langit biasanya biru pada siang hari.";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"its adalah", "taukah kamu its"})) {
            // Jawaban bertahap dengan penundaan
            std::cout << "Bot: ITS adalah Institut Teknologi terbaik di Indonesia..." << std::endl;
            #ifdef _WIN32
                Sleep(2000);  // Untuk Windows
            #else
                sleep(2);  // Untuk Unix
            #endif
            return "Setelah ITB :)";
        } else if (KeywordMatcher::containsKeyword(q_lower, {"its robocon", "taukah kamu its robocon"})) {
            return "ITS Robocon Team adalah tim riset robotika tertua, terkeren, terketche, terhebat, terganteng, tercanggih, terinovatif, terkuat, terjago, ter2 di ITS.";
        } else {
            return "Maaf, saya tidak tahu jawabannya.";
        }
    }

    // Method untuk belajar pertanyaan baru dari pengguna
    void learnNewQuestion(const std::string& question, const std::string& answer) {
        learned_questions_and_answers[QuestionLoader::toLowerCase(question)] = answer;  // Simpan ke map
        QuestionLoader::saveQuestionToFile(filename, question, answer);  // Simpan juga ke file
    }
};

// Program utama
int main() {
    QuestionAnsweringBot bot("ChatRoboCon", "learned_questions.txt");  // Inisialisasi bot dengan nama dan file

    while (true) {
        std::string question;
        std::cout << "Kamu: ";
        std::getline(std::cin, question);  // Baca input dari pengguna

        // Jika pengguna mengetik 'exit', keluar dari loop
        if (question == "exit") {
            std::cout << "Bot: Terima kasih! Sampai jumpa!" << std::endl;
            break;
        }

        // Cek apakah input kosong
        if (question.empty()) {
            std::cout << "Input tidak boleh kosong. Silakan coba lagi." << std::endl;
            continue;
        }

        // Dapatkan jawaban dari bot
        std::string answer = bot.getAnswer(question);
        std::cout << "Bot: " << answer << std::endl;

        // Jika bot tidak tahu jawabannya, minta pengguna untuk mengajarkan jawaban baru
        if (answer == "Maaf, saya tidak tahu jawabannya.") {
            std::cout << "Ajari saya jawaban untuk pertanyaan ini: ";
            std::string new_answer;
            std::getline(std::cin, new_answer);

            // Cek apakah jawaban yang diberikan valid
            if (!new_answer.empty()) {
                bot.learnNewQuestion(question, new_answer);
                std::cout << "Terima kasih! Saya sudah belajar jawaban baru." << std::endl;
            } else {
                std::cout << "Jawaban tidak boleh kosong." << std::endl;
            }
        }
    }

    return 0;
}
