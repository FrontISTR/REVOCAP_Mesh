require 'test/unit'

require 'RevocapMesh'
include RevocapMesh

class TestRevocapMesh < Test::Unit::TestCase

	def setup
	end

	def teardown
	end

	def test_nodeId
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode(3)
		mesh.addNodeWithId(0.0,0.0,100.0,100)
		mesh.addNodeWithId(0.0,0.0,200.0,200)
		assert_equal( mesh.addNodeWithId(0.0,0.0,250.0,200), -1 )
		mesh.addNodeWithId(0.0,0.0,300.0,300)
		mesh.endNode
		mesh.beginElement
		mesh.addElement("TRIANGLE",[100,200,300])
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 100 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(100,-20), false )
		assert_equal( mesh.replaceNodeId(100,50), true ) # [50,200,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 50 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(50,100), true ) # [100,200,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 100 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(200,100), false )
		assert_equal( mesh.replaceNodeId(200,250), true ) # [100,250,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 100 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(250,200), true ) # [100,200,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 100 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(100,150), true ) # [150,200,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 150 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(150,100), true ) # [100,200,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 100 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(100,250), true ) # [200,250,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 200 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		assert_equal( mesh.replaceNodeId(300,150), true ) # [150,200,250]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 150 )
		assert_equal( mesh.getMaxNodeId(), 250 )
		assert_equal( mesh.replaceNodeId(250,300), true ) # [150,200,300]
		assert_equal( mesh.getNodeCount(), 3 )
		assert_equal( mesh.getMinNodeId(), 150 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		mesh.createData("test","NODEVARIABLE","INTEGER")
		mesh.setValueAtId("test",150,150)
		mesh.setValueAtId("test",200,200)
		pt = mesh.getNodePoint(150)
		mesh.deleteNode(150)
		# delete しても Point は残っている
		assert_equal( pt.x, 0.0 )
		assert_equal( pt.y, 0.0 )
		assert_equal( pt.z, 300.0 )
		assert_equal( mesh.getNodeCount(), 2 )
		assert_equal( mesh.getMinNodeId(), 200 )
		assert_equal( mesh.getMaxNodeId(), 300 )
		# deleteNode しても Value と Element は残る
		assert_equal( mesh.getElement(0).include?(150), true )
		assert_equal( mesh.getValueAtId("test",150), 150 )
		# replaceNode しても delete された Node は無視したまま
		assert_equal( mesh.replaceNodeId(200,250), true ) # [250,300]
		assert_equal( mesh.getElement(0).include?(150), true )
		assert_equal( mesh.getElement(0).include?(250), true )
		assert_equal( mesh.getValueAtId("test",150), 150 )
		assert_equal( mesh.getValueAtId("test",250), 200 )
	end

	def test_node
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode
		mesh.addNode(0.0,0.0,0.0)
		mesh.addNode(1.0,0.0,0.0)
		mesh.addNode(2.0,0.0,0.0)
		mesh.endNode
		mesh.endModel
		point = mesh.getNodePoint(0)
		x = point.x
		y = point.y
		z = point.z
		point.x( x+1 )
		point.y( y+1 )
		point.z( z+1 )
		point = mesh.getNodePoint(0)
		assert_equal( point.x, 0.0 )
		assert_equal( point.y, 0.0 )
		assert_equal( point.z, 0.0 )
	end
	
	def test_clearBody
		mesh = MeshDB.new
		load File.join(File.dirname($0),'boxMesh.rb')
		mesh.beginModel
		restoreMesh(mesh)
		mesh.endModel
		mesh.clearBody(0)
		assert_equal( mesh.getElement(0), nil )
		assert_equal( mesh.getElementCount(0), 0 )
		mesh.eachElement(0){ |e|
			assert( false )
		}
	end

	def test_bodyname
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginElement
		mesh.endElement
		mesh.beginElement
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getBodyName(0), "" )
		mesh.setBodyName(0,"ahoaho")
		assert_equal( mesh.getBodyName(0), "ahoaho" )
	end

	def test_face
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginElement(2)
		mesh.addElement("TETRAHEDRON",[100,101,102,103])
		mesh.addElement("HEXAHEDRON",[0,1,5,4,3,2,6,7])
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getFaces(0)[0], [101,102,103] )
		assert_equal( mesh.getFaces(0)[1], [100,103,102] )
		assert_equal( mesh.getFaces(0)[2], [100,101,103] )
		assert_equal( mesh.getFaces(0)[3], [100,102,101] )
		assert_equal( mesh.isFace(0,[101,102,103]), 1 )
		assert_equal( mesh.isFace(0,[102,103,101]), 1 )
		assert_equal( mesh.isFace(0,[103,101,102]), 1 )

		assert_equal( mesh.isFace(0,[101,103,102]), -1 )
		assert_equal( mesh.isFace(0,[102,101,103]), -1 )
		assert_equal( mesh.isFace(0,[103,102,101]), -1 )

		assert_equal( mesh.isFace(0,[101,102,104]), 0 )

		assert_equal( mesh.isFace(0,[100,103,102]), 1 )
		assert_equal( mesh.isFace(0,[100,101,103]), 1 )
		assert_equal( mesh.isFace(0,[100,102,101]), 1 )

		# for hexa
