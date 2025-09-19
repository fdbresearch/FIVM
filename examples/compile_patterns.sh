#!/bin/bash

mkdir -p generated/cpp/pattern
mkdir -p bin/pattern

../bin/run_backend.sh --batch -o generated/cpp/pattern/query1.hpp queries/pattern/query1.m3
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -include generated/cpp/pattern/query1.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query1_BATCH_1000
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -DSNAPSHOT_INTERVAL=100000 -DON_SNAPSHOT_CHECKPOINT -include generated/cpp/pattern/query1.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query1_BATCH_1000_trace

../bin/run_backend.sh --batch -o generated/cpp/pattern/query1_reeval.hpp queries/pattern/query1_reeval.m3
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -include generated/cpp/pattern/query1_reeval.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query1_reeval_BATCH_1000
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -DSNAPSHOT_INTERVAL=100000 -DON_SNAPSHOT_CHECKPOINT -include generated/cpp/pattern/query1_reeval.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query1_reeval_BATCH_1000_trace

../bin/run_backend.sh --batch -o generated/cpp/pattern/query2.hpp queries/pattern/query2.m3
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -include generated/cpp/pattern/query2.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query2_BATCH_1000
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -DSNAPSHOT_INTERVAL=150000 -DON_SNAPSHOT_CHECKPOINT -include generated/cpp/pattern/query2.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query2_BATCH_1000_trace

../bin/run_backend.sh --batch -o generated/cpp/pattern/query2_reeval.hpp queries/pattern/query2_reeval.m3
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -include generated/cpp/pattern/query2_reeval.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query2_reeval_BATCH_1000
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -DSNAPSHOT_INTERVAL=150000 -DON_SNAPSHOT_CHECKPOINT -include generated/cpp/pattern/query2_reeval.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query2_reeval_BATCH_1000_trace

../bin/run_backend.sh --batch -o generated/cpp/pattern/query3.hpp queries/pattern/query3.m3
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -include generated/cpp/pattern/query3.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query3_BATCH_1000
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -DSNAPSHOT_INTERVAL=300000 -DON_SNAPSHOT_CHECKPOINT -include generated/cpp/pattern/query3.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query3_BATCH_1000_trace

../bin/run_backend.sh --batch -o generated/cpp/pattern/query3_reeval.hpp queries/pattern/query3_reeval.m3
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -include generated/cpp/pattern/query3_reeval.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query3_reeval_BATCH_1000
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic src/main.cpp -I ../backend/lib -I src -I src/lib -DBATCH_SIZE=1000 -DSNAPSHOT_INTERVAL=300000 -DON_SNAPSHOT_CHECKPOINT -include generated/cpp/pattern/query3_reeval.hpp -include src/application/pattern/application_pattern.hpp -o bin/pattern/query3_reeval_BATCH_1000_trace

./bin/pattern/query1_BATCH_1000_trace --no-output
./bin/pattern/query1_reeval_BATCH_1000_trace --no-output
./bin/pattern/query2_BATCH_1000_trace --no-output
./bin/pattern/query2_reeval_BATCH_1000_trace --no-output
./bin/pattern/query3_BATCH_1000_trace --no-output
./bin/pattern/query3_reeval_BATCH_1000_trace --no-output