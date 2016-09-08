!/*----------------------------------------------------------------------
!#                                                                      #
!# Software Name : REVOCAP_Refiner version 1.2                          #
!# Program Name : rcapRefiner                                           #
!#                                                                      #
!#                                Written by                            #
!#                                           K. Tokunaga 2016/06/18     #
!#                                                                      #
!#      Contact Address: IIS, The University of Tokyo CISS              #
!#                                                                      #
!#      "Large Scale Assembly, Structural Correspondence,               #
!#                                     Multi Dynamics Simulator"        #
!#                                                                      #
!----------------------------------------------------------------------*/
module RcapRefiner
  use, intrinsic :: ISO_C_BINDING
  implicit none
  integer(kind=1), parameter :: RCAP_UNKNOWNTYPE = -1
  integer(kind=1), parameter :: RCAP_SEGMENT = 0
  integer(kind=1), parameter :: RCAP_SEGMENT2 = 1
  integer(kind=1), parameter :: RCAP_TRIANGLE = 2
  integer(kind=1), parameter :: RCAP_TRIANGLE2 = 3
  integer(kind=1), parameter :: RCAP_QUAD = 4
  integer(kind=1), parameter :: RCAP_QUAD2 = 5
  integer(kind=1), parameter :: RCAP_TETRAHEDRON = 6
  integer(kind=1), parameter :: RCAP_TETRAHEDRON2 = 7
  integer(kind=1), parameter :: RCAP_WEDGE = 8
  integer(kind=1), parameter :: RCAP_WEDGE2 = 9
  integer(kind=1), parameter :: RCAP_PYRAMID = 10
  integer(kind=1), parameter :: RCAP_PYRAMID2 = 11
  integer(kind=1), parameter :: RCAP_HEXAHEDRON = 12
  integer(kind=1), parameter :: RCAP_HEXAHEDRON2 = 13
  integer(kind=2), parameter :: RCAP_TETRAFACES(12) = (/ &
    2, 3, 4, &
    1, 4, 3, &
    1, 2, 4, &
    1, 3, 2 /)
  integer(kind=2), parameter :: RCAP_HEXAFACES(24) = (/ &
    4, 3, 2, 1, &
    5, 6, 7, 8, &
    2, 6, 5, 1, &
    2, 3, 7, 6, &
    4, 8, 7, 3, &
    5, 8, 4, 1 /)
  integer(kind=2), parameter :: RCAP_PYRAFACES(20) = (/ &
    1, 2, 3,-1, &
    1, 3, 4,-1, &
    1, 4, 5,-1, &
    1, 5, 2,-1, &
    5, 4, 3, 2 /)
  integer(kind=2), parameter :: RCAP_WEDGEFACES(20) = (/ &
    1, 3, 2,-1, &
    4, 5, 6,-1, &
    1, 2, 5, 4, &
    2, 3, 6, 5, &
    3, 1, 4, 6 /)