#		mesh.addElement("HEXAHEDRON",[0,1,5,4,3,2,6,7])			# 1
		assert_equal( mesh.getFaces(1)[0], [4,5,1,0] )
		assert_equal( mesh.getFaces(1)[1], [3,2,6,7] )
		assert_equal( mesh.getFaces(1)[2], [1,2,3,0] )
		assert_equal( mesh.getFaces(1)[3], [1,5,6,2] )
		assert_equal( mesh.getFaces(1)[4], [4,7,6,5] )
		assert_equal( mesh.getFaces(1)[5], [3,7,4,0] )
		assert_equal( mesh.isFace(1,[4,5,1,0]), 1)
		assert_equal( mesh.isFace(1,[5,1,0,4]), 1)
		assert_equal( mesh.isFace(1,[0,1,5,4]), -1)
		# 対角線でクロスする
		assert_equal( mesh.isFace(1,[0,5,1,4]), 0)
		assert_equal( mesh.isFace(1,[4,5,1]), 0)
		assert_equal( mesh.isFace(1,[4,5,1,2,3]), 0)
		assert_equal( mesh.isFace(1,[3,2,6,7]), 1)
		assert_equal( mesh.isFace(1,[3,7,6,2]), -1)
	end

	def test_move_delete_element
		mesh = MeshDB.new
		load File.join(File.dirname($0),'boxMesh.rb')
		mesh.beginModel
		restoreMesh(mesh)
		mesh.endModel
		# bodyId = 0 から elementId = 0 を除く
		assert_equal( mesh.getElementCount(0), 5 )
		assert_equal( mesh.deleteElement(0,0), 0 ) # 削除した id を返す
		assert_equal( mesh.getElementCount(0), 4 )
		assert_equal( mesh.getElementCount(1), 5 )
		# bodyId = 0 から bodyId = 1 へ elementId = 1 を移動する
		assert( mesh.moveElement(0,1,1) )
		assert_equal( mesh.getElementCount(0), 3 )
		assert_equal( mesh.getElementCount(1), 6 )
		# 移動できない時は false
		assert_equal( mesh.moveElement(0,1,1), false ) # 要素がない時
		assert_equal( mesh.moveElement(0,5,2), false ) # メッシュがない時
	end

	def test_mergeBody
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode(20)
		20.times{
			mesh.addNode( rand(), rand(), rand() )
		}
		mesh.endNode
		mesh.beginElement
		10.times{ |i|
			mesh.addElement("TETRAHEDRON",[i,i+1,i+2,i+3])
		}
		mesh.endElement
		mesh.beginElement
		15.times{ |i|
			mesh.addElement("TETRAHEDRON",[19-i,18-i,17-i,16-i])
		}
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getBodyCount, 2 )
		assert_equal( mesh.getNodeCount, 20 )
		assert_equal( mesh.getElementCount(0), 10 )
		assert_equal( mesh.getElementCount(1), 15 )
		hash = mesh.getElementCountByType(0)
		assert_equal( hash['TETRAHEDRON'], 10 )
		hash = mesh.getElementCountByType(1)
		assert_equal( hash['TETRAHEDRON'], 15 )
		mesh.mergeBody([0,1])
		assert_equal( mesh.getBodyCount, 3 )
		assert_equal( mesh.getNodeCount, 20 )
		assert_equal( mesh.getElementCount(0), 0 )
		assert_equal( mesh.getElementCount(1), 0 )
		assert_equal( mesh.getElementCount(2), 25 )
		hash = mesh.getElementCountByType(0)
		assert_equal( hash.key?('TETRAHEDRON'), false )
		hash = mesh.getElementCountByType(1)
		assert_equal( hash.key?('TETRAHEDRON'), false )
		hash = mesh.getElementCountByType(2)
		assert_equal( hash['TETRAHEDRON'], 25 )
	end

	def test_mergeBody2
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode(20)
		20.times{
			mesh.addNode( rand(), rand(), rand() )
		}
		mesh.endNode

		mesh.beginElement
		10.times{ |i|
			mesh.addElement("TETRAHEDRON",[i,i+1,i+2,i+3])
		}
		mesh.endElement

		mesh.beginElement
		15.times{ |i|
			mesh.addElement("TETRAHEDRON",[19-i,18-i,17-i,16-i])
		}
		mesh.endElement

		mesh.beginElement
		20.times{ |i|
			mesh.addElement("TETRAHEDRON",[rand(20),rand(20),rand(20),rand(20)])
		}
		mesh.endElement
		mesh.endModel

		mesh.mergeBody(1,0)
		assert( mesh.getBodyCount == 3 )
		assert( mesh.getNodeCount == 20 )
		assert( mesh.getElementCount(0) == 25 )
		assert( mesh.getElementCount(1) == 0 )
		assert( mesh.getElementCount(2) == 20 )
		mesh.mergeBody(2,0,false)
		assert( mesh.getElementCount(0) == 45 )
		assert( mesh.getElementCount(1) == 0 )
		assert( mesh.getElementCount(2) == 20 )
		15.times{ |i|
			assert( mesh.getElement(10+i,0) == ["TETRAHEDRON",19-i,18-i,17-i,16-i] )
		}
		20.times{ |i|
			assert( mesh.getElement(45+i,0) == mesh.getElement(25+i,2) )
		}
	end

	def test_dimension_degree
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode(20)
		20.times{
			mesh.addNode( rand(), rand(), rand() )
		}
		mesh.endNode
		mesh.beginElement
		10.times{ |i|
			mesh.addElement("TETRAHEDRON2",[i,i+1,i+2,i+3,i+4,i+5,i+6,i+7,i+8,i+9])
		}
		mesh.endElement
		mesh.beginElement
		5.times{ |i|
			mesh.addElement("QUAD",[19-i,18-i,17-i,16-i])
		}
		mesh.endElement
		mesh.beginElement
		5.times{ |i|
			mesh.addElement("TRIANGLE2",[19-i,18-i,17-i,16-i,15-i,14-i])
		}
		mesh.endElement
		mesh.beginElement
		5.times{ |i|
			mesh.addElement("PYRAMID",[19-i,18-i,17-i,16-i,15-i])
		}
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getDimension(0), 3 )
		assert_equal( mesh.getDegree(0), 2 )
		assert_equal( mesh.getDimension(1), 2 )
		assert_equal( mesh.getDegree(1), 1 )
		assert_equal( mesh.getDimension(2), 2 )
		assert_equal( mesh.getDegree(2), 2 )
		assert_equal( mesh.getDimension(3), 3 )
		assert_equal( mesh.getDegree(3), 1 )
	end

	def test_convertToLinear
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode(20)
		20.times{
			mesh.addNode( rand(), rand(), rand() )
		}
		mesh.endNode
		mesh.beginElement
		10.times{ |i|
			mesh.addElement("TETRAHEDRON2",[i,i+1,i+2,i+3,i+4,i+5,i+6,i+7,i+8,i+9])
		}
		mesh.endElement
		mesh.beginElement
		5.times{ |i|
			mesh.addElement("WEDGE2",[19-i,18-i,17-i,16-i,15-i,14-i,13-i,12-i,11-i,10-i,9-i,8-i,7-i,6-i,5-i])
		}
		mesh.endElement
		mesh.endModel
		mesh.convertToLinearBody(0)
		mesh.convertToLinearBody(1)
		assert( mesh.getElementCountByType(0)["TETRAHEDRON"] == 10 )
		assert( mesh.getElementCountByType(1)["WEDGE"] == 5 )
		assert( mesh.getElement(3) == ["TETRAHEDRON",3,4,5,6] )
		assert( mesh.getElement(12) == ["WEDGE",17,16,15,14,13,12] )
	end

	def test_node_clear
		mesh = MeshDB.new
		load File.join(File.dirname($0),'boxMesh.rb')
		mesh.beginModel
		restoreMesh(mesh)
		mesh.endModel
		mesh.clearNodes()
		assert( mesh.getNodeCount == 0 )
		mesh.beginNode(2)
		mesh.addNode(0.0,1.0,2.0)
		mesh.addNode(3.0,4.0,5.0)
		mesh.endNode
		assert( mesh.getNodeCount == 2 )
	end

	def test_node_defrag
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode
		mesh.addNodeWithId(0.0,0.0,0.0,0)
		mesh.addNodeWithId(0.0,0.0,2.0,2)
		mesh.addNodeWithId(0.0,0.0,4.0,4)
		mesh.addNodeWithId(0.0,0.0,8.0,8)
		mesh.endNode
		mesh.beginElement
		mesh.addElement("TETRAHEDRON",[0,2,4,8])
		mesh.endElement
		mesh.createData("group","NODEGROUP","NONE")
		mesh.addId("group",0)
		mesh.addId("group",2)
		mesh.addId("group",4)
		mesh.createData("each","NODEVARIABLE","INTEGER")
		mesh.setValueAtId("each",0,0)
		mesh.setValueAtId("each",2,2)
		mesh.setValueAtId("each",4,4)
		mesh.endModel
		assert_equal("DataVariable",mesh.getDataContainerType("each"))
		assert_equal("DataBindingsGroup",mesh.getDataContainerType("group"))
		# 物理量の対応
		mesh.eachId("group"){ |id|
			assert( [0,2,4].include?(id) )
		}
		mesh.eachIdWithValue("each"){ |id,val|
			assert( val == mesh.getNode(id)[2].round )
		}
		mesh.nodeIdDefragmentation()
		# node の間の対応
		# node の original な id は z を round すればよい
		idmap = Hash.new
		mesh.eachNodeWithId{ |node,id|
			# idmap[newid] = oldid
			idmap[id] = node[2].round
		}
		# 要素の間の対応が同じかどうか
		mesh.eachElement(0){ |e|
			e.shift
			e.each{ |id|
				assert( idmap[id] == mesh.getNode(id)[2].round )
			}
		}
		# 物理量の対応
		mesh.eachIdWithValue("each"){ |id,val|
			assert( val == mesh.getNode(id)[2].round )
		}
		mesh.eachId("group"){ |id|
			assert( idmap.include?(id) )
		}
		mesh.nodeIdDefragmentation(100)
		# node の間の対応
		# node の original な id は z を round すればよい
		idmap = Hash.new
		mesh.eachNodeWithId{ |node,id|
			# idmap[newid] = oldid
			idmap[id] = node[2].round
		}
		# 要素の間の対応が同じかどうか
		mesh.eachElement(0){ |e|
			e.shift
			e.each{ |id|
				assert( idmap[id] == mesh.getNode(id)[2].round )
			}
		}
		# 物理量の対応
		mesh.eachIdWithValue("each"){ |id,val|
			assert( val == mesh.getNode(id)[2].round )
		}
		mesh.eachId("group"){ |id|
			assert( idmap.include?(id) )
		}
	end

	def test_data_binding
		mesh = MeshDB.new
		assert( mesh.getDataCount() == 0 )

		mesh.createData("test","NODEGROUP","SCALAR")
		assert( mesh.getDataCount() == 1 )
		mesh.addId("test",0)
		mesh.addId("test",1)
		mesh.addId("test",2)
		mesh.setValue("test",3.14)

		assert( mesh.hasData("test") )
		assert( mesh.getDataMode("test") == "NODEGROUP" )
		assert( mesh.getDataValueType("test") == "SCALAR" )
		assert( mesh.getIdCount("test") == 3 )
		assert( mesh.hasId("test",0) == true )
		assert( mesh.hasId("test",1) == true )
		assert( mesh.hasId("test",2) == true )
		assert( mesh.hasId("test",3) == false )
		assert( mesh.hasId("test",4) == false )
		assert( mesh.getValue("test") == 3.14 )

		assert( !mesh.hasData("test2") )
		assert( mesh.getDataMode("test2") == "UNDEFINED" )
		assert( mesh.getDataValueType("test2") == "UNDEFINED" )
		assert( mesh.getIdCount("test2") == 0 )

		mesh.createData("eachtest","NODEVARIABLE","VECTOR3")
		assert( mesh.getDataCount() == 2 )
		mesh.addId("eachtest",0)
		mesh.addId("eachtest",1)
		mesh.addId("eachtest",2)
		mesh.setValue("eachtest",3.14)
		# each mode では無効のはす
		assert( mesh.hasId("eachtest",0) == false )
		assert( mesh.hasId("eachtest",2) == false )
		assert( mesh.hasId("eachtest",4) == false )
		assert( mesh.getIdCount("eachtest") == 0 )
		assert( mesh.getValue("eachtest") == nil )
		
		assert( mesh.getDataMode("eachtest") == "NODEVARIABLE" )
		assert( mesh.getDataValueType("eachtest") == "VECTOR3" )
		mesh.setValueAtId("eachtest",3,[0.0,0.0,3.0])
		mesh.setValueAtId("eachtest",4,[0.0,0.0,4.0])
		mesh.setValueAtId("eachtest",5,[0.0,0.0,5.0])
		mesh.setValueAtId("eachtest",6,[0.0,0.0,6.0])
		assert( mesh.getIdCount("eachtest") == 4 )
		assert( mesh.hasId("eachtest",2) == false )
		assert( mesh.hasId("eachtest",3) == true )
		assert( mesh.hasId("eachtest",4) == true )
		assert( mesh.getValueAtId("eachtest",3) == [0.0,0.0,3.0] )
		assert( mesh.getValueAtId("eachtest",4) == [0.0,0.0,4.0] )
		assert( mesh.getValueAtId("eachtest",5) == [0.0,0.0,5.0] )
		assert( mesh.getValueAtId("eachtest",6) == [0.0,0.0,6.0] )

		mesh.eachIdWithValue("eachtest"){ |id,val|
			assert( [0.0,0.0,id.to_f] == val )
		}
	end

	def test_volume
		require 'setBoxMesh'
		mesh = MeshDB.new
		setMeshBox(mesh)
		assert_in_delta( mesh.getElementVolume(0), 1.0/6.0, 0.000000001 )
		assert_in_delta( mesh.getElementVolume(1), 1.0/6.0, 0.000000001 )
		assert_in_delta( mesh.getElementVolume(2), 1.0/6.0, 0.000000001 )
		assert_in_delta( mesh.getElementVolume(3), 1.0/6.0, 0.000000001 )
		assert_in_delta( mesh.getElementVolume(4), 1.0/3.0, 0.000000001 )
	end

	def test_boundingBox
		mesh = MeshDB.new
		load File.join(File.dirname($0),'boxMesh.rb')
		mesh.beginModel
		restoreMesh(mesh)
		mesh.endModel
		bbox = mesh.getBoundingBox
		bbox0 = mesh.getBoundingBox(0)
		bbox1 = mesh.getBoundingBox(1)
		bbox2 = mesh.getBoundingBox(2)
		assert_equal( bbox.maxX, 4.0 )
		assert_equal( bbox.maxY, 2.0 )
		assert_equal( bbox.maxZ, 2.0 )
		assert_equal( bbox.minX, 1.0 )
		assert_equal( bbox.minY, 1.0 )
		assert_equal( bbox.minZ, 1.0 )
		assert_equal( bbox0.maxX, 2.0 )
		assert_equal( bbox0.maxY, 2.0 )
		assert_equal( bbox0.maxZ, 2.0 )
		assert_equal( bbox0.minX, 1.0 )
		assert_equal( bbox0.minY, 1.0 )
		assert_equal( bbox0.minZ, 1.0 )
		assert_equal( bbox1.maxX, 3.0 )
		assert_equal( bbox1.maxY, 2.0 )
		assert_equal( bbox1.maxZ, 2.0 )
		assert_equal( bbox1.minX, 2.0 )
		assert_equal( bbox1.minY, 1.0 )
		assert_equal( bbox1.minZ, 1.0 )
		assert_equal( bbox2.maxX, 4.0 )
		assert_equal( bbox2.maxY, 2.0 )
		assert_equal( bbox2.maxZ, 2.0 )
		assert_equal( bbox2.minX, 3.0 )
		assert_equal( bbox2.minY, 1.0 )
		assert_equal( bbox2.minZ, 1.0 )
	end

	def test_reverse
		mesh = MeshDB.new
		mesh.beginModel()
		mesh.beginNode(8)
		mesh.addNode(0.0, 0.0, 0.0)  # 0 
		mesh.addNode(1.0, 0.0, 0.0)  # 1
		mesh.addNode(1.0, 1.0, 0.0)  # 2
		mesh.addNode(0.0, 1.0, 0.0)  # 3
		mesh.addNode(0.0, 0.0, 1.0)  # 4
		mesh.addNode(1.0, 0.0, 1.0)  # 5
		mesh.addNode(1.0, 1.0, 1.0)  # 6
		mesh.addNode(0.0, 1.0, 1.0)  # 7
		mesh.endNode()
		mesh.beginElement(18)
		mesh.addElement("SEGMENT",[0,1]) # 0
		mesh.addElement("SEGMENT2",[0,1,2]) # 1
		mesh.addElement("TRIANGLE",[0,1,2]) # 2
		mesh.addElement("TRIANGLE",[0,1,2]) # 3
		mesh.addElement("TRIANGLE2",[1,2,0,3,4,5]) # 4
		mesh.addElement("TRIANGLE2",[1,2,0,3,4,5]) # 5
		mesh.addElement("QUAD",[0,1,2,3]) # 6
		mesh.addElement("QUAD",[0,1,2,3]) # 7
		mesh.addElement("QUAD2",[1,2,3,0,4,5,6,7]) # 8
		mesh.addElement("QUAD2",[1,2,3,0,4,5,6,7]) # 9
		mesh.addElement("TETRAHEDRON",[0,1,2,4]) # 10
		mesh.addElement("TETRAHEDRON",[0,1,2,4]) # 11
		# PYRAMID は底面が同一平面上になければ体積に意味はない
		# 他の要素も同様
		mesh.addElement("PYRAMID",[4,0,1,2,3]) # 12
		mesh.addElement("PYRAMID",[4,1,2,3,0]) # 13
		mesh.addElement("WEDGE",[0,1,2,4,5,6]) # 14
		mesh.addElement("WEDGE",[2,0,1,6,4,5]) # 15
		mesh.addElement("HEXAHEDRON",[0,1,2,3,4,5,6,7]) # 16
		mesh.addElement("HEXAHEDRON",[1,2,3,0,5,6,7,4]) # 17
		mesh.endElement
		mesh.endModel

		mesh.reverseElement(0)
		mesh.reverseElement(1)
		assert( mesh.getElement(0) == ["SEGMENT",1,0] )
		assert( mesh.getElement(1) == ["SEGMENT2",1,0,2] )

		assert( mesh.getElementRelation(2,3) == "EQUAL" )
		mesh.reverseElement(2)
		assert( mesh.getElementRelation(2,3) == "REVERSE" )
		mesh.reverseElement(2)
		assert( mesh.getElementRelation(2,3) == "EQUAL" )

		assert( mesh.getElementRelation(3,5) == "EQUAL" )

		assert( mesh.getElementRelation(4,5) == "EQUAL" )
		mesh.reverseElement(5)
		assert( mesh.getElementRelation(4,5) == "REVERSE" )
		mesh.reverseElement(5)
		assert( mesh.getElementRelation(4,5) == "EQUAL" )

		assert( mesh.getElementRelation(6,7) == "EQUAL" )
		mesh.reverseElement(6)
		assert( mesh.getElementRelation(6,7) == "REVERSE" )
		mesh.reverseElement(7)
		assert( mesh.getElementRelation(6,7) == "EQUAL" )

		assert( mesh.getElementRelation(6,8) == "REVERSE" )

		assert( mesh.getElementRelation(8,9) == "EQUAL" )
		mesh.reverseElement(8)
		assert( mesh.getElementRelation(8,9) == "REVERSE" )
		mesh.reverseElement(9)
		assert( mesh.getElementRelation(8,9) == "EQUAL" )

		assert( mesh.getElementRelation(10,11) == "EQUAL" )
		mesh.reverseElement(10)
		assert( mesh.getElementRelation(10,11) == "REVERSE" )
		assert( mesh.getElementVolume(10) == -mesh.getElementVolume(11) )

		assert( mesh.getElementRelation(12,13) == "EQUAL" )
		mesh.reverseElement(12)
		assert( mesh.getElementRelation(12,13) == "REVERSE" )
		assert( mesh.getElementVolume(12) == -mesh.getElementVolume(13) )

		assert( mesh.getElementRelation(14,15) == "EQUAL" )
		mesh.reverseElement(14)
		assert( mesh.getElementRelation(14,15) == "REVERSE" )
		assert( mesh.getElementVolume(14) == -mesh.getElementVolume(15) )

		assert( mesh.getElementRelation(16,17) == "EQUAL" )
		mesh.reverseElement(16)
		assert( mesh.getElementRelation(16,17) == "REVERSE" )
		assert( mesh.getElementVolume(16) == -mesh.getElementVolume(17) )
	end

	def test_getElementCountByType
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginElement
		mesh.addElement("TRIANGLE",[1,2,3])
		mesh.addElement("TRIANGLE",[1,2,3])
		mesh.addElement("TRIANGLE",[1,2,3])
		mesh.addElement("TRIANGLE2",[1,2,3,4,5,6])
		mesh.addElement("TRIANGLE2",[1,2,3,4,5,6])
		mesh.addElement("TETRAHEDRON",[1,2,3,4])
		mesh.addElement("TETRAHEDRON",[1,2,3,4])
		mesh.addElement("TETRAHEDRON",[1,2,3,4])
		mesh.addElement("TETRAHEDRON",[1,2,3,4])
		mesh.addElement("TETRAHEDRON2",[1,2,3,4,5,6,7,8,9,10])
		mesh.addElement("TETRAHEDRON2",[1,2,3,4,5,6,7,8,9,10])
		mesh.addElement("TETRAHEDRON2",[1,2,3,4,5,6,7,8,9,10])
		mesh.addElement("TETRAHEDRON2",[1,2,3,4,5,6,7,8,9,10])
		mesh.addElement("TETRAHEDRON2",[1,2,3,4,5,6,7,8,9,10])
		mesh.addElement("TETRAHEDRON2",[1,2,3,4,5,6,7,8,9,10])
		mesh.endElement
		mesh.endModel
		hash = mesh.getElementCountByType(0)
		assert( hash["TRIANGLE"] == 3 )
		assert( hash["TRIANGLE2"] == 2 )
		assert( hash["TETRAHEDRON"] == 4 )
		assert( hash["TETRAHEDRON2"] == 6 )
	end

	def test_deleteUselessNodes
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginNode
		xyz = Array.new
		20.times{
			x = rand
			y = rand
			z = rand
			xyz << [x,y,z]
			mesh.addNode( x,y,z )
		}
		mesh.endNode
		mesh.beginElement
		mesh.addElement("TETRAHEDRON",[13,15,17,19])
		mesh.addElement("TETRAHEDRON",[5,6,7,8])
		mesh.addElement("TETRAHEDRON",[9,10,11,12])
		mesh.endElement
		mesh.endModel
		assert( mesh.getNodeCount == 20 )
		assert( mesh.deleteUselessNodes == 8 )
		assert( mesh.getNodeCount == 12 )
		assert( mesh.getNode( mesh.getElement(0,0)[1] ) == xyz[13] )
		mesh.nodeIdDefragmentation
		assert( mesh.getNodeCount == 12 )
		assert( mesh.getNode( mesh.getElement(0,0)[1] ) == xyz[13] )
	end

	def test_triangulation
		mesh = MeshDB.new
		mesh.beginModel()
		mesh.beginNode(8)
		mesh.addNode(0.0, 0.0, 0.0)  # 0 
		mesh.addNode(1.0, 0.0, 0.0)  # 1
		mesh.addNode(1.0, 1.0, 0.0)  # 2
		mesh.addNode(0.0, 1.0, 0.0)  # 3
		mesh.addNode(0.0, 0.0, 1.0)  # 4
		mesh.addNode(1.0, 0.0, 1.0)  # 5
		mesh.addNode(1.0, 1.0, 1.0)  # 6
		mesh.addNode(0.0, 1.0, 1.0)  # 7
		mesh.endNode()
		mesh.beginElement(1)
		mesh.addElement("HEXAHEDRON",[0,1,2,3,4,5,6,7])
		mesh.endElement
		mesh.beginElement(1)
		mesh.addElement("QUAD",[0,1,2,3,4,5,6,7])
		mesh.endElement
		mesh.endModel
		vol0 = mesh.getElementVolume(0)
		assert_in_delta( vol0, 1.0, 4*Float::EPSILON )
		mesh.triangulation(0)
		assert_equal( mesh.getElementCountByType(0).key?("HEXAHEDRON"), false )
		assert( mesh.getElementCountByType(0)["TETRAHEDRON"] > 1 )
		vol0 = 0
		mesh.eachElementWithId(0){ |e,id|
			vol0 += mesh.getElementVolume(id)
		}
		assert_in_delta( vol0, 1.0, 4*Float::EPSILON )
		vol1 = mesh.getElementVolume(1)
		mesh.triangulation(1)
		assert_equal( mesh.getElementCountByType(1).key?("QUAD"), false )
		assert( mesh.getElementCountByType(1)["TRIANGLE"] > 1 )
		vol2 = 0
		mesh.eachElementWithId(1){ |e,id|
			vol2 += mesh.getElementVolume(id)
		}
		assert_in_delta( vol1, vol2, 4*Float::EPSILON )
	end

	def test_trans
		mesh = MeshDB.new
		mesh.beginModel()
		mesh.beginNode(8)
		mesh.addNode(0.0, 0.0, 0.0)  # 0 
		mesh.addNode(1.0, 0.0, 0.0)  # 1
		mesh.addNode(1.0, 1.0, 0.0)  # 2
		mesh.addNode(0.0, 1.0, 0.0)  # 3
		mesh.addNode(0.0, 0.0, 1.0)  # 4
		mesh.addNode(1.0, 0.0, 1.0)  # 5
		mesh.addNode(1.0, 1.0, 1.0)  # 6
		mesh.addNode(0.0, 1.0, 1.0)  # 7
		mesh.endNode()
		mesh.beginElement(1)
		mesh.addElement("HEXAHEDRON",[0,1,2,3,4,5,6,7])
		mesh.endElement
		mesh.endModel
		mesh.translateAllNodes(10.0,10.0,10.0)
		assert( mesh.getNode(0) == [10.0,10.0,10.0] )
		assert( mesh.getNode(1) == [11.0,10.0,10.0] )
		assert( mesh.getNode(2) == [11.0,11.0,10.0] )
		assert( mesh.getNode(3) == [10.0,11.0,10.0] )
		assert( mesh.getNode(4) == [10.0,10.0,11.0] )
		assert( mesh.getNode(5) == [11.0,10.0,11.0] )
		assert( mesh.getNode(6) == [11.0,11.0,11.0] )
		assert( mesh.getNode(7) == [10.0,11.0,11.0] )
		mesh.translateAllNodes(-10.0,-10.0,-10.0)
		mesh.scaleAllNodes(10.0)
		assert( mesh.getNode(0) == [0.0,0.0,0.0] )
		assert( mesh.getNode(1) == [10.0,0.0,0.0] )
		assert( mesh.getNode(2) == [10.0,10.0,0.0] )
		assert( mesh.getNode(3) == [0.0,10.0,0.0] )
		assert( mesh.getNode(4) == [0.0,0.0,10.0] )
		assert( mesh.getNode(5) == [10.0,0.0,10.0] )
		assert( mesh.getNode(6) == [10.0,10.0,10.0] )
		assert( mesh.getNode(7) == [0.0,10.0,10.0] )
		mesh.scaleAllNodes(0.1)
		mesh.convertAllNodes(
			Matrix4x4.new(10.0,0.0,0.0,0.0,
										0.0,1.0,0.0,0.0,
										0.0,0.0,1.0,10.0,
										0.0,0.0,0.0,1.0) )
		assert( mesh.getNode(0) == [0.0,0.0,10.0] )
		assert( mesh.getNode(1) == [10.0,0.0,10.0] )
		assert( mesh.getNode(2) == [10.0,1.0,10.0] )
		assert( mesh.getNode(3) == [0.0,1.0,10.0] )
		assert( mesh.getNode(4) == [0.0,0.0,11.0] )
		assert( mesh.getNode(5) == [10.0,0.0,11.0] )
		assert( mesh.getNode(6) == [10.0,1.0,11.0] )
		assert( mesh.getNode(7) == [0.0,1.0,11.0] )
	end

	def test_trans2
		mesh = MeshDB.new
		mesh.beginModel()
		mesh.beginNode(8)
		mesh.addNode(0.0, 0.0, 0.0)  # 0 
		mesh.addNode(1.0, 0.0, 0.0)  # 1
		mesh.addNode(1.0, 1.0, 0.0)  # 2
		mesh.addNode(0.0, 1.0, 0.0)  # 3
		mesh.addNode(0.0, 0.0, 1.0)  # 4
		mesh.addNode(1.0, 0.0, 1.0)  # 5
		mesh.addNode(1.0, 1.0, 1.0)  # 6
		mesh.addNode(0.0, 1.0, 1.0)  # 7
		mesh.endNode()
		mesh.convertAllNodes(
			Matrix4x4.new(1.0,0.0,0.0,0.0,
										0.0,0.0,-1.0,0.0,
										0.0,1.0,0.0,0.0,
										0.0,0.0,0.0,1.0) )
		assert( mesh.getNode(3) == [0.0,0.0,1.0] )
	end

	def test_containertype
		mesh = MeshDB.new
		mesh.beginModel
		mesh.beginElement
		mesh.addElement("TRIANGLE",[0,1,2])
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getDefaultElementContainerType, "stl::map" )
		assert_equal( mesh.getDefaultElementContainerType, mesh.getElementContainerType(0) )
		mesh = MeshDB.new
		mesh.setDefaultContainerType("Post")
		mesh.beginModel
		mesh.beginNode(1)
		mesh.addNode(0.0, 0.0, 0.0)
		mesh.endNode
		mesh.beginElement(1)
