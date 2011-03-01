/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkImageRegion.h"
#include "itkContinuousIndex.h"
#include "vcl_limits.h"

int itkImageRegionTest(int, char* [] )
{

  const unsigned int dimension = 3;

  typedef double                         TCoordRepType;
  typedef itk::ImageRegion< dimension >  RegionType;
  typedef RegionType::IndexType          IndexType;
  typedef RegionType::SizeType           SizeType;
  typedef RegionType::SliceRegion        SliceRegionType;
  typedef itk::ContinuousIndex< TCoordRepType, dimension >
                                         ContinuousIndexType;

  typedef itk::NumericTraits<IndexType::IndexValueType>
                                         IndexNumericTraits;
  typedef itk::NumericTraits<ContinuousIndexType::ValueType>
                                         ContinuousIndexNumericTraits;

  bool passed = true;

  SizeType sizeA = {{ 10, 20, 30 }};
  SizeType sizeB = {{  5, 10, 15 }};

  IndexType startA = {{ 12, 12, 12 }};
  IndexType startB = {{ 14, 14, 14 }};

  RegionType regionA;
  RegionType regionB;

  regionA.SetSize(  sizeA  );
  regionA.SetIndex( startA );

  // Take slices of a region
  try
    {
    SliceRegionType sliceA;
    sliceA = regionA.Slice(0);
    std::cout << "regionA.Slice(0): " << sliceA;
    }
  catch (itk::ExceptionObject &err)
    {
    std::cout << "Caught unexpected exception" << err;
    return EXIT_FAILURE;
    }

  try
    {
    SliceRegionType sliceA;
    sliceA = regionA.Slice(1);
    std::cout << "regionA.Slice(1): " << sliceA;
    }
  catch (itk::ExceptionObject &err)
    {
    std::cout << "Caught unexpected exception" << err;
    return EXIT_FAILURE;
    }

  try
    {
    SliceRegionType sliceA;
    sliceA = regionA.Slice(2);
    std::cout << "regionA.Slice(2): " << sliceA;
    }
  catch (itk::ExceptionObject &err)
    {
    std::cout << "Caught unexpected exception" << err;
    return EXIT_FAILURE;
    }

  try
    {
    SliceRegionType sliceA;
    sliceA = regionA.Slice(20);
    std::cout << "regionA.Slice(20): " << sliceA;
    std::cout << "Failed to catch expected exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch (itk::ExceptionObject &err)
    {
    std::cout << "Caught expected exception" << err;
    }

  regionB.SetSize(  sizeB  );
  regionB.SetIndex( startB );

  //Test IsInside( integerIndex )
  IndexType index = startA;
  if( !regionA.IsInside( index ) )
    {
    std::cout << "Error with IsInside 1." << std::endl;
    passed = false;
    }
  index[0] = startA[0] + sizeA[0] - 1;
  if( !regionA.IsInside( index ) )
    {
    std::cout << "Error with IsInside 2." << std::endl;
    passed = false;
    }
  index[0] = startA[0]-1;
  if( regionA.IsInside( index ) )
    {
    std::cout << "Error with IsInside 3. Expected false." << std::endl;
    passed = false;
    }
  index[0] = IndexNumericTraits::max();
  if( regionA.IsInside( index ) )
    {
    std::cout << "Error with IsInside 4. Expected false." << std::endl;
    passed = false;
    }
  if( IndexNumericTraits::is_signed )
    {
    index[0] = IndexNumericTraits::min();
    if( regionA.IsInside( index ) )
      {
      std::cout << "Error with IsInside 5. Expected false." << std::endl;
      passed = false;
      }
    }

  //Test IsInside( ContinuousIndex )
  ContinuousIndexType indexC;
  indexC[0] = startA[0];
  indexC[1] = startA[1];
  indexC[2] = startA[2];
  if( !regionA.IsInside( indexC ) )
    {
    std::cout << "Error with IsInside 1C." << std::endl;
    passed = false;
    }
  indexC[0] = startA[0] + sizeA[0] - 0.5;
  indexC[1] = startA[1] + sizeA[1] - 0.5;
  indexC[2] = startA[2] + sizeA[2] - 0.5;
  if( !regionA.IsInside( indexC ) )
    {
    std::cout << "Error with IsInside 2C." << std::endl;
    passed = false;
    }
  indexC[0] = startA[0]-1;
  if( regionA.IsInside( indexC ) )
    {
    std::cout << "Error with IsInside 3C. Expected false." << std::endl
              << "  indexC: " << indexC << std::endl
              << "  start & size: "
              << startA << " " << sizeA << std::endl;
    passed = false;
    }
  indexC[0] = ContinuousIndexNumericTraits::max();
  if( regionA.IsInside( indexC ) )
    {
    std::cout << "Error with IsInside 4C. Expected false." << std::endl;
    passed = false;
    }
  indexC[0] = ContinuousIndexNumericTraits::min();
  if( regionA.IsInside( indexC ) )
    {
    std::cout << "Error with IsInside 5C. Expected false." << std::endl;
    passed = false;
    }
  /* Note for NaN. IsInside doesn't properly catch NaN. It gets cast to integer
   * which means it becomes a large negative number so it falls outside of
   * region bounds. In this way the test returns false appropriately, but for
   * the wrong reasons. If this test fails, then the compiler is handling a
   * cast of NaN to integer differently. */
  if( ContinuousIndexNumericTraits::has_quiet_NaN )
    {
    indexC[0] = ContinuousIndexNumericTraits::quiet_NaN();
    if( regionA.IsInside( indexC ) )
      {
      std::cout << "Error with IsInside 6C. Expected false." << std::endl;
      passed = false;
      }
    }
  /* Note that signaling_NaN seems to simply wrap quiet_NaN */
  if( ContinuousIndexNumericTraits::has_signaling_NaN )
    {
    indexC[0] = ContinuousIndexNumericTraits::signaling_NaN();
    if( regionA.IsInside( indexC ) )
      {
      std::cout << "Error with IsInside 7C. Expected false." << std::endl;
      passed = false;
      }
    }
  indexC[0] = ContinuousIndexNumericTraits::infinity();
  if( regionA.IsInside( indexC ) )
    {
    std::cout << "Error with IsInside 8C. Expected false." << std::endl;
    passed = false;
    }

  //Test IsInside( region )
  if( ! regionA.IsInside( regionB ) )
    {
    passed = false;
    }

  if( regionB.IsInside( regionA ) )
    {
    passed = false;
    }

  if (passed)
    {
    std::cout << "ImageRegion test passed." << std::endl;
    return EXIT_SUCCESS;
    }
  else
    {
    std::cout << "ImageRegion test failed." << std::endl;
    return EXIT_FAILURE;
    }

}
