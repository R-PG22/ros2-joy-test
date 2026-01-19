#include <memory>
#include <fstream>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "joy/joy.hpp"
#include <sensor_msgs/msg/joy.hpp>

std::ofstream ofs;

class Listener : public rclcpp::Node
{
public:
  Listener()
  : Node("listener")
  {
    auto joy_callback =
      [this](std::shared_ptr<sensor_msgs::msg::Joy> msg) -> void {
        RCLCPP_INFO(this->get_logger(), "\nbuttons:\n%d\n%d\n%d\n%d", msg->buttons[0],msg->buttons[1],msg->buttons[2],msg->buttons[3]);
        if (ofs) {
          if (msg->buttons[0]) {ofs << "a"; ofs.flush();}
          if (msg->buttons[1]) {ofs << "b"; ofs.flush();}
          if (msg->buttons[2]) {ofs << "c"; ofs.flush();}
          if (msg->buttons[3]) {ofs << "d"; ofs.flush();}
        }else{
          RCLCPP_ERROR(this->get_logger(), "File open error.");
        }
      };
    sub_ = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, joy_callback);
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
};

int main(int argc, char * argv[])
{
  ofs.open("/dev/ttyACM0");
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  ofs.close();
  return 0;
}