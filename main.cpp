#include <stdexcept>
#include <iostream>
#include <boost/program_options.hpp>
#include "cppkafka/producer.h"
#include "cppkafka/configuration.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using std::string;
using std::exception;
using std::getline;
using std::cin;
using std::cout;
using std::endl;

using cppkafka::Producer;
using cppkafka::Configuration;
using cppkafka::Topic;
using cppkafka::MessageBuilder;

typedef unsigned char byte;

int main() {
    string brokers="127.0.0.1:9092";
    string topic_name1="my_topic";
    int partition_value = -1;


    // Create a message builder for this topic
    MessageBuilder builder(topic_name1);

    // Get the partition we want to write to. If no partition is provided, this will be
    // an unassigned one
    if (partition_value != -1) {
        builder.partition(partition_value);
    }

    // Construct the configuration
    Configuration config = {
            { "metadata.broker.list", brokers }
    };

    // Create the producer
    Producer producer(config);

    cout << "Producing messages into topic " << topic_name1 << endl;

    // Now read lines and write them into kafka

    string img="a.png";
    Mat image=imread(img);
    std::vector<byte> bayt;
    //imshow("asd",image);
    //cv::resize(image, image, cv::Size(image.cols / 5, image.rows / 5));
    cout<<image.cols<<" "<<image.rows<<endl;
    int size = image.total() * image.elemSize();
    std::vector<byte> img_bytes(size);
    img_bytes.assign(image.datastart, image.dataend);

    string line;
    while (getline(cin, line)) {
        // Set the payload on this builder
        builder.payload((cppkafka::Buffer &&) img_bytes);
        // Actually produce the message we've built
        producer.produce(builder);
    }

}