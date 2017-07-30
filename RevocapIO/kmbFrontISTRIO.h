/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : FrontISTRIO                                             #
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
#include "MeshDB/kmbElement.h"
#include <string>
#include <vector>
#include <ostream>

namespace kmb{

class MeshData;
class FrontISTRMaterial;

class FrontISTRAnalysis{
public:
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRAnalysis &analysis);
protected:
	virtual void output(std::ostream &os) const;
};

class FrontISTRStaticAnalysis : public FrontISTRAnalysis{
protected:
	virtual void output(std::ostream &os) const;
};

class FrontISTREigenAnalysis : public FrontISTRAnalysis{
protected:
	virtual void output(std::ostream &os) const;
public:
	int nset;
	double lcztol;
	int lczmax;
};

class FrontISTRHeatAnalysis : public FrontISTRAnalysis{
protected:
	virtual void output(std::ostream &os) const;
public:
	double dt;
	double etime;
	double dtmin;
	double deltmx;
	int itmax;
	double eps;
};

class FrontISTRDynamicAnalysis : public FrontISTRAnalysis{
protected:
	virtual void output(std::ostream &os) const;
public:
	enum DynamicType{
		kLINEAR,
		kNONLINEAR
	} type;
	enum DynamicMethod{
		kNEWMARK_BETA = 1,
		kCENTERED_DIFFERENCE = 11
	} idx_eqa;
	enum DynamicKind{
		kTIME_HISTORY = 1,
		kFREQUENCY_RESPONSE = 2
	} idx_resp;
	enum MassMatrixType{
		kLUMPED_MASS = 1,
		kCONSISTENT_MASS = 2
	} massType;
	enum DumpingType{
		kRAYLEIGH = 1
	} idxDmp;
	double rayleigh_m;
	double rayleigh_k;
};

class FrontISTRTimeHistoryAnalysis : public FrontISTRDynamicAnalysis{
protected:
	virtual void output(std::ostream &os) const;
public:
	int nStep;
	double deltat;
	double gamma;
	double beta;
	kmb::nodeIdType node_monitor;
	int node_monitor_interval;
	int iout_list[6]; // 変位,速度,加速度,反力,ひずみ,応力
};

class FrontISTRFrequencyResponseAnalysis : public FrontISTRDynamicAnalysis{
protected:
	virtual void output(std::ostream &os) const;
public:
	double f_start;
	double f_end;
	int nFreq;
	double f_disp;
	double t_start;
	double t_end;
	int nout;
	enum VisualizeType{
		kMODE_SPACE = 1,
		kPHYSICAL_SPACE = 2
	} vistype;
	kmb::nodeIdType node_monitor;
	int iout_list[6]; // 変位,速度,加速度
	std::string eigenlog;
	int start_mode;
	int end_mode;
};

class FrontISTRCoupledAnalysis : public FrontISTRAnalysis{
protected:
	virtual void output(std::ostream &os) const;
public:
	enum CoupleType{
		kUNDEF = 0,
		kONEWAY_RECEIVE = 1,
		kONEWAY_SEND = 2,
		kSTAGGERED_RECEIVE = 3,
		kSTAGGERED_SEND = 4,
		kSEPARATING_ITERATION_RECEIVE = 5,
		kSEPARATING_ITERATION_SEND = 6,
	} coupleType;
	int istep;
	double window;
	std::string name;
};

class FrontISTRStep{
public:
	enum StepType{
		kSTATIC,
		kVISCO
	} type;
	int substeps;
	double converg;
	int maxiter;
	double dtime;
	double etime;
	std::vector<int> boundaries;
	std::vector<int> loads;
	std::vector<int> contacts;
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRStep &dat);
};

