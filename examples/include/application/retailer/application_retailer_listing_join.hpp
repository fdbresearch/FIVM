#ifndef APPLICATION_RETAILER_LISTING_JOIN_HPP
#define APPLICATION_RETAILER_LISTING_JOIN_HPP

#include <stddef.h>

#include <iostream>

#include "application.hpp"

class RetailerListingJoinApplication : public Application {
 public:
  void onSnapshot(const dbtoaster::data_t& data) override {
    onEndProcessing(data, false);
  }

  void onBeginProcessing(const dbtoaster::data_t& data) override {}

  void onEndProcessing(const dbtoaster::data_t& data,
                       bool print_result) override {
    std::cout << "\nEnumerating listing join result... " << std::endl;

    const auto& viewIIWLC = data.get_V_locn_IIWLC1();

    size_t output_size = viewIIWLC.store.size();
    size_t total_multiplicity = 0;

    for (auto& t : viewIIWLC.store) {
      if (print_result) std::cout << t.first << " -> " << t.second << std::endl;
      total_multiplicity += t.second;
    }

    std::cout << "Number of output tuples: " << output_size << std::endl;
    std::cout << "Total multiplicity: " << total_multiplicity << std::endl;
  }
};

std::unique_ptr<Application> createApplication() {
  return std::make_unique<RetailerListingJoinApplication>();
}

#endif /* APPLICATION_RETAILER_LISTING_JOIN_HPP */