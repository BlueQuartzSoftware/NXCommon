/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the following contracts:
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include "NX/Common/NXCommon_export.hpp"

#include <array>
#include <cstddef>

#if defined(COMPLEX_ENABLE_MULTICORE) || defined(NXCOMMON_ENABLE_MULTICORE)
#include <tbb/blocked_range2d.h>
#endif
namespace NX::Common
{
/**
 * @class Range2D Range2D.h SIMPLib/Common/Range2D.h
 * @brief The Range2D class defines a range between set of minimum and
 * maximum values. The purpose of this class is mainly to allow a more unified
 * control flow during parallelization between builds using TBB and those that
 * do not.  Because tbb::blocked_range is used in an implicit conversion constructor,
 * a single operator accepting a Range can be used TBB parallelized and
 * non-paralleled versions without a branching code base.
 */
class NXCOMMON_EXPORT Range2D
{
public:
  /**
   * @brief The 4 values are stored in the order of Col Start, Col End, Row Start, Row End
   * with the assumption that the ordering of the data is Column moving the fastest
   * then the Rows.
   */
  using RangeType = std::array<size_t, 4>; // {  Col Start, Col End, Row Start, Row End }

  Range2D();
  /**
   * @brief Creates a 2D Range where X is the fastest moving axis, and then Y
   * @param colStart Starting Colum Index
   * @param colEnd Ending Column Index (non inclusive)
   * @param rowStart Starting Row Index
   * @param rowEnd Ending row index (non inclusive)
   */
  Range2D(size_t colStart, size_t colEnd, size_t rowStart, size_t rowEnd);
#ifdef NXCOMMON_ENABLE_MULTICORE
  Range2D(const tbb::blocked_range2d<size_t, size_t>& r);
#endif

  /**
   * @brief Returns an array representation of the range.
   * @return
   */
  RangeType getRange() const;

  /**
   * @brief Returns the minimum row index in the range.
   * @return
   */
  size_t minRow() const;

  /**
   * @brief Returns the minimum column index in the range.
   * @return
   */
  size_t minCol() const;

  /**
   * @brief Returns the maximum row index in the range.
   * @return
   */
  size_t maxRow() const;

  /**
   * @brief Returns the maximum column index in the range.
   * @return
   */
  size_t maxCol() const;

  /**
   * @brief Returns the number of rows in the range.
   * @return
   */
  size_t numRows() const;

  /**
   * @brief Returns the number of columns in the range.
   * @return
   */
  size_t numCols() const;

  /**
   * @brief Returns the number of indices in the range.
   * @return
   */
  size_t size() const;

  /**
   * @brief Returns true if the range is empty.  Returns false otherwise.
   * @return
   */
  bool empty() const;

  /**
   * @brief Returns the range based on the specified index.  The range is
   * organized as [min, max]
   */
  size_t operator[](size_t index) const;

private:
  RangeType m_Range;
};

} // namespace NX::Common
