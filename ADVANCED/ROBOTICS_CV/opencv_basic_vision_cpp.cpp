// ADVANCED ROBOTICS_CV: Computer Vision Concepts Without OpenCV
//
// HONEST FRAMING:
// OpenCV for C++ on Windows requires a non-trivial build (vcpkg or pre-built).
// This file demonstrates the core CV algorithms in pure C++ so you understand
// what OpenCV does under the hood.
//
// If you install OpenCV via vcpkg:
//   vcpkg install opencv4 --triplet x64-mingw-static
// Then compile with:
//   g++ -std=c++17 -o opencv_demo.exe opencv_basic_vision_cpp.cpp $(pkg-config --cflags --libs opencv4)
// And enable the OpenCV version by defining USE_OPENCV.
//
// CONCEPTS (pure C++ implementations):
// 1. Grayscale conversion (luminosity method)
// 2. Gaussian blur (2D convolution with Gaussian kernel)
// 3. Sobel edge detection (gradient magnitude)
// 4. Simple threshold / binarization
// 5. Connected components concept (flood fill)
// 6. Histogram computation

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <array>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <string>
#include <sstream>

// ============================================================
// Simple image representation
// ============================================================
struct Image {
    int width, height;
    std::vector<uint8_t> data;

    Image(int w, int h) : width(w), height(h), data(w * h, 0) {}
    Image(int w, int h, uint8_t fill) : width(w), height(h), data(w * h, fill) {}

    uint8_t& at(int y, int x)       { return data[y * width + x]; }
    uint8_t  at(int y, int x) const { return data[y * width + x]; }

    uint8_t get_clamped(int y, int x) const {
        x = std::clamp(x, 0, width  - 1);
        y = std::clamp(y, 0, height - 1);
        return data[y * width + x];
    }
};

// ============================================================
// 1. Grayscale conversion
// ============================================================
// Real images have R,G,B per pixel. We simulate with a synthetic test image.
Image make_test_image(int w, int h) {
    Image img(w, h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // Gradient + circular pattern to have edges
            float cx = static_cast<float>(x - w/2), cy = static_cast<float>(y - h/2);
            float dist = std::sqrt(cx*cx + cy*cy);
            float val  = 128.0f + 100.0f * std::sin(dist / 5.0f);
            img.at(y, x) = static_cast<uint8_t>(std::clamp(val, 0.0f, 255.0f));
        }
    }
    return img;
}

// Luminosity grayscale: 0.299R + 0.587G + 0.114B
// (We already work in grayscale, but the formula is the important part)
void demo_grayscale() {
    std::cout << "\n=== 1. Grayscale Conversion ===\n";
    std::cout << "RGB -> Gray formula (BT.601 luminosity):\n";
    std::cout << "  Gray = 0.299*R + 0.587*G + 0.114*B\n";
    std::cout << "  Green has highest weight because the human eye is most sensitive to it.\n\n";

    // Demo: some example RGB -> Gray conversions
    struct RGB { int r, g, b; };
    std::vector<RGB> samples = {{255,0,0}, {0,255,0}, {0,0,255}, {128,128,128}, {255,255,255}};
    std::cout << std::setw(15) << "Color" << std::setw(10) << "Gray\n";
    std::cout << std::string(25, '-') << "\n";
    for (const auto& c : samples) {
        int gray = static_cast<int>(0.299*c.r + 0.587*c.g + 0.114*c.b);
        std::cout << "  (" << std::setw(3) << c.r << "," << std::setw(3) << c.g
                  << "," << std::setw(3) << c.b << ")"
                  << std::setw(10) << gray << "\n";
    }
}

// ============================================================
// 2. 2D Convolution (used by blur, edge detection, sharpening)
// ============================================================
Image convolve(const Image& src, const std::vector<std::vector<float>>& kernel) {
    int kh = static_cast<int>(kernel.size());
    int kw = static_cast<int>(kernel[0].size());
    int ky = kh / 2, kx = kw / 2;

    Image dst(src.width, src.height);
    for (int y = 0; y < src.height; ++y) {
        for (int x = 0; x < src.width; ++x) {
            float sum = 0;
            for (int ky_off = 0; ky_off < kh; ++ky_off)
                for (int kx_off = 0; kx_off < kw; ++kx_off)
                    sum += kernel[ky_off][kx_off] *
                           src.get_clamped(y + ky_off - ky, x + kx_off - kx);
            dst.at(y, x) = static_cast<uint8_t>(std::clamp(sum, 0.0f, 255.0f));
        }
    }
    return dst;
}

// 3x3 Gaussian kernel (sigma ≈ 1)
Image gaussian_blur(const Image& src) {
    std::vector<std::vector<float>> gauss = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };
    return convolve(src, gauss);
}

// ============================================================
// 3. Sobel Edge Detection
// ============================================================
Image sobel_edges(const Image& src) {
    // Sobel kernels
    std::vector<std::vector<float>> Kx = {{-1,0,1},{-2,0,2},{-1,0,1}};
    std::vector<std::vector<float>> Ky = {{-1,-2,-1},{0,0,0},{1,2,1}};

    Image Gx = convolve(src, Kx);
    Image Gy = convolve(src, Ky);

    Image edges(src.width, src.height);
    for (int i = 0; i < src.width * src.height; ++i) {
        float gx = static_cast<float>(Gx.data[i]);
        float gy = static_cast<float>(Gy.data[i]);
        float mag = std::sqrt(gx*gx + gy*gy);
        edges.data[i] = static_cast<uint8_t>(std::clamp(mag, 0.0f, 255.0f));
    }
    return edges;
}

