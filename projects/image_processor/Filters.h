#pragma once

#include <string>
#include <vector>

#include "Image.h"

class IFilter {
public:
    virtual void Apply(Image &image) const = 0;
};

class IConvFilter : public IFilter {
public:
    void Apply(Image &image) const override;

    virtual std::vector<std::vector<float>> GetKernel() const = 0;
};

class NegFilter : public IFilter {
public:
    void Apply(Image &image) const override;
};

class CropFilter : public IFilter {
public:
    explicit CropFilter(const std::vector<std::string> &params);

    void Apply(Image &image) const override;

private:
    unsigned int x_;
    unsigned int y_;
    unsigned int width_;
    unsigned int height_;
};

class SharpFilter : public IConvFilter {
public:
    std::vector<std::vector<float>> GetKernel() const override;
};

class GrayScaleFilter : public IFilter {
public:
    void Apply(Image &image) const override;
};

class EdgeFilter : public IConvFilter {
public:
    explicit EdgeFilter(const std::vector<std::string> &params);

    std::vector<std::vector<float>> GetKernel() const override;

    void Apply(Image &image) const override;

private:
    float threshold_;
};

class BlurFilter : public IConvFilter {
public:
    explicit BlurFilter(const std::vector<std::string> &params);

    std::vector<std::vector<float>> GetKernel() const override;

private:
    float sigma_;
};
