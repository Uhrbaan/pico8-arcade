#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <print>
#include <memory>

#include "rpi-rgb-led-matrix/include/led-matrix.h"

using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;

int main()
{
    RGBMatrix::Options options;
    options.rows = 64;
    options.cols = 64;
    options.chain_length = 4;
    options.parallel = 1;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = true;
    options.brightness = 100;

    rgb_matrix::RuntimeOptions runtime_options;
    runtime_options.gpio_slowdown = 3;

    std::unique_ptr<RGBMatrix> matrix{RGBMatrix::CreateFromOptions(options, runtime_options)};
    if (!matrix)
    {
        std::println(stderr, "Error: Failed to initialize LED Matrix.");
        return 1;
    }

    rgb_matrix::FrameCanvas *canvas = matrix->CreateFrameCanvas();

    // Connect to the X11 virtual framebuffer
    Display *display = XOpenDisplay(":99");
    if (!display)
    {
        fprintf(stderr, "Error: Cannot connect to Xvfb display :99\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    int xvfb_w = gwa.width;
    int xvfb_h = gwa.height;
    int matrix_w = matrix->width();
    int matrix_h = matrix->height();
    printf("[Success] Connected to Xvfb! Resolution: %dx%d\n", xvfb_w, xvfb_h);

    unsigned long frame_count = 0;
    while (true)
    {
        // Get image from virtual framebuffer
        XImage *image = XGetImage(display, root, 0, 0, xvfb_w, xvfb_h, AllPlanes, ZPixmap);

        if (image != nullptr)
        {
            // Since we connect the matrices sequentially, we have to map the screen to a long rectangle
            for (int y = 0; y < matrix_h; ++y)
            {
                for (int x = 0; x < matrix_w; ++x)
                {
                    int source_x = 0;
                    int source_y = 0;

                    if (x > 128)
                    {
                        source_y = 0 + y;
                        source_x = x - 0;
                    }
                    else
                    {
                        source_y = 64 + y;
                        source_x = x - 128;
                    }

                    unsigned long pixel = XGetPixel(image, source_x, source_y);

                    uint8_t r = (pixel & image->red_mask) >> 16;
                    uint8_t g = (pixel & image->green_mask) >> 8;
                    uint8_t b = (pixel & image->blue_mask);

                    canvas->SetPixel(x, y, r, g, b);
                }
            }

            canvas = matrix->SwapOnVSync(canvas);
            XDestroyImage(image);
        }
        usleep(16666);
    }

    XCloseDisplay(display);
    return 0;
}