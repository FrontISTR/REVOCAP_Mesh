require 'test/unit'
require 'RevocapMesh'
include RevocapMesh

class TestBoundaryExtract < Test::Unit::TestCase

	def setup
	end

	def teardown
	end

	def test_exist
		assert( BoundaryExtractor )
		assert_instance_of( Class, BoundaryExtractor )
		bext = BoundaryExtractor.new
		assert_instance_of( BoundaryExtractor, bext )
	end

	def test_extract_boundary
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode
		8.times{
			mesh.addNode( rand, rand, rand )
		}
		mesh.endNode
		mesh.beginElement
		mesh.addElement( "TRIANGLE", [0,1,2] )
		mesh.addElement( "TRIANGLE", [0,2,3] )
		mesh.addElement( "TRIANGLE", [4,5,6] )
		mesh.addElement( "TRIANGLE", [4,6,7] )
		mesh.endElement
		mesh.endModel
		bext = BoundaryExtractor.new
		bext.setMesh(mesh)
		bext.appendBody(0)
		boundId = bext.getBoundary(0)
		bext.clear
		assert_equal( boundId, 1 )
		assert_equal( mesh.getElementCount(boundId), 8 )
		assert_equal( mesh.getElementCountByType(boundId)['SEGMENT'], 8 )
	end

	def test_extract_elementgroup
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode
		8.times{
			mesh.addNode( rand, rand, rand )
		}
		mesh.endNode
		mesh.beginElement
		mesh.addElement( "TRIANGLE", [0,1,2] )
		mesh.addElement( "TRIANGLE", [0,2,3] )
		mesh.addElement( "TRIANGLE", [4,5,6] )
		mesh.addElement( "TRIANGLE", [4,6,7] )
		mesh.endElement
		mesh.createData( "EG", "ELEMENTGROUP", "NONE", "", 0 )
		mesh.addId( "EG", 0 )
		mesh.addId( "EG", 1 )
		mesh.endModel
		bext = BoundaryExtractor.new
		bext.setMesh(mesh)
		bext.appendData( "EG" )
		boundId = bext.getBoundary(0)
		bext.clear
		assert_equal( boundId, 1 )
		assert_equal( mesh.getElementCount(boundId), 4 )
		assert_equal( mesh.getElementCountByType(boundId)['SEGMENT'], 4 )
	end

	def test_extract_erase
		require 'RevocapIO'
		yamlIO = RevocapIO::RevocapNeutralIO.new
		mesh = MeshDB.new
		mesh.beginModel
		yamlIO.loadFromRNFFile(File.join(File.dirname($0),'boxMesh.rnf'),mesh)
		mesh.createData("EG","ELEMENTGROUP","NONE","",0)
		mesh.addId("EG",2)
		mesh.addId("EG",4)
		mesh.createData("FG","FACEGROUP","NONE","",0)
		mesh.addId("FG",[3,1])
		mesh.addId("FG",[3,2])
		mesh.endModel
		bext = BoundaryExtractor.new
		bext.setMesh(mesh)
		bext.appendBody(0)
		bext.eraseData("EG")
		surfId0 = bext.getBoundary(0)
		assert_equal( mesh.getElementCount( surfId0 ), 9 )
		bext.eraseData("FG")
		surfId1 = bext.getBoundary(0)
		assert_equal( mesh.getElementCount( surfId1 ), 7 )
	end

	def test_extract_surface
		require 'RevocapIO'
		yamlIO = RevocapIO::RevocapNeutralIO.new
		mesh = MeshDB.new
		mesh.beginModel
		yamlIO.loadFromRNFFile(File.join(File.dirname($0),'boxMesh.rnf'),mesh)
		mesh.endModel
		bext = BoundaryExtractor.new
		bext.setMesh(mesh)
		bext.appendBody(0)
		sdiv = SurfaceOperation.new
		sdiv.setMesh(mesh)
		surfId0 = bext.getBoundary(0)
		bext.clear
		assert_equal( mesh.getElementCount(surfId0), 12 )
		dividedIds = sdiv.divideSurface(surfId0,120)
		assert_equal( dividedIds.size, 6 )
		dividedIds.each{ |id|
			assert_equal( mesh.getElementCount(id), 2 )
		}
		bext.setMesh(mesh)
		bext.appendBody(1)
		surfId1 = bext.getBoundary(1)
		bext.clear
		assert_equal( mesh.getElementCount(surfId1), 12 )
	end

	def test_extract_surface_hex
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginElement(2)
		mesh.addElement("HEXAHEDRON",[0,1,2,3,4,5,6,7])
		mesh.addElement("HEXAHEDRON",[1,8,9,2,5,10,11,6])
		mesh.endElement
		mesh.endModel
		bext = BoundaryExtractor.new
		bext.setMesh(mesh)
		bext.appendBody(0)
		surfId = bext.getBoundary(0)
		assert_equal( mesh.getElementCount(surfId), 10 )
		assert_equal( mesh.getElementCountByType(surfId)['QUAD'], 10 )
	end

	def test_closed_check
		require 'RevocapIO'
		tripatchIO = RevocapIO::TripatchPcmIO.new
		mesh = MeshDB.new
		mesh.beginModel
		tripatchIO.loadFromFile( File.join(File.dirname($0),'bridge.pcm'), mesh )
		mesh.endModel
		bext = BoundaryExtractor.new
		bext.setMesh(mesh)
		bext.appendBody(0)
		assert( bext.isClosed(0) )
	end

