#ifndef APPLICATION_RETAILER_REGRESSION_HPP
#define APPLICATION_RETAILER_REGRESSION_HPP

#include <cmath>
#include <iomanip>
#include <sys/time.h>
#include "application_retailer_base.hpp"
#include "bgd_solver.hpp"
#include "csvreader.hpp"

struct RetailerTuple {
    int locn;
    int dateid;
    int ksn;
    int inventoryunits;
    int8_t subcategory;
    int8_t category;
    int8_t categoryCluster;
    double prize;
    int8_t rain;
    int8_t snow;
    int maxtemp;
    int mintemp;
    double meanwind;
    int8_t thunder;

    int zip;
    int rgn_cd;
    int clim_zn_nbr;
    int tot_area_sq_ft;
    int sell_area_sq_ft;
    int avghhi;
    double supertargetdistance;
    double supertargetdrivetime;
    double targetdistance;
    double targetdrivetime;
    double walmartdistance;
    double walmartdrivetime;
    double walmartsupercenterdistance;
    double walmartsupercenterdrivetime;
    int population;
    int white;
    int asian;
    int pacific;
    int black;
    double medianage;
    int occupiedhouseunits;
    int houseunits;
    int families;
    int households;
    int husbwife;
    int males;
    int females;
    int householdschildren;
    int hispanic;

    RetailerTuple(const std::vector<std::string>& fields) {
        locn = std::stoi(fields[0]);
        dateid = std::stoi(fields[1]);
        ksn = std::stoi(fields[2]);
        inventoryunits = std::stoi(fields[3]);
        subcategory = (int8_t) std::stoi(fields[4]);
        category = (int8_t) std::stoi(fields[5]);
        categoryCluster = (int8_t) std::stoi(fields[6]);
        prize = std::stof(fields[7]);
        rain = (int8_t) std::stoi(fields[8]);
        snow = (int8_t) std::stoi(fields[9]);
        maxtemp = std::stoi(fields[10]);
        mintemp = std::stoi(fields[11]);
        meanwind = std::stof(fields[12]);
        thunder = (int8_t) std::stoi(fields[13]);
        zip = std::stoi(fields[14]);
        rgn_cd = std::stoi(fields[15]);
        clim_zn_nbr = std::stoi(fields[16]);
        tot_area_sq_ft = std::stoi(fields[17]);
        sell_area_sq_ft = std::stoi(fields[18]);
        avghhi = std::stoi(fields[19]);
        supertargetdistance = std::stof(fields[20]);
        supertargetdrivetime = std::stof(fields[21]);
        targetdistance = std::stof(fields[22]);
        targetdrivetime = std::stof(fields[23]);
        walmartdistance = std::stof(fields[24]);
        walmartdrivetime = std::stof(fields[25]);
        walmartsupercenterdistance = std::stof(fields[26]);
        walmartsupercenterdrivetime = std::stof(fields[27]);
        population = std::stoi(fields[28]);
        white = std::stoi(fields[29]);
        asian = std::stoi(fields[30]);
        pacific = std::stoi(fields[31]);
        black = std::stoi(fields[32]);
        medianage = std::stof(fields[33]);
        occupiedhouseunits = std::stoi(fields[34]);
        houseunits = std::stoi(fields[35]);
        families = std::stoi(fields[36]);
        households = std::stoi(fields[37]);
        husbwife = std::stoi(fields[38]);
        males = std::stoi(fields[39]);
        females = std::stoi(fields[40]);
        householdschildren = std::stoi(fields[41]);
        hispanic = std::stoi(fields[42]);
    }
};

struct ModelEvaluator {

    void load_test_dataset(std::vector<RetailerTuple>& dataset) {
        std::ifstream input("test_retailer_full_join.txt");      
        if (!input) {
            std::cerr << "test_retailer_full_join.txt does not exist.\n";
            exit(1);
        }
        CSVAdaptor row(',');
        while (input >> row) {
            dataset.push_back(RetailerTuple(row.data));
        }

        input.close();
    }

    double compute_rmse(double params[]) {
        std::vector<RetailerTuple> test_dataset;
        load_test_dataset(test_dataset);

        double diff, error = 0.0;
        for (RetailerTuple& tuple : test_dataset) {
            diff =
                params[0] +
                tuple.inventoryunits * params[1] +
                tuple.subcategory * params[2] +
                tuple.category * params[3] +
                tuple.categoryCluster * params[4] +
                tuple.prize * params[5] +
                tuple.rain * params[6] +
                tuple.snow * params[7] +
                tuple.maxtemp * params[8] +
                tuple.mintemp * params[9] +
                tuple.meanwind * params[10] +
                tuple.thunder * params[11] +
                tuple.rgn_cd * params[12] +
                tuple.clim_zn_nbr * params[13] +
                tuple.tot_area_sq_ft * params[14] +
                tuple.sell_area_sq_ft * params[15] +
                tuple.avghhi * params[16] +
                tuple.supertargetdistance * params[17] +
                tuple.supertargetdrivetime * params[18] +
                tuple.targetdistance * params[19] +
                tuple.targetdrivetime * params[20] +
                tuple.walmartdistance * params[21] +
                tuple.walmartdrivetime * params[22] +
                tuple.walmartsupercenterdistance * params[23] +
                tuple.walmartsupercenterdrivetime * params[24] +
                tuple.population * params[25] +
                tuple.white * params[26] +
                tuple.asian * params[27] +
                tuple.pacific * params[28] +
                tuple.black * params[29] +
                tuple.medianage * params[30] +
                tuple.occupiedhouseunits * params[31] +
                tuple.houseunits * params[32] +
                tuple.families * params[33] +
                tuple.households * params[34] +
                tuple.husbwife * params[35] +
                tuple.males * params[36] +
                tuple.females * params[37] +
                tuple.householdschildren * params[38] +
                tuple.hispanic * params[39];

            error += diff * diff;
        }
        error /= test_dataset.size();
        return sqrt(error);
    }
};

// ModelEvaluator model_evaluator;

BGDSolver<39, 0> bgd_solver;

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

    const auto& cofactor_matrix = data.get_V_locn_IIWLC1();

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

size_t output_semaphore = 0;

void Application::on_snapshot(dbtoaster::data_t& data) {
    bool can_print = (output_semaphore == 0);

#ifdef CHECKPOINT_SUPPRESS_FACTOR
    if (output_semaphore == 0) {
        output_semaphore = CHECKPOINT_SUPPRESS_FACTOR;
    }
    --output_semaphore;
#endif

#ifdef ON_SNAPSHOT_CHECKPOINT
    if (can_print) {
        print_checkpoint(data);
    }
#endif

#ifdef ON_SNAPSHOT_TRAIN
    size_t num_iterations = 0;
    double error = 0;
    long train_time_ms;
    train(data, num_iterations, error, train_time_ms);

    #ifdef ON_SNAPSHOT_TRAIN_CHECKPOINT
        if (can_print) {
            std::cout << "tuples = " << data.tN
                    << "   num_iterations: " << num_iterations
                    << "   train_time_ms = " << train_time_ms
                    << "   error: " << error
                    << "   total_num_training = " << total_num_training
                    << "   total_num_iterations = " << total_num_iterations
                    << "   total_train_time_ms = " << total_train_time_ms
                    << std::endl;
        }
    #endif
#endif
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
    bgd_solver.reset();
    total_num_training = 0L;
    total_num_iterations = 0L;
    total_train_time_ms = 0L;
    output_semaphore = 0;

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

#endif /* APPLICATION_RETAILER_REGRESSION_HPP */