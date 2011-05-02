/*
 *  Multi Marker Pose Estimation using ARToolkit
 *  Copyright (C) 2010, CCNY Robotics Lab, 2011 ILS Robotics Lab
 *  Ivan Dryanovski <ivan.dryanovski@gmail.com>
 *  William Morris <morris@ee.ccny.cuny.edu>
 *  Gautier Dumonteil <gautier.dumonteil@gmail.com>
 *  http://robotics.ccny.cuny.edu
 * 
 *  Michael Ferguson <ferguson@cs.albany.edu>
 *  http://robotics.ils.albany.edu
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AR_POSE_AR_MULTI_H
#define AR_POSE_AR_MULTI_H

#include <string.h>
#include <stdarg.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/arMulti.h>

#include <ros/ros.h>
#include <ros/package.h>
#include <ros/console.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <visualization_msgs/Marker.h>
#include <pcl/ros/conversions.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>

#include <opencv/cv.h>
#include <cv_bridge/CvBridge.h>

#include <ar_pose/ARMarkers.h>
#include <ar_pose/ARMarker.h>
#include <ar_kinect/object.h>

const std::string cameraImageTopic_ = "/camera/rgb/image_color";
const std::string cameraInfoTopic_  = "/camera/rgb/camera_info";
const std::string cloudTopic_ = "/camera/rgb/points";

const double AR_TO_ROS = 0.001;
typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloud;
typedef pcl::PointCloud<pcl::PointXYZRGBNormal> PointCloudWithNormals; 

namespace ar_pose
{
  class ARPublisher
  {
  public:
    ARPublisher (ros::NodeHandle & n);
    ~ARPublisher (void);

  private:
    void arInit ();
    void getTransformationCallback (const sensor_msgs::ImageConstPtr &);
    void camInfoCallback (const sensor_msgs::CameraInfoConstPtr &);
    void cloudCallback (const sensor_msgs::PointCloud2ConstPtr& msg);

    ros::NodeHandle n_;
    tf::TransformBroadcaster broadcaster_;
    ros::Subscriber sub_;
    image_transport::Subscriber cam_sub_;
    ros::Subscriber cloud_sub_;
    ros::Publisher arMarkerPub_;

    image_transport::ImageTransport it_;
    sensor_msgs::CvBridge bridge_;
    sensor_msgs::CameraInfo cam_info_;
    PointCloudWithNormals cloud_;

    // **** for visualisation in rviz
    ros::Publisher rvizMarkerPub_;
    visualization_msgs::Marker rvizMarker_;

    // **** parameters
    ARParam cam_param_;         // Camera Calibration Parameters
    ARMultiMarkerInfoT *config; // AR Marker Info
    ar_object::ObjectData_T * object;
    int objectnum;
    char pattern_filename_[FILENAME_MAX];
    char data_directory_[FILENAME_MAX];

    ar_pose::ARMarkers arPoseMarkers_;
    int threshold_;
    bool getCamInfo_;
    bool publishTf_;
    bool publishVisualMarkers_;
    CvSize sz_;
    IplImage *capture_;
    bool gotcloud_;
    int cloud_width_;

  };                            // end class ARPublisher
}                               //end namespace ar_pose

#endif