#		mesh.addElement("TRIANGLE",[0,1,2])
		mesh.endElement
		mesh.endModel
		assert_equal( mesh.getDefaultElementContainerType, "element_marray" )
		assert_equal( mesh.getDefaultElementContainerType, mesh.getElementContainerType(0) )
		assert_equal( mesh.getDefaultNodeContainerType, "double_array" )
		assert_equal( mesh.getDefaultNodeContainerType, mesh.getNodeContainerType )
		assert_equal( mesh.getDefaultDataContainerType, "Post" )
	end

	def test_elementContainerMA
		mesh0 = MeshDB.new
		mesh0.beginModel
		mesh0.beginElement(100)
		25.times{
			mesh0.addElement("TRIANGLE",[rand(100),100+rand(100),200+rand(100)])
			mesh0.addElement("TETRAHEDRON",[rand(100),100+rand(100),200+rand(100),300+rand(100)])
			mesh0.addElement("QUAD",[rand(100),100+rand(100),200+rand(100),300+rand(100)])
			mesh0.addElement("SEGMENT",[rand(100),100+rand(100)])
		}
		mesh0.endElement
		mesh0.endModel
		mesh1 = MeshDB.new
		mesh1.setDefaultElementContainerType("mixed_array")
		mesh1.beginModel
		mesh1.beginElement( mesh0.getElementCount(0) )
		mesh0.eachElementWithId(0){ |e,id|
			type = e.shift
			mesh1.addElement(type,e)
		}
		mesh1.endElement
		mesh1.endModel
		assert_equal( mesh1.getElementCount(0), 100 )
		assert_equal( mesh1.getElementContainerType(0), "mixed_array" )
		100.times{ |i|
			assert_equal( mesh0.getElement(i), mesh1.getElement(i) )
		}
	end
end
