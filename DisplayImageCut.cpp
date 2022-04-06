#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std;
using namespace cv;

/*  This function gets a copy of the object pointer new_image. Since the we
    are only changing the data of new_image, and not the pointer itself,
    we can pass the pointer argument by copy. */
void gamma_correct (Mat image, Mat new_image, float gamma) {
    
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                new_image.at<Vec3b> (y,x)[c] =
                  saturate_cast<uchar> (pow (image.at<Vec3b> (y,x)[c] / 255.0, gamma) * 255);
            }
        }
    }
    return;
}

/*  This function gets a Mat object and splits it horizontally into n parts of type Mat.
    Returns an vector of pointers to the Mat parts. The cuts are achieved using OpenCV
    Rect class, which returns a pointer to the parts' data in the original image--No copying
    is performed */
vector<Mat> cut (Mat image, int n) {
    vector<Mat> parts(n);
    // For a balanced number of height in each part, divide the image rows by the total
    // parts and round to the nearest integer.
    int h_rect = round (image.rows / n);
    // We need to figure out the last rectangle height.
    int l_rect;
    if (image.rows % n == 0)
        l_rect = h_rect;
    else
        l_rect = image.rows - (h_rect * (n - 1));
    
    // Horizontal cuts: all rectanles have the same width.
    int w_rect = image.cols;
    
    // Loop n - 1 times to extract the parts using h_rect height
    for (int i = 0; i < (n - 1); i++) {
        Rect roi (0, i * h_rect, w_rect, h_rect);
        Mat p (image, roi);
        parts [i] = p;
    }
    
    // Get the last part using l_rect height.
    Rect roi (0, (n - 1) * h_rect, w_rect, l_rect);
    Mat p (image, roi);
    parts[n - 1] = p;
    
    return parts;
}

int main (int argc, char** argv) {
    if ( argc < 3 || argc > 4 ) {
        printf("usage: DisplayImage <Image_Path> <gamma_value> [nb_of_processors]\n");
        return -1;
    }
    
    cout << "Image = " << argv[1] << endl;
    
    float gamma = stof (argv[2]);
    cout << "Gamma value = " << gamma << endl;
  
    unsigned logic_processors;
    if (argc == 4)
        logic_processors = stod (argv[3]);
    else
        logic_processors = std::thread::hardware_concurrency();
        
    // start timer
    auto start = high_resolution_clock::now();
    
    Mat image;
    image = imread (argv[1], 1);

    if (!image.data) {
        cout << "No image data \n";
        return -1;
    }
    
    cout << "Image height = " << image.rows << " pixels" << endl;
    cout << "Image width  = " << image.cols << " pixels" << endl;

    // Create a new image from the old so we can keep both
    Mat new_image = Mat::zeros (image.size(), image.type());
    // Cut the new image into parts
    
    cout << "logic_processors = " << logic_processors << endl;
    
    vector<Mat> new_image_parts = cut (new_image, logic_processors);
    // Cut the old image into parts
    vector<Mat> image_parts = cut (image, logic_processors);
    // Gamma correct the individual parts
    for (int i = 0; i < image_parts.size(); i++)
        gamma_correct (image_parts[i], new_image_parts[i], gamma);

    // Reassemble the new_image_parts into new_image using Mat push_back method,
    // which adds a new Mat to the bottom.
    Mat final_image;
    for (int i = 0; i < new_image_parts.size(); i++)
        final_image.push_back (new_image_parts[i]);
    
    // Stop timer and report time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken for gamma correction: " << duration.count() / 1000000.0
         << " seconds" << endl;
    
    // Display the images in a smaller window than full size to make them fit
    // on screen. 
    resize (final_image, final_image, Size(), 0.25, 0.25, INTER_CUBIC);
    resize (image, image, Size(), 0.25, 0.25, INTER_CUBIC);
    
    cout << "Image height after resize = " << final_image.rows << " pixels" << endl;
    cout << "Image width after resize  = " << final_image.cols << " pixels" << endl;
    
    namedWindow("Display New Image", WINDOW_AUTOSIZE );
    imshow("Display New Image", final_image);

    namedWindow ("Display Old Image", WINDOW_AUTOSIZE);
    imshow ("Display Old Image", image);

    waitKey (0);
    return 0;
}
