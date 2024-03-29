#pragma once
#include "../include/VDTK/common/CommonDataTypes.h"

namespace VDTK {

class BinarySliceImporter {
public:
    BinarySliceImporter();
    ~BinarySliceImporter();

    static bool import(VolumeData* const volumeData, const std::filesystem::path& directoryPath,
                       const uint8_t bitsPerVoxel, const VolumeAxis axis,
                       const VDTK::VolumeSize size, const VDTK::VolumeSpacing spacing);

private:
    static bool loadSlice(VolumeSlice* const volumeSlice, const std::filesystem::path& filePath,
                          const uint8_t bitsPerVoxel, const VolumeAxis axis,
                          const std::size_t width, const std::size_t height);
    static const std::vector<uint16_t> convertTo16Bit(std::vector<char>* buffer,
                                                      const uint8_t bitsPerPixel,
                                                      const uint64_t pixelCount);
};
} // namespace VDTK