class FrontISTRSolver{
public:
	enum{
		kCG,
		kBiCGSTAB,
		kGMRES,
		kGPBiCG,
		kDIRECT,
		kDIRECTmkl,
		kMUMPS
	} method;
	enum{
		kSSOR = 1,
		kDIAGONAL_SCALING = 3,
		kAMG = 5,
		kILU0 = 10,
		kILU1 = 11,
		kILU2 = 12
	} precond;
	bool iterLog;
	bool timeLog;
	bool useJAD;
	bool scaling;
	enum{
		kNONE,
		kMM,
		kCSR,
		kBSR
	} dumpType;
	bool dumpExit;
	enum{
		kUNDEF = -1,
		kPENALTY = 1,
		kMPC_CG = 2,
		kEXPLICIT_ERASE = 3
	} mpcMethod;
	int estCond;
	int nIter;
	int iterPREmax;
	int nRest;
	int nColor_in;
	double residue;
	double sigma_diag;
	FrontISTRSolver(void);
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRSolver &dat);
};

class FrontISTRVisual{
public:
	enum{
		kPSR
	} method;
	int start_step;
	int end_step;
	int interval_step;
	class Display{
	public:
		enum {
			kSMOOTH_CONTOUR = 1,
			kLINE_CONTOUR = 2,
			kSMOOTH_LINE_CONTOUR = 3,
			kSOLID_COLOR = 4,
			kSOLID_CONTOUR = 5
		} method;
		virtual void output(std::ostream &os) const;
	};
	class SmoothContour : public Display{
	public:
		std::string comp_name;
		int subcomp;
		virtual void output(std::ostream &os) const;
	};
	class LineContour : public Display{
	public:
		int isoline_number;
		float isoline_color[3];
		virtual void output(std::ostream &os) const;
	};
	class SmoothLineContour : public Display{
	public:
		std::string comp_name;
		int subcomp;
		int isoline_number;
		float isoline_color[3];
		virtual void output(std::ostream &os) const;
	};
	class SolidColor : public Display{
	public:
		float specified_color;
		virtual void output(std::ostream &os) const;
	};
	class SolidContour : public Display{
	public:
		int isoline_number;
		float isoline_color[3];
		virtual void output(std::ostream &os) const;
	};
	class Surface{
	public:
		enum SurfaceStyle{
			kUNDEF = -1,
			kBOUNDARY = 1,
			kISOSURF = 2,
			kQUADRATIC = 3
		} style;
		Display* display;
		enum DisplayStyle{
			kNONE = 0,
			kMESH = 1,
			kGRAY = 2,
			kSHADING = 3,
			kDOTTED = 4
		};
		DisplayStyle initial_style;
		DisplayStyle deform_style;
		float initial_line_color[3];
		float deform_line_color[3];
		double deform_scale;
		std::string deform_comp_name;
		Surface(void);
		Surface(SurfaceStyle _style);
		virtual ~Surface(void);
		virtual void output(std::ostream &os) const;
	};
	class BoundarySurface : public Surface{
	public:
		BoundarySurface(void);
		virtual void output(std::ostream &os) const;
	};
	class Isosurface : public Surface{
	public:
		Isosurface(void);
		std::string data_comp_name;
		int data_subcomp;
		double iso_value;
		virtual void output(std::ostream &os) const;
	};
	class QuadraticSurface : public Surface{
	public:
		QuadraticSurface(void);
		float coef[10];
		int method;
		virtual void output(std::ostream &os) const;
	};
	std::vector< Surface* > surfaces;
	class Output{
	public:
		enum {
			kAVS,
			kBMP,
			kCOMPLETE_AVS,
			kCOMPLETE_REORDER_AVS,
			kSEPARATE_COMPLETE_AVS,
			kCOMPLETE_MICROAVS,
			kBIN_COMPLETE_AVS,
			kFSTR_FEMAP_NEUTRAL
		} output_type;
		virtual void output(std::ostream &os) const;
	};
	class BMPOutput : public Output{
	public:
		int x_resolution;
		int y_resolution;
		int num_of_lights;
		float *position_of_lights;
		float viewpoint[3];
		float look_at_point[3];
		float up_direction[3];
		float ambient;
		float diffuse;
		float specular;
		enum {
			kX_AXIS = 1,
			kY_AXIS = 2,
			kZ_AXIS = 3,
			kVIEWPOINT = 4,
		} rotate_style;
		int rotate_num;
		bool color_mapping_bar;
		bool scale_marking;
		int num_of_scales;
		float font_size;
		float font_color[3];
		float background_color[3];
		virtual void output(std::ostream &os) const;
	};
	Output* output;
	FrontISTRVisual(void);
	~FrontISTRVisual(void);
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRVisual &dat);
};

