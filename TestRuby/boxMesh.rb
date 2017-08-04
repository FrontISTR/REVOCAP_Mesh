#
# このファイルを require or load して
# mesh.beginModel
# restoreMesh(mesh)
# mesh.endModel
# すると mesh に
# 元のメッシュの内容が代入されます。
# 必要に応じて 
# mesh.endModel の前に
# restoreMeshSpec(mesh)
# を追加してください。
#
def restoreMesh(mesh)
	mesh.beginNode(16)
	mesh.addNode(1.000000000000000e+000,1.000000000000000e+000,1.000000000000000e+000) # 0
	mesh.addNode(1.000000000000000e+000,1.000000000000000e+000,2.000000000000000e+000) # 1
	mesh.addNode(1.000000000000000e+000,2.000000000000000e+000,1.000000000000000e+000) # 2
	mesh.addNode(1.000000000000000e+000,2.000000000000000e+000,2.000000000000000e+000) # 3
	mesh.addNode(2.000000000000000e+000,1.000000000000000e+000,1.000000000000000e+000) # 4
	mesh.addNode(2.000000000000000e+000,1.000000000000000e+000,2.000000000000000e+000) # 5
	mesh.addNode(2.000000000000000e+000,2.000000000000000e+000,1.000000000000000e+000) # 6
	mesh.addNode(2.000000000000000e+000,2.000000000000000e+000,2.000000000000000e+000) # 7
	mesh.addNode(3.000000000000000e+000,1.000000000000000e+000,1.000000000000000e+000) # 8
	mesh.addNode(3.000000000000000e+000,1.000000000000000e+000,2.000000000000000e+000) # 9
	mesh.addNode(3.000000000000000e+000,2.000000000000000e+000,1.000000000000000e+000) # 10
	mesh.addNode(3.000000000000000e+000,2.000000000000000e+000,2.000000000000000e+000) # 11
	mesh.addNode(4.000000000000000e+000,1.000000000000000e+000,1.000000000000000e+000) # 12
	mesh.addNode(4.000000000000000e+000,1.000000000000000e+000,2.000000000000000e+000) # 13
	mesh.addNode(4.000000000000000e+000,2.000000000000000e+000,1.000000000000000e+000) # 14
	mesh.addNode(4.000000000000000e+000,2.000000000000000e+000,2.000000000000000e+000) # 15
	mesh.endNode
	mesh.beginElement(5) # 0
	mesh.addElement("TETRAHEDRON",[     0,     4,     2,     1]) # 0 0
	mesh.addElement("TETRAHEDRON",[     6,     2,     4,     7]) # 1 1
	mesh.addElement("TETRAHEDRON",[     3,     1,     2,     7]) # 2 2
	mesh.addElement("TETRAHEDRON",[     5,     4,     1,     7]) # 3 3
	mesh.addElement("TETRAHEDRON",[     4,     2,     1,     7]) # 4 4
	mesh.endElement
	mesh.beginElement(5) # 1
	mesh.addElement("TETRAHEDRON",[     8,     4,     9,    10]) # 5 5
	mesh.addElement("TETRAHEDRON",[     6,     4,    10,     7]) # 6 6
	mesh.addElement("TETRAHEDRON",[     5,     4,     7,     9]) # 7 7
	mesh.addElement("TETRAHEDRON",[    11,    10,     9,     7]) # 8 8
	mesh.addElement("TETRAHEDRON",[     4,     7,     9,    10]) # 9 9
	mesh.endElement
	mesh.beginElement(5) # 2
	mesh.addElement("TETRAHEDRON",[     8,    12,    10,     9]) # 10 10
	mesh.addElement("TETRAHEDRON",[    14,    10,    12,    15]) # 11 11
	mesh.addElement("TETRAHEDRON",[    11,     9,    10,    15]) # 12 12
	mesh.addElement("TETRAHEDRON",[    13,    12,     9,    15]) # 13 13
	mesh.addElement("TETRAHEDRON",[    12,    10,     9,    15]) # 14 14
	mesh.endElement
	mesh.createData("test1","ELEMENTGROUP","NONE","SurfPress",1)
	mesh.addId("test1", 5)
	mesh.addId("test1", 6)
	mesh.createData("test2","FACEGROUP","NONE","Weight",1)
	mesh.addId("test2", [5,2])
	mesh.addId("test2", [6,3])
	mesh.createData("test3","FACEGROUP","NONE","Weight",2)
	mesh.addId("test3", [10,2])
	mesh.addId("test3", [11,3])
	mesh.createData("test4","FACEGROUP","NONE","Weight")
	mesh.addId("test4", [10,2])
	mesh.addId("test4", [11,3])
	mesh.createData("test5","FACEGROUP","NONE","Weight",0)
	mesh.addId("test5", [0,1])
	mesh.addId("test5", [2,3])
	mesh.addId("test5", [3,2])
	mesh.addId("test5", [1,1])
end

def restoreMeshSpec(mesh)
end
