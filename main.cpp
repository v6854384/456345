#include <iostream>
#include <thread>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include "picosha2.h"

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

using namespace std;

void initLog_true(){
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log(
        keywords::file_name = "sample.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );

    logging::add_common_attributes();
}

void initLog_false(){
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log(
        keywords::file_name = "sample.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    logging::core::get()->set_filter(
        logging::trivial::severity <= logging::trivial::trace
    );

    //logging::core::get()->add_global_attribute("MyAttr", attrs::constant<string>(hashHelper));

    logging::add_common_attributes();
}

void generKey(){
    bool flag = false;
    string hashHelper;

    while (flag == false){

        int a = rand();

        const vector<int> data_vector{a};
        hashHelper = picosha2::hash256_hex_string(data_vector);

        cout << hashHelper << endl;

        int j = 0;

        for (size_t i = hashHelper.length() - 3; i <= hashHelper.length(); i++) {
            if (hashHelper[i] == '0') {
                j++;
                if (j == 4)
                    flag = true;
            }
            else
                flag = false;
        }

        if (flag == true) {
            initLog_true();
            BOOST_LOG_TRIVIAL(info) << hashHelper;
        }
        else {
            initLog_false();
            BOOST_LOG_TRIVIAL(trace) << a <<"   " <<hashHelper;
        }
        flag = false;
    }
}

int main() {
    auto countThrd = thread::hardware_concurrency();

    cout << "max count of threads = " << countThrd << endl;

    thread t1([](){
        this_thread::sleep_for(chrono::milliseconds(0));
        generKey();
    });
    thread t2([](){
        this_thread::sleep_for(chrono::milliseconds(100));
        generKey();
    });
    thread t3([](){
        this_thread::sleep_for(chrono::milliseconds(200));
        generKey();
    });
    thread t4([](){
        this_thread::sleep_for(chrono::milliseconds(300));
        generKey();
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
