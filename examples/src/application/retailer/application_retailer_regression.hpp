#ifndef APPLICATION_RETAILER_REGRESSION_HPP
#define APPLICATION_RETAILER_REGRESSION_HPP

#include <cmath>
#include <iomanip>
#include "application_retailer_base.hpp"
#include "bgd_solver.hpp"
#include "csvreader.hpp"

struct RetailerTuple {
    int locn;
    int dateid;
    int ksn;
    double inventoryunits;
    int subcategory;
    int category;
    int categoryCluster;
    double prize;
    int rain;
    int snow;
    int maxtemp;
    int mintemp;
    double meanwind;
    int thunder;

    int zip;
    double rgn_cd;
    double clim_zn_nbr;
    double tot_area_sq_ft;
    double sell_area_sq_ft;
    double avghhi;
    double supertargetdistance;
    double supertargetdrivetime;
    double targetdistance;
    double targetdrivetime;
    double walmartdistance;
    double walmartdrivetime;
    double walmartsupercenterdistance;
    double walmartsupercenterdrivetime;
    double population;
    double white;
    double asian;
    double pacific;
    double black;
    double medianage;
    double occupiedhouseunits;
    double houseunits;
    double families;
    double households;
    double husbwife;
    double males;
    double females;
    double householdschildren;
    double hispanic;

    RetailerTuple(const std::vector<std::string>& fields) {
        locn = std::stoi(fields[0]);
        dateid = std::stoi(fields[1]);
        ksn = std::stoi(fields[2]);
        inventoryunits = std::stof(fields[3]);
        subcategory = std::stoi(fields[4]);
        category = std::stoi(fields[5]);
        categoryCluster = std::stoi(fields[6]);
        prize = std::stof(fields[7]);
        rain = std::stoi(fields[8]);
        snow = std::stoi(fields[9]);
        maxtemp = std::stoi(fields[10]);
        mintemp = std::stoi(fields[11]);
        meanwind = std::stof(fields[12]);
        thunder = std::stoi(fields[13]);
        zip = std::stoi(fields[14]);
        rgn_cd = std::stof(fields[15]);
        clim_zn_nbr = std::stof(fields[16]);
        tot_area_sq_ft = std::stof(fields[17]);
        sell_area_sq_ft = std::stof(fields[18]);
        avghhi = std::stof(fields[19]);
        supertargetdistance = std::stof(fields[20]);
        supertargetdrivetime = std::stof(fields[21]);
        targetdistance = std::stof(fields[22]);
        targetdrivetime = std::stof(fields[23]);
        walmartdistance = std::stof(fields[24]);
        walmartdrivetime = std::stof(fields[25]);
        walmartsupercenterdistance = std::stof(fields[26]);
        walmartsupercenterdrivetime = std::stof(fields[27]);
        population = std::stof(fields[28]);
        white = std::stof(fields[29]);
        asian = std::stof(fields[30]);
        pacific = std::stof(fields[31]);
        black = std::stof(fields[32]);
        medianage = std::stof(fields[33]);
        occupiedhouseunits = std::stof(fields[34]);
        houseunits = std::stof(fields[35]);
        families = std::stof(fields[36]);
        households = std::stof(fields[37]);
        husbwife = std::stof(fields[38]);
        males = std::stof(fields[39]);
        females = std::stof(fields[40]);
        householdschildren = std::stof(fields[41]);
        hispanic = std::stof(fields[42]);
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
                tuple.locn * params[1] +
                tuple.dateid * params[2] +
                tuple.ksn * params[3] +
                tuple.inventoryunits * params[4] +
                tuple.subcategory * params[5] +
                tuple.category * params[6] +
                tuple.categoryCluster * params[7] +
                tuple.prize * params[8] +
                tuple.rain * params[9] +
                tuple.snow * params[10] +
                tuple.maxtemp * params[11] +
                tuple.mintemp * params[12] +
                tuple.meanwind * params[13] +
                tuple.thunder * params[14] +
                tuple.zip * params[15] +
                tuple.rgn_cd * params[16] +
                tuple.clim_zn_nbr * params[17] +
                tuple.tot_area_sq_ft * params[18] +
                tuple.sell_area_sq_ft * params[19] +
                tuple.avghhi * params[20] +
                tuple.supertargetdistance * params[21] +
                tuple.supertargetdrivetime * params[22] +
                tuple.targetdistance * params[23] +
                tuple.targetdrivetime * params[24] +
                tuple.walmartdistance * params[25] +
                tuple.walmartdrivetime * params[26] +
                tuple.walmartsupercenterdistance * params[27] +
                tuple.walmartsupercenterdrivetime * params[28] +
                tuple.population * params[29] +
                tuple.white * params[30] +
                tuple.asian * params[31] +
                tuple.pacific * params[32] +
                tuple.black * params[33] +
                tuple.medianage * params[34] +
                tuple.occupiedhouseunits * params[35] +
                tuple.houseunits * params[36] +
                tuple.families * params[37] +
                tuple.households * params[38] +
                tuple.husbwife * params[39] +
                tuple.males * params[40] +
                tuple.females * params[41] +
                tuple.householdschildren * params[42] +
                tuple.hispanic * params[43];

            error += diff * diff;
        }
        error /= test_dataset.size();
        return sqrt(error);
    }
};

BGDSolver<43, 3> bgd_solver;
ModelEvaluator model_evaluator;

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
    // double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    // std::cout << "RMSE: " << rmse << std::endl;
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {
    const auto& cofactor_matrix = data.get_V_locn_IIWLC1();

    bgd_solver.build_sigma_matrix(cofactor_matrix);

    // std::cout << "initial params: [ ";
    // for (size_t i = 0; i < bgd_solver.NUM_PARAMS; i++) {
    //     std::cout << std::setprecision(10) <<  bgd_solver.params[i] << " ";
    // }
    // std::cout << " ]" << std::endl;

    size_t num_iterations = 0.0;
    double error = 0.0;
    bgd_solver.run(num_iterations, error);

    std::cout << "number of iterations: " << num_iterations << "   error: " << error << std::endl;

    // std::cout << "params: [ ";
    // for (size_t i = 0; i < bgd_solver.NUM_PARAMS; i++) {
    //     std::cout << bgd_solver.params[i] << " ";
    // }
    // std::cout << " ]" << std::endl;

    // double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    // std::cout << "RMSE: " << rmse << std::endl;
}

#endif /* APPLICATION_RETAILER_REGRESSION_HPP */