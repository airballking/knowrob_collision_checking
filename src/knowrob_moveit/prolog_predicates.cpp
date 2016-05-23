// IMPORTANT: without the next define SWI-CPP and BOOST-MATH will clash
#define PL_SAFE_ARG_MACROS
#include <SWI-cpp.h>

#include <memory>
#include <iostream>
#include <ros/ros.h>
#include <knowrob_moveit/planning_scene.hpp>
#include <knowrob_moveit/utils.hpp>

using namespace knowrob_moveit;

// TODO: move this into a separate file for clarification
// TODO: make unit-tests checking this
PlCompound fromHeaderMsg(const std_msgs::Header& msg)
{
  return PlCompound("header", PlTermv(PlTerm(msg.frame_id.c_str()), PlTerm(msg.stamp.toSec())));
}

std_msgs::Header toHeaderMsg(PlTerm& term)
{
  std_msgs::Header msg;
  if(std::string(term.name()).compare("header") != 0)
    throw std::runtime_error("Expected name 'header' for compound term.");

  // TODO: check arity
  msg.frame_id = (char *) term[1];
  msg.stamp = ros::Time((double) term[2]);
  // TODO: finish me
  return msg;
}

sensor_msgs::JointState toJointStateMsg(PlTerm& term)
{
  sensor_msgs::JointState msg;
  if(std::string(term.name()).compare("joint_state") != 0)
    throw std::runtime_error("Expected name 'joint_state' for compound term.");

  // TODO: check arity
  PlTerm header_term = term[1];
  msg.header = toHeaderMsg(header_term);
  // TODO: finish me
  return msg;
}
 
PlCompound fromPointMsg(const geometry_msgs::Point& msg)
{
  return PlCompound("point", PlTermv(PlTerm(msg.x), PlTerm(msg.y), PlTerm(msg.z)));
}

PlCompound fromContactMsg(const moveit_msgs::ContactInformation& msg)
{
  return PlCompound("contact", 
      PlTermv(fromHeaderMsg(msg.header), fromPointMsg(msg.position), 
          PlTerm(msg.contact_body_1.c_str()), PlTerm(msg.contact_body_2.c_str()),
          PlTerm(msg.depth)));
}

int fromContactMsgList(const std::vector<moveit_msgs::ContactInformation>& msg, PlTail& list)
{
  for(size_t i=0; i<msg.size(); ++i)
    list.append(fromContactMsg(msg[i]));

  return list.close();
}

PREDICATE(eat_header_msg, 1)
{
  PlTerm first_term = PL_av[0];
  std_msgs::Header msg = toHeaderMsg(first_term);
  std::cout << msg << std::endl;
  return TRUE;
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
//    joint_states.velocity.push_back(0.0);
//    joint_states.effort.push_back(0.0);

    std::vector<moveit_msgs::ContactInformation> contacts =
      p->checkCollisions(urdf_file, srdf_file, joint_states, 20);

    PlTail contact_list(PL_av[3]);
    return fromContactMsgList(contacts, contact_list);
  }
  catch(const std::exception& e)
  {
    std::cout << "ERROR: " << e.what() << std::endl;
    return FALSE;
  }

  return TRUE;
}
