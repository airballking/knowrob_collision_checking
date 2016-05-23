// IMPORTANT: without the next define SWI-CPP and BOOST-MATH will clash
#define PL_SAFE_ARG_MACROS
#include <SWI-cpp.h>

#include <memory>
#include <iostream>
#include <ros/ros.h>
#include <knowrob_moveit/planning_scene.hpp>
#include <knowrob_moveit/utils.hpp>

using namespace knowrob_moveit;

PlTermv toPl(const std_msgs::Header& msg)
{
  PlCompound frame_id("frame_id", PlTermv(msg.frame_id.c_str()));
  PlCompound stamp("stamp", PlTermv(PlTerm(msg.stamp.toSec())));
  return PlTermv(frame_id, stamp);
}

PlTermv toPl(const geometry_msgs::Point& msg)
{
  PlCompound x("x", PlTermv(PlTerm(msg.x)));
  PlCompound y("y", PlTermv(PlTerm(msg.y)));
  PlCompound z("z", PlTermv(PlTerm(msg.z)));
  return PlTermv(x, y, z);
}

PlTermv toPl(const moveit_msgs::ContactInformation& msg)
{
  PlCompound header("header", toPl(msg.header));
  PlCompound position("position", toPl(msg.position));
  PlCompound body1("contact_body_1", PlTermv(msg.contact_body_1.c_str()));
  PlCompound body2("contact_body_2", PlTermv(msg.contact_body_2.c_str()));
  PlCompound depth("depth", PlTermv(PlTerm(msg.depth)));
  return PlTermv(header, position, body1, body2, depth);
}

int toPl(const std::vector<moveit_msgs::ContactInformation>& msg, PlTail& list)
{
  for(size_t i=0; i<msg.size(); ++i)
    list.append(PlCompound("contact", toPl(msg[i])));

  return list.close();
}

PREDICATE(hello_cpp, 1)
{
   return PL_av[0] = "cpp";
}

PREDICATE(new_planning_scene, 1)
{
  PlanningScene* p = new PlanningScene();
  return PL_av[0] = (void*) p;
}

// TODO: get JointStates from outside
// TODO: get maximum number of collisions
PREDICATE(check_collisions, 4)
{
  void* input_ptr = (void*) PL_av[0];
  PlanningScene* p = (PlanningScene*) input_ptr;

  std::string urdf_file = readFileFromPath(resourceUrlToPath((char*) PL_av[1]));
  std::string srdf_file = readFileFromPath(resourceUrlToPath((char*) PL_av[2]));
  try
  {
    sensor_msgs::JointState joint_states;
    joint_states.name.push_back("r_upper_arm_roll_joint");
    joint_states.position.push_back(-1.32);
    joint_states.velocity.push_back(0.0);
    joint_states.effort.push_back(0.0);

    std::vector<moveit_msgs::ContactInformation> contacts =
      p->checkCollisions(urdf_file, srdf_file, joint_states, 20);

    PlTail contact_list(PL_av[3]);
    return toPl(contacts, contact_list);
  }
  catch(const std::exception& e)
  {
    std::cout << "ERROR: " << e.what() << std::endl;
    return FALSE;
  }

  return TRUE;
}
