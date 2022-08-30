/**
 * メッシュを引数とする関数
 * メッシュそのものには何の副作用も与えないこと
 */

#pragma once

#include "Geometry/kmbBoundingBox.h"
#include "MeshDB/kmbElementType.h"
#include "MeshDB/kmbTypes.h"

#include <string>

namespace kmb {
	class MeshFunction {
	public:
		// Bounding Box を計算する
		template<typename MeshT>
		static kmb::BoundingBox calcBoundingBox(const MeshT* mesh);

		// bodyId に含まれる要素が単一の型かどうかを判定する
		template<typename MeshT>
		static bool isUniqueElementType(const MeshT* mesh, kmb::bodyIdType bodyId, kmb::elementType etype);

		// 座標 (x,y,z) を与えて最も近い節点の節点番号を返す
		template<typename MeshT>
		static kmb::nodeIdType findNode(const MeshT* mesh, double x, double y, double z);

		// 座標 (x,y,z) を与えて最も近い要素の要素番号を返す
		// tolerance を与えて、その値以下のものを探す
		// tolerance = 0.0 の時は、点が要素に含まれている場合
		template<typename MeshT>
		static kmb::elementIdType searchElement(const MeshT* mesh, kmb::bodyIdType bodyId, double x, double y, double z, double tolerance = 0.0);
	};
}