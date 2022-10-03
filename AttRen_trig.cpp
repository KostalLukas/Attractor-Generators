/*
 * Clifford Attractor Renderer v3.0
 *
 * Lukas Kostal, 25.9.2022, Bratislava
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

int main() {

    //array of coefficients describing the attractor
    double a[] = {-2,-2.4,1.1,-0.9};

    //specify initial points
    double x_i = 0.1;
    double y_i = 0.1;

    //specify number of attractor being generated
    unsigned int num = 10;

    //specify intensity by which to increment each pixel
    int intensity = 2;

    //specify number of iterations
    unsigned int n_iter = 10000000;

    //specify the resolution of generated image
    unsigned int x_res = 1000;
    unsigned int y_res = 1000;

    //define variables for iterating
    double x;
    double y;
    double x_n;
    double y_n;

    //define variables to store min and max values
    float x_min = 0;
    float y_min = 0;
    float x_max = 0;
    float y_max = 0;

    //define vectors to store points making up the attractor
    vector<float> x_vec(n_iter, 0);
    vector<float> y_vec(n_iter, 0);

    //define indices used to increment value of pixels
    unsigned int x_ind;
    unsigned int y_ind;

    //define variable to store value of single pixel
    unsigned int img_val;
    //define vector to store entire image
    vector<vector<uint8_t>> img_vec(x_res, vector<uint8_t> (y_res, 0));

    //define variable to store progress of current process
    unsigned int prog;


    //rector the time at the start of the program
    auto starttim = std::chrono::system_clock::now();

    //set initial coordinates
    x = x_i;
    y = y_i;

    //loop to create the attractor set
    for (int i = 0; i < n_iter; i++) {

        //calculate coordinates of new point from previous coordinates
        x_n = sin(a[0]*y) + a[2]*cos(a[0]*x);
        y_n = sin(a[1]*x) + a[3]*cos(a[1]*y);

        //update the old coordinates to the new coordinates
        x = x_n;
        y = y_n;

        //append the values to the vectors
        x_vec[i] = x;
        y_vec[i] = y;

        //determine the min and max coordinates
        if (x < x_min) {
            x_min = x;
        }
        if (y < y_min) {
            y_min = y;
        }
        if (x > x_max) {
            x_max = x;
        }
        if (y > y_max) {
            y_max = y;
        }

        //write a progress bar
        if ( i%(n_iter/100) == 0) {
            prog = (double)i/(double)n_iter * 100 + 1;
            cout << '\r' << "Iterating: " << prog << '%' << "\t" << "[";

            for(int k = 1; k<=50; k++) {
                if(k <= ceil(prog/2)) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "]";
            cout.flush();
        }

    }

    cout << endl;

    //loop to index the image array and increment its value
    for (int i=0; i < n_iter; i++) {

        //remap the coordinates onto the indices of the image array
        x_ind = floor((x_vec[i] - x_min) * (x_res - 1) / (x_max - x_min));
        y_ind = floor((y_vec[i] - y_min) * (y_res - 1) / (y_max - y_min));

        //increment the image array at given indices unless its saturated
        if (img_vec[x_ind][y_ind] <= 255-intensity) {
            img_vec[x_ind][y_ind] += intensity;
        }

        //write a progress bar
        if ( i%(n_iter/100) == 0) {
            prog = (double)i/(double)n_iter * 100 + 1;
            cout << '\r' << "Indexing:  " << prog << '%' << "\t" << "[";

            for(int k = 1; k<=50; k++) {
                if(k <= ceil(prog/2)) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "]";
            cout.flush();
        }

    }

    cout << endl;

    //create the .ppm image file
    ofstream image;
    image.open((string)"Attractor_" + to_string(num) + ".ppm");

    //specify the properties of the image file to be generated
    image << "P3" << endl;
    image << x_res << " " << y_res << endl;
    image << "255" << endl;

    //double loop to go over every element in the image array and write the image
    for (int i=0; i < x_res; i++) {
        for (int j=0; j < y_res; j++) {

            //convert the image value from unsigned 8bit and write to the .ppm file
            img_val = +img_vec[j][y_res - i];
            image << img_val << " " << img_val << " " << img_val << endl;
        }

        //write a progress bar
        if ( i%(x_res/100) == 0) {
            prog = (double)i/(double)x_res * 100 + 1;
            cout << '\r' << "Writing:   " << prog << '%' << "\t" << "[";

            for(int k = 1; k<=50; k++) {
                if(k <= ceil(prog/2)) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "]";
            cout.flush();
        }

    }

    //close the image
    image.close();

    //attempted to call ImageMagick from C++ to convert .ppm to .tiff
    //system((string) "convert" + "Attractor_" + to_string(num) + ".ppm Attractor_" + to_string(num) + ".tiff");

    //record the time when the program finishes generating and find time taken
    auto endtim = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = endtim - starttim;

    //print out parameters
    cout << endl;
    cout << endl;
    cout << "Finished generating " << (string) "Attractor_" + to_string(num) << endl;
    cout << "Initial conditions: x = " << x_i << " y = " << y_i << endl;
    cout << "Coefficients: ";
    for (int i=0; i < 3; i++) {
        cout << a[i] << ", ";
    }
    cout << a[3] << endl;

    cout << endl;
    cout << "Iterations: " << n_iter << endl;
    cout << "Resolution: " << x_res << " x " << y_res << endl;
    cout << "Intensity: " << intensity << endl;

    cout << endl;
    cout << "Time taken: " << elapsed.count() << "s" << endl;

    return 0;
}
