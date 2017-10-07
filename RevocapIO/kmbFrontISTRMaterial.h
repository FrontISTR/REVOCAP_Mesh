/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : FrontISTRMaterial                                       #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2016/03/22     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

#include "Geometry/kmbIdTypes.h"
#include "MeshDB/kmbTypes.h"
#include <string>
#include <vector>
#include <ostream>

namespace kmb{

class FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRMaterialItem &mat);
};

class FrontISTRMaterial{
public:
	std::string name;
	std::vector< FrontISTRMaterialItem* > materials;
	FrontISTRMaterial(void){};
	~FrontISTRMaterial(void);
	void clear(void);
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRMaterial &mat);
};

class FrontISTRMaterial_Elastic_Isotropic : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double youngs;
	double poisson;
};

class FrontISTRMaterial_Elastic_Isotropic_Dependent : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	struct Elastic{
		double youngs;
		double poisson;
		double temperature;
	};
	std::vector< struct Elastic > elastics;
};

class FrontISTRMaterial_Elastic_Orthotropic : public FrontISTRMaterialItem{
public:
	double e1,e2,e3;
	double v12,v13,v23;
	double g12,g13,g23;
};

class FrontISTRMaterial_Elastic_User : public FrontISTRMaterialItem{
public:
	double v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
};

class FrontISTRMaterial_Plastic_Mises_Bilinear : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double yield0;
	double h;
};

class FrontISTRMaterial_Plastic_Mises_MultiLinear : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	struct StressStrain{
		double yield;
		double pstrain;
	};
	std::vector< struct StressStrain > ss_points;
};

class FrontISTRMaterial_Plastic_Mises_MultiLinear_Dependent : public FrontISTRMaterialItem{
public:
	struct StressStrain{
		double yield;
		double pstrain;
	};
	struct TempDependentCurve{
		std::vector< struct StressStrain > ss_points;
		double temperature;
	};
	std::vector< struct TempDependentCurve > deps;
};

class FrontISTRMaterial_Plastic_Mises_Swift : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double epsilon0;
	double k;
	double n;
};

class FrontISTRMaterial_Plastic_Mises_RambergOsgood : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double epsilon0;
	double d;
	double n;
};

class FrontISTRMaterial_Plastic_Mises_Kinematic : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double yield0;
	double kinematic_coeff; // 線形移動硬化係数
};

class FrontISTRMaterial_Plastic_Mises_Combined : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double yield0;
	double h;
	double kinematic_coeff; // 線形移動硬化係数
};

class FrontISTRMaterial_Plastic_MohrCoulomb_Bilinear : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double visco; // 粘着力
	double fai; // 内部摩擦角
	double h;
};

class FrontISTRMaterial_Plastic_MohrCoulomb_MultiLinear : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double fai; // 内部摩擦角
	struct StressStrain{
		double pstrain;
		double visco; // 粘着力
	};
	std::vector< struct StressStrain > ss_points;
};

class FrontISTRMaterial_Plastic_DruckerPrager_Bilinear : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double visco; // 粘着力
	double fai; // 内部摩擦角
	double h;
};

class FrontISTRMaterial_Plastic_DruckerPrager_MultiLinear : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double fai; // 内部摩擦角
	struct StressStrain{
		double pstrain;
		double visco; // 粘着力
	};
	std::vector< struct StressStrain > ss_points;
};

class FrontISTRMaterial_Plastic_User : public FrontISTRMaterialItem{
public:
	double v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
};

class FrontISTRMaterial_Hyperelastic_Neohooke : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double c10;
	double d;
};

class FrontISTRMaterial_Hyperelastic_MooneyRivlin : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double c10;
	double c01;
	double d;
};

class FrontISTRMaterial_Hyperelastic_ArrudaBoyce : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double mu;
	double lambda_m;
	double d;
};

class FrontISTRMaterial_Hyperelastic_User : public FrontISTRMaterialItem{
public:
	double v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
};

class FrontISTRMaterial_Viscoelastic : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double g;
	double t;
	enum TRSDefinition{
		kUNDEF,
		kWLF,
		kARRHENUS
	};
	struct {
		TRSDefinition def;
		double theta0;
		double c1;
		double c2;
	} trs;
};

class FrontISTRMaterial_Creep_Norton : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double a;
	double n;
	double m;
};

class FrontISTRMaterial_Density : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double density;
};

class FrontISTRMaterial_Expansion_Isotropic : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double expansion;
};

class FrontISTRMaterial_Expansion_Orthotropic : public FrontISTRMaterialItem{
protected:
	virtual void output(std::ostream &os) const;
public:
	double alpha11;
	double alpha22;
	double alpha33;
};

class FrontISTRMaterial_User : public FrontISTRMaterialItem{
public:
	int nstatus;
	double v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
};

}

#include "RevocapIO/kmbFrontISTRMaterial_inline.h"