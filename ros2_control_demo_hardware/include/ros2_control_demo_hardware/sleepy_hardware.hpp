#include "ros2_control_demo_hardware/rrbot_system_position_only.hpp"

namespace ros2_control_demo_hardware
{
class SleepyRRBotHW : public RRBotSystemPositionOnlyHardware {
public:
  ROS2_CONTROL_DEMO_HARDWARE_PUBLIC
  hardware_interface::return_type read(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;

  ROS2_CONTROL_DEMO_HARDWARE_PUBLIC
  hardware_interface::return_type write(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;

   int sleep_base = 10;

};
}