/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkVoronoiDiagram2D_h
#define itkVoronoiDiagram2D_h


#include "itkMesh.h"
#include "itkDefaultDynamicMeshTraits.h"
#include "itkPolygonCell.h"
#include <vector>

namespace itk
{
/** \class VoronoiDiagram2D
 * \brief Implements the 2-Dimensional Voronoi Diagram.
 *
 * Given a set of seed points, the Voronoi Diagram partitions the plane into
 * regions, each region is a collection of all pixels that is closest to one
 * particular seed point than to other seed points.
 * VoronoiDiagram2D is a mesh structure for storing the Voronoi Diagram,
 * can be Generated by itkVoronoiDiagram2DGenerator.
 *
 * Template parameters for VoronoiDiagram2D:
 *
 * TCoordType = The type associated with the coordination of the seeds and
 * the resulting vertices.
 *
 * \ingroup MeshObjects
 * \ingroup ITKVoronoi
 *
 * \sphinx
 * \sphinxexample{Segmentation/Voronoi/VoronoiDiagram,Voronoi Diagram}
 * \endsphinx
 */
template <typename TCoordType>
class ITK_TEMPLATE_EXPORT VoronoiDiagram2D
  : public Mesh<TCoordType, 2, DefaultDynamicMeshTraits<TCoordType, 2, 2, TCoordType>>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(VoronoiDiagram2D);

  /** Standard class type aliases. */
  using Self = VoronoiDiagram2D;
  using Superclass = Mesh<TCoordType, 2, DefaultDynamicMeshTraits<TCoordType, 2, 2, TCoordType>>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** \see LightObject::GetNameOfClass() */
  itkOverrideGetNameOfClassMacro(VoronoiDiagram2D);

  /** Define the mesh traits. */
  using MeshTraits = DefaultDynamicMeshTraits<TCoordType, 2, 2, TCoordType>;

  /** Dimensions of the points and topology. */
  static constexpr unsigned int PointDimension = MeshTraits::PointDimension;
  static constexpr unsigned int MaxTopologicalDimension = MeshTraits::MaxTopologicalDimension;

  /** Typedefs from itkMesh */
  using PixelType = typename MeshTraits::PixelType;
  using CoordinateType = typename MeshTraits::CoordinateType;
#ifndef ITK_FUTURE_LEGACY_REMOVE
  using CoordRepType ITK_FUTURE_DEPRECATED(
    "ITK 6 discourages using `CoordRepType`. Please use `CoordinateType` instead!") = CoordinateType;
#endif
  using InterpolationWeightType = typename MeshTraits::InterpolationWeightType;
  using PointIdentifier = typename MeshTraits::PointIdentifier;
  using CellIdentifier = typename MeshTraits::CellIdentifier;
  using CellFeatureIdentifier = typename MeshTraits::CellFeatureIdentifier;
  using PointType = typename MeshTraits::PointType;
  using PointsContainer = typename MeshTraits::PointsContainer;
  using CellTraits = typename MeshTraits::CellTraits;
  using CellsContainer = typename MeshTraits::CellsContainer;
  using PointCellLinksContainer = typename MeshTraits::PointCellLinksContainer;
  using CellLinksContainer = typename MeshTraits::CellLinksContainer;
  using PointDataContainer = typename MeshTraits::PointDataContainer;
  using CellDataContainer = typename MeshTraits::CellDataContainer;
  using genericCellPointer = typename MeshTraits::CellAutoPointer;
  using BoundingBoxType = BoundingBox<PointIdentifier, Self::PointDimension, CoordinateType, PointsContainer>;
  using PointsContainerPointer = typename PointsContainer::Pointer;
  using CellsContainerPointer = typename CellsContainer::Pointer;
  using CellLinksContainerPointer = typename CellLinksContainer::Pointer;
  using PointDataContainerPointer = typename PointDataContainer::Pointer;
  using CellDataContainerPointer = typename CellDataContainer::Pointer;
  using BoundingBoxPointer = typename BoundingBoxType::Pointer;
  using PointsContainerConstIterator = typename PointsContainer::ConstIterator;
  using PointsContainerIterator = typename PointsContainer::Iterator;
  using CellsContainerConstIterator = typename CellsContainer::ConstIterator;
  using CellsContainerIterator = typename CellsContainer::Iterator;
  using CellLinksContainerIterator = typename CellLinksContainer::ConstIterator;
  using PointDataContainerIterator = typename PointDataContainer::ConstIterator;
  using CellDataContainerIterator = typename CellDataContainer::ConstIterator;
  using PointCellLinksContainerIterator = typename PointCellLinksContainer::const_iterator;

  using CellFeatureCount = CellFeatureIdentifier;
  using typename Superclass::CellType;
  using typename Superclass::CellAutoPointer;
  using PolygonCellType = PolygonCell<CellType>;
  using EdgeInfo = Point<int, 2>;
  using EdgeInfoDQ = std::deque<EdgeInfo>;
  using CellMultiVisitorType = typename CellType::MultiVisitor;
  using SeedsType = std::vector<PointType>;
  using SeedsIterator = typename SeedsType::iterator;
  using Edge = LineCell<CellType>;
  using EdgeAutoPointer = typename Edge::SelfAutoPointer;
  using PointList = std::list<PointType>;
  using INTvector = std::vector<int>;
  using NeighborIdIterator = typename INTvector::iterator;
  using VertexIterator = PointsContainerIterator;
  /** Get the number of Voronoi seeds. */
  itkGetConstMacro(NumberOfSeeds, unsigned int);

