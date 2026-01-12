#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "joy/joy.hpp"
#include <sensor_msgs/msg/joy.hpp>

class Listener : public rclcpp::Node
{
public:
  Listener()
  : Node("listener")
  {
    auto joy_callback =
      [this](std::shared_ptr<sensor_msgs::msg::Joy> msg) -> void {
        RCLCPP_INFO(this->get_logger(), "\naxes:\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\nbuttons:\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d", msg->axes[0],msg->axes[1],msg->axes[2],msg->axes[3],msg->axes[4],msg->axes[5],msg->axes[6],msg->axes[7],msg->buttons[0],msg->buttons[1],msg->buttons[2],msg->buttons[3],msg->buttons[4],msg->buttons[5],msg->buttons[6],msg->buttons[7],msg->buttons[8],msg->buttons[9],msg->buttons[10],msg->buttons[11],msg->buttons[12]);
      };
    sub_ = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, joy_callback);
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}