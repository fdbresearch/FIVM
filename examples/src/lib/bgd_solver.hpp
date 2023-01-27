#ifndef BGD_SOLVER_HPP
#define BGD_SOLVER_HPP

template <size_t SZ, size_t TARGET_IDX>
struct BGDSolver {
    static constexpr size_t NUM_PARAMS = SZ + 1;
    static constexpr size_t LABEL_IDX = TARGET_IDX + 1;

    /*  
        INVARIANTS:
          params[LABEL_IDX] = prev_params[LABEL_IDX] = -1
          grad[LABEL_IDX] = prev_grad[LABEL_IDX] = 0
    */

    double sigma[NUM_PARAMS][NUM_PARAMS];
    double params[NUM_PARAMS];
    double grad[NUM_PARAMS];
    double prev_params[NUM_PARAMS];
    double prev_grad[NUM_PARAMS];
    double lambda;
    
    BGDSolver() {
        reset();
    }

    void reset() {
        memset(sigma, 0, sizeof(sigma));
        memset(params, 0, sizeof(params));
        // for(size_t i = 0; i < NUM_PARAMS; i++) {
        //     params[i] = ((double) (rand() % 800 + 1) - 400) / 100;
        // }
        params[LABEL_IDX] = -1;
        memset(grad, 0, sizeof(grad));
        memset(prev_params, 0, sizeof(prev_params));
        prev_params[LABEL_IDX] = -1;
        memset(prev_grad, 0, sizeof(prev_grad));
        lambda = 0.1;
    }

    inline double count() { return sigma[0][0]; }

    void build_sigma_matrix(const RingCofactor<0, double, SZ> cofactor) {
        sigma[0][0] = cofactor.count;

        for (size_t i = 0; i < SZ; i++) {
            sigma[0][i + 1] = cofactor.sum1[i];
            sigma[i + 1][0] = cofactor.sum1[i];
        }

        size_t idx = 0;
        for (size_t i = 0; i < SZ; i++) {
            sigma[i + 1][i + 1] = cofactor.sum2[i];

            for (size_t j = i + 1; j < SZ; j++) {
                sigma[i + 1][j + 1] = cofactor.degree2[idx];
                sigma[j + 1][i + 1] = cofactor.degree2[idx];
                idx++;
            }
        }
    }

    void compute_gradient() {
        /* Compute Sigma * Theta */
        for (size_t i = 0; i < NUM_PARAMS; i++) {
            grad[i] = 0.0;
            for (size_t j = 0; j < NUM_PARAMS; j++) {
                grad[i] += sigma[i][j] * params[j];
            }
            grad[i] /= count();
        }
        grad[LABEL_IDX] = 0.0;
    }

    double compute_error() {
        double error = 0.0;

        /* Compute 1/N * Theta^T * Sigma * Theta */
        for (size_t i = 0; i < NUM_PARAMS; i++) {
            double tmp = 0.0;
            for (size_t j = 0; j < NUM_PARAMS; j++) {
                tmp += sigma[i][j] * params[j];
            }
            error += params[i] * tmp;
        }
        error /= count();

        /* Add the regulariser to the error */
        double param_norm = 0.0;
        for (size_t i = 1; i < NUM_PARAMS; i++) {
           param_norm += params[i] * params[i];
        }
        assert(params[LABEL_IDX] == -1 && params[LABEL_IDX] == -1);
        param_norm -= 1;    // param_norm -= params[LABEL_IDX] * params[LABEL_IDX];
        error += lambda * param_norm;

        return error / 2;
    }

    inline double compute_step_size(double step_size) {
        double DSS = 0.0, GSS = 0.0, DGS = 0.0;

        assert(params[LABEL_IDX] == -1 && prev_params[LABEL_IDX] == -1);
        assert(grad[LABEL_IDX] == 0 && prev_grad[LABEL_IDX] == 0);

        for (size_t i = 0; i < NUM_PARAMS; i++) {

            double paramDiff = params[i] - prev_params[i];
            double gradDiff = grad[i] - prev_grad[i];

            DSS += paramDiff * paramDiff;
            GSS += gradDiff * gradDiff;
            DGS += paramDiff * gradDiff;
        }
        
        if (DGS == 0.0 || GSS == 0.0) return step_size;
        
        double Ts = DSS / DGS;
        double Tm = DGS / GSS;
        
        if (Tm < 0.0 || Ts < 0.0) return step_size;
        
        return (Tm / Ts > 0.5) ? Tm : Ts - 0.5 * Tm;
    }

    void run(size_t &num_iterations, double &error) {
        if (count() == 0) return;

        compute_gradient();

        // Compute first gradient norm
        double gradient_norm = grad[0] * grad[0];
        for (size_t i = 1; i < NUM_PARAMS; i++) {
            double upd = grad[i] + lambda * params[i];
            gradient_norm += upd * upd;
        }
        gradient_norm -= lambda * lambda;   // label correction
        double first_gradient_norm = sqrt(gradient_norm);

        double prev_error = compute_error();

        double update[NUM_PARAMS];
        double step_size = 0.1;
        num_iterations = 1;
        do {
            // Update parameters and compute gradient norm
            update[0] = grad[0];
            gradient_norm = update[0] * update[0];
            prev_params[0] = params[0];
            prev_grad[0] = grad[0];
            params[0] = params[0] - step_size * update[0];
            double dparam_norm = update[0] * update[0];
            for (size_t i = 1; i < NUM_PARAMS; i++) {
                update[i] = grad[i] + lambda * params[i];
                gradient_norm += update[i] * update[i];
                prev_params[i] = params[i];
                prev_grad[i] = grad[i];
                params[i] = params[i] - step_size * update[i];
                dparam_norm += update[i] * update[i];
            }
            params[LABEL_IDX] = -1;
            gradient_norm -= lambda * lambda;   // label correction
            dparam_norm = step_size * sqrt(dparam_norm);

            error = compute_error();

            /* Backtracking Line Search: Decrease step_size until condition is satisfied */
            size_t backtracking_steps = 0;
            while (error > prev_error - (step_size / 2) * gradient_norm && backtracking_steps < 500) {
                step_size /= 2;            // Update parameters based on the new step_size.
                
                dparam_norm = 0.0;
                for (size_t i = 0; i < NUM_PARAMS; i++) {
                    double newp = prev_params[i] - step_size * update[i];
                    double dp = params[i] - newp;
                    params[i] = newp;
                    dparam_norm += dp * dp;
                }
                dparam_norm = sqrt(dparam_norm);

                params[LABEL_IDX] = -1;          
                error = compute_error();
                backtracking_steps++;
            }

            /* Normalized residual stopping condition */
            gradient_norm = sqrt(gradient_norm);
            if (dparam_norm < 1e-20 || 
                    gradient_norm / (first_gradient_norm + 0.001) < 1e-5) {
                break;
            }

            if (abs(error-prev_error) < 1e-8) {
                break;
            }

            compute_gradient();
            step_size = compute_step_size(step_size);
            prev_error = error;
            num_iterations++;

        } while (num_iterations < 1000);
    }
};

#endif /* BGD_SOLVER_HPP */