  /** Input the seeds information, will overwrite if seeds already
   * exists. */
  void
  SetSeeds(int num, SeedsIterator begin);

  /** The boundary that encloses the whole Voronoi diagram. */
  void
  SetBoundary(PointType vorsize);

  void
  SetOrigin(PointType vorsize);

  /** Iterators for the neighborhood cells around the given cell. */
  NeighborIdIterator
  NeighborIdsBegin(int seeds);

  NeighborIdIterator
  NeighborIdsEnd(int seeds);

  /** Iterators for all the vertices of the voronoi diagram. */
  VertexIterator
  VertexBegin();

  VertexIterator
  VertexEnd();

  /** Return the given indexed seed. */
  PointType
  GetSeed(int SeedID);

  /** Return the required cell pointer. */
  void
  GetCellId(CellIdentifier cellId, CellAutoPointer &);

  /** Return the given vertex of the voronoi diagram. */
  void
  GetPoint(int pId, PointType * answer);

  class VoronoiEdge
  {
  public:
    PointType m_Left;
    PointType m_Right;
    int       m_LeftID;
    int       m_RightID;
    int       m_LineID;
    VoronoiEdge() = default;
    ~VoronoiEdge() = default;
  };

  /** The iterator for Voronoi edges, */
  using VoronoiEdgeIterator = typename std::vector<VoronoiEdge>::iterator;

  /** The Iterator of all the edges for the Voronoi diagram. */
  VoronoiEdgeIterator
  EdgeBegin();

  VoronoiEdgeIterator
  EdgeEnd();

  /** Find the two seed point that around the given edge. */
  EdgeInfo
  GetSeedsIDAroundEdge(VoronoiEdge * task);

  /********************************************************/

  void
  Reset();

  void
  InsertCells();

  void
  AddCellNeighbor(EdgeInfo x)
  {
    m_CellNeighborsID[x[0]].push_back(x[1]);
    m_CellNeighborsID[x[1]].push_back(x[0]);
  }

  void
  ClearRegion(int i)
  {
    m_VoronoiRegions[i]->ClearPoints();
  }

  void
  VoronoiRegionAddPointId(int id, int x)
  {
    m_VoronoiRegions[id]->AddPointId(x);
  }

  void
  BuildEdge(int id)
  {
    m_VoronoiRegions[id]->BuildEdges();
  }

  void
  LineListClear()
  {
    m_LineList.clear();
  }

  void
  EdgeListClear()
  {
    m_EdgeList.clear();
  }

  void
  VertexListClear()
  {
    if (this->m_PointsContainer.IsNull())
    {
      this->m_PointsContainer = PointsContainer::New();
    }

    this->m_PointsContainer->Initialize();
  }

  int
  LineListSize()
  {
    return static_cast<int>(m_LineList.size());
  }

  int
  EdgeListSize()
  {
    return static_cast<int>(m_EdgeList.size());
  }

  int
  VertexListSize()
  {
    return static_cast<int>(this->m_PointsContainer->Size());
  }

  void
  AddLine(EdgeInfo x)
  {
    m_LineList.push_back(x);
  }

  void
  AddEdge(VoronoiEdge x)
  {
    m_EdgeList.push_back(x);
  }

  void
  AddVert(PointType x)
  {
    this->m_PointsContainer->InsertElement(this->m_PointsContainer->Size(), x);
  }

  EdgeInfo
  GetLine(int id)
  {
    return m_LineList[id];
  }

  VoronoiEdge
  GetEdge(int id)
  {
    return m_EdgeList[id];
  }

  PointType
  GetVertex(int id)
  {
    return this->m_PointsContainer->ElementAt(id);
  }

  EdgeInfo
  GetEdgeEnd(int id)
  {
    EdgeInfo x;

    x[0] = m_EdgeList[id].m_LeftID;
    x[1] = m_EdgeList[id].m_RightID;
    return x;
  }

  int
  GetEdgeLineID(int id)
  {
    return m_EdgeList[id].m_LineID;
  }

protected:
  VoronoiDiagram2D();
  ~VoronoiDiagram2D() override = default;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

private:
  SeedsType                      m_Seeds{};
  unsigned int                   m_NumberOfSeeds{};
  std::vector<PolygonCellType *> m_VoronoiRegions{};
  PointType                      m_VoronoiBoundary{};
  PointType                      m_VoronoiBoundaryOrigin{};
  std::vector<std::vector<int>>  m_CellNeighborsID{};

  std::vector<EdgeInfo>    m_LineList{};
  std::vector<VoronoiEdge> m_EdgeList{};
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkVoronoiDiagram2D.hxx"
#endif

#endif
