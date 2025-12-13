#ifndef APPLICATION_HOUSING_REGRESSION_HPP
#define APPLICATION_HOUSING_REGRESSION_HPP

#include <cmath>
#include <string>

#include "application.hpp"
#include "bgd_solver.hpp"
#include "csv_adaptor.hpp"
#include "stopwatch.hpp"

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
      diff = params[0] + tuple.postcode * params[1] + tuple.house * params[2] +
             tuple.flat * params[3] + tuple.unknown * params[4] +
             tuple.parking * params[5] + tuple.nbbedrooms * params[6] +
             tuple.nbbathrooms * params[7] + tuple.garden * params[8] +
             tuple.kitchensize * params[9] + tuple.livingarea * params[10] +
             tuple.price * params[11] + tuple.sainsburys * params[12] +
             tuple.tesco * params[13] + tuple.ms * params[14] +
             tuple.pricerangeshop * params[15] +
             tuple.openinghoursshop * params[16] +
             tuple.typeeducation * params[17] +
             tuple.sizeinstitution * params[18] +
             tuple.pricerangerest * params[19] +
             tuple.openinghoursrest * params[20] +
             tuple.unemployment * params[21] + tuple.nbhospitals * params[22] +
             tuple.crimesperyear * params[23] +
             tuple.averagesalary * params[24] + tuple.nbbuslines * params[25] +
             tuple.nbtrainstations * params[26] +
             tuple.distancecitycentre * params[27];

      error += diff * diff;
    }
    error /= test_dataset.size();
    return sqrt(error);
  }
};

class HousingRegressionApplication : public Application {
 public:
  void onSnapshot(const dbtoaster::data_t& data) override {
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

  void onBeginProcessing(const dbtoaster::data_t& data) override {
    bgd_solver.reset();
    total_num_training = 0L;
    total_num_iterations = 0L;
    total_train_time_ms = 0L;
    output_semaphore = 0;

    // double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    // std::cout << "RMSE: " << rmse << std::endl;
  }

  void onEndProcessing(const dbtoaster::data_t& data,
                       bool print_result) override {
    size_t num_iterations = 0;
    double error = 0;
    long train_time_ms;
    train(data, num_iterations, error, train_time_ms);

    std::cout << std::endl
              << "tuples = " << data.tN
              << "   num_iterations: " << num_iterations
              << "   train_time_ms = " << train_time_ms << "   error: " << error
              << "   total_num_training = " << total_num_training
              << "   total_num_iterations = " << total_num_iterations
              << "   total_train_time_ms = " << total_train_time_ms
              << std::endl;

    // double rmse = model_evaluator.compute_rmse(bgd_solver.params);
    // std::cout << "RMSE: " << rmse << std::endl;
  }

 private:
  void train(const dbtoaster::data_t& data /* in */,
             size_t& num_iterations /* out */, double& error /* out */,
             long& train_time_ms /* out */
  ) {
    Stopwatch sw;
    sw.restart();

    const auto& cofactor_matrix = data.get_V_postcode_HSIRDT1();

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

  // ModelEvaluator model_evaluator;
  BGDSolver<27, 10> bgd_solver;
  long total_num_training = 0L;
  long total_num_iterations = 0L;
  long total_train_time_ms = 0L;
  size_t output_semaphore = 0;
};

std::unique_ptr<Application> createApplication() {
  return std::make_unique<HousingRegressionApplication>();
}

#endif /* APPLICATION_HOUSING_REGRESSION_HPP */