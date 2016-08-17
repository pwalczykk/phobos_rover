#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
    ros::init(argc, argv, "tf_broadcaster");
    ros::NodeHandle nh("~");

    std::string parent_link, child_link;
    float pose_x, pose_y, pose_z;
    float orient_x, orient_y, orient_z, orient_w;

    nh.param("parent_link", parent_link, (std::string)"parent_link");
    nh.param("child_link", child_link, (std::string)"child_link");
    nh.param("pose_x", pose_x, (float) 0.0);
    nh.param("pose_y", pose_y, (float) 0.0);
    nh.param("pose_z", pose_z, (float) 0.0);
    nh.param("orient_x", orient_x, (float) 0.0);
    nh.param("orient_y", orient_y, (float) 0.0);
    nh.param("orient_z", orient_z, (float) 0.0);
    nh.param("orient_w", orient_w, (float) 1.0);

    tf::TransformBroadcaster br;
    tf::Transform transform;

    ros::Rate loop_rate(10);

    while(ros::ok()){
        transform.setOrigin(tf::Vector3(pose_x, pose_y, pose_z));
        transform.setRotation(tf::Quaternion(orient_x, orient_y, orient_z, orient_w));

        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), parent_link, child_link));

        loop_rate.sleep();
    }

    return 0;
}