// ============================================================
// 4. Threshold
// ============================================================
Image threshold(const Image& src, uint8_t thresh) {
    Image bin(src.width, src.height);
    for (int i = 0; i < src.width * src.height; ++i)
        bin.data[i] = (src.data[i] >= thresh) ? 255 : 0;
    return bin;
}

// ============================================================
// 5. Histogram
// ============================================================
void print_histogram(const Image& img, const std::string& label) {
    std::array<int, 16> buckets{};
    for (uint8_t v : img.data) buckets[v / 16]++;

    int max_count = *std::max_element(buckets.begin(), buckets.end());
    const int BAR_W = 30;

    std::cout << "\nHistogram: " << label << "\n";
    for (int i = 0; i < 16; ++i) {
        int bar_len = buckets[i] * BAR_W / (max_count + 1);
        std::cout << std::setw(4) << i*16 << "-" << std::setw(3) << (i*16+15)
                  << " |" << std::string(bar_len, '#')
                  << " (" << buckets[i] << ")\n";
    }
}

// ============================================================
// 6. Visualize small image as ASCII
// ============================================================
void ascii_visualize(const Image& img, const std::string& label, int scale = 1) {
    const std::string chars = " .:-=+*#%@";
    std::cout << "\n" << label << " (" << img.width << "x" << img.height << "):\n";
    for (int y = 0; y < img.height; y += scale) {
        for (int x = 0; x < img.width; x += (scale > 1 ? scale/2 : 1)) {
            int idx = img.data[y * img.width + x] * (chars.size()-1) / 255;
            std::cout << chars[idx];
        }
        std::cout << "\n";
    }
}

// ============================================================
// Connected components (flood fill concept)
// ============================================================
void demo_flood_fill() {
    std::cout << "\n=== 5. Connected Components / Flood Fill ===\n";
    std::cout << "Used in object detection to label separate blobs\n\n";

    // Small binary image with two blobs
    const int W = 12, H = 8;
    Image bin(W, H);
    // Blob 1 (top-left region)
    for (int y = 1; y <= 3; ++y)
        for (int x = 1; x <= 4; ++x)
            bin.at(y, x) = 255;
    // Blob 2 (bottom-right region)
    for (int y = 5; y <= 7; ++y)
        for (int x = 7; x <= 10; ++x)
            bin.at(y, x) = 255;

    // Label map
    std::vector<int> labels(W * H, 0);
    int next_label = 1;

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (bin.at(y, x) == 255 && labels[y*W+x] == 0) {
                // BFS flood fill
                std::vector<std::pair<int,int>> queue = {{y, x}};
                labels[y*W+x] = next_label;
                size_t qi = 0;
                while (qi < queue.size()) {
                    auto [cy, cx] = queue[qi++];
                    int dx[] = {1,-1,0,0};
                    int dy[] = {0,0,1,-1};
                    for (int d = 0; d < 4; ++d) {
                        int ny = cy+dy[d], nx = cx+dx[d];
                        if (ny>=0 && ny<H && nx>=0 && nx<W &&
                            bin.at(ny,nx)==255 && labels[ny*W+nx]==0) {
                            labels[ny*W+nx] = next_label;
                            queue.push_back({ny, nx});
                        }
                    }
                }
                ++next_label;
            }
        }
    }

    std::cout << "Binary image (# = white pixel):\n";
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x)
            std::cout << (bin.at(y,x) ? '#' : '.');
        std::cout << "\n";
    }
    std::cout << "\nLabeled components (1, 2 = different blobs):\n";
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int l = labels[y*W+x];
            std::cout << (l ? std::to_string(l)[0] : '.');
        }
        std::cout << "\n";
    }
    std::cout << "Found " << (next_label - 1) << " connected components\n";
}

int main() {
    std::cout << "=== Computer Vision Algorithms (Pure C++) ===\n";
    std::cout << "These are the algorithms OpenCV implements under the hood.\n";
    std::cout << "Understanding them helps you use OpenCV intelligently.\n";

    demo_grayscale();

    // Build a test image
    Image src = make_test_image(32, 20);
    Image blurred = gaussian_blur(src);
    Image edges   = sobel_edges(blurred);
    Image binary  = threshold(edges, 80);

    ascii_visualize(src,     "Source (sinusoidal gradient)", 1);
    ascii_visualize(blurred, "After Gaussian Blur", 1);
    ascii_visualize(edges,   "Sobel Edge Detection", 1);
    ascii_visualize(binary,  "Thresholded Edges (>80)", 1);

    print_histogram(src,    "Source");
    print_histogram(edges,  "Edges");
    print_histogram(binary, "Binary");

    demo_flood_fill();

    std::cout << "\n=== How to use real OpenCV ===\n";
    std::cout << "Install: vcpkg install opencv4 --triplet x64-mingw-static\n";
    std::cout << "The OpenCV equivalents:\n";
    std::cout << "  Gaussian blur:      cv::GaussianBlur(src, dst, {5,5}, 1.5)\n";
    std::cout << "  Sobel edges:        cv::Sobel(src, dst, CV_32F, 1, 0)\n";
    std::cout << "  Threshold:          cv::threshold(src, dst, 80, 255, cv::THRESH_BINARY)\n";
    std::cout << "  Connected comps:    cv::connectedComponents(binary, labels)\n";
    std::cout << "  Color tracking:     cv::inRange(hsv, lower, upper, mask)\n";
    std::cout << "  Camera feed:        cv::VideoCapture cap(0)\n";

    return 0;
}
