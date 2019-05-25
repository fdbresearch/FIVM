#ifndef APPLICATION_TPCH_FQ2_FACTORIZED_JOIN_HPP
#define APPLICATION_TPCH_FQ2_FACTORIZED_JOIN_HPP

#include "application_tpch_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating factorized join result... " << endl;

    // cout << "  sizeof(V_orderkey_LPCNO1) = " << data.get_V_orderkey_LPCNO1().store.size() << endl;
    // cout << "  sizeof(V_partkey_LP1) = " << data.get_V_partkey_LP1().count() << endl;
    // cout << "  sizeof(V_l_suppkey_L1) = " << data.get_V_l_suppkey_L1().count() << endl;
    // cout << "  sizeof(V_p_name_P1) = " << data.get_V_p_name_P1().count() << endl;
    // cout << "  sizeof(V_custkey_CNO1) = " << data.get_V_custkey_CNO1().count() << endl;
    // cout << "  sizeof(V_nationkey_CN1) = " << data.get_V_nationkey_CN1().count() << endl;
    // cout << "  sizeof(V_c_name_C1) = " << data.get_V_c_name_C1().count() << endl;
    // cout << "  sizeof(V_n_name_N1) = " << data.get_V_n_name_N1().count() << endl;
    // cout << "  sizeof(V_o_orderstatus_O1) = " << data.get_V_o_orderstatus_O1().count() << endl;

    size_t output_size = 0; 
    size_t total_multiplicity = 0;

    const auto& v_orderkey_LPCNO1 = data.get_V_orderkey_LPCNO1();

    // For each v_orderkey_LPCNO1(orderkey)
    for (auto &t1 : v_orderkey_LPCNO1.store) {
        auto &key1 = t1.first;
        auto orderkey = std::get<0>(key1);

        { // For each V_partkey_LP1(orderkey) -> partkey
            V_partkey_LP1_entry e2;
            const auto& rel2 = data.get_V_partkey_LP1().getValueOrDefault(e2.modify(orderkey));
            for (auto &t2 : rel2.store) {
                auto &key2 = t2.first;
                auto partkey = std::get<0>(key2);

                // For each V_l_suppkey_L1(orderkey, partkey) -> (l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment)
                V_l_suppkey_L1_entry e3;
                const auto& rel3 = data.get_V_l_suppkey_L1().getValueOrDefault(e3.modify(orderkey, partkey));
                for (auto &t3 : rel3.store) {
                    auto &key3 = t3.first;
                    long value3 = t3.second;

                    // For each V_p_name_P1(partkey) -> (p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment)
                    V_p_name_P1_entry e4;
                    const auto& rel4 = data.get_V_p_name_P1().getValueOrDefault(e4.modify(partkey));
                    for (auto &t4 : rel4.store) {
                        auto &key4 = t4.first;
                        long value4 = t4.second;
                        long value34 = value3 * value4;

                        // For each V_custkey_CNO1(orderkey) -> custkey
                        V_custkey_CNO1_entry e5;
                        const auto& rel5 = data.get_V_custkey_CNO1().getValueOrDefault(e5.modify(orderkey));
                        for (auto &t5 : rel5.store) {
                            auto &key5 = t5.first;
                            auto custkey = std::get<0>(key5);

                            // For each V_nationkey_CN1(custkey) -> nationkey
                            V_nationkey_CN1_entry e6;
                            const auto& rel6 = data.get_V_nationkey_CN1().getValueOrDefault(e6.modify(custkey));
                            for (auto &t6 : rel6.store) {
                                auto &key6 = t6.first;
                                auto nationkey = std::get<0>(key6);

                                // For each V_c_name_C1(custkey, nationkey) -> (c_name, c_address, c_phone, c_acctbal, c_mktsegment, c_comment)
                                V_c_name_C1_entry e7;
                                const auto& rel7 = data.get_V_c_name_C1().getValueOrDefault(e7.modify(custkey, nationkey));
                                for (auto &t7 : rel7.store) {
                                    auto &key7 = t7.first;
                                    long value7 = t7.second;
                                    long value347 = value34 * value7;

                                    // For each V_n_name_N1(nationkey) -> (n_name, n_regionkey, n_comment)
                                    V_n_name_N1_entry e8;
                                    const auto& rel8 = data.get_V_n_name_N1().getValueOrDefault(e8.modify(nationkey));
                                    for (auto &t8 : rel8.store) {
                                        auto &key8 = t8.first;
                                        long value8 = t8.second;
                                        long value3478 = value347 * value8;

                                        // For each V_o_orderstatus_O1(orderkey, custkey) -> (o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment)
                                        V_o_orderstatus_O1_entry e9;
                                        const auto& rel9 = data.get_V_o_orderstatus_O1().getValueOrDefault(e9.modify(orderkey, custkey));
                                        for (auto &t9 : rel9.store) {
                                            auto &key9 = t9.first;
                                            long value9 = t9.second;

                                            auto key = std::tuple_cat(key1, key2, key3, key4, key5, key6, key7, key8, key9);
                                            long value = value3478 * value9;

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
        }
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_TPCH_FQ2_FACTORIZED_JOIN_HPP */