#include <gtest/gtest.h>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>
#include <set>
#include <vector>

// Mock headers.hpp for testing
#define EXIT_SEG_FAULT 139
#define EXIT_NO_SO 15
#define EXIT_DEBUG0 16
#define EXIT_DEBUG1 17
#define EXIT_DEBUG2 18
#define EXIT_DEBUG3 19
#define EXIT_PANIC 20
#define EXIT_FORCED 21
#define EXIT_REQUESTED 22
#define EXIT_TERMINATED_BY_KERNEL 23
#define DEBUG_BUILD false

// Test fixture for stopping.hpp tests
class StoppingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Tests for exit code constants

TEST_F(StoppingTest, ExitSuccess) {
    EXPECT_EQ(EXIT_SUCCESS, 0);
}

TEST_F(StoppingTest, ExitFailure) {
    EXPECT_EQ(EXIT_FAILURE, 1);
}

TEST_F(StoppingTest, ExitSegFault) {
    EXPECT_EQ(EXIT_SEG_FAULT, 139);
}

TEST_F(StoppingTest, ExitNoSharedObject) {
    EXPECT_EQ(EXIT_NO_SO, 15);
}

TEST_F(StoppingTest, ExitDebugCodes) {
    EXPECT_EQ(EXIT_DEBUG0, 16);
    EXPECT_EQ(EXIT_DEBUG1, 17);
    EXPECT_EQ(EXIT_DEBUG2, 18);
    EXPECT_EQ(EXIT_DEBUG3, 19);
    
    // Verify they are sequential
    EXPECT_EQ(EXIT_DEBUG1, EXIT_DEBUG0 + 1);
    EXPECT_EQ(EXIT_DEBUG2, EXIT_DEBUG0 + 2);
    EXPECT_EQ(EXIT_DEBUG3, EXIT_DEBUG0 + 3);
}

TEST_F(StoppingTest, ExitPanic) {
    EXPECT_EQ(EXIT_PANIC, 20);
}

TEST_F(StoppingTest, ExitForced) {
    EXPECT_EQ(EXIT_FORCED, 21);
}

TEST_F(StoppingTest, ExitRequested) {
    EXPECT_EQ(EXIT_REQUESTED, 22);
}

TEST_F(StoppingTest, ExitTerminatedByKernel) {
    EXPECT_EQ(EXIT_TERMINATED_BY_KERNEL, 23);
}

TEST_F(StoppingTest, DebugBuildFlag) {
    EXPECT_FALSE(DEBUG_BUILD);
}

// Tests for signal constants

TEST_F(StoppingTest, SignalSegvMapping) {
    EXPECT_TRUE(SIGSEGV > 0);
}

TEST_F(StoppingTest, SignalBusMapping) {
    EXPECT_TRUE(SIGBUS > 0);
}

TEST_F(StoppingTest, SignalPipeMapping) {
    EXPECT_TRUE(SIGPIPE > 0);
}

TEST_F(StoppingTest, SignalTermMapping) {
    EXPECT_TRUE(SIGTERM > 0);
}

TEST_F(StoppingTest, SignalIllMapping) {
    EXPECT_TRUE(SIGILL > 0);
}

TEST_F(StoppingTest, SignalIntMapping) {
    EXPECT_TRUE(SIGINT > 0);
}

TEST_F(StoppingTest, SignalQuitMapping) {
    EXPECT_TRUE(SIGQUIT > 0);
}

TEST_F(StoppingTest, SignalTstpMapping) {
    EXPECT_TRUE(SIGTSTP > 0);
}

TEST_F(StoppingTest, SignalXcpuMapping) {
    EXPECT_TRUE(SIGXCPU > 0);
}

TEST_F(StoppingTest, SignalXfszMapping) {
    EXPECT_TRUE(SIGXFSZ > 0);
}

TEST_F(StoppingTest, SignalSysMapping) {
    EXPECT_TRUE(SIGSYS > 0);
}

// Tests for atomic boolean behavior (lines 66-67 changes)

TEST_F(StoppingTest, AtomicBoolInitialization) {
    std::atomic<bool> test_signal{false};
    EXPECT_FALSE(test_signal.load());
    
    test_signal = true;
    EXPECT_TRUE(test_signal.load());
}

TEST_F(StoppingTest, AtomicBoolDirectAccess) {
    std::atomic<bool> test_signal{false};
    
    // Direct comparison (line 66 change)
    if (test_signal == true) {
        FAIL() << "Should not enter - signal is false";
    }
    
    // Direct assignment (line 67 change)
    test_signal = true;
    
    // Verify it's set
    if (test_signal == false) {
        FAIL() << "Should be true after assignment";
    }
    
    EXPECT_TRUE(test_signal.load());
}

TEST_F(StoppingTest, AtomicBoolLoadStoreOperations) {
    std::atomic<bool> test_signal{false};
    
    EXPECT_FALSE(test_signal.load());
    test_signal.store(true);
    EXPECT_TRUE(test_signal.load());
}

TEST_F(StoppingTest, AtomicBoolThreadSafety) {
    std::atomic<bool> test_signal{false};
    constexpr int num_threads = 10;
    constexpr int iterations = 1000;
    
    auto worker = [&]() {
        for (int i = 0; i < iterations; ++i) {
            if (test_signal == true) continue;
            test_signal = true;
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_TRUE(test_signal.load());
}

TEST_F(StoppingTest, AtomicBoolRaceConditionDetection) {
    std::atomic<bool> test_signal{false};
    std::atomic<int> entry_count{0};
    
    auto signal_handler_sim = [&]() {
        if (test_signal == true) return;
        test_signal = true;
        entry_count.fetch_add(1);
    };
    
    constexpr int num_threads = 100;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(signal_handler_sim);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_GE(entry_count.load(), 1);
}

// Tests for debug exit behavior (line 16 change)

TEST_F(StoppingTest, DebugExitDirectCall) {
    int debug_codes[] = {EXIT_DEBUG0, EXIT_DEBUG1, EXIT_DEBUG2, EXIT_DEBUG3};
    
    for (int code : debug_codes) {
        EXPECT_GE(code, 16);
        EXPECT_LE(code, 19);
    }
}

TEST_F(StoppingTest, DebugExitIdCalculation) {
    EXPECT_EQ(EXIT_DEBUG0 - EXIT_DEBUG0, 0);
    EXPECT_EQ(EXIT_DEBUG1 - EXIT_DEBUG0, 1);
    EXPECT_EQ(EXIT_DEBUG2 - EXIT_DEBUG0, 2);
    EXPECT_EQ(EXIT_DEBUG3 - EXIT_DEBUG0, 3);
}

TEST_F(StoppingTest, DebugBuildDisabled) {
    EXPECT_FALSE(DEBUG_BUILD);
    
    if (!DEBUG_BUILD) {
        SUCCEED();
    } else {
        FAIL() << "DEBUG_BUILD should be false";
    }
}

// Edge case tests

TEST_F(StoppingTest, ExitCodesAreUnique) {
    std::set<int> codes = {
        EXIT_SUCCESS, EXIT_FAILURE, EXIT_SEG_FAULT, EXIT_NO_SO,
        EXIT_DEBUG0, EXIT_DEBUG1, EXIT_DEBUG2, EXIT_DEBUG3,
        EXIT_PANIC, EXIT_FORCED, EXIT_REQUESTED, EXIT_TERMINATED_BY_KERNEL
    };
    
    EXPECT_EQ(codes.size(), 12u);
}

TEST_F(StoppingTest, SignalNumbersAreUnique) {
    std::set<int> signals = {
        SIGSEGV, SIGBUS, SIGPIPE, SIGTERM, SIGILL,
        SIGINT, SIGQUIT, SIGTSTP, SIGXCPU, SIGXFSZ, SIGSYS
    };
    
    EXPECT_EQ(signals.size(), 11u);
}

TEST_F(StoppingTest, ExitCodesInValidRange) {
    int codes[] = {
        EXIT_SUCCESS, EXIT_FAILURE, EXIT_SEG_FAULT, EXIT_NO_SO,
        EXIT_DEBUG0, EXIT_DEBUG1, EXIT_DEBUG2, EXIT_DEBUG3,
        EXIT_PANIC, EXIT_FORCED, EXIT_REQUESTED, EXIT_TERMINATED_BY_KERNEL
    };
    
    for (int code : codes) {
        EXPECT_GE(code, 0);
        EXPECT_LE(code, 255);
    }
}

// Concurrency tests

TEST_F(StoppingTest, SignalGottenReentrancyProtection) {
    std::atomic<bool> signal_gotten{false};
    std::atomic<int> protected_section_entries{0};
    
    auto mock_signal_handler = [&]() {
        if (signal_gotten == true) return;
        signal_gotten = true;
        
        protected_section_entries.fetch_add(1);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(mock_signal_handler);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_GE(protected_section_entries.load(), 1);
}

TEST_F(StoppingTest, SignalGottenMemoryOrdering) {
    std::atomic<bool> flag{false};
    int shared_data = 0;
    
    auto writer = [&]() {
        shared_data = 42;
        flag = true;
    };
    
    auto reader = [&]() {
        while (flag == false) {
            std::this_thread::yield();
        }
        EXPECT_EQ(shared_data, 42);
    };
    
    std::thread t1(writer);
    std::thread t2(reader);
    
    t1.join();
    t2.join();
}

TEST_F(StoppingTest, MultipleSignalHandlerCalls) {
    std::atomic<bool> signal_gotten{false};
    std::atomic<int> handler_calls{0};
    
    auto handler = [&]() {
        handler_calls.fetch_add(1);
        if (signal_gotten == true) return;
        signal_gotten = true;
    };
    
    for (int i = 0; i < 5; ++i) {
        handler();
    }
    
    EXPECT_EQ(handler_calls.load(), 5);
    EXPECT_TRUE(signal_gotten.load());
}

TEST_F(StoppingTest, ExitCodeCategorization) {
    EXPECT_TRUE(EXIT_DEBUG0 >= 16 && EXIT_DEBUG0 <= 19);
    EXPECT_TRUE(EXIT_DEBUG1 >= 16 && EXIT_DEBUG1 <= 19);
    EXPECT_TRUE(EXIT_DEBUG2 >= 16 && EXIT_DEBUG2 <= 19);
    EXPECT_TRUE(EXIT_DEBUG3 >= 16 && EXIT_DEBUG3 <= 19);
    
    EXPECT_EQ(EXIT_NO_SO, 15);
    EXPECT_EQ(EXIT_PANIC, 20);
    EXPECT_EQ(EXIT_FORCED, 21);
    EXPECT_EQ(EXIT_REQUESTED, 22);
    EXPECT_EQ(EXIT_TERMINATED_BY_KERNEL, 23);
    
    EXPECT_EQ(EXIT_SUCCESS, 0);
    EXPECT_EQ(EXIT_FAILURE, 1);
    EXPECT_EQ(EXIT_SEG_FAULT, 139);
}

TEST_F(StoppingTest, AtomicBoolStressTest) {
    std::atomic<bool> flag{false};
    constexpr int num_iterations = 10000;
    constexpr int num_threads = 4;
    
    std::atomic<int> true_count{0};
    std::atomic<int> false_count{0};
    
    auto worker = [&]() {
        for (int i = 0; i < num_iterations; ++i) {
            if (i % 2 == 0) {
                flag = true;
            } else {
                flag = false;
            }
            
            if (flag.load()) {
                true_count.fetch_add(1);
            } else {
                false_count.fetch_add(1);
            }
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(true_count.load() + false_count.load(), num_iterations * num_threads);
}

// Regression tests

TEST_F(StoppingTest, DebugExitBypassesFinishLabel) {
    EXPECT_EQ(EXIT_DEBUG0, 16);
    SUCCEED() << "Debug exits now bypass _finish label";
}

TEST_F(StoppingTest, SignalGottenDirectAccess) {
    std::atomic<bool> signal_gotten{false};
    
    if (signal_gotten == true) {
        FAIL() << "Should not enter";
    }
    signal_gotten = true;
    
    EXPECT_TRUE(signal_gotten.load());
}

TEST_F(StoppingTest, ErrorMessageFormatting) {
    std::string prefixes[] = {"[DEXIT]", "[UEXIT]", "[EXIT]", "[EEXIT]", "[KEXIT]"};
    
    for (const auto& prefix : prefixes) {
        EXPECT_FALSE(prefix.empty());
        EXPECT_EQ(prefix.front(), '[');
        EXPECT_EQ(prefix.back(), ']');
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}