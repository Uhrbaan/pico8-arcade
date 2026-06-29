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
    options.brightness = 60;

    rgb_matrix::RuntimeOptions runtime_options;
    runtime_options.gpio_slowdown = 3;

    std::unique_ptr<RGBMatrix> matrix{RGBMatrix::CreateFromOptions(options, runtime_options)};
    if (!matrix)
    {
        std::println(stderr, "Error: Failed to initialize LED Matrix.");
        return 1;
    }

    rgb_matrix::FrameCanvas *canvas = matrix->CreateFrameCanvas();

    // 1. Connect to the Xvfb display server.
    // Ensure you change ":99" if your Xvfb is running on a different display number.
    Display *display = XOpenDisplay(":99");
    if (!display)
    {
        fprintf(stderr, "Error: Cannot connect to Xvfb display :99\n");
        return 1;
    }

    // 2. Get the root window ID and its attributes (dimensions)
    Window root = DefaultRootWindow(display);
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    printf("[Success] Connected to Xvfb! Resolution: %dx%d\n", gwa.width, gwa.height);
    printf("Starting loop. Press Ctrl+C to stop.\n");

    unsigned long frame_count = 0;

    while (true)
    {
        // Pull raw window pixels directly from Xvfb memory
        XImage *image = XGetImage(display, root, 0, 0, 128, 128, AllPlanes, ZPixmap);

        if (image != nullptr)
        {
            // Map Xvfb pixels to Matrix pins.
            // If resolutions match perfectly (e.g. 128x128 to a 128x128 matrix wall), 1:1 mapping applies.
            // If they differ, downscale here.
            for (int y = 0; y < 128; ++y)
            {
                for (int x = 0; x < 128; ++x)
                {
                    // Map local iteration safely to Xvfb boundaries
                    int source_x = x;
                    int source_y = y;

                    unsigned long pixel = XGetPixel(image, source_x, source_y);

                    // Deconstruct X11 native hex colors into RGB bytes
                    uint8_t r = (pixel & image->red_mask) >> 16;
                    uint8_t g = (pixel & image->green_mask) >> 8;
                    uint8_t b = (pixel & image->blue_mask);

                    // Push to our hardware offscreen canvas
                    canvas->SetPixel(x, y, r, g, b);
                }
            }

            // Instantly swap the memory chunk over the GPIO pins at hardware VSync
            canvas = matrix->SwapOnVSync(canvas);

            // CRITICAL: Immediately clear out the XImage memory allocation loop context
            XDestroyImage(image);
        }

        // Modest sleep parameter (~60fps) to keep the host CPU entirely cool
        usleep(16666);
    }

    XCloseDisplay(display);
    return 0;
}