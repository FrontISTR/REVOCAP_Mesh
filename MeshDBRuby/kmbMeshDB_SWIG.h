/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshDB                                                  #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once
// このファイルは C++ 版では使わずに SWIG だけで使われる関数などを定義する
// meshdb.i の中から Callback で呼ばれる種類のメソッドなどを書く。
// meshdb.i に書いてしまうと SWIG が名前を変えてしまうから。
// copymesh で使う関数群

VALUE _eachNodeWithId(VALUE mesh){
	rb_funcall(mesh, rb_intern("eachNodeWithId"),0);
	return Qnil;
}

VALUE _eachElement(VALUE mesh_with_bodyID){
	Check_Type(mesh_with_bodyID,T_ARRAY);
	VALUE mesh = rb_ary_entry(mesh_with_bodyID,0);
	VALUE bodyID = rb_ary_entry(mesh_with_bodyID,1);
	rb_funcall(mesh, rb_intern("eachElement"),1,bodyID);
	return Qnil;
}

VALUE meshDB_addNodeWithId(VALUE block,VALUE data,VALUE self){
	Check_Type(block,T_ARRAY);
	VALUE node = rb_ary_entry(block,0);
	VALUE id = rb_ary_entry(block,1);
	VALUE x = rb_ary_entry(node,0);
	VALUE y = rb_ary_entry(node,1);
	VALUE z = rb_ary_entry(node,2);
	rb_funcall(data, rb_intern("addNodeWithId"), 4, x, y, z, id );
	return Qnil;
}

VALUE meshDB_addElement(VALUE block,VALUE data,VALUE self){
	Check_Type(block,T_ARRAY);
	VALUE eType = rb_ary_entry(block,0);
	kmb::elementType type = kmb::ElementBase::getType( StringValuePtr(eType) );
	int len = kmb::ElementBase::getNodeCount(type);
	VALUE a = rb_ary_new2(len);
	for(int i=0; i<len; ++i){
		rb_ary_store(a,i,rb_ary_entry(block,i+1));
	}
	rb_funcall(data, rb_intern("addElement"), 2, eType, a);
	return Qnil;
}

kmb::Face faceFromVALUE(VALUE v){
	if( TYPE(v) == T_ARRAY && RARRAY_LEN(v) == 2){
		int id0 = NUM2INT(rb_ary_entry(v,0));
		int id1 = NUM2INT(rb_ary_entry(v,1));
		return kmb::Face(id0,id1);
	}
	return kmb::Face();
}
