#ifndef APPLICATION_HOUSING_LISTING_JOIN_HPP
#define APPLICATION_HOUSING_LISTING_JOIN_HPP

#include "application_housing_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating listing join result... " << endl;

    const auto& v_postcode_HSIRDT1 = data.get_V_postcode_HSIRDT1();

    size_t output_size = v_postcode_HSIRDT1.store.size();
    size_t total_multiplicity = 0;

    for (auto &t : v_postcode_HSIRDT1.store) {
        if (print_result) cout << t.first << " -> " << t.second << endl;
        total_multiplicity += t.second;
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_HOUSING_LISTING_JOIN_HPP */