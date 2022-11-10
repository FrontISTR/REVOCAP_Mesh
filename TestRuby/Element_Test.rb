=begin
コンテナ機能・形状関数・補間関数のテスト
=end

require 'test/unit'
require 'RevocapMesh'
include RevocapMesh

class TestElement < Test::Unit::TestCase

	def setup
	end

	def teardown
	end

	def test_element
		mesh = MeshDB.new
		assert_equal( mesh.getBodyCount, 0 )
		mesh.beginModel
		mesh.beginElement
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getBodyCount, 1 )
		assert_equal( mesh.getElementCount(0), 0 )
	end

	def test_tetra
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(4)
		mesh.addNode( 0.0, 0.0, 0.0 )
		mesh.addNode( 1.0, 0.0, 0.0 )
		mesh.addNode( 0.0, 1.0, 0.0 )
		mesh.addNode( 0.0, 0.0, 1.0 )
		mesh.endNode
		mesh.beginElement(1)
		mesh.addElement("TETRAHEDRON",[0,1,2,3])
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getBodyCount, 1 )
		assert_equal( mesh.getNodeCount, 4 )
		assert_equal( mesh.getElementCount(0), 1 )
		assert_equal( mesh.getNode(0), [0.0, 0.0, 0.0] )
		assert_equal( mesh.getNode(1), [1.0, 0.0, 0.0] )
		assert_equal( mesh.getNode(2), [0.0, 1.0, 0.0] )
		assert_equal( mesh.getNode(3), [0.0, 0.0, 1.0] )
		assert_equal( mesh.getElement(0), ["TETRAHEDRON",0,1,2,3] )
	end

	def test_tetra_natural_coords
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(4)
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 1.0+rand*0.1 )
		mesh.endNode
		mesh.beginElement(1)
		mesh.addElement("TETRAHEDRON",[0,1,2,3])
		mesh.endElement
		mesh.endModel
		10.times{
			x = rand
			y = rand
			z = rand
			s,t,u = mesh.getNaturalCoordinates(0,x,y,z)
			x0,y0,z0 = mesh.getPhysicalCoordinates(0,s,t,u)
			s0,t0,u0 = mesh.getNaturalCoordinates(0,x0,y0,z0)
			assert_in_delta( x, x0, Float::EPSILON*64 )
			assert_in_delta( y, y0, Float::EPSILON*64 )
			assert_in_delta( z, z0, Float::EPSILON*64 )
			assert_in_delta( s, s0, Float::EPSILON*64 )
			assert_in_delta( t, t0, Float::EPSILON*64 )
			assert_in_delta( u, u0, Float::EPSILON*64 )
		}
	end

	def test_tetra2_natural_coords
		thresh = 1.0e-8
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(10)

		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 1.0+rand*0.1 )

		mesh.addNode( 0.5+rand*0.1, 0.5+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.5+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.5+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 0.5+rand*0.1 )
		mesh.addNode( 0.5+rand*0.1, 0.0+rand*0.1, 0.5+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.5+rand*0.1, 0.5+rand*0.1 )

		mesh.endNode
		mesh.beginElement(1)
		mesh.addElement("TETRAHEDRON2",[0,1,2,3,4,5,6,7,8,9])
		mesh.endElement
		mesh.endModel
		10.times{
			s = rand
			t = rand
			u = rand
			l = s + t + u
			s /= l
			t /= l
			u /= l
			x,y,z = mesh.getPhysicalCoordinates(0,s,t,u)
			s0,t0,u0 = mesh.getNaturalCoordinates(0,x,y,z)
			x0,y0,z0 = mesh.getPhysicalCoordinates(0,s0,t0,u0)
			assert_in_delta( x, x0, thresh )
			assert_in_delta( y, y0, thresh )
			assert_in_delta( z, z0, thresh )
			assert_in_delta( s, s0, thresh )
			assert_in_delta( t, t0, thresh )
			assert_in_delta( u, u0, thresh )
		}
	end

	def test_pyramid_natural_coords
		thresh = 1.0e-6
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(5)

		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )

		mesh.endNode
		mesh.beginElement(1)
		mesh.addElement("PYRAMID",[0,1,2,3,4])
		mesh.endElement
		mesh.endModel
		10.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			x,y,z = mesh.getPhysicalCoordinates(0,s,t,u)
			s0,t0,u0 = mesh.getNaturalCoordinates(0,x,y,z)
			x0,y0,z0 = mesh.getPhysicalCoordinates(0,s0,t0,u0)
			assert_in_delta( x, x0, thresh )
			assert_in_delta( y, y0, thresh )
			assert_in_delta( z, z0, thresh )
			assert_in_delta( s, s0, thresh )
			assert_in_delta( t, t0, thresh )
			assert_in_delta( u, u0, thresh )
		}
	end

	def test_wedge_natural_coords
		thresh = 1.0e-6
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(6)

		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 1.0+rand*0.1, 1.0+rand*0.1 )

		mesh.endNode
		mesh.beginElement(1)
		mesh.addElement("WEDGE",[0,1,2,3,4,5])
		mesh.endElement
		mesh.endModel
		10.times{
			s = rand
			t = rand
			l = s + t
			s /= l
			t /= l
			u = 2.0*rand - 1.0
			x,y,z = mesh.getPhysicalCoordinates(0,s,t,u)
			s0,t0,u0 = mesh.getNaturalCoordinates(0,x,y,z)
			x0,y0,z0 = mesh.getPhysicalCoordinates(0,s0,t0,u0)
			assert_in_delta( x, x0, thresh )
			assert_in_delta( y, y0, thresh )
			assert_in_delta( z, z0, thresh )
			assert_in_delta( s, s0, thresh )
			assert_in_delta( t, t0, thresh )
			assert_in_delta( u, u0, thresh )
		}
	end

	def hexaPoint(s,t,u,node)
		xyz = Array.new
		3.times{ |i|
			xyz << 0.125 * (
							(1-s)*(1-t)*(1-u)*node[0][i] +
							(1+s)*(1-t)*(1-u)*node[1][i] +
							(1+s)*(1+t)*(1-u)*node[2][i] +
							(1-s)*(1+t)*(1-u)*node[3][i] +
							(1-s)*(1-t)*(1+u)*node[4][i] +
							(1+s)*(1-t)*(1+u)*node[5][i] +
							(1+s)*(1+t)*(1+u)*node[6][i] +
							(1-s)*(1+t)*(1+u)*node[7][i] )
		}
		return xyz
	end

	def test_hexa_natural_coords
		thresh = 1.0e-6
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(12)
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 0.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 0.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 0.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 1.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 1.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 0.0+rand*0.1, 2.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 0.0+rand*0.1, 2.0+rand*0.1 )
		mesh.addNode( 1.0+rand*0.1, 1.0+rand*0.1, 2.0+rand*0.1 )
		mesh.addNode( 0.0+rand*0.1, 1.0+rand*0.1, 2.0+rand*0.1 )
		mesh.endNode
		mesh.beginElement(2)
		mesh.addElement("HEXAHEDRON",[0,1,2,3,4,5,6,7])
		mesh.addElement("HEXAHEDRON",[4,5,6,7,8,9,10,11])
		mesh.endElement
		mesh.endModel
		nodes = Array.new
		8.times{ |i|
			nodes << mesh.getNode(i)
		}
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			x,y,z = mesh.getPhysicalCoordinates(0,s,t,u)
			xyz = hexaPoint(s,t,u,nodes)
			s0,t0,u0 = mesh.getNaturalCoordinates(0,x,y,z)
			x0,y0,z0 = mesh.getPhysicalCoordinates(0,s0,t0,u0)
			s1,t1,u1 = mesh.getNaturalCoordinates( 1, x, y, z)
			x1,y1,z1 = mesh.getPhysicalCoordinates(1,s1,t1,u1)
			assert_in_delta( x, xyz[0], thresh )
			assert_in_delta( y, xyz[1], thresh )
			assert_in_delta( z, xyz[2], thresh )
			assert_in_delta( s, s0, thresh )
			assert_in_delta( t, t0, thresh )
			assert_in_delta( u, u0, thresh )
			assert_in_delta( x, x0, thresh )
			assert_in_delta( y, y0, thresh )
			assert_in_delta( z, z0, thresh )
			assert_in_delta( x, x1, thresh )
			assert_in_delta( y, y1, thresh )
			assert_in_delta( z, z1, thresh )
			assert( (1-u0)*(1+u1) <= 0 )
		}
	end

	def test_shape_segment
		assert_equal( RevocapMesh::MeshDB::shapeFunction("SEGMENT", [0] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("SEGMENT", [1] )[1], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("SEGMENT", [s] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_segment2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("SEGMENT2", [0] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("SEGMENT2", [1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("SEGMENT2", [0.5] )[2], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("SEGMENT", [s] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_triangle
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE", [0,0] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE", [1,0] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE", [0,1] )[2], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("TRIANGLE", [s,t] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_triangle2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [0,0] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [1,0] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [0,1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [0.5,0.5] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [  0,0.5] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [0.5,  0] )[5], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("TRIANGLE2", [s,t] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_quad
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD", [-1,-1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD", [ 1,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD", [ 1, 1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD", [-1, 1] )[3], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("QUAD", [s,t] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_quad2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [-1,-1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [ 1,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [ 1, 1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [-1, 1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [ 0,-1] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [ 1, 0] )[5], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [ 0, 1] )[6], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("QUAD2", [-1, 0] )[7], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("QUAD2", [s,t] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_tetrahedron
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON", [ 0, 0, 0] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON", [ 1, 0, 0] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON", [ 0, 1, 0] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON", [ 0, 0, 1] )[3], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_tetrahedron2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [ 0, 0, 0] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [ 1, 0, 0] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [ 0, 1, 0] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [ 0, 0, 1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [0.5,0.5,  0] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [  0,0.5,  0] )[5], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [0.5,  0,  0] )[6], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [  0,  0,0.5] )[7], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [0.5,  0,0.5] )[8], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [  0,0.5,0.5] )[9], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("TETRAHEDRON2", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_hexahedron
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [-1,-1,-1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [ 1,-1,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [ 1, 1,-1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [-1, 1,-1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [-1,-1, 1] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [ 1,-1, 1] )[5], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [ 1, 1, 1] )[6], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [-1, 1, 1] )[7], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_hexahedron2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1,-1,-1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1,-1,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1, 1,-1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1, 1,-1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1,-1, 1] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1,-1, 1] )[5], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1, 1, 1] )[6], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1, 1, 1] )[7], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 0,-1,-1] )[8], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1, 0,-1] )[9], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 0, 1,-1] )[10], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1, 0,-1] )[11], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 0,-1, 1] )[12], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1, 0, 1] )[13], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 0, 1, 1] )[14], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1, 0, 1] )[15], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1,-1, 0] )[16], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1,-1, 0] )[17], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [ 1, 1, 0] )[18], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [-1, 1, 0] )[19], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("HEXAHEDRON2", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_wedge
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE", [ 0, 0,-1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE", [ 1, 0,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE", [ 0, 1,-1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE", [ 0, 0, 1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE", [ 1, 0, 1] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE", [ 0, 1, 1] )[5], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("WEDGE", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_wedge2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [ 0, 0,-1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [ 1, 0,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [ 0, 1,-1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [ 0, 0, 1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [ 1, 0, 1] )[4], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [ 0, 1, 1] )[5], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [0.5,0.5,-1] )[6], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [  0,0.5,-1] )[7], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [0.5,  0,-1] )[8], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [0.5,0.5, 1] )[9], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [  0,0.5, 1] )[10], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [0.5,  0, 1] )[11], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [  0,  0, 0] )[12], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [  1,  0, 0] )[13], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("WEDGE2", [  0,  1, 0] )[14], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("WEDGE2", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_pyramid
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID", [ 0, 0, 1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID", [-1,-1,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID", [ 1,-1,-1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID", [ 1, 1,-1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID", [-1, 1,-1] )[4], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("PYRAMID", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_shape_pyramid2
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 0, 0, 1] )[0], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-1,-1,-1] )[1], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 1,-1,-1] )[2], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 1, 1,-1] )[3], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-1, 1,-1] )[4], 1.0 )
#		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-0.5,-0.5, 0] )[5], 1.0 )
#		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 0.5,-0.5, 0] )[6], 1.0 )
#		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 0.5, 0.5, 0] )[7], 1.0 )
#		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-0.5, 0.5, 0] )[8], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-1.0,-1.0, 0] )[5], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 1.0,-1.0, 0] )[6], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 1.0, 1.0, 0] )[7], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-1.0, 1.0, 0] )[8], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 0,-1,-1] )[9], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 1, 0,-1] )[10], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [ 0, 1,-1] )[11], 1.0 )
		assert_equal( RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [-1, 0,-1] )[12], 1.0 )
		100.times{
			s = 2.0*rand - 1.0
			t = 2.0*rand - 1.0
			u = 2.0*rand - 1.0
			vals = RevocapMesh::MeshDB::shapeFunction("PYRAMID2", [s,t,u] )
			sum = 0.0
			vals.each{ |v|
				sum += v
			}
			assert_in_delta( sum, 1.0, 1.0e-10 )
		}
	end

	def test_face
		mesh = RevocapMesh::MeshDB.new
		mesh.beginModel
		mesh.beginNode(4)
		mesh.addNode( 0.0, 0.0, 0.0 )
		mesh.addNode( 1.0, 0.0, 0.0 )
		mesh.addNode( 0.0, 1.0, 0.0 )
		mesh.addNode( 0.0, 0.0, 1.0 )
		mesh.endNode
		mesh.beginElement(1)
		mesh.addElement("TETRAHEDRON",[0,1,2,3])
		mesh.endElement
		mesh.endModel
		assert( mesh.elementToFace("TRIANGLE",[1,2,3],0) == [0,0] )
		assert( mesh.elementToFace("TRIANGLE",[2,3,1],0) == [0,0] )
		assert( mesh.elementToFace("TRIANGLE",[3,1,2],0) == [0,0] )

		assert( mesh.elementToFace("TRIANGLE",[0,3,2],0) == [0,1] )
		assert( mesh.elementToFace("TRIANGLE",[3,2,0],0) == [0,1] )
		assert( mesh.elementToFace("TRIANGLE",[2,0,3],0) == [0,1] )

		assert( mesh.elementToFace("TRIANGLE",[0,1,3],0) == [0,2] )
		assert( mesh.elementToFace("TRIANGLE",[1,3,0],0) == [0,2] )
		assert( mesh.elementToFace("TRIANGLE",[3,0,1],0) == [0,2] )

		assert( mesh.elementToFace("TRIANGLE",[0,2,1],0) == [0,3] )
		assert( mesh.elementToFace("TRIANGLE",[2,1,0],0) == [0,3] )
		assert( mesh.elementToFace("TRIANGLE",[1,0,2],0) == [0,3] )
	end
end
