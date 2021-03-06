#include "../global.hpp"
#include "main_corrector.hpp"

//inflow boundary condition
Variables Main_Corrector::bc_inflow_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node		=	pars.max_node;
	const double gamma		=	pars.gamma;
	const double pressure_exit	=	pars.pressure_exit;

	//local vars
	std::vector<double> &rho	=	vars.rho;
	std::vector<double> &T		=	vars.T;
	std::vector<double> &U_1	=	vars.U_1;
	std::vector<double> &U_2	=	vars.U_2;
	std::vector<double> &U_3	=	vars.U_3;
	std::vector<double> &v		=	vars.v;
	std::vector<double> area	=	vars.area;

	//BC for rho
	rho[0]		=	1.0;

	//BC for T
	T[0]		=	1.0;

	//BC for U_1
	U_1[0]		=	rho[0]*area[0];

	//BC for U_2
	U_2[0]		=	2*U_2[1] - U_2[2];

	//BC for v
	v[0]		=	U_2[0]/U_1[0];

	//BC for U_3
	U_3[0]		=	U_1[0]*((T[0]/(gamma - 1)) + (0.5*gamma*pow(v[0],2)));

	return vars;
}

//calculate F_1_corrector
std::vector<double> Main_Corrector::calc_F_1_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;

	//local vars
	std::vector<double> U_2		=	vars.U_2;

	//processed variable
	std::vector<double> F_1_corrector(max_node);

	//process F_1_corrector
	for (auto i = 0; i < max_node - 1; i++) {
		
		F_1_corrector[i]	=	U_2[i];
	}

	return F_1_corrector;
}

//calculate F_2_corrector
std::vector<double> Main_Corrector::calc_F_2_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double gamma	=	pars.gamma;

	//local vars
	std::vector<double> U_1		=	vars.U_1;
	std::vector<double> U_2		=	vars.U_2;
	std::vector<double> U_3		=	vars.U_3;

	//processed variable
	std::vector<double> F_2_corrector(max_node);

	//process F_2_corrector
	for (auto i = 0; i < max_node - 1; i++) {
		
		double temp_1	=	pow(U_2[i],2)/U_1[i];
		double temp_2	=	U_3[i] - 0.5*gamma*temp_1;

		F_2_corrector[i]		=	temp_1 + ((gamma - 1)/(gamma))*temp_2;
	}

	return F_2_corrector;
}

//calculate F_3_corrector
std::vector<double> Main_Corrector::calc_F_3_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double gamma	=	pars.gamma;

	//local vars
	std::vector<double> U_1		=	vars.U_1;
	std::vector<double> U_2		=	vars.U_2;
	std::vector<double> U_3		=	vars.U_3;

	//processed variable
	std::vector<double> F_3_corrector(max_node);

	//process F_3_corrector
	for (auto i = 0; i < max_node - 1; i++) {
		
		double temp_1	=	U_2[i]*U_3[i]/U_1[i];
		double temp_2	=	pow(U_2[i],3)/pow(U_1[i],2);

		F_3_corrector[i]		=	gamma*temp_1 - 0.5*gamma*(gamma - 1)*temp_2;
	}

	return F_3_corrector;
}

//calculate J_2_corrector
std::vector<double> Main_Corrector::calc_J_2_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double gamma	=	pars.gamma;

	//local vars
	std::vector<double> area	=	vars.area;
	std::vector<double> x		=	vars.x;
	std::vector<double> rho		=	vars.rho;
	std::vector<double> T		=	vars.T;
	//processed variable
	std::vector<double> J_2(max_node);

	//process J_2
	for (auto i = 1; i < max_node - 1; i++) {
		
		double temp_1	=	(area[i] - area[i-1])/(x[i] - x[i-1]);
		J_2[i]		=	(1/gamma)*rho[i]*T[i]*temp_1;
	}

	return J_2;
}

