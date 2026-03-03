#include <memory>
#include <fstream>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "rclcpp/rclcpp.hpp"
#include "joy/joy.hpp"
#include <sensor_msgs/msg/joy.hpp>
class Listener : public rclcpp::Node
{
public:
  Listener()
  : Node("listener")
  {
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
      RCLCPP_ERROR(this->get_logger(), "Failed to open /dev/ttyACM0");
    }
    struct termios tty;
    tcgetattr(fd, &tty);
    cfmakeraw(&tty);
    cfsetospeed(&tty, B115200);
    tcsetattr(fd, TCSANOW, &tty);

    auto joy_callback =
      [this](const std::shared_ptr<sensor_msgs::msg::Joy> msg) -> void{
      last_msg = msg;
      };
    auto timer_callback =
      [this]() -> void{
        if (!last_msg) {
          RCLCPP_WARN(this->get_logger(), "No Joy message received yet.");
          return;
        }

        bool hasNum = 0;
        for(int i = 0; i < 8; i++){
          hasNum |= static_cast<bool>(last_msg->axes[i]);
        }
        for(int i = 0; i < 13; i++){
          hasNum |= static_cast<bool>(last_msg->buttons[i]);
        }
        if (hasNum == 0) return;
        // if (last_msg->axes.size() < 8) {
        //   RCLCPP_WARN(this->get_logger(), "axes size too small: %zu", last_msg->axes.size());
        //   return;
        // }
        // if (last_msg->buttons.size() < 13) {
        //   RCLCPP_WARN(this->get_logger(), "buttons size too small: %zu", last_msg->buttons.size());
        //   return;
        // }

        RCLCPP_INFO(this->get_logger(), "\naxes:\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\nbuttons:\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d", last_msg->axes[0],last_msg->axes[1],last_msg->axes[2],last_msg->axes[3],last_msg->axes[4],last_msg->axes[5],last_msg->axes[6],last_msg->axes[7],last_msg->buttons[0],last_msg->buttons[1],last_msg->buttons[2],last_msg->buttons[3],last_msg->buttons[4],last_msg->buttons[5],last_msg->buttons[6],last_msg->buttons[7],last_msg->buttons[8],last_msg->buttons[9],last_msg->buttons[10],last_msg->buttons[11],last_msg->buttons[12]);
        if (fd >= 0) {
          send_msg.clear();
          send_msg += "n";
          send_msg += ":";
          send_msg += std::to_string(last_msg->axes[0]);
          send_msg += ":";
          send_msg += std::to_string(last_msg->axes[1]);
          send_msg += ":";
          send_msg += std::to_string(last_msg->axes[3]);
          send_msg += ":";
          send_msg += std::to_string(last_msg->axes[4]); 
          send_msg += "|";
          send_msg += (last_msg->buttons[4] ? "L1:p" : "L1:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[6] ? "L2:p" : "L2:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[11] ? "L3:p" : "L3:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[5] ? "R1:p" : "R1:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[7] ? "R2:p" : "R2:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[12] ? "R3:p" : "R3:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[10] ? "PS:p" : "PS:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[8] ? "SH:p" : "SH:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[9] ? "OP:p" : "OP:no_p");
          send_msg += "|";
          send_msg += (last_msg->axes[7]==1.0f ? "u:p" : "u:no_p");
          send_msg += "|";
          send_msg += (last_msg->axes[7]==-1.0f ? "d:p" : "d:no_p");
          send_msg += "|";
          send_msg += (last_msg->axes[6]==1.0f ? "l:p" : "l:no_p");
          send_msg += "|";
          send_msg += (last_msg->axes[6]==-1.0f ? "r:p" : "r:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[1] ? "ci:p" : "ci:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[0] ? "cr:p" : "cr:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[2] ? "tri:p" : "tri:no_p");
          send_msg += "|";
          send_msg += (last_msg->buttons[3] ? "sq:p" : "sq:no_p");
          send_msg += "|";
          // ofs.flush();
          write(fd, send_msg.c_str(), strlen(send_msg.c_str()));
        }else{
          RCLCPP_ERROR(this->get_logger(), "Can't open file.");
        }
      };
    
    sub_ = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, joy_callback);

    using namespace std::chrono_literals;
    timer_ = this->create_wall_timer(50ms, timer_callback);
  }
  ~Listener(){close(fd);}

private:
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  sensor_msgs::msg::Joy::SharedPtr last_msg;
  int fd;
  std::string send_msg = "";
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}