class FrontISTRData{
public:
	std::string version;
	enum Solution{
		kSTATIC,
		kNLSTATIC,
		kHEAT,
		kEIGEN,
		kDYNAMIC,
		kSTATICEIGEN,
		kELEMCHECK
	} solution;
	struct Write_Visual{
		int frequency;
	} write_visual;
	struct Write_Result{
		int frequency;
	} write_result;
	struct Write_Log{
		int frequency;
	} write_log;
	struct Output_Vis{
		bool disp;
		bool rot;
		bool reaction;
		bool nstrain;
		bool nstress;
		bool nmises;
		bool th_nstrain;
		bool vel;
		bool acc;
	} output_vis;
	struct Output_Res{
		bool disp;
		bool rot;
		bool reaction;
		bool nstrain;
		bool nstress;
		bool nmises;
		bool estrain;
		bool estress;
		bool emises;
		bool istrain;
		bool istress;
		bool pl_istrain;
		bool th_nstrain;
		bool th_estrain;
		bool th_istrain;
		bool vel;
		bool acc;
	} output_res;
	struct Restart{
		int frequency;
	} restart;
	bool echo;
	struct Orientation{
		std::string name;
		virtual void output(std::ostream& os) const;
	};
	struct OrientationByCoordinates : Orientation{
		double a[3];
		double b[3];
		double c[3];
		virtual void output(std::ostream& os) const;
	};
	struct OrientationByNodes : Orientation{
		kmb::nodeIdType a;
		kmb::nodeIdType b;
		kmb::nodeIdType c;
		virtual void output(std::ostream& os) const;
	};
	std::vector< Orientation* > orientations;
	// Ver.3.6 の User Manual P120 の Section の定義
	struct Section{
		int secnum;
		std::string orientation;
	};
	std::vector< Section* > sections;
	std::vector< FrontISTRMaterial* > materials;
	std::vector< FrontISTRStep* > steps;
	FrontISTRAnalysis* analysis;
	FrontISTRSolver solver;
	FrontISTRVisual visual;
	FrontISTRData(void)
		: version("3.6")
		, solution(kSTATIC)
		, write_visual()
		, write_result()
		, write_log()
		, output_res()
		, restart()
		, echo(false)
		, analysis(NULL)
		, solver()
		, visual()
	{}
	~FrontISTRData(void);
	void clear(void);
	friend std::ostream& operator<<(std::ostream &os, const FrontISTRData &dat);
};

class FrontISTRIO{
protected:
	kmb::nodeIdType offsetNodeId;
	kmb::elementIdType offsetElementId;
	size_t nodeCount;
	size_t elementCount;
	int getGroupId(std::string name,std::string stype);
	// 面の変換 Revo => Fistr
	static int tetFmap[];
	static int wedFmap[];
	static int hexFmap[];
	static int pyrFmap[];
	// 面の変換 Fistr => Revo
	static int tetRmap[];
	static int wedRmap[];
	static int hexRmap[];
	static int pyrRmap[];
public:
	FrontISTRIO();
	int saveCntFile(const char* filename,const kmb::FrontISTRData* fdata,const kmb::MeshData* mesh);
	template<typename MContainer>
	int loadMesh(std::string filename, MContainer* mesh);
	template<typename MContainer>
	int saveMesh(std::string filename, MContainer* mesh);
protected:
	void count(std::string filename);
	void writeDummyMaterial(std::ostream& output);
	static kmb::elementType getRevocapType(int etypeHec);
};

}

#include "RevocapIO/kmbFrontISTRIO_inline.h"