//calculate dU_1_dt_corrector
std::vector<double> Main_Corrector::calc_dU_1_dt_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	
	//local vars
	std::vector<double> delta_x	=	vars.delta_x;
	std::vector<double> F_1		=	vars.F_1;

	//processed variable
	std::vector<double> dU_1_dt_corrector(max_node);

	//process dU_1_dt_corrector
	for (auto i = 1; i < max_node - 1; i++) {
		dU_1_dt_corrector[i]	=	-1*((F_1[i] - F_1[i-1])/delta_x[i-1]);
	}

	return dU_1_dt_corrector;
}

//calculate dU_2_dt_corrector
std::vector<double> Main_Corrector::calc_dU_2_dt_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	
	//local vars
	std::vector<double> delta_x	=	vars.delta_x;
	std::vector<double> F_2		=	vars.F_2;
	std::vector<double> J_2		=	vars.J_2;

	//processed variable
	std::vector<double> dU_2_dt_corrector(max_node);

	//process dU_2_dt_corrector
	for (auto i = 1; i < max_node - 1; i++) {
		dU_2_dt_corrector[i]	=	-1*((F_2[i] - F_2[i-1])/delta_x[i-1]) + J_2[i];
	}

	return dU_2_dt_corrector;
}

//calculate dU_3_dt_corrector
std::vector<double> Main_Corrector::calc_dU_3_dt_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	
	//local vars
	std::vector<double> delta_x	=	vars.delta_x;
	std::vector<double> F_3		=	vars.F_3;

	//processed variable
	std::vector<double> dU_3_dt_corrector(max_node);

	//process dU_3_dt_corrector
	for (auto i = 1; i < max_node - 1; i++) {
		dU_3_dt_corrector[i]	=	-1*((F_3[i] - F_3[i-1])/delta_x[i-1]);
	}

	return dU_3_dt_corrector;
}

//calculate S_1 (smoothing on U_1)
std::vector<double> Main_Corrector::calc_S_1_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double C_x	=	pars.smoothing_constant;

	//local vars
	std::vector<double> p	=	vars.p;
	std::vector<double> U_1	=	vars.U_1;
	
	//processed variable
	std::vector<double> S_1(max_node);

	for (auto i = 1; i < max_node - 1; i++) {
	
		double temp_1	=	std::abs(p[i+1] - 2*p[i] + p[i-1]);
		double temp_2	=	p[i+1] + 2*p[i] + p[i-1];
		double temp_3	=	U_1[i+1] - 2*U_1[i] + U_1[i-1];

		S_1[i]	=	C_x*(temp_1/temp_2)*temp_3;
	}

	return S_1;
}

//calculate S_2 (smoothing on U_2)
std::vector<double> Main_Corrector::calc_S_2_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double C_x	=	pars.smoothing_constant;

	//local vars
	std::vector<double> p	=	vars.p;
	std::vector<double> U_2	=	vars.U_2;
	
	//processed variable
	std::vector<double> S_2(max_node);

	for (auto i = 1; i < max_node - 1; i++) {
	
		double temp_1	=	std::abs(p[i+1] - 2*p[i] + p[i-1]);
		double temp_2	=	p[i+1] + 2*p[i] + p[i-1];
		double temp_3	=	U_2[i+1] - 2*U_2[i] + U_2[i-1];

		S_2[i]	=	C_x*(temp_1/temp_2)*temp_3;
	}

	return S_2;
}

//calculate S_3 (smoothing on U_3)
std::vector<double> Main_Corrector::calc_S_3_corrector(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double C_x	=	pars.smoothing_constant;

	//local vars
	std::vector<double> p	=	vars.p;
	std::vector<double> U_3	=	vars.U_3;
	
	//processed variable
	std::vector<double> S_3(max_node);

	for (auto i = 1; i < max_node - 1; i++) {
	
		double temp_1	=	std::abs(p[i+1] - 2*p[i] + p[i-1]);
		double temp_2	=	p[i+1] + 2*p[i] + p[i-1];
		double temp_3	=	U_3[i+1] - 2*U_3[i] + U_3[i-1];

		S_3[i]	=	C_x*(temp_1/temp_2)*temp_3;
	}

	return S_3;
}