=begin
	# 複数領域対応版
	def test_extract_surface2
		require 'RevocapIO'
		tetmesh = RevocapIO::TetMeshMIO.new
		mesh = MeshDB.new
		tetmesh.loadFromFile( File.join(File.dirname($0),'..','TestData','cakecs.msh'),mesh)
		bext = BoundaryExtractor.new
		surf = Array.new
		# ４領域それぞれを境界抽出
		4.times{ |i|
			bext.setMesh(mesh)
			bext.appendBody(i)
			surf << bext.getBoundary(i)
			bext.clear
		}
		assert( mesh.getElementCount(surf[0]) == 1854 )
		assert( mesh.getElementCount(surf[1]) == 1192 )
		assert( mesh.getElementCount(surf[2]) == 1008 )
		assert( mesh.getElementCount(surf[3]) == 724 )
		# 表面分割
		sdiv = SurfaceOperation.new
		sdiv.setMesh(mesh)
		surfs0 = sdiv.divideSurfaceWithRidge(surf[0],120) # key = surface, value = edge の Hash
		surfs1 = sdiv.divideSurfaceWithRidge(surf[1],120)
		surfs2 = sdiv.divideSurfaceWithRidge(surf[2],120)
		surfs3 = sdiv.divideSurfaceWithRidge(surf[3],120)
		# surf0 と surf1 には一致する面がある
		matching = Matching.new
		flag = false
		surfs0.each{ |k0,v0|
			surfs1.each{ |k1,v1|
				if matching.getSurfaceRelation( mesh, k0, k1 ) == -1
					flag = true
				end
			}
		}
		assert( flag )
		flag = false
		surfs1.each{ |k1,v1|
			surfs2.each{ |k2,v2|
				if matching.getSurfaceRelation( mesh, k1, k2 ) == -1
					flag = true
				end
			}
		}
		assert( flag )
		flag = false
		surfs2.each{ |k2,v2|
			surfs3.each{ |k3,v3|
				if matching.getSurfaceRelation( mesh, k2, k3 ) == -1
					flag = true
				end
			}
		}
		assert( flag )
		# 面ではなく、Face で抽出
		sdiv.setMesh(mesh)
		bext.setMesh(mesh)
		bext.appendBody(0)
		bext.getBoundaryFace(0,"face0")
		bext.clear
		bext.setMesh(mesh)
		bext.appendBody(1)
		bext.getBoundaryFace(1,"face1")
		bext.clear
		bext.setMesh(mesh)
		bext.appendBody(2)
		bext.getBoundaryFace(2,"face2")
		bext.clear
		bext.setMesh(mesh)
		bext.appendBody(3)
		bext.getBoundaryFace(3,"face3")
		bext.clear
		assert( mesh.getDataMode("face0") == "FACEGROUP" )
		assert( mesh.getDataMode("face1") == "FACEGROUP" )
		assert( mesh.getDataMode("face2") == "FACEGROUP" )
		assert( mesh.getDataMode("face3") == "FACEGROUP" )
		faces0 = sdiv.divideFaceGroupWithRidge("face0",120)
		faces1 = sdiv.divideFaceGroupWithRidge("face1",120)
		faces2 = sdiv.divideFaceGroupWithRidge("face2",120)
		faces3 = sdiv.divideFaceGroupWithRidge("face3",120)
		flag = false
		faces0.each{ |k0,v0|
			faces1.each{ |k1,v1|
				if matching.getFaceRelation( mesh, k0, k1 ) == -1
					flag = true
				end
			}
		}
		assert( flag )
		flag = false
		faces1.each{ |k0,v0|
			faces2.each{ |k1,v1|
				if matching.getFaceRelation( mesh, k0, k1 ) == -1
					flag = true
				end
			}
		}
		assert( flag )
		flag = false
		faces2.each{ |k0,v0|
			faces3.each{ |k1,v1|
				if matching.getFaceRelation( mesh, k0, k1 ) == -1
					flag = true
				end
			}
		}
		assert( flag )
	end
