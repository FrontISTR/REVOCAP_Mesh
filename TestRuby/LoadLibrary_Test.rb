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

	def test_exist
		assert( RevocapShape )
		assert_instance_of( Module, RevocapShape )

		assert_instance_of( Class, RevocapShape::ShapeData )
		assert_instance_of( Class, RevocapShape::PatchGenerator )

		assert_instance_of( Class, RevocapShape::CADFileIO )
		shape = RevocapShape::ShapeData.new
		assert_instance_of( RevocapShape::ShapeData, shape )

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

		assert_instance_of( Class, RevocapMesh::PatchOperation )
		patchOp = RevocapMesh::PatchOperation.new
		assert_instance_of( RevocapMesh::PatchOperation, patchOp )

		assert_instance_of( Class, RevocapMesh::SurfaceOperation )
		surfOp = RevocapMesh::SurfaceOperation.new
		assert_instance_of( RevocapMesh::SurfaceOperation, surfOp )

		assert( RevocapGL )
		assert_instance_of( Module, RevocapGL )

		assert_instance_of( Class, RevocapGL::MeshGL )
		meshGL = RevocapGL::MeshGL.new
		assert_instance_of( RevocapGL::MeshGL, meshGL )

		assert_instance_of( Class, RevocapGL::ColorMap )
		colorMap = RevocapGL::ColorMap.new
		assert_instance_of( RevocapGL::ColorMap, colorMap )

		assert( RevocapMeshGen )
		assert_instance_of( Module, RevocapMeshGen )

		assert_instance_of( Class, RevocapMeshGen::BLMGenerator )
		blm = RevocapMeshGen::BLMGenerator.new
		assert_instance_of( RevocapMeshGen::BLMGenerator, blm )

	end

end
