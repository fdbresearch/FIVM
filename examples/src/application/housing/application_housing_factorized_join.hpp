#ifndef APPLICATION_HOUSING_FACTORIZED_JOIN_HPP
#define APPLICATION_HOUSING_FACTORIZED_JOIN_HPP

#include "application_housing_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating factorized join result... " << endl;

    // cout << "  sizeof(V_postcode_HSIRDT1) = " << data.get_V_postcode_HSIRDT1().store.size() << endl;
    // cout << "  sizeof(V_house_H1) = " << data.get_V_house_H1().count() << endl;
    // cout << "  sizeof(V_sainsburys_S1) = " << data.get_V_sainsburys_S1().count() << endl;
    // cout << "  sizeof(V_typeeducation_I1) = " << data.get_V_typeeducation_I1().count() << endl;
    // cout << "  sizeof(V_pricerangerest_R1) = " << data.get_V_pricerangerest_R1().count() << endl;
    // cout << "  sizeof(V_unemployment_D1) = " << data.get_V_unemployment_D1().count() << endl;
    // cout << "  sizeof(V_nbbuslines_T1) = " << data.get_V_nbbuslines_T1().count() << endl;

    size_t output_size = 0; 
    size_t total_multiplicity = 0;

    const auto& v_postcode_HSIRDT1 = data.get_V_postcode_HSIRDT1();

    // For each V_postcode_HSIRDT1(postcode)
    for (auto &t1 : v_postcode_HSIRDT1.store) {
        auto &key1 = t1.first;
        auto postcode = std::get<0>(key1);

        { // For each V_house_H1(postcode) -> (livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking)
            V_house_H1_entry e2;
            const auto& rel2 = data.get_V_house_H1().getValueOrDefault(e2.modify(postcode));
            for (auto &t2 : rel2.store) {
                auto &key2 = t2.first;
                long value2 = t2.second;

                // For each V_sainsburys_S1(postcode) -> (openinghoursshop, pricerangeshop, sainsburys, tesco, ms)
                V_sainsburys_S1_entry e3;
                const auto& rel3 = data.get_V_sainsburys_S1().getValueOrDefault(e3.modify(postcode));
                for (auto &t3 : rel3.store) {
                    auto &key3 = t3.first;
                    long value3 = t3.second;
                    long value23 = value2 * value3;

                    // For each V_typeeducation_I1(postcode) -> (typeeducation, sizeinstitution)
                    V_typeeducation_I1_entry e4;
                    const auto& rel4 = data.get_V_typeeducation_I1().getValueOrDefault(e4.modify(postcode));
                    for (auto &t4 : rel4.store) {
                        auto &key4 = t4.first;
                        long value4 = t4.second;
                        long value234 = value23 * value4;

                        // For each V_pricerangerest_R1(postcode) -> (openinghoursrest, pricerangerest)
                        V_pricerangerest_R1_entry e5;
                        const auto& rel5 = data.get_V_pricerangerest_R1().getValueOrDefault(e5.modify(postcode));
                        for (auto &t5 : rel5.store) {
                            auto &key5 = t5.first;
                            long value5 = t5.second;
                            long value2345 = value234 * value5;

                            // For each V_unemployment_D1(postcode) -> (averagesalary, crimesperyear, unemployment, nbhospitals)
                            V_unemployment_D1_entry e6;
                            const auto& rel6 = data.get_V_unemployment_D1().getValueOrDefault(e6.modify(postcode));
                            for (auto &t6 : rel6.store) {
                                auto &key6 = t6.first;
                                long value6 = t6.second;
                                long value23456 = value2345 * value6;

                                // For each V_nbbuslines_T1(postcode) -> (nbbuslines, nbtrainstations, distancecitycentre)
                                V_nbbuslines_T1_entry e7;
                                const auto& rel7 = data.get_V_nbbuslines_T1().getValueOrDefault(e7.modify(postcode));
                                for (auto &t7 : rel7.store) {
                                    auto &key7 = t7.first;
                                    long value7 = t7.second;

                                    auto key = std::tuple_cat(key1, key2, key3, key4, key5, key6, key7);
                                    long value = value23456 * value7;

                                    output_size++;
                                    total_multiplicity += value;

                                    if (print_result) cout << key << " -> " << value << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_HOUSING_FACTORIZED_JOIN_HPP */