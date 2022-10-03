'''
Polynomial Attractor Generator v2.0

Lukas Kostal, Bratislava, 29.9.2022
'''

import numpy as np
import matplotlib.pyplot as plt
from PIL import Image as im


# specify numebr of attractors to generate
num = 3

# specify numebr of iterations
n_iter = 50000

# specify resolution of preview image
x_res = 800
y_res = 800

# specify limits for divergence and convergence
div_lim = 1e10
conv_lim = 1e-10

# specify sensitivity and Lyapunov exponent limit
lyapunov_lim = 5000
sens = 500


# loop to generate num attractors
for repeat in range(num):

    done = False

    # loop untill an attractor is generated
    while not done:

        # randlomly generate initial coordinates
        x = np.random.uniform(-1/2, 1/2)
        y = np.random.uniform(-1/2, 1/2)

        # randomly generate coefficients of polynomial
        a = np.random.uniform(-2, 2, 12)

        # randomly generate variations in position
        dx = np.random.uniform(-1, 1) / sens
        dy = np.random.uniform(-1, 1) / sens
        dr = np.sqrt(dx**2 + dy**2)

        xl = x + dx
        yl = y + dy

        # arrays to hold cooridnates
        x_arr = np.array(x)
        y_arr = np.array(y)

        # set value of Lyapunov eponent to 0
        lyapunov = 0

        # set the divergence and convergence to false prior to checking
        divergence = False
        convergence = False

        # loop which generates the attractor
        for i in range(n_iter):

            x_new = np.sin(a[0]*y) + a[2] * np.cos(a[0]*x)
            y_new = np.sin(a[0]*x) + a[3] * np.cos(a[1]*y)

            # check for divergence
            if x_new > div_lim or y_new > div_lim or x_new < -div_lim or y_new < -div_lim:
                divergence = True
                break

            # check for convergence
            if np.absolute(x - x_new) < conv_lim and np.absolute(y - y_new) < conv_lim:
                convergence = True
                break

            # after 1000 iterations calcualte Lyapunov exponent of the attractor
            if i > 1000:
                xl_new = a[0] + a[1]*xl + a[2]*xl**2 + a[3]*yl + a[4]*yl**2 + a[5]*xl*yl
                yl_new = a[6] + a[7]*xl + a[8]*xl**2 + a[9]*yl + a[10]*yl**2 + a[11]*xl*yl

                dx = xl_new - x_new
                dy = yl_new - y_new
                dr_new = np.sqrt(dx**2 + dy**2)

                lyapunov += np.log(np.absolute(dr_new/dr))

                xl = x_new + dx * dr/dr_new
                yl = y_new + dy * dr/dr_new

            # update the coordinates to the ones generated previously
            x = x_new
            y = y_new

            # append the generated coordinates to the arrays
            x_arr = np.append(x_arr, x_new)
            y_arr = np.append(y_arr, y_new)

        # check that attractor doesnt diverge or converge and has a sufficient Lyapunov exonent
        if divergence == False and convergence == False:
            if lyapunov > lyapunov_lim:
                # set done to show generated attractor and start generating next one
                done = True

    # print out coefficients of polynomial and initial coordinates
    print("-------------------------------------------------------------------------------------")
    print("Attractor ", repeat+1)
    print()
    print("%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f" % (a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11]))
    print()
    print("%f" % x_arr[0])
    print("%f" % y_arr[0])
    print()
    print("-------------------------------------------------------------------------------------")


    # offset the coordinates to start at origin
    x_img = x_arr - np.amin(x_arr)
    y_img = y_arr - np.amin(y_arr)

    # rescale the coordinates onto the specified resolution
    x_img = x_img * (x_res -1)/np.amax(x_img)
    y_img = y_img * (y_res -1)/np.amax(y_img)

    # convert all image array elements into integers
    x_img = x_img.astype(int)
    y_img = y_img.astype(int)

    # create 2D array which will hold the image
    img_arr = np.zeros((x_res, y_res))
    # index the pixels in the image array
    img_arr[x_img, y_img] = 225
    # rotate the image array
    img_arr = np.rot90(img_arr)
    # convert all image array elements into unsigned 8 bit integers
    img_arr = img_arr.astype(np.uint8)

    #generate an image from the array and display it
    img = im.fromarray(img_arr, 'L')
    img.show()
