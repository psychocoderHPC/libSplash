/**
 * Copyright 2013, 2015 Felix Schmitt, Axel Huebl
 *
 * This file is part of libSplash.
 *
 * libSplash is free software: you can redistribute it and/or modify
 * it under the terms of of either the GNU General Public License or
 * the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libSplash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License and the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the GNU Lesser General Public License along with libSplash.
 * If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef _DIMENSIONS_H
#define	_DIMENSIONS_H

#include <sstream>
#include <hdf5.h>

#define DSP_DIM_MAX 3

namespace splash
{

    /**
     * Manages 1-3D size information.
     * Can be used for MPI-positions/-dimensions or data-dimensions.
     */
    class Dimensions
    {
    private:
        hsize_t s[DSP_DIM_MAX];
    public:

        /**
         * Default constructor.
         * Initializes all dimensions to 1.
         */
        Dimensions(void)
        {
            set(1, 1, 1);
        }

        /**
         * Constructor
         * @param x first dimension
         * @param y second dimension
         * @param z third dimension
         */
        Dimensions(hsize_t x, hsize_t y, hsize_t z)
        {
            set(x, y, z);
        }

        /**
         * Returns element \p t
         * 
         * @param t element index (1-3)
         * @return element
         */
        hsize_t & operator[](const hsize_t t)
        {
            return s[t];
        }

        /**
         * Returns element \p t
         * 
         * @param t element index (1-3)
         * @return element
         */
        const hsize_t & operator[](const hsize_t t) const
        {
            return s[t];
        }

        /**
         * Element-wise addition of two Dimensions
         * 
         * @param other other Dimensions
         * @return new added Dimensions
         */
        Dimensions operator+(Dimensions const& other) const
        {
            return Dimensions(s[0] + other[0], s[1] + other[1], s[2] + other[2]);
        }

        /**
         * Element-wise substraction of two Dimensions
         * 
         * @param other other Dimensions
         * @return new substracted Dimensions
         */
        Dimensions operator-(Dimensions const& other) const
        {
            return Dimensions(s[0] - other[0], s[1] - other[1], s[2] - other[2]);
        }

        /**
         * Element-wise multiplication of two Dimensions
         * 
         * @param other other Dimensions
         * @return new multiplied Dimensions
         */
        Dimensions operator*(Dimensions const& other) const
        {
            return Dimensions(s[0] * other[0], s[1] * other[1], s[2] * other[2]);
        }

        /**
         * Element-wise division of two Dimensions
         * 
         * @param other other Dimensions
         * @return new divided Dimensions
         */
        Dimensions operator/(Dimensions const& other) const
        {
            return Dimensions(s[0] / other[0], s[1] / other[1], s[2] / other[2]);
        }

        /**
         * Element-wise addition of Dimensions \p rhs to this Dimensions
         * 
         * @param rhs other Dimensions
         * @return this Dimensions
         */
        Dimensions operator+=(Dimensions const& rhs)
        {
            *this = *this + rhs;
            return *this;
        }

        /**
         * Element-wise substraction of Dimensions \p rhs from this Dimensions
         * 
         * @param rhs other Dimensions
         * @return this Dimensions
         */
        Dimensions operator-=(Dimensions const& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        /**
         * Element-wise comparison of two Dimensions
         * 
         * @param other other Dimensions
         * @return true if equal, false otherwise
         */
        bool operator==(Dimensions const& other) const
        {
            return s[0] == other[0] && s[1] == other[1] && s[2] == other[2];
        }

        /**
         * Element-wise comparison of two Dimensions
         * 
         * @param other other Dimensions
         * @return false if equal, true otherwise
         */
        bool operator!=(Dimensions const& other) const
        {
            return !(*this == other);
        }

        /**
         * Returns the string representation of this object
         * 
         * @return string representation
         */
        std::string toString() const
        {
            std::stringstream stream;
            stream << "(" << s[0] << "," << s[1] << "," << s[2] << ")";
            return stream.str();
        }

        /**
         * Get pointer to internal data array.
         * @return Pointer to the internal data array.
         */
        inline hsize_t *getPointer()
        {
            return s;
        }
        
        /**
         * Get pointer to internal data array.
         * @return Pointer to the internal data array.
         */
        inline const hsize_t *getPointer() const
        {
            return s;
        }

        /**
         * Get the size in bytes of the data array.
         * @return Size in bytes of data array.
         */
        inline static size_t getSize()
        {
            return DSP_DIM_MAX * sizeof (hsize_t);
        }

        /**
         * Get the scalar size.
         * @return Scalar number of elements spanned by all dimensions.
         */
        inline size_t getScalarSize() const
        {
            return s[0] * s[1] * s[2];
        }

        /**
         * Set dimensions.
         * @param x First dimension.
         * @param y Second dimension.
         * @param z Third dimension.
         */
        inline void set(hsize_t x, hsize_t y, hsize_t z)
        {
            s[0] = x;
            s[1] = y;
            s[2] = z;
        }

        /**
         * Set dimensions.
         * @param d Dimensions object to copy data from.
         */
        inline void set(const Dimensions d)
        {
            s[0] = d[0];
            s[1] = d[1];
            s[2] = d[2];
        }
        
        /**
         * Get the number of dimensions.
         * @return Number of dimensions.
         */
        inline uint32_t getDims(void) const
        {
            uint32_t dims = DSP_DIM_MAX;
            if (s[2] == 1)
            {
                dims = 2;
                if (s[1] == 1)
                    dims = 1;
            }
            
            return dims;
        }

        /**
         * Swaps the dimensions depending on \p dims.
         * @param dims Number of dimensions for swapping.
         */
        void swapDims(uint32_t dims)
        {
            hsize_t tmp1 = s[0];
            hsize_t tmp2[DSP_DIM_MAX] = {s[2], s[1], s[0]};

            switch (dims)
            {
                case 2:
                    s[0] = s[1];
                    s[1] = tmp1;
                    break;
                case 3:
                    for (uint32_t i = 0; i < 3; i++)
                        s[i] = tmp2[i];
                    break;
                default:
                    return;
            }
        }

    };

}

#endif	/* _DIMENSIONS_H */

