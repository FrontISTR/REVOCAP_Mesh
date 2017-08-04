puts "PWD=" + Dir.pwd
puts $:

require 'RevocapMesh'
require 'RevocapMeshGen'
require 'RevocapGL'
require 'RevocapIO'
require 'RevocapShape'

puts RevocapMesh::VERSION
puts RevocapMesh::COPYRIGHT
puts RevocapMeshGen::VERSION
puts RevocapMeshGen::COPYRIGHT
puts RevocapShape::VERSION
puts RevocapShape::COPYRIGHT
puts RevocapGL::VERSION
puts RevocapGL::COPYRIGHT
puts RevocapIO::VERSION
puts RevocapIO::COPYRIGHT

require 'test/unit'

class TestRevocapMesh < Test::Unit::TestCase

	def setup
	end

	def teardown
	end

	def test_exist_shape
		assert( RevocapShape )
		assert_instance_of( Module, RevocapShape )
		assert_instance_of( Class, RevocapShape::ShapeData )
		assert_instance_of( Class, RevocapShape::PatchGenerator )
		assert_instance_of( Class, RevocapShape::CADFileIO )
		shape = RevocapShape::ShapeData.new
		assert_instance_of( RevocapShape::ShapeData, shape )
	end

	def test_exist_mesh
		assert( RevocapMesh )
		assert_instance_of( Module, RevocapMesh )

		assert_instance_of( Class, RevocapMesh::BoundaryExtractor )
		bext = RevocapMesh::BoundaryExtractor.new
		assert_instance_of( RevocapMesh::BoundaryExtractor, bext )

		assert_instance_of( Class, RevocapMesh::Matching )
		matching = RevocapMesh::Matching.new
		assert_instance_of( RevocapMesh::Matching, matching )

		assert_instance_of( Class, RevocapMesh::MeshDB )
		mesh = RevocapMesh::MeshDB.new
		assert_instance_of( RevocapMesh::MeshDB, mesh )

		assert( RevocapMesh.const_defined?(:MeshData) )
		assert_instance_of( Class, RevocapMesh::MeshData )
		assert_kind_of( RevocapMesh::MeshData, mesh )

		assert_instance_of( Class, RevocapMesh::SurfaceOperation )
		surfOp = RevocapMesh::SurfaceOperation.new
		assert_instance_of( RevocapMesh::SurfaceOperation, surfOp )

		assert( defined?( RevocapMesh::MeshDB::getElementTypeMap ) )
	end

	def test_exist_gl
		assert( RevocapGL )
		assert_instance_of( Module, RevocapGL )

		assert_instance_of( Class, RevocapGL::MeshGL )
		meshGL = RevocapGL::MeshGL.new
		assert_instance_of( RevocapGL::MeshGL, meshGL )

		assert_instance_of( Class, RevocapGL::ColorMap )
		colorMap = RevocapGL::ColorMap.new
		assert_instance_of( RevocapGL::ColorMap, colorMap )
	end

	def test_exist_meshgen
		assert( RevocapMeshGen )
		assert_instance_of( Module, RevocapMeshGen )

		assert_instance_of( Class, RevocapMeshGen::PatchOperation )
		patchOp = RevocapMeshGen::PatchOperation.new
		assert_instance_of( RevocapMeshGen::PatchOperation, patchOp )

		assert_instance_of( Class, RevocapMeshGen::BLMGenerator )
		blm = RevocapMeshGen::BLMGenerator.new
		assert_instance_of( RevocapMeshGen::BLMGenerator, blm )

		assert( RevocapMeshGen.const_defined?(:MeshRefiner) )
		assert( RevocapMeshGen.const_defined?(:BLMGenerator) )
		assert( RevocapMeshGen.const_defined?(:PatchModifier) )
		assert( RevocapMeshGen.const_defined?(:EdgeStitcher) )
		assert( RevocapMeshGen.const_defined?(:MeshSmoother) )
	end

	def test_exist_io
		assert( RevocapIO )
		assert_instance_of( Module, RevocapIO )

		assert( RevocapIO.const_defined?(:TripatchPcmIO) )
		assert( RevocapIO.const_defined?(:TetMeshMIO) )
		assert( RevocapIO.const_defined?(:HecmwIO) )
		assert( RevocapIO.const_defined?(:FFbIO) )
		assert( RevocapIO.const_defined?(:RevocapCouplerIO) )
		assert( RevocapIO.const_defined?(:MicroAVSIO) )
		assert( RevocapIO.const_defined?(:STLIO) )
		assert( RevocapIO.const_defined?(:RevocapNeutralIO) )
	end

end
