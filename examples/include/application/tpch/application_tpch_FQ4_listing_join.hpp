#ifndef APPLICATION_TPCH_FQ4_LISTING_JOIN_HPP
#define APPLICATION_TPCH_FQ4_LISTING_JOIN_HPP

#include <stddef.h>

#include <iostream>

#include "application.hpp"

class TpchFQ4ListingJoinApplication : public Application {
 public:
  void onSnapshot(const dbtoaster::data_t& data) override {
    onEndProcessing(data, false);
  }

  void onBeginProcessing(const dbtoaster::data_t& data) override {}

  void onEndProcessing(const dbtoaster::data_t& data,
                       bool print_result) override {
    std::cout << "\nEnumerating listing join result... " << std::endl;

    const auto& v_suppkey_LPS1 = data.get_V_suppkey_LPS1();

    size_t output_size = v_suppkey_LPS1.store.size();
    size_t total_multiplicity = 0;

    for (auto& t : v_suppkey_LPS1.store) {
      if (print_result) cout << t.first << " -> " << t.second << endl;
      total_multiplicity += t.second;
    }

    std::cout << "Number of output tuples: " << output_size << std::endl;
    std::cout << "Total multiplicity: " << total_multiplicity << std::endl;
  }
};

std::unique_ptr<Application> createApplication() {
  return std::make_unique<TpchFQ4ListingJoinApplication>();
}

#endif /* APPLICATION_TPCH_FQ4_LISTING_JOIN_HPP */