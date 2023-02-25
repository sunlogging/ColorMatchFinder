#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <vector>
class Logging {
public:
    static void info(const std::string& photo_color, const std::string& photo_bw, const std::vector<double>& similarity_percentages) {
        const int num_columns = similarity_percentages.size() + 2;
        const int num_rows = 1;
        const int column_width = 25;
        const std::string row_delimiter = "|";

        std::string data[num_rows][2] = {
            {photo_color, photo_bw}
        };

        setColor(GREEN);
        std::cout << "INFO: ";
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_columns; j++) {
                if (j == 0 || j == 1) {
                    std::cout << row_delimiter << std::setw(column_width) << std::left << data[i][j];
                }
                else {
                    int index = j - 2;
                    std::cout << row_delimiter << std::setw(3) << std::left << std::to_string((int)similarity_percentages[index]) << "%";
                }
            }
        }
        resetColor(); // Перенесли resetColor() за внешний цикл
        std::cout << std::endl; // Добавляем перенос строки
    }


    static void info(const std::string& message) {
        setColor(GREEN);
        std::cout << "INFO: " << message << std::endl;
        resetColor();
    }

    static void warn(const std::string& message) {
        setColor(YELLOW);
        std::cout << "WARNING: " << message << std::endl;
        resetColor();
    }

    static void critical(const std::string& message) {
        setColor(RED);
        std::cout << "CRITICAL: " << message << std::endl;
        resetColor();
    }

    static void error(const std::string& message) {
        setColor(DARK_RED);
        std::cout << "ERROR: " << message << std::endl;
        resetColor();
    }

private:
    enum ConsoleColor {
        BLACK,
        DARK_BLUE,
        DARK_GREEN,
        DARK_CYAN,
        DARK_RED,
        DARK_MAGENTA,
        DARK_YELLOW,
        GRAY,
        DARK_GRAY,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        YELLOW,
        WHITE
    };

    static void setColor(ConsoleColor color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
    }

    static void resetColor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(GRAY));
    }
};