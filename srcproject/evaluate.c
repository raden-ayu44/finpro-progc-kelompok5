#include "finpro.h"
#include "evaluate.h"


// fungsi untuk mengevaluasi fungsi yang diberikan user, digunakan untuk validasi input bracket bisection & false-position,
// dan digunakan untuk perhitungan di keempat metode

double evaluateFunction(FunctionConfig *cfg, double x) {
    double result = 0.0;
    switch (cfg->funcType) {
        case POLYNOMIAL:
            switch (cfg->degree) {
                case LINEAR:
                    result = cfg->param.linear[0] * x + cfg->param.linear[1];
                    break;
                case QUADRATIC:
                    result = cfg->param.quadratic[0] * pow(x,2) + cfg->param.quadratic[1] * x + cfg->param.quadratic[2];
                    break;
                case CUBIC:
                    result = cfg->param.cubic[0] * pow(x,3) + cfg->param.cubic[1] * pow(x,2) + cfg->param.cubic[2] * x + cfg->param.cubic[3];
                    break;
                default:
                   result = 0.0;
            }
            break;
        case EULER: {
            double exponent = cfg->param.euler[0] * x + cfg->param.euler[1];
            result = exp(exponent) + cfg->param.euler[2] * x + cfg->param.euler[3];
            break;
        }
        default:
            return 0.0;
    }
    if (!isfinite(result)) {
        return (result > 0) ? DBL_MAX : -DBL_MAX;
    }
    return result;
}

// fungsi untuk menurunkan fungsi yang diberikan user, digunakan untuk perhitungan newton-raphson

double evaluateDerivative(FunctionConfig *cfg, double x) {
    switch (cfg->funcType) {
        case POLYNOMIAL:
	        switch (cfg->degree) {
		        case LINEAR:
			        return cfg->param.linear[0];
                case QUADRATIC:
	                return 2 * cfg->param.quadratic[0] * x + cfg->param.quadratic[1];
                case CUBIC:
	                return 3 * cfg->param.cubic[0] * pow(x,2) + 2 * cfg->param.cubic[1] * x + cfg->param.cubic[2];
                default:
                   return 0.0; 
            }
        case EULER:
            return cfg->param.euler[0] * exp(cfg->param.euler[0] * x + cfg->param.euler[1]) + cfg->param.euler[2];
        default:
            return 0.0;
    }                          
}


// fungsi untuk mencari semua akar real polinomial dan mengembalikan yang paling dekat dengan xr

double findClosestRoot(FunctionConfig *cfg, double xr) {
    double roots[3];
    int nRoots = 0;
    switch (cfg->degree) {
        case LINEAR: {
            roots[0] = -(cfg->param.linear[1] / cfg->param.linear[0]);
            nRoots = 1;
            break;
        }
        case QUADRATIC: {
            double a = cfg->param.quadratic[0];
            double b = cfg->param.quadratic[1];
            double c = cfg->param.quadratic[2];
            double disc = b*b - 4*a*c;
            if (disc < 0) {
                return cfg->xt_quad;
            }
            roots[0] = (-b + sqrt(disc)) / (2*a);
            roots[1] = (-b - sqrt(disc)) / (2*a);
            nRoots = 2;
            break;
        }
        case CUBIC: {
            double a = cfg->param.cubic[0];
            double b = cfg->param.cubic[1];
            double c = cfg->param.cubic[2];
            double d = cfg->param.cubic[3];
            double p = (3*a*c - b*b) / (3*a*a);
            double q = (2*b*b*b - 9*a*b*c + 27*a*a*d) / (27*a*a*a);
            double delta = pow(q/2, 2) + pow(p/3, 3);
            if (delta > 0) {
                double complex u = cpow(-q/2 + csqrt(delta), 1.0/3.0);
                double complex v = cpow(-q/2 - csqrt(delta), 1.0/3.0);
                roots[0] = creal(u + v) - b/(3*a);
                nRoots = 1;
            } else {
                double r     = 2 * sqrt(-p/3.0);
                double theta = acos((3*q / (2*p)) * sqrt(-3.0/p));
                for (int i = 0; i < 3; i++) {
                    roots[i] = r * cos((theta + 2*M_PI*i) / 3.0) - b/(3*a);
                }
                nRoots = 3;
            }
            break;
        }
        default:
            return 0;
    }
    double closest = roots[0];
    for (int i = 1; i < nRoots; i++) {
        if (fabs(roots[i] - xr) < fabs(closest - xr))
            closest = roots[i];
    }
    return closest;
}

// fungsi untuk menghitung true error khusus untuk fungsi polinomial

double calcTrueError(FunctionConfig *cfg, double xr) {
    double xt = findClosestRoot(cfg, xr);
    return fabs(xt - xr);
}

// fungsi untuk menghitung true error relative khusus untuk fungsi polinomial

double calcTrueErrorRelative(FunctionConfig *cfg, double xr) {
    double xt = findClosestRoot(cfg, xr);
    if (fabs(xt) < 1e-12) return 0;   // root at zero — et undefined, avoid div/0
    return fabs(xt - xr) / fabs(xt) * 100;
}