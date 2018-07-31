#ifndef NDT_MAPPING_H_INCLUDED
#define NDT_MAPPING_H_INCLUDED

// headers in ROS
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

// headers in pcl
#include <pcl/point_types.h>
#include <pcl/registration/ndt.h>
#include <pcl_conversions/pcl_conversions.h>

// headers in boost
#include <boost/circular_buffer.hpp>

/**
 * @brief ndt_mapping class
 *
 */
class ndt_mapping {
 public:
  struct parameters {
    std::string pointcloud_topic;
    std::string odom_topic;
    parameters() {
      ros::param::param<std::string>(ros::this_node::getName() + "/pointcloud_topic", pointcloud_topic,
                                     ros::this_node::getName() + "/input_pointcloud");
      ros::param::param<std::string>(ros::this_node::getName() + "/odom_topic", odom_topic, "/odom");
    }
  };
  /**
   * @brief Construct a new ndt mapping object
   *
   */
  ndt_mapping();
  /**
   * @brief Destroy the ndt mapping object
   *
   */
  ~ndt_mapping();

 private:
  /**
   * @brief parameters for ndt_mapping class
   *
   */
  const parameters params_;
  /**
   * @brief ros NodeHandle
   *
   */
  ros::NodeHandle nh_;
  /**
   * @brief subscriber for robot odometry
   *
   */
  message_filters::Subscriber<nav_msgs::Odometry> odom_sub_;
  /**
   * @brief subscriber for pointcloud
   *
   */
  message_filters::Subscriber<sensor_msgs::PointCloud2> pointcloud_sub_;
  /**
   * @brief TimeSynchronizer
   * @sa ndt_mapping::pointcloud_sub_
   * @sa ndt_mapping::odom_sub_
   */
  message_filters::TimeSynchronizer<nav_msgs::Odometry, sensor_msgs::PointCloud2> sync_;
  /**
   * @brief callback function for odometry and pointcloud
   *
   * @param odom_msg
   * @param pointcloud_msg
   *
   * see also http://pointclouds.org/documentation/tutorials/normal_distributions_transform.php
   */
  void callback_(const nav_msgs::OdometryConstPtr& odom_msg,
                 const sensor_msgs::PointCloud2ConstPtr& pointcloud_msg);
  /**
   * @brief circuler buffer for pointcloud datas
   *
   */
  boost::circular_buffer<pcl::PointCloud<pcl::PointXYZ>::Ptr> pointcloud_buf_;
  /**
   * @brief circuler buffer for odometry datas
   *
   */
  boost::circular_buffer<nav_msgs::Odometry> odom_buf_;
  /**
   * @brief pcl::NormalDistributionsTransform class
   *
   */
  pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt_;
};
#endif  // NDT_MAPPING_H_INCLUDED