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
!  subroutine rcapSetNode64(num,coords,globalIds,localIds) BIND(C,Name='rcapSetNode64')
!    use, intrinsic :: ISO_C_BINDING
!    implicit none
!    integer(kind=4), intent(in) :: num
!    type(c_ptr), value, intent(in) :: coords
!    type(c_ptr), intent(in) :: globalIds(:)
!    type(c_ptr), intent(in) :: localIds(:)
!  end subroutine
!  subroutine rcapSetNode32(num,coords,globalIds,localIds) BIND(C,Name='rcapSetNode32')
!    use, intrinsic :: ISO_C_BINDING
!    implicit none
!    integer(kind=4), intent(in) :: num
!    type(c_ptr), intent(in) :: coords(:)
!    type(c_ptr), intent(in) :: globalIds(:)
!    type(c_ptr), intent(in) :: localIds(:)
!  end subroutine
  function rcapGetNodeCount() BIND(C,Name='rcapGetNodeCount')
    implicit none
    integer(kind=4) rcapGetNodeCount
  end function
  function rcapGetRefineElementCount(num,etype) BIND(C,Name='rcapGetRefineElementCount')
    implicit none
    integer(kind=4) rcapGetRefineElementCount
    integer(kind=4), intent(in) :: num
    integer(kind=1), intent(in) :: etype
  end function
  subroutine rcapCommit() BIND(C,Name='rcapCommit')
  end subroutine
!  subroutine rcapAppendNodeGroup(dataname,num,nodeArray) BIND(C,Name='rcapAppendNodeGroup')
!    implicit none
!	character(len=1), intent(in) :: dataname
!    integer(kind=4), intent(in) :: num
!    integer(kind=4), intent(in) :: nodeArray(:)
!  end subroutine
  function rcapGetNodeGroupCount(dataname) BIND(C,Name='rcapGetNodeGroupCount')
    implicit none
    integer(kind=4) rcapGetNodeGroupCount
    character(len=1), intent(in) :: dataname
  end function
  subroutine rcapSetInterpolateMode(mode) BIND(C,Name='rcapSetInterpolateMode')
    implicit none
    character(len=1), intent(in) :: mode
  end subroutine
  subroutine rcapGetInterpolateMode(mode) BIND(C,Name='rcapGetInterpolateMode')
    implicit none
    character(len=1), intent(out) :: mode
  end subroutine
end interface
  external :: rcapSetNode64
  external :: rcapSetNode32
  external :: rcapAppendNodeGroup
end module
