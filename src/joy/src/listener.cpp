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
        RCLCPP_INFO(this->get_logger(), "\naxes:\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\nbuttons:\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d", msg->axes[0],msg->axes[1],msg->axes[2],msg->axes[3],msg->axes[4],msg->axes[5],msg->axes[6],msg->axes[7],msg->buttons[0],msg->buttons[1],msg->buttons[2],msg->buttons[3],msg->buttons[4],msg->buttons[5],msg->buttons[6],msg->buttons[7],msg->buttons[8],msg->buttons[9],msg->buttons[10],msg->buttons[11],msg->buttons[12]);
        if (ofs) {
          ofs << "n";
          ofs << ":" << msg->axes[0];
          ofs << ":" << msg->axes[1];
          ofs << ":" << msg->axes[3];
          ofs << ":" << msg->axes[4];
          ofs << "|" << (msg->buttons[4] ? "L1:p" : "L1:no_p");
          ofs << "|" << (msg->buttons[6] ? "L2:p" : "L2:no_p");
          ofs << "|" << (msg->buttons[11] ? "L3:p" : "L3:no_p");
          ofs << "|" << (msg->buttons[5] ? "R1:p" : "R1:no_p");
          ofs << "|" << (msg->buttons[7] ? "R2:p" : "R2:no_p");
          ofs << "|" << (msg->buttons[12] ? "R3:p" : "R3:no_p");
          ofs << "|" << (msg->buttons[10] ? "PS:p" : "PS:no_p");
          ofs << "|" << (msg->buttons[8] ? "SH:p" : "SH:no_p");
          ofs << "|" << (msg->buttons[9] ? "OP:p" : "OP:no_p");
          ofs << "|" << (msg->axes[7]==1.0f ? "u:p" : "u:no_p");
          ofs << "|" << (msg->axes[7]==-1.0f ? "d:p" : "d:no_p");
          ofs << "|" << (msg->axes[6]==1.0f ? "l:p" : "l:no_p");
          ofs << "|" << (msg->axes[6]==-1.0f ? "r:p" : "r:no_p");
          ofs << "|" << (msg->buttons[1] ? "ci:p" : "ci:no_p");
          ofs << "|" << (msg->buttons[0] ? "cr:p" : "cr:no_p");
          ofs << "|" << (msg->buttons[2] ? "tri:p" : "tri:no_p");
          ofs << "|" << (msg->buttons[3] ? "sq:p" : "sq:no_p");
          ofs << "|";
          ofs.flush();
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