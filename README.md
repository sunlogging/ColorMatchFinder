# ColorMatchFinder
This is a C++ program that performs image processing on a set of images. It takes in a folder name as input and searches for all .jpg files in the folder. It then searches for black and white versions of these images (by searching for .jpg files in another folder) and compares the color and black and white images to determine if they are a match.

If a match is found, the program creates a new folder with the name of the input folder and saves the color image and a corresponding .eps file to the new folder. The original color image is also deleted from the original folder.

The program uses OpenCV for image processing and OpenMP for parallelization. It also uses std::filesystem for file system operations.

The program takes in two thresholds, threshold_1 and threshold_2, which are used to determine if a color and black and white image match. If the correlation coefficient between the images is greater than or equal to threshold_1 and the squared difference between the images is less than or equal to threshold_2, then the images are considered a match.
