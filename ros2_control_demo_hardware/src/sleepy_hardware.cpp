#include "ros2_control_demo_hardware/sleepy_hardware.hpp"
#include <random>
#include <fstream>
#include <thread>

#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"


namespace ros2_control_demo_hardware
{
hardware_interface::return_type SleepyRRBotHW::read(
  const rclcpp::Time & time, const rclcpp::Duration & period)
{
    if (sleep_base != 0) 
    {
        std::mt19937_64 eng{std::random_device{}()};
        std::uniform_int_distribution<> dist{10, 300};
    
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_base + dist(eng)));
    }

    auto now = std::chrono::system_clock::now();
    read_call_date.push_back(std::make_pair(std::chrono::system_clock::to_time_t(now)
                                          , std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000));

    if (read_call_date.size() == 300) 
    {
        std::ofstream generated_csv;
        generated_csv.open("/tmp/sleepy_read.csv");
        generated_csv << "sleepy read\n";
        for (auto& element : read_call_date) 
        {
            generated_csv << std::put_time(localtime(&element.first), "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << element.second.count() << ",\n";
        }
        generated_csv.close();
    }
    
    for (uint i = 0; i < hw_states_.size(); i++)
    {
        // Simulate RRBot's movement
        hw_states_[i] = hw_states_[i] + (hw_commands_[i] - hw_states_[i]) / hw_slowdown_;
        RCLCPP_INFO(
        rclcpp::get_logger("SleepyRRBotHW"), "Got state %.5f for joint %d!",
        hw_states_[i], i);
    }
    RCLCPP_INFO(rclcpp::get_logger("SleepyRRBotHW"), "Joints successfully read!");
    // END: This part here is for exemplary purposes - Please do not copy to your production code
    return hardware_interface::return_type::OK;

}

hardware_interface::return_type SleepyRRBotHW::write(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
{
    if (sleep_base != 0) 
    {
        std::mt19937_64 eng{std::random_device{}()};
        std::uniform_int_distribution<> dist{10, 300};
    
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_base + dist(eng)));
    }

    auto now = std::chrono::system_clock::now();
    write_call_date.push_back(std::make_pair(std::chrono::system_clock::to_time_t(now)
                                          , std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000));

    if (write_call_date.size() == 300) 
    {
        std::ofstream generated_csv;
        generated_csv.open("/tmp/write_sleep.csv");
        generated_csv << "sleepy write\n";
        for (auto& element : write_call_date) 
        {
            generated_csv << std::put_time(localtime(&element.first), "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << element.second.count() << ",\n";
        }
        generated_csv.close();
    }
    
    for (uint i = 0; i < hw_commands_.size(); i++)
    {
        // Simulate sending commands to the hardware
        RCLCPP_INFO(
        rclcpp::get_logger("SleepyRRBotHW"), "Got command %.5f for joint %d!",
        hw_commands_[i], i);
    }
    RCLCPP_INFO(
        rclcpp::get_logger("SleepyRRBotHW"), "Joints successfully written!");
    // END: This part here is for exemplary purposes - Please do not copy to your production code

    return hardware_interface::return_type::OK;
}
}

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
  ros2_control_demo_hardware::SleepyRRBotHW, hardware_interface::SystemInterface)