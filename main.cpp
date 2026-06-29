#include "rpi-rgb-led-matrix/include/led-matrix.h"
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <print> // Modern C++23 printing
#include <memory>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

constexpr int PICO_WIDTH = 128;
constexpr int PICO_HEIGHT = 128;

// RAII structural wrapper to automatically clean up system resources
struct Framebuffer
{
  int fd = -1;
  uint32_t *mem = nullptr;
  size_t size = 0;
  fb_var_screeninfo vinfo{};

  ~Framebuffer()
  {
    if (mem && mem != MAP_FAILED)
      munmap(mem, size);
    if (fd >= 0)
      close(fd);
  }
};

int main()
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

  std::unique_ptr<RGBMatrix> matrix{RGBMatrix::CreateFromOptions(options, runtime_options)};
  if (!matrix)
  {
    std::println(stderr, "Error: Failed to initialize LED Matrix.");
    return 1;
  }

  rgb_matrix::FrameCanvas *canvas = matrix->CreateFrameCanvas();
  Framebuffer fb;

  // 2. Open & Map Linux Framebuffer
  fb.fd = open("/dev/fb0", O_RDONLY);
  if (fb.fd < 0)
  {
    std::println(stderr, "Error: Cannot open framebuffer device /dev/fb0. Run as sudo?");
    return 1;
  }

  if (ioctl(fb.fd, FBIOGET_VSCREENINFO, &fb.vinfo))
  {
    std::println(stderr, "Error: Failed to read variable screen information.");
    return 1;
  }

  fb.size = fb.vinfo.xres * fb.vinfo.yres * fb.vinfo.bits_per_pixel / 8;
  fb.mem = static_cast<uint32_t *>(mmap(nullptr, fb.size, PROT_READ, MAP_SHARED, fb.fd, 0));

  if (fb.mem == MAP_FAILED)
  {
    std::println(stderr, "Error: Failed to mmap framebuffer.");
    return 1;
  }

  std::println("Bridge started successfully. Press Ctrl+C to exit.");

  // 3. Main Loop: Blit framebuffer to LED Matrix
  while (true)
  {
    for (int y = 0; y < PICO_HEIGHT; ++y)
    {
      for (int x = 0; x < PICO_WIDTH; ++x)
      {
        // Read 32-bit pixel directly from memory map
        uint32_t pixel = fb.mem[y * fb.vinfo.xres + x];

        uint8_t r = (pixel >> 16) & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t b = pixel & 0xFF;

        // Split 128x128 PICO-8 frame across the matrix physical dimensions
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

    canvas = matrix->SwapOnVSync(canvas);
    usleep(16000); // Target ~60 FPS
  }

  return 0;
}