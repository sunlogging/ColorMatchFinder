# ColorMatchFinder

> __How to start?__

`Download Visual Studio`

`Download OpenCV use vcpkg`

`Additional Include Libraries`   - \vcpkg-master\vcpkg-master\packages\opencv4_x64-windows\include

`Additional include directories` - \vcpkg-master\vcpkg-master\packages\opencv4_x64-windows\lib

`Additional dependencies` - opencv_calib3d4.lib;opencv_core4.lib;opencv_dnn4.lib;opencv_features2d4.lib;opencv_flann4.lib;opencv_highgui4.lib;opencv_imgcodecs4.lib;opencv_imgproc4.lib;opencv_ml4.lib;opencv_objdetect4.lib;opencv_photo4.lib;opencv_stitching4.lib;opencv_video4.lib;opencv_videoio4.lib;%(AdditionalDependencies)

`Open Visual Studio Developer Command Prompt and write msbuild /t:Build /p:Configuration=Release`

> __What does this script do?__

This is a C++ program that performs image processing on a set of images. It takes in a folder name as input and searches for all .jpg files in the folder. It then searches for black and white versions of these images (by searching for .jpg files in another folder) and compares the color and black and white images to determine if they are a match.

If a match is found, the program creates a new folder with the name of the input folder and saves the color image and a corresponding .eps file to the new folder. The original color image is also deleted from the original folder.

The program uses OpenCV for image processing and OpenMP for parallelization. It also uses std::filesystem for file system operations.

The program takes in two thresholds, threshold_1 and threshold_2, which are used to determine if a color and black and white image match. If the correlation coefficient between the images is greater than or equal to threshold_1 and the squared difference between the images is less than or equal to threshold_2, then the images are considered a match.

> __How can you optimize or rewrite the code:__

You can use an algorithm to compare color and black and white images to determine how well an image is suitable for printing in black and white.
You can use an algorithm to detect duplicate images in different folders or on different devices.