interface
  subroutine rcapGetVersion() BIND(C,Name='rcapGetVersion')
  end subroutine
  subroutine rcapInitRefiner(nodeOffset,elementOffset) BIND(C,Name='rcapInitRefiner')
    implicit none
    integer(kind=4), intent(in) :: nodeOffset
    integer(kind=4), intent(in) :: elementOffset
  end subroutine
  subroutine rcapClearRefiner() BIND(C,Name='rcapClearRefiner')
  end subroutine
  subroutine rcapTermRefiner() BIND(C,Name='rcapTermRefiner')
  end subroutine
  subroutine rcapSetCADFilename(filename) BIND(C,Name='rcapSetCADFilename')
    implicit none
    character(len=1), intent(in) :: filename
  end subroutine
  subroutine rcapSetSecondFitting(flag) BIND(C,Name='rcapSetSecondFitting')
    implicit none
    integer(kind=4), intent(in) :: flag
  end subroutine
  subroutine rcapSetSmoothing(flag) BIND(C,Name='rcapSetSmoothing')
    implicit none
    integer(kind=4), intent(in) :: flag
  end subroutine
  subroutine rcapSetPartitionFilename(filename) BIND(C,Name='rcapSetPartitionFilename')
    implicit none
    character(len=1), intent(in) :: filename
  end subroutine
  subroutine rcapSetNode64(num,coords,globalIds,localIds) BIND(C,Name='rcapSetNode64')
    implicit none
    integer(kind=4), intent(in) :: num
    real(kind=8), dimension(3*num), intent(in) :: coords
    integer(kind=4), dimension(num), intent(in) :: globalIds
    integer(kind=4), dimension(num), intent(in) :: localIds
  end subroutine
  subroutine rcapSetNode32(num,coords,globalIds,localIds) BIND(C,Name='rcapSetNode32')
    implicit none
    integer(kind=4), intent(in) :: num
    real(kind=4), dimension(3*num), intent(in) :: coords
    integer(kind=4), dimension(num), intent(in) :: globalIds
    integer(kind=4), dimension(num), intent(in) :: localIds
  end subroutine
  subroutine rcapGetNode64(num,localIds,coords) BIND(C,Name='rcapGetNode64')
    implicit none
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: localIds
    real(kind=8), dimension(3*num), intent(out) :: coords
  end subroutine
  subroutine rcapGetNode32(num,localIds,coords) BIND(C,Name='rcapGetNode32')
    implicit none
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: localIds
    real(kind=4), dimension(3*num), intent(out) :: coords
  end subroutine
  subroutine rcapGetNodeSeq64(num,initId,coords) BIND(C,Name='rcapGetNodeSeq64')
    implicit none
    integer(kind=4), intent(in) :: num
    integer(kind=4), intent(in) :: initId
    real(kind=8), dimension(3*num), intent(out) :: coords
  end subroutine
  subroutine rcapGetNodeSeq32(num,initId,coords) BIND(C,Name='rcapGetNodeSeq32')
    implicit none
    integer(kind=4), intent(in) :: num
    integer(kind=4), intent(in) :: initId
    real(kind=4), dimension(3*num), intent(out) :: coords
  end subroutine
  function rcapRefineElement(num,etype,nodeArray,resultNodeArray) BIND(C,Name='rcapRefineElement')
    implicit none
    integer(kind=4) rcapRefineElement
    integer(kind=4), intent(in) :: num
    integer(kind=1), intent(in) :: etype
    integer(kind=4), dimension(*), intent(in) :: nodeArray
    integer(kind=4), dimension(*), intent(out) :: resultNodeArray
  end function
  function rcapGetRefineElementCount(num,etype) BIND(C,Name='rcapGetRefineElementCount')
    implicit none
    integer(kind=4) rcapGetRefineElementCount
    integer(kind=4), intent(in) :: num
    integer(kind=1), intent(in) :: etype
  end function
  function rcapRefineElementMulti(num,etypeArray,nodeArray,refinedNum, &
    resultEtypeArray,resultNodeArray) BIND(C,Name='rcapRefineElementMulti')
    implicit none
    integer(kind=4) rcapRefineElementMulti
    integer(kind=4), intent(in) :: num
    integer(kind=1), dimension(*), intent(in) :: etypeArray
    integer(kind=4), dimension(*), intent(in) :: nodeArray
    integer(kind=4), intent(out) :: refinedNum
    integer(kind=1), dimension(*), intent(out) :: resultEtypeArray
    integer(kind=4), dimension(*), intent(out) :: resultNodeArray
  end function
  function rcapGetRefineElementMultiCount(num,etypeArray,refinedNum) BIND(C,Name='rcapGetRefineElementMultiCount')
    implicit none
    integer(kind=4) rcapGetRefineElementMultiCount
    integer(kind=4), intent(in) :: num
    integer(kind=1), dimension(*), intent(in) :: etypeArray
    integer(kind=4), intent(out) :: refinedNum
  end function
  function rcapGetNodeCount() BIND(C,Name='rcapGetNodeCount')
    implicit none
    integer(kind=4) rcapGetNodeCount
  end function
  subroutine rcapCommit() BIND(C,Name='rcapCommit')
  end subroutine
  subroutine rcapAppendNodeGroup_C(dataname,num,nodeArray) BIND(C,Name='rcapAppendNodeGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: nodeArray
  end subroutine
  function rcapGetNodeGroupCount_C(dataname) BIND(C,Name='rcapGetNodeGroupCount')
    implicit none
    integer(kind=4) rcapGetNodeGroupCount_C
    character(len=1), intent(in) :: dataname
  end function
  subroutine rcapGetNodeGroup_C(dataname,num,nodeArray) BIND(C,Name='rcapGetNodeGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: nodeArray
  end subroutine
  subroutine rcapAppendBNodeGroup_C(dataname,num,nodeArray) BIND(C,Name='rcapAppendBNodeGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: nodeArray
  end subroutine
  function rcapGetBNodeGroupCount_C(dataname) BIND(C,Name='rcapGetBNodeGroupCount')
    implicit none
    integer(kind=4) rcapGetBNodeGroupCount_C
    character(len=1), intent(in) :: dataname
  end function
  subroutine rcapGetBNodeGroup_C(dataname,num,nodeArray) BIND(C,Name='rcapGetBNodeGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: nodeArray
  end subroutine
  subroutine rcapAppendBNodeVarInt_C(dataname,num,nodeArray,nodeVars) BIND(C,Name='rcapAppendBNodeVarInt')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: nodeArray
    integer(kind=4), dimension(num), intent(in) :: nodeVars
  end subroutine
  function rcapGetBNodeVarIntCount_C(dataname) BIND(C,Name='rcapGetBNodeVarIntCount')
    implicit none
    integer(kind=4) rcapGetBNodeVarIntCount_C
    character(len=1), intent(in) :: dataname
  end function
  subroutine rcapGetBNodeVarInt_C(dataname,num,nodeArray,nodeVars) BIND(C,Name='rcapGetBNodeVarInt')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: nodeArray
    integer(kind=4), dimension(num), intent(in) :: nodeVars
  end subroutine
  subroutine rcapAppendElementGroup_C(dataname,num,elementArray) BIND(C,Name='rcapAppendElementGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: elementArray
  end subroutine
  function rcapGetElementGroupCount_C(dataname) BIND(C,Name='rcapGetElementGroupCount')
    implicit none
    integer(kind=4) rcapGetElementGroupCount_C
    character(len=1), intent(in) :: dataname
  end function
  subroutine rcapGetElementGroup_C(dataname,num,elementArray) BIND(C,Name='rcapGetElementGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: elementArray
  end subroutine
  subroutine rcapAppendFaceGroup_C(dataname,num,faceArray) BIND(C,Name='rcapAppendFaceGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(2*num), intent(in) :: faceArray
  end subroutine
  function rcapGetFaceGroupCount_C(dataname) BIND(C,Name='rcapGetFaceGroupCount')
    implicit none
    integer(kind=4) rcapGetFaceGroupCount_C
    character(len=1), intent(in) :: dataname
  end function
  subroutine rcapGetFaceGroup_C(dataname,num,faceArray) BIND(C,Name='rcapGetFaceGroup')
    implicit none
    character(len=1), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(2*num), intent(out) :: faceArray
  end subroutine
  function rcapGetOriginal(localNodeId,originalNodeArray) BIND(C,Name='rcapGetOriginal')
    implicit none
    integer(kind=1) rcapGetOriginal
    integer(kind=4), intent(in) :: localNodeId
    integer(kind=4), dimension(8), intent(out) :: originalNodeArray
  end function
  function rcapGetMiddle(etype,originalNodeArray) BIND(C,Name='rcapGetMiddle')
    implicit none
    integer(kind=4) rcapGetMiddle
    integer(kind=1), intent(in) :: etype
    integer(kind=4), dimension(8), intent(in) :: originalNodeArray
  end function
  subroutine rcapSetInterpolateMode_C(mode) BIND(C,Name='rcapSetInterpolateMode')
    implicit none
    character(len=1), intent(in) :: mode
  end subroutine
  subroutine rcapGetInterpolateMode(mode) BIND(C,Name='rcapGetInterpolateMode')
    implicit none
    character(len=1), intent(out) :: mode
  end subroutine
  subroutine rcapQualityReport_C(mode,filename) BIND(C,Name='rcapQualityReport')
    implicit none
    character(len=1), intent(in) :: mode
    character(len=1), intent(in) :: filename
  end subroutine
  function rcapLoadGFFile_C(gffile,bounfile) BIND(C,Name='rcapLoadGFFile')
    implicit none
    integer(kind=4) rcapLoadGFFile_C 
    character(len=1), intent(in) :: gffile
    character(len=1), intent(in) :: bounfile
  end function
  function rcapLoadHECFile_C(hecfile) BIND(C,Name='rcapLoadHECFile')
    implicit none
    integer(kind=4) rcapLoadHECFile_C 
    character(len=1), intent(in) :: hecfile
  end function
  function rcapSaveGFFile_C(gffile,bounfile) BIND(C,Name='rcapSaveGFFile')
    implicit none
    integer(kind=4) rcapSaveGFFile_C 
    character(len=1), intent(in) :: gffile
    character(len=1), intent(in) :: bounfile
  end function
  function rcapSaveHECFile_C(hecfile) BIND(C,Name='rcapSaveHECFile')
    implicit none
    integer(kind=4) rcapSaveHECFile_C 
    character(len=1), intent(in) :: hecfile
  end function
  function rcapSaveRNFFile_C(rnffile) BIND(C,Name='rcapSaveRNFFile')
    implicit none
    integer(kind=4) rcapSaveRNFFile_C 
    character(len=1), intent(in) :: rnffile
  end function
  subroutine rcapRefineFFbModel() BIND(C,Name='rcapRefineFFbModel')
    implicit none
  end subroutine
end interface
contains
  function cstring(string)
    use Iso_C_Binding
    character(*,C_char),intent(In) :: string
    character(:,C_char),allocatable :: cstring
    cstring = trim(string)//C_null_char
  end Function
  subroutine rcapAppendNodeGroup(dataname,num,nodeArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: nodeArray
    call rcapAppendNodeGroup_C(cstring(dataname),num,nodeArray)
  end subroutine
  function rcapGetNodeGroupCount(dataname)
    implicit none
    integer(kind=4) rcapGetNodeGroupCount
    character(len=*), intent(in) :: dataname
    rcapGetNodeGroupCount = rcapGetNodeGroupCount_C(cstring(dataname))
  end function
  subroutine rcapGetNodeGroup(dataname,num,nodeArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: nodeArray
    call rcapGetNodeGroup_C(cstring(dataname),num,nodeArray)
  end subroutine
  subroutine rcapAppendBNodeGroup(dataname,num,nodeArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: nodeArray
    call rcapAppendBNodeGroup_C(cstring(dataname),num,nodeArray)
  end subroutine
  function rcapGetBNodeGroupCount(dataname)
    implicit none
    integer(kind=4) rcapGetBNodeGroupCount
    character(len=*), intent(in) :: dataname
    rcapGetBNodeGroupCount = rcapGetBNodeGroupCount_C(cstring(dataname))
  end function
  subroutine rcapGetBNodeGroup(dataname,num,nodeArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: nodeArray
    call rcapGetBNodeGroup_C(cstring(dataname),num,nodeArray)
  end subroutine
  subroutine rcapAppendBNodeVarInt(dataname,num,nodeArray,nodeVars)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: nodeArray
    integer(kind=4), dimension(num), intent(in) :: nodeVars
    call rcapAppendBNodeVarInt_C(cstring(dataname),num,nodeArray,nodeVars)
  end subroutine
  function rcapGetBNodeVarIntCount(dataname)
    implicit none
    integer(kind=4) rcapGetBNodeVarIntCount
    character(len=*), intent(in) :: dataname
    rcapGetBNodeVarIntCount = rcapGetBNodeVarIntCount_C(cstring(dataname))
  end function
  subroutine rcapGetBNodeVarInt(dataname,num,nodeArray,nodeVars)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: nodeArray
    integer(kind=4), dimension(num), intent(in) :: nodeVars
    call rcapGetBNodeVarInt_C(cstring(dataname),num,nodeArray,nodeVars)
  end subroutine
  subroutine rcapAppendElementGroup(dataname,num,elementArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(in) :: elementArray
    call rcapAppendElementGroup_C(cstring(dataname),num,elementArray)
  end subroutine
  function rcapGetElementGroupCount(dataname)
    implicit none
    integer(kind=4) rcapGetElementGroupCount
    character(len=*), intent(in) :: dataname
    rcapGetElementGroupCount = rcapGetElementGroupCount_C(cstring(dataname))
  end function
  subroutine rcapGetElementGroup(dataname,num,elementArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(num), intent(out) :: elementArray
    call rcapGetElementGroup_C(cstring(dataname),num,elementArray)
  end subroutine
  subroutine rcapAppendFaceGroup(dataname,num,faceArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(2*num), intent(in) :: faceArray
    call rcapAppendFaceGroup_C(cstring(dataname),num,faceArray)
  end subroutine
  function rcapGetFaceGroupCount(dataname)
    implicit none
    integer(kind=4) rcapGetFaceGroupCount
    character(len=*), intent(in) :: dataname
    rcapGetFaceGroupCount = rcapGetFaceGroupCount_C(cstring(dataname))
  end function
  subroutine rcapGetFaceGroup(dataname,num,faceArray)
    implicit none
    character(len=*), intent(in) :: dataname
    integer(kind=4), intent(in) :: num
    integer(kind=4), dimension(2*num), intent(out) :: faceArray
    call rcapGetFaceGroup_C(cstring(dataname),num,faceArray)
  end subroutine
  subroutine rcapSetInterpolateMode(mode)
    implicit none
    character(len=*), intent(in) :: mode
    call rcapSetInterpolateMode_C(cstring(mode))
  end subroutine
  subroutine rcapQualityReport(mode,filename)
    implicit none
    character(len=*), intent(in) :: mode
    character(len=*), intent(in) :: filename
    call rcapQualityReport_C(cstring(mode),cstring(filename))
  end subroutine
  function rcapLoadGFFile(gffile,bounfile)
    implicit none
    integer(kind=4) rcapLoadGFFile
    character(len=*), intent(in) :: gffile
    character(len=*), intent(in) :: bounfile
    rcapLoadGFFile = rcapLoadGFFile_C(cstring(gffile),cstring(bounfile))
  end function
  function rcapLoadHECFile(hecfile)
    implicit none
    integer(kind=4) rcapLoadHECFile
    character(len=*), intent(in) :: hecfile
    rcapLoadHECFile = rcapLoadHECFile_C(cstring(hecfile))
  end function
  function rcapSaveGFFile(gffile,bounfile)
    implicit none
    integer(kind=4) rcapSaveGFFile 
    character(len=*), intent(in) :: gffile
    character(len=*), intent(in) :: bounfile
    rcapSaveGFFile = rcapSaveGFFile_C(cstring(gffile),cstring(bounfile))
  end function
  function rcapSaveHECFile(hecfile)
    implicit none
    integer(kind=4) rcapSaveHECFile
    character(len=*), intent(in) :: hecfile
    rcapSaveHECFile = rcapSaveHECFile_C(cstring(hecfile))
  end function
  function rcapSaveRNFFile(rnffile)
    implicit none
    integer(kind=4) rcapSaveRNFFile
    character(len=*), intent(in) :: rnffile
    rcapSaveRNFFile = rcapSaveRNFFile_C(cstring(rnffile))
  end function
end module