=end
	# もともとの面について、その要素の位相を取り替えたときに、面情報を更新する
	# fg0 - delElem + appendElem => reface
	def test_remesh
		require 'RevocapMesh'
		require 'RevocapIO'
		mesh = RevocapMesh::MeshDB.new
		mesh.setDefaultContainerType("Pre")
		tetmesh = RevocapIO::TetMeshMIO.new
		mesh.beginModel
		tetmesh.loadFromFile( File.join(File.dirname($0),'Box27Tetra.msh'), mesh )
		mesh.endModel

		assert_equal( mesh.getElementCount(0), 162 )

		bc = RevocapMesh::BoundaryExtractor.new
		bc.setMesh( mesh )
		bc.appendBody( 0 )
		wholeFaceName = "WholeFace"
		bc.getBoundaryFace( 0, wholeFaceName )
		bc.clear
		divider = RevocapMesh::SurfaceOperation.new
		divider.setMesh( mesh )
		faces = divider.divideFaceGroup(wholeFaceName,120.0)
		mesh.deleteData( wholeFaceName )

		fg0 = nil
		faces.each{|fg|
			mesh.eachId(fg){|f|
				if f[0] == 26
					fg0 = fg
					break
				end
			}
		}

		assert_equal( mesh.getIdCount(fg0), 18 )

		# 面の中心に接する６面体１つ分のテトラ６個の削除

		mesh.createData("delElem","ELEMENTGROUP","NONE","",0)
		(24..29).each{|i|
			mesh.addId("delElem",i)
		}
		assert_equal( mesh.getIdCount("delElem"), 6 )

		# 面の中心に接する６面体１つ分のテトラ６個の削除

		mesh.createData("appendElem","ELEMENTGROUP","NONE","",0)
		id0 = mesh.insertElement( 0, "TETRAHEDRON", [6, 9, 5, 26] )
		mesh.addId("appendElem",id0)
		id1 = mesh.insertElement( 0, "TETRAHEDRON", [6, 10, 9, 26] )
		mesh.addId("appendElem",id1)
		i = mesh.insertElement( 0, "TETRAHEDRON", [5, 22, 26, 21] )
		mesh.addId("appendElem",i)
		i = mesh.insertElement( 0, "TETRAHEDRON", [5, 6,  26, 22] )
		mesh.addId("appendElem",i)
		i = mesh.insertElement( 0, "TETRAHEDRON", [5, 26, 25, 21] )
		mesh.addId("appendElem",i)
		i = mesh.insertElement( 0, "TETRAHEDRON", [5, 26, 9,  25] )
		mesh.addId("appendElem",i)

		# 再度境界を求める

		bc.setMesh( mesh )
		bc.appendData( fg0 )
		bc.appendData( "appendElem" )
		bc.eraseData( "delElem" )
		bc.getBoundaryFace( 0, "reface" )

		assert_equal( mesh.getIdCount("reface"), 18 )

		# reface と fg0 は [5,6,10],[5,10,9] => [6,9,5],[6,10,9] の違いだけ
		mesh.eachId("reface"){|f|
			if !mesh.hasId(fg0,f)
				assert( f == [id0,3] || f == [id1,3] )
			end
		}

		assert( !mesh.hasId( "reface", [26,3] ) )
		assert( !mesh.hasId( "reface", [28,3] ) )
	end
end
