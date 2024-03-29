
#include "EdgeCutter.h"

namespace VDTK {
EdgeCutter::EdgeCutter() {}

EdgeCutter::~EdgeCutter() {}

void EdgeCutter::cutBorders(VolumeData* const volume, const uint16_t threshold) {
    std::size_t lowerBorderX = 0;
    std::size_t upperBorderX = 0;
    std::size_t lowerBorderY = 0;
    std::size_t upperBorderY = 0;
    std::size_t lowerBorderZ = 0;
    std::size_t upperBorderZ = 0;

    bool borderNotFound = true;

    // find lowerBorderX
    for (std::size_t positionX = 0; borderNotFound && positionX < volume->getSize().getX();
         positionX++) {
        for (std::size_t positionY = 0; borderNotFound && positionY < volume->getSize().getY();
             positionY++) {
            for (std::size_t positionZ = 0; borderNotFound && positionZ < volume->getSize().getZ();
                 positionZ++) {
                if (volume->getVoxelValue(positionX, positionY, positionZ) > threshold) {
                    lowerBorderX = positionX;
                    borderNotFound = false;
                }
            }
        }
    }

    borderNotFound = true;

    // find upperBorderX
    for (std::size_t positionX = volume->getSize().getX() - 1;
         borderNotFound && positionX > lowerBorderX; positionX--) {
        for (std::size_t positionY = volume->getSize().getY() - 1; borderNotFound && positionY > 0;
             positionY--) {
            for (std::size_t positionZ = volume->getSize().getZ() - 1;
                 borderNotFound && positionZ > 0; positionZ--) {
                if (volume->getVoxelValue(positionX, positionY, positionZ) > threshold) {
                    upperBorderX = positionX;
                    borderNotFound = false;
                }
            }
        }
    }

    borderNotFound = true;

    // find lowerBorderY
    for (std::size_t positionY = 0; borderNotFound && positionY < volume->getSize().getY();
         positionY++) {
        for (std::size_t positionX = lowerBorderX; borderNotFound && positionX <= upperBorderX;
             positionX++) {
            for (std::size_t positionZ = 0; borderNotFound && positionZ < volume->getSize().getZ();
                 positionZ++) {
                if (volume->getVoxelValue(positionX, positionY, positionZ) > threshold) {
                    lowerBorderY = positionY;
                    borderNotFound = false;
                }
            }
        }
    }

    borderNotFound = true;

    // find upperBorderY
    for (std::size_t positionY = volume->getSize().getY() - 1;
         borderNotFound && positionY > lowerBorderY; positionY--) {
        for (std::size_t positionX = lowerBorderX; borderNotFound && positionX <= upperBorderX;
             positionX++) {
            for (std::size_t positionZ = 0; borderNotFound && positionZ < volume->getSize().getZ();
                 positionZ++) {
                if (volume->getVoxelValue(positionX, positionY, positionZ) > threshold) {
                    upperBorderY = positionY;
                    borderNotFound = false;
                }
            }
        }
    }

    borderNotFound = true;

    // find lowerBorderZ
    for (std::size_t positionZ = 0; borderNotFound && positionZ < volume->getSize().getZ();
         positionZ++) {
        for (std::size_t positionX = lowerBorderX; borderNotFound && positionX < upperBorderX;
             positionX++) {
            for (std::size_t positionY = lowerBorderY; borderNotFound && positionY < upperBorderY;
                 positionY++) {
                if (volume->getVoxelValue(positionX, positionY, positionZ) > threshold) {
                    lowerBorderZ = positionZ;
                    borderNotFound = false;
                }
            }
        }
    }

    borderNotFound = true;

    // find upperBorderZ
    for (std::size_t positionZ = volume->getSize().getZ() - 1;
         borderNotFound && positionZ > lowerBorderZ; positionZ--) {
        for (std::size_t positionX = lowerBorderX; borderNotFound && positionX <= upperBorderX;
             positionX++) {
            for (std::size_t positionY = lowerBorderY; borderNotFound && positionY < upperBorderY;
                 positionY++) {
                if (volume->getVoxelValue(positionX, positionY, positionZ) > threshold) {
                    upperBorderZ = positionZ;
                    borderNotFound = false;
                }
            }
        }
    }

    // + 1 because size starts counting with 1 and not 0
    const std::size_t newVolumeSizeX = upperBorderX - lowerBorderX + 1;
    const std::size_t newVolumeSizeY = upperBorderY - lowerBorderY + 1;
    const std::size_t newVolumeSizeZ = upperBorderZ - lowerBorderZ + 1;
    const VDTK::VolumeSize newVolumeSize(newVolumeSizeX, newVolumeSizeY, newVolumeSizeZ);

    // if no borders have to be cut, no creation of a new volume is needed
    if (newVolumeSize != volume->getSize()) {
        VolumeData newVolume(newVolumeSize, volume->getSpacing());

        // copy values into new volume
        for (std::size_t positionX = 0; positionX < newVolumeSizeX; positionX++) {
            for (std::size_t positionY = 0; positionY < newVolumeSizeY; positionY++) {
                for (std::size_t positionZ = 0; positionZ < newVolumeSizeZ; positionZ++) {
                    const uint16_t value =
                        volume->getVoxelValue(positionX + lowerBorderX, positionY + lowerBorderY,
                                              positionZ + lowerBorderZ);
                    newVolume.setVoxelValue(positionX, positionY, positionZ, value);
                }
            }
        }

        *volume = newVolume;
    }
}
} // namespace VDTK