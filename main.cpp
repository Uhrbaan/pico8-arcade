#include "rpi-rgb-led-matrix/include/led-matrix.h"
#include <fcntl.h>
#include <iostream>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

// Match your hardware layout
#define PICO_WIDTH 128
#define PICO_HEIGHT 128

int main(int argc, char *argv[])
{
  // 1. Initialize RGB Matrix Options
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

  RGBMatrix *matrix = RGBMatrix::CreateFromOptions(options, runtime_options);
  if (matrix == nullptr)
  {
    std::cerr << "Failed to initialize LED Matrix." << std::endl;
    return 1;
  }

  rgb_matrix::FrameCanvas *canvas = matrix->CreateFrameCanvas();

  // 2. Open Linux Framebuffer
  int fb_fd = open("/dev/fb0", O_RDONLY);
  if (fb_fd < 0)
  {
    std::cerr << "Error: Cannot open framebuffer device /dev/fb0. Run as sudo?"
              << std::endl;
    return 1;
  }

  // Get variable screen information (to map screen dimensions correctly)
  struct fb_var_screeninfo vinfo;
  if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo))
  {
    std::cerr << "Error reading variable information." << std::endl;
    close(fb_fd);
    return 1;
  }

  // Calculate memory mapping sizing
  long screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
  uint32_t *fbp =
      (uint32_t *)mmap(0, screensize, PROT_READ, MAP_SHARED, fb_fd, 0);
  if ((long)fbp == -1)
  {
    std::cerr << "Error: Failed to mmap framebuffer." << std::endl;
    close(fb_fd);
    return 1;
  }

  std::cout << "Bridge started successfully. Press Ctrl+C to exit."
            << std::endl;

  // 3. Main Loop: Blit framebuffer to LED Matrix
  while (true)
  {
    // PICO-8 outputs 128x128. Let's capture that region from the top-left of
    // /dev/fb0
    for (int y = 0; y < PICO_HEIGHT; ++y)
    {
      for (int x = 0; x < PICO_WIDTH; ++x)
      {
        // Get pixel from framebuffer (assuming 32-bit depth ARGB/XRGB)
        uint32_t pixel = fbp[y * vinfo.xres + x];
        uint8_t r = (pixel >> 16) & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t b = pixel & 0xFF;

        // // Software Contrast Enhancement (Roughly equivalent to Python's
        // // Enhance) Adjust factor (3.0) as needed
        // auto enhance = [](uint8_t val) -> uint8_t
        // {
        //   int dynamic = static_cast<int>((val - 128) * 3.0 + 128);
        //   return (dynamic > 255) ? 255 : ((dynamic < 0) ? 0 : dynamic);
        // };

        // r = enhance(r);
        // g = enhance(g);
        // b = enhance(b);

        // Rearrange layout logic:
        // Upper box (0-128 wide, 0-64 high) -> drawn at Canvas X=128
        // Lower box (0-128 wide, 64-128 high) -> drawn at Canvas X=0
        if (y < 64)
        {
          canvas->SetPixel(x + 128, y, r, g, b);
        }
        else
        {
          canvas->SetPixel(x, y - 64, r, g, b);
        }
      }
    }

    // Swap buffer onto the matrix natively
    canvas = matrix->SwapOnVSync(canvas);

    // Minor delay to keep CPU from spinning at 100% thread utility
    // 16ms roughly targets ~60 FPS matching PICO-8's inner engine
    usleep(16000);
  }

  // Cleanup (unreachable here, but proper practice)
  munmap(fbp, screensize);
  close(fb_fd);
  delete matrix;
  return 0;
}
