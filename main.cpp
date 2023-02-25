#include "logging.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <future>
#include <algorithm>
#include <fstream>
#include <memory>
#include <omp.h>
#include <numeric>

#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

std::mutex mutex;

int threshold_1;
int threshold_2;

std::vector<std::string> glob(std::string folder_path, std::string extension)
{
    std::vector<std::string> files;
    std::vector<fs::directory_entry> entries;

    std::copy_if(fs::directory_iterator(folder_path), fs::directory_iterator(),
        std::back_inserter(entries), [](const auto& entry) {
            return entry.is_regular_file();
        });

#pragma omp parallel for
    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        std::string file_extension = entries[i].path().extension().string();
        if (file_extension == extension)
        {
#pragma omp critical
            {
                files.push_back(entries[i].path().string());
            }
        }
    }

    return files;
}

std::vector<std::string> findDiff(std::vector<std::string> x, std::vector<std::string> y) {

    std::vector<std::string> diff;
    std::sort(x.begin(), x.end());
    std::sort(y.begin(), y.end());
    std::set_difference(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(diff));

    return diff;
}

std::vector<std::string> getNames(std::vector<std::string> vec) {

    for (int index = 0; index < vec.size(); index++) {
        std::size_t pos = vec[index].find_last_of("\\/");
        vec[index] = vec[index].substr(pos + 1);
    }

    return vec;
}


bool is_matching(const std::string& photo_color, const std::string& photo_bw)
{
    cv::Mat color_img, bw_img, gray_img, result_ccoeff, result_sqdiff;
    double similarity_percentage_ccoeff, similarity_percentage_sqdiff;

    color_img = cv::imread(photo_color, cv::IMREAD_UNCHANGED);
    bw_img = cv::imread(photo_bw, cv::IMREAD_GRAYSCALE);

    if (!color_img.empty() && !bw_img.empty()) {

        cv::resize(bw_img, bw_img, cv::Size(1000, 1000), 0, 0, cv::INTER_NEAREST);
        cv::resize(color_img, color_img, cv::Size(1000, 1000), 0, 0, cv::INTER_NEAREST);

        cv::cvtColor(color_img, gray_img, cv::COLOR_BGR2GRAY);

        cv::matchTemplate(gray_img, bw_img, result_ccoeff, cv::TM_CCOEFF_NORMED);
        double ssim_ccoeff = result_ccoeff.at<float>(0, 0);
        cv::matchTemplate(gray_img, bw_img, result_sqdiff, cv::TM_SQDIFF_NORMED);
        double ssim_sqdiff = result_sqdiff.at<float>(0, 0);

        similarity_percentage_ccoeff = ssim_ccoeff * 100;
        similarity_percentage_sqdiff = ssim_sqdiff * 100;

        mutex.lock();
        Logging::info(photo_color, photo_bw, std::vector <double> { similarity_percentage_ccoeff, similarity_percentage_sqdiff});
        mutex.unlock();

        return similarity_percentage_ccoeff >= threshold_1 && similarity_percentage_sqdiff <= threshold_2;
    }
    return false;
}



void copy_files(std::vector<std::string>& c_photos, const std::string& c_photo, const std::string& wb_photo, const std::string& name_folder) {
    if (!is_matching(c_photo, wb_photo)) return;

    try {
        std::string name_save_files = wb_photo.substr(wb_photo.find_last_of("\\/") + 1);
        std::vector<std::future<void>> futures;
        futures.push_back(std::async(std::launch::async, [&]() {
            std::filesystem::copy(c_photo.substr(0, c_photo.find_last_of(".")) + ".eps", name_folder + "\\" + name_save_files.substr(0, name_save_files.find_last_of(".")) + ".eps", std::filesystem::copy_options::overwrite_existing);
            }));
        futures.push_back(std::async(std::launch::async, [&]() {
            std::filesystem::copy(c_photo, name_folder + "\\" + name_save_files, std::filesystem::copy_options::overwrite_existing);
            }));
        futures.push_back(std::async(std::launch::async, [&]() {
            c_photos.erase(std::remove(c_photos.begin(), c_photos.end(), c_photo), c_photos.end());
            }));
        for (auto& f : futures) {
            f.wait();
        }
    }
    catch (const std::exception& e) {
        mutex.lock();
        Logging::critical("Error copying files: " + std::string(e.what()));
        mutex.unlock();
    }

}

int main(int argc, char* argv[])
{
    Logging::info("Started");

    std::cout << "Input folder name: ";
    std::string name_folder;
    std::cin >> name_folder;

    std::cout << "threshold_1: ";
    std::cin >> threshold_1;

    std::cout << "threshold_2: ";
    std::cin >> threshold_2;

    system("pause");

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::string> photos_color = glob(".\\1", ".jpg");
    std::vector<std::string> photos_wb = glob(".\\2", ".jpg");

    std::vector<std::future<void>> futures;

    for (const std::string& photo_wb : photos_wb) {
        for (const std::string& photo_color : photos_color) {
            futures.push_back(std::async(std::launch::async, copy_files, std::ref(photos_color), photo_color, photo_wb, name_folder));
        }
        std::for_each(futures.begin(), futures.end(), [](std::future<void>& f) { f.wait(); });
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Elapsed time: " << duration_ms.count() << " ms" << std::endl;
    system("pause");

    std::vector<std::string> photos_notworked = getNames(glob(".\\2", ".jpg"));
    std::vector<std::string> photos_worked = getNames(glob(name_folder, ".jpg"));

    Logging::info("Checking Vector Sizes...");

    std::vector<std::string> not_processed_files;

    for (const auto& file_2 : photos_notworked) {
        if (std::find(photos_worked.begin(), photos_worked.end(), file_2) == photos_worked.end()) {
            not_processed_files.push_back(file_2);
        }
    }

    for (const auto& file : not_processed_files) {
        Logging::warn(file);
    }
    
    system("pause");
    return 0;
}

