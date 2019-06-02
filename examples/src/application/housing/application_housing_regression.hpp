#ifndef APPLICATION_HOUSING_REGRESSION_HPP
#define APPLICATION_HOUSING_REGRESSION_HPP

#include <cmath>
#include "application_housing_base.hpp"
#include "bgd_solver.hpp"
#include "csvreader.hpp"

struct HousingTuple {
    double postcode;
    double house;
    double flat;
    double unknown;
    double parking;
    double nbbedrooms;
    double nbbathrooms;
    double garden;
    double kitchensize;
    double livingarea;
    double price;
    double sainsburys;
    double tesco;
    double ms;
    double pricerangeshop;
    double openinghoursshop;
    double typeeducation;
    double sizeinstitution;
    double pricerangerest;
    double openinghoursrest;    
    double unemployment;
    double nbhospitals;
    double crimesperyear;
    double averagesalary;
    double nbbuslines;
    double nbtrainstations;
    double distancecitycentre;

    HousingTuple(const std::vector<std::string>& fields) {
        postcode = std::stof(fields[0]);
        house = std::stof(fields[1]);
        flat = std::stof(fields[2]);
        unknown = std::stof(fields[3]);
        parking = std::stof(fields[4]);
        nbbedrooms = std::stof(fields[5]);
        nbbathrooms = std::stof(fields[6]);
        garden = std::stof(fields[7]);
        kitchensize = std::stof(fields[8]);
        livingarea = std::stof(fields[9]);
        price = std::stof(fields[10]);
        sainsburys = std::stof(fields[11]);
        tesco = std::stof(fields[12]);
        ms = std::stof(fields[13]);
        pricerangeshop = std::stof(fields[14]);
        openinghoursshop = std::stof(fields[15]);
        typeeducation = std::stof(fields[16]);
        sizeinstitution = std::stof(fields[17]);
        pricerangerest = std::stof(fields[18]);
        openinghoursrest = std::stof(fields[19]);    
        unemployment = std::stof(fields[20]);
        nbhospitals = std::stof(fields[21]);
        crimesperyear = std::stof(fields[22]);
        averagesalary = std::stof(fields[23]);
        nbbuslines = std::stof(fields[24]);
        nbtrainstations = std::stof(fields[25]);
        distancecitycentre = std::stof(fields[26]);
    }
};

struct ModelEvaluator {

    void load_test_dataset(std::vector<HousingTuple>& dataset) {
        std::ifstream input("test_housing_full_join.txt");      
        if (!input) {
            std::cerr << "test_housing_full_join.txt does not exist.\n";
            exit(1);
        }
        CSVAdaptor row(',');
        while (input >> row) {
            dataset.push_back(HousingTuple(row.data));
        }

        input.close();
    }

    double compute_rmse(double params[]) {
        std::vector<HousingTuple> test_dataset;
        load_test_dataset(test_dataset);

        double diff, error = 0.0;
        for (HousingTuple& tuple : test_dataset) {
            diff =
                params[0] +
                tuple.postcode * params[1] +
                tuple.house * params[2] +
                tuple.flat * params[3] +
                tuple.unknown * params[4] +
                tuple.parking * params[5] +
                tuple.nbbedrooms * params[6] +
                tuple.nbbathrooms * params[7] +
                tuple.garden * params[8] +
                tuple.kitchensize * params[9] +
                tuple.livingarea * params[10] +
                tuple.price * params[11] +
                tuple.sainsburys * params[12] +
                tuple.tesco * params[13] +
                tuple.ms * params[14] +
                tuple.pricerangeshop * params[15] +
                tuple.openinghoursshop * params[16] +
                tuple.typeeducation * params[17] +
                tuple.sizeinstitution * params[18] +
                tuple.pricerangerest * params[19] +
                tuple.openinghoursrest * params[20] +
                tuple.unemployment * params[21] +
                tuple.nbhospitals * params[22] +
                tuple.crimesperyear * params[23] +
                tuple.averagesalary * params[24] +
                tuple.nbbuslines * params[25] +
                tuple.nbtrainstations * params[26] +
                tuple.distancecitycentre * params[27];

            error += diff * diff;
        }
        error /= test_dataset.size();
        return sqrt(error);
    }
};

BGDSolver<27, 10> bgd_solver;
ModelEvaluator model_evaluator;

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
    double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    std::cout << "RMSE: " << rmse << std::endl;
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {
    const auto& cofactor_matrix = data.get_V_postcode_HSIRDT1();

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

    double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    std::cout << "RMSE: " << rmse << std::endl;
}

#endif /* APPLICATION_HOUSING_REGRESSION_HPP */