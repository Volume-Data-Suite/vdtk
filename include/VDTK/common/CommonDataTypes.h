#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <assert.h>
#include <stdint.h>

namespace VDTK {

enum class ScaleMode { NearestNeighbor, Linear, Cubic };

enum class VolumeAxis { YZAxis, XZAxis, XYAxis };

// VOI LUT functions
enum class WindowingFunction { Linear, LinearExact, Sigmoid };

template <typename T>
class Vector3D {
public:
    Vector3D(T x, T y, T z) {
        m_X = x;
        m_Y = y;
        m_Z = z;
    }

    Vector3D(T x) {
        m_X = m_Y = m_Z = x;
    }

    T getX() const {
        return m_X;
    }
    T getY() const {
        return m_Y;
    }
    T getZ() const {
        return m_Z;
    }

    void setX(const T x) {
        m_X = x;
    }
    void setY(const T y) {
        m_Y = y;
    }
    void setZ(const T z) {
        m_Z = z;
    }

    Vector3D& operator+(const Vector3D& rhs) {

        this->m_X += rhs.m_X;
        this->m_Y += rhs.m_Y;
        this->m_Z += rhs.m_Z;

        return *this;
    }
    Vector3D& operator+=(const Vector3D& rhs) {

        this->m_X += rhs.m_X;
        this->m_Y += rhs.m_Y;
        this->m_Z += rhs.m_Z;

        return *this;
    }
    Vector3D& operator-(const Vector3D& rhs) {

        this->m_X -= rhs.m_X;
        this->m_Y -= rhs.m_Y;
        this->m_Z -= rhs.m_Z;

        return *this;
    }
    Vector3D& operator-=(const Vector3D& rhs) {

        this->m_X -= rhs.m_X;
        this->m_Y -= rhs.m_Y;
        this->m_Z -= rhs.m_Z;

        return *this;
    }
    template <typename Number>
    Vector3D& operator*(const Number& factor) {
        this->m_X *= factor;
        this->m_Y *= factor;
        this->m_Z *= factor;

        return *this;
    }
    template <typename Number>
    const Vector3D operator*(const Number& factor) const {
        Vector3D result(static_cast<T>(static_cast<Number>(m_X) * factor),
                        static_cast<T>(static_cast<Number>(m_Y) * factor),
                        static_cast<T>(static_cast<Number>(m_Z) * factor));
        return result;
    }
    template <typename Number>
    Vector3D& operator*=(const Number& factor) {
        this->m_X *= factor;
        this->m_Y *= factor;
        this->m_Z *= factor;

        return *this;
    }
    template <typename Number>
    Vector3D& operator/(const Number& divisor) {
        this->m_X /= divisor;
        this->m_Y /= divisor;
        this->m_Z /= divisor;

        return *this;
    }
    template <typename Number>
    const Vector3D operator/(const Number& factor) const {
        Vector3D result(static_cast<T>(static_cast<Number>(m_X) / factor),
                        static_cast<T>(static_cast<Number>(m_Y) / factor),
                        static_cast<T>(static_cast<Number>(m_Z) / factor));
        return result;
    }
    template <typename Number>
    Vector3D& operator/=(const Number& divisor) {
        this->m_X /= divisor;
        this->m_Y /= divisor;
        this->m_Z /= divisor;

        return *this;
    }
    bool operator==(const Vector3D& rhs) const {

        return this->m_X == rhs.m_X && this->m_Y == rhs.m_Y && this->m_Z == rhs.m_Z;
    }
    bool operator!=(const Vector3D& rhs) const {

        return this->m_X != rhs.m_X || this->m_Y != rhs.m_Y || this->m_Z != rhs.m_Z;
    }

private:
    T m_X = T();
    T m_Y = T();
    T m_Z = T();
};

typedef Vector3D<std::size_t> VolumeSize;
typedef Vector3D<float> VolumeSpacing;

class VolumeSlice {
public:
    VolumeSlice(const VolumeAxis axis, const std::size_t width, const std::size_t height)
        : m_axis(axis), m_width(width), m_height(height) {
        m_pixelData = std::vector<uint16_t>(width * height);
    }

    VolumeSlice(const VolumeAxis axis, const std::vector<uint16_t>& pixelData,
                const std::size_t width, const std::size_t height)
        : m_axis(axis), m_width(width), m_height(height), m_pixelData(pixelData) {}

    std::size_t getWidth() const {
        return m_width;
    }
    std::size_t getHeigth() const {
        return m_height;
    }
    VolumeAxis getAxis() const {
        return m_axis;
    }

    uint16_t getPixel(const std::size_t x, const std::size_t y) const {
        // check if position is within slice size
        assert(x < getWidth() && y < getHeigth());
        return m_pixelData[y + (m_height * x)];
    }
    void setPixel(const std::size_t x, const std::size_t y, const uint16_t value) {
        // check if position is within slice size
        assert(x < getWidth() && y < getHeigth());
        m_pixelData[y + (m_height * x)] = value;
    }
	
private:
    VolumeAxis m_axis = VolumeAxis::YZAxis;
    std::size_t m_width = 0;
    std::size_t m_height = 0;
    std::vector<uint16_t> m_pixelData = std::vector<uint16_t>(0);
};

class VolumeData {
public:
    VolumeData(const VDTK::VolumeSize size, const VDTK::VolumeSpacing spacing) {
        m_Size = size;
        m_Spacing = spacing;
        m_VoxelCount = m_Size.getX() * m_Size.getY() * m_Size.getZ();

        // initialize volume data with size and all values equal to zero
        m_Data = std::vector<uint16_t>(m_VoxelCount, 0);
    }

    const VDTK::VolumeSize getSize() const {
        return m_Size;
    }
    const VDTK::VolumeSpacing getSpacing() const {
        return m_Spacing;
    }
    uint64_t getVoxelCount() const {
        return m_VoxelCount;
    }

    // Individual voxel access
    void setVoxelValue(const std::size_t x, const std::size_t y, const std::size_t z,
                       const uint16_t value) {
        assert(x < m_Size.getX() && y < m_Size.getY() && z < m_Size.getZ());

        // data is stored in zyx order
        m_Data[x + m_Size.getX() * (y + (m_Size.getY() * z))] = value;
    }
    void setVoxelValue(const float x, const float y, const float z, const float value) {
        assert(value <= static_cast<float>(UINT16_MAX) && value >= 0.0f);
        setVoxelValue(static_cast<std::size_t>(x), static_cast<std::size_t>(y),
                      static_cast<std::size_t>(z), static_cast<uint16_t>(value));
    }
    void setVoxelValue(const double x, const double y, const double z, const double value) {
        assert(value <= static_cast<double>(UINT16_MAX) && value >= 0.0);
        setVoxelValue(static_cast<std::size_t>(x), static_cast<std::size_t>(y),
                      static_cast<std::size_t>(z), static_cast<uint16_t>(value));
    }
    uint16_t getVoxelValue(const std::size_t x, const std::size_t y, const std::size_t z) const {
        assert(x < m_Size.getX() && y < m_Size.getY() && z < m_Size.getZ());

        // data is stored in zyx order
        return m_Data[x + m_Size.getX() * (y + (m_Size.getY() * z))];
    }
    float getVoxelValue(const float x, const float y, const float z) const {
        assert(x >= 0.0f && y >= 0.0f && z >= 0.0f);
        return static_cast<float>(getVoxelValue(
            static_cast<std::size_t>(x), static_cast<std::size_t>(y), static_cast<std::size_t>(z)));
    }
    double getVoxelValue(const double x, const double y, const double z) const {
        assert(x >= 0.0 && y >= 0.0 && z >= 0.0);
        return static_cast<double>(getVoxelValue(
            static_cast<std::size_t>(x), static_cast<std::size_t>(y), static_cast<std::size_t>(z)));
        ;
    }

    // Axis providen by slice argument
    void setSlice(const VolumeSlice& slice, const std::size_t sliceIndex) {
        switch (slice.getAxis()) {
        case VolumeAxis::YZAxis: {
            setSliceYZ(slice, sliceIndex);
            break;
        }
        case VolumeAxis::XZAxis: {
            setSliceXZ(slice, sliceIndex);
            break;
        }
        case VolumeAxis::XYAxis: {
            setSliceXY(slice, sliceIndex);
            break;
        }
        default: { break; }
        }
    }
    const VolumeSlice getSlice(const VolumeAxis axis, const std::size_t sliceIndex) const {
        switch (axis) {
        case VolumeAxis::YZAxis: {
            return getSliceYZ(sliceIndex);
            break;
        }
        case VolumeAxis::XZAxis: {
            return getSliceXZ(sliceIndex);
            break;
        }
        case VolumeAxis::XYAxis: {
            return getSliceXY(sliceIndex);
            break;
        }
        default: {
            return getSliceYZ(sliceIndex);
            break;
        }
        }
    }

    // Whole volume access
    void setRawVolumeData(const std::vector<uint16_t>& data) {
        assert(data.size() == m_Data.size());

        m_Data = data;
    }
    const std::vector<uint16_t>& getRawVolumeData() const {
        return m_Data;
    }

private:
    uint64_t m_VoxelCount = 0;
    VDTK::VolumeSize m_Size = VDTK::VolumeSize(0, 0, 0);
    VDTK::VolumeSpacing m_Spacing = VDTK::VolumeSpacing(0.0, 0.0, 0.0);

    // internaly use always 16 bit for the volume data
    std::vector<uint16_t> m_Data;

    // Individual slice access (starts counting at 0)
    // YZ Axis
    void setSliceYZ(const VolumeSlice& slice, const std::size_t x) {
        // check if slice size fits
        assert(slice.getWidth() == m_Size.getY() && slice.getHeigth() == m_Size.getZ());

        for (std::size_t y = 0; y < m_Size.getY(); y++) {
            for (std::size_t z = 0; z < m_Size.getZ(); z++) {
                setVoxelValue(x, y, z, slice.getPixel(y, z));
            }
        }
    }
    const VolumeSlice getSliceYZ(const std::size_t x) const {
        // check if requested slice is a valid slice index
        assert(x < m_Size.getX());

        VolumeSlice slice(VolumeAxis::YZAxis, m_Size.getY(), m_Size.getZ());

        for (std::size_t y = 0; y < m_Size.getY(); y++) {
            for (std::size_t z = 0; z < m_Size.getZ(); z++) {
                slice.setPixel(y, z, getVoxelValue(x, y, z));
            }
        }

        return slice;
    }
    // XZ Axis
    void setSliceXZ(const VolumeSlice& slice, const std::size_t y) {
        // check if slice size fits
        assert(slice.getWidth() == m_Size.getX() && slice.getHeigth() == m_Size.getZ());

        for (std::size_t x = 0; x < m_Size.getX(); x++) {
            for (std::size_t z = 0; z < m_Size.getZ(); z++) {
                setVoxelValue(x, y, z, slice.getPixel(x, z));
            }
        }
    }
    const VolumeSlice getSliceXZ(const std::size_t y) const {
        // check if requested slice is a valid slice index
        assert(y < m_Size.getY());

        VolumeSlice slice(VolumeAxis::XZAxis, m_Size.getX(), m_Size.getZ());

        for (std::size_t x = 0; x < m_Size.getX(); x++) {
            for (std::size_t z = 0; z < m_Size.getZ(); z++) {
                slice.setPixel(x, z, getVoxelValue(x, y, z));
            }
        }

        return slice;
    }
    // XY Axis
    void setSliceXY(const VolumeSlice& slice, const std::size_t z) {
        // check if slice size fits
        assert(slice.getWidth() == m_Size.getX() && slice.getHeigth() == m_Size.getY());

        for (std::size_t x = 0; x < m_Size.getX(); x++) {
            for (std::size_t y = 0; y < m_Size.getY(); y++) {
                setVoxelValue(x, y, z, slice.getPixel(x, y));
            }
        }
    }
    const VolumeSlice getSliceXY(const std::size_t z) const {
        // check if requested slice is a valid slice index
        assert(z < m_Size.getZ());

        VolumeSlice slice(VolumeAxis::XYAxis, m_Size.getX(), m_Size.getY());

        for (std::size_t x = 0; x < m_Size.getX(); x++) {
            for (std::size_t y = 0; y < m_Size.getY(); y++) {
                slice.setPixel(x, y, getVoxelValue(x, y, z));
            }
        }

        return slice;
    }
};

class FilterKernel {
public:
    // only kernel size 3x3x3 and 5x5x5 are supported
    // if input is not valid filter gets initialized with default filter which does not change image
    // at all when applied to volume
    FilterKernel(const uint8_t kernelSize,
                 const std::vector<std::vector<std::vector<double>>> filterGrid)
        : kernelSize(kernelSize), filterGrid(filterGrid) {
        // check if kernel size is supported
        const bool supportedKernelSize = kernelSize == 3 || kernelSize == 5;
        assert(supportedKernelSize);
        if (!supportedKernelSize) {
            this->kernelSize = 3;
        }

        // check if filtergrid fits the kernel size
        const bool validFilterGridSize = this->kernelSize == filterGrid.size() &&
                                         this->kernelSize == filterGrid.data()->size() &&
                                         this->kernelSize == filterGrid.data()->data()->size();
        assert(validFilterGridSize);
        if (!validFilterGridSize) {
            this->filterGrid = {
                {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
                {{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}},
                {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
            };
        }
    }

    const std::vector<std::vector<std::vector<double>>>& getFilterGrid() const {
        return filterGrid;
    }

    std::size_t getKernelSize() const {
        return kernelSize;
    }

private:
    std::size_t kernelSize = 3;
    std::vector<std::vector<std::vector<double>>> filterGrid;
};

} // namespace VDTK
