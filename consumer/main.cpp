#include <stdexcept>
#include <iostream>
#include <csignal>
#include <boost/program_options.hpp>
#include "cppkafka/consumer.h"
#include "cppkafka/configuration.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using std::string;
using std::exception;
using std::cout;
using std::endl;

using cppkafka::Consumer;
using cppkafka::Configuration;
using cppkafka::Message;
using cppkafka::TopicPartitionList;


bool running = true;
typedef unsigned char byte;

int main(int argc, char* argv[]) {
    string brokers="127.0.0.1:9092";
    string topic_name="my_topic";
    string group_id="0";


    // Stop processing on SIGINT
    signal(SIGINT, [](int) { running = false; });

    // Construct the configuration
    Configuration config = {
            { "metadata.broker.list", brokers },
            { "group.id", group_id },
            // Disable auto commit
            { "enable.auto.commit", false }
    };

    // Create the consumer
    Consumer consumer(config);

    // Print the assigned partitions on assignment
    consumer.set_assignment_callback([](const TopicPartitionList& partitions) {
        cout << "Got assigned: " << partitions << endl;
    });

    // Print the revoked partitions on revocation
    consumer.set_revocation_callback([](const TopicPartitionList& partitions) {
        cout << "Got revoked: " << partitions << endl;
    });

    // Subscribe to the topic
    consumer.subscribe({ topic_name });

    cout << "Consuming messages from topic " << topic_name << endl;

    // Now read lines and write them into kafka
    while (running) {
        // Try to consume a message
        Message msg = consumer.poll();
        if (msg) {
            // If we managed to get a message
            if (msg.get_error()) {
                // Ignore EOF notifications from rdkafka
                if (!msg.is_eof()) {
                    cout << "[+] Received error notification: " << msg.get_error() << endl;
                }
            }
            else {
                // Print the key (if any)
                if (msg.get_key()) {
                    cout << msg.get_key() << " -> ";
                }
                // Print the payload
                cout << "::::"<<msg.get_payload() << endl;
                std::vector<byte> bytes(msg.get_payload().get_size());
                bytes.assign(msg.get_payload().begin(), msg.get_payload().end());
                cv::Mat image = cv::Mat(89,96,CV_8UC3,bytes.data()).clone();
                imshow("asd",image);
                waitKey(0);
                // Now commit the message
                consumer.commit(msg);
            }
        }
    }
}