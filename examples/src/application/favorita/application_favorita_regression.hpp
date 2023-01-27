#ifndef APPLICATION_FAVORITA_REGRESSION_HPP
#define APPLICATION_FAVORITA_REGRESSION_HPP

#include <cmath>
#include <iomanip>
#include "application_favorita_base.hpp"
#include "bgd_solver.hpp"
#include "csvreader.hpp"

BGDSolver<15, 0> bgd_solver;

long total_num_training = 0L;
long total_num_iterations = 0L;
long total_train_time_ms = 0L;

void train(
        dbtoaster::data_t& data /* in */, 
        size_t &num_iterations /* out */, 
        double &error /* out */,
        long &train_time_ms /* out */
    ) {
    Stopwatch sw;
    sw.restart();

    const auto& cofactor_matrix = data.get_V_dateid_SITSOH1();

#ifdef TRAIN_FROM_SCRATCH
    bgd_solver.reset();
#endif
    
    bgd_solver.build_sigma_matrix(cofactor_matrix);
    bgd_solver.run(num_iterations, error);

    sw.stop();
    train_time_ms = sw.elapsedTimeInMilliSeconds();
    total_num_training += 1;
    total_num_iterations += num_iterations;
    total_train_time_ms += train_time_ms;
}

void Application::on_snapshot(dbtoaster::data_t& data) {
#ifdef ON_SNAPSHOT_CHECKPOINT
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    std::cout << data.tN << " tuples processed at "
              << tp.tv_sec * 1000 + tp.tv_usec / 1000
              << " ms" << std::endl;
#endif
#ifdef ON_SNAPSHOT_TRAIN
    size_t num_iterations = 0;
    double error = 0;
    long train_time_ms;
    train(data, num_iterations, error, train_time_ms);

#ifdef ON_SNAPSHOT_TRAIN_CHECKPOINT
    std::cout << "tuples = " << data.tN
              << "   num_iterations: " << num_iterations 
              << "   train_time_ms = " << train_time_ms
              << "   error: " << error               
              << "   total_num_training = " << total_num_training
              << "   total_num_iterations = " << total_num_iterations
              << "   total_train_time_ms = " << total_train_time_ms
              << std::endl;
#endif
#endif
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
    bgd_solver.reset();
    total_num_training = 0L;
    total_num_iterations = 0L;
    total_train_time_ms = 0L;

    // double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    // std::cout << "RMSE: " << rmse << std::endl;
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {
    size_t num_iterations = 0;
    double error = 0;
    long train_time_ms;
    train(data, num_iterations, error, train_time_ms);

    std::cout << std::endl
              << "tuples = " << data.tN
              << "   num_iterations: " << num_iterations 
              << "   train_time_ms = " << train_time_ms
              << "   error: " << error           
              << "   total_num_training = " << total_num_training
              << "   total_num_iterations = " << total_num_iterations
              << "   total_train_time_ms = " << total_train_time_ms
              << std::endl;

    // double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    // std::cout << "RMSE: " << rmse << std::endl;
}

#endif /* APPLICATION_FAVORITA_REGRESSION_HPP */