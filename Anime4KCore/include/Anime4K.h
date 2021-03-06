#pragma once

#include<iostream>
#include<sstream>
#include<functional>
#include<atomic>

#include<opencv2/opencv.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/core/hal/interface.h>

#include"VideoIO.h"

#ifdef _MSC_VER
#ifndef DLL
#define DLL __declspec(dllimport)
#else
#undef DLL
#define DLL __declspec(dllexport)
#endif
#else
#ifndef DLL
#define DLL
#endif
#endif

namespace Anime4KCPP
{
    struct DLL Parameters;
    class DLL Anime4K;

    enum class ProcessorType;

    enum BGRA
    {
        B = 0, G = 1, R = 2, A = 3
    };

    enum FilterType : uint8_t
    {
        MEDIAN_BLUR = 1, MEAN_BLUR = 2, CAS_SHARPENING = 4,
        GAUSSIAN_BLUR_WEAK = 8, GAUSSIAN_BLUR = 16,
        BILATERAL_FILTER = 32, BILATERAL_FILTER_FAST = 64
    };

    typedef unsigned char* RGBA;
    typedef unsigned char* Line;

    enum YUV
    {
        Y = 0, U = 1, V = 2
    };

    typedef double* Chan;
    typedef unsigned char* PIXEL;
    typedef unsigned char* LineC;
    typedef double* LineF;
}

struct Anime4KCPP::Parameters
{
    int passes;
    int pushColorCount;
    float strengthColor;
    float strengthGradient;
    float zoomFactor;
    bool fastMode;
    bool videoMode;
    bool preprocessing;
    bool postprocessing;
    uint8_t preFilters;
    uint8_t postFilters;
    unsigned int maxThreads;

    void reset();

    Parameters(
        int passes = 2,
        int pushColorCount = 2,
        float strengthColor = 0.3F,
        float strengthGradient = 1.0F,
        float zoomFactor = 2.0F,
        bool fastMode = false,
        bool videoMode = false,
        bool preprocessing = false,
        bool postprocessing = false,
        uint8_t preFilters = 4,
        uint8_t postFilters = 40,
        unsigned int maxThreads = std::thread::hardware_concurrency()
    );
};

enum class Anime4KCPP::ProcessorType
{
    CPU, GPU, CPUCNN, GPUCNN
};

class Anime4KCPP::Anime4K
{
public:
    Anime4K(const Parameters& parameters);
    virtual ~Anime4K();

    void setArguments(const Parameters& parameters);
    void setVideoMode(const bool flag);

    void loadVideo(const std::string& srcFile);
    void loadImage(const std::string& srcFile);
    void loadImage(cv::InputArray srcImage);
    void loadImage(int rows, int cols, unsigned char* data, size_t bytesPerLine = 0ULL, bool inputAsYUV = false);
    void loadImage(int rows, int cols, unsigned char* r, unsigned char* g, unsigned char* b, bool inputAsYUV = false);

    void setVideoSaveInfo(const std::string& dstFile, const CODEC codec = CODEC::MP4V);
    void saveImage(const std::string& dstFile);
    void saveImage(cv::Mat& dstImage);
    void saveImage(unsigned char*& data);
    void saveImage(unsigned char*& r, unsigned char*& g, unsigned char*& b);
    void saveVideo();

    void showInfo();
    void showFiltersInfo();

    std::string getInfo();
    std::string getFiltersInfo();
    size_t getResultDataLength();
    size_t getResultDataPerChannelLength();

    void showImage();
    virtual void process() = 0;

protected:
    int orgH, orgW, H, W;
    double fps;
    double totalFrameCount;
    cv::Mat orgImg, dstImg;
    bool inputYUV = false;

protected://arguments
    int ps, pcc;
    float sc, sg, zf;
    bool fm, vm, pre, post;
    uint8_t pref, postf;
    unsigned int mt;
};

