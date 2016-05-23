// IMPORTANT: without the next define SWI-CPP and BOOST-MATH will clash
#define PL_SAFE_ARG_MACROS
#include <SWI-cpp.h>

#include <memory>
#include <iostream>
#include <ros/ros.h>
#include <knowrob_moveit/planning_scene.hpp>
#include <knowrob_moveit/utils.hpp>

using namespace knowrob_moveit;

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
// TODO: report actual collisions
PREDICATE(check_collisions, 4)
{
  void* input_ptr = (void*) PL_av[0];
  PlanningScene* p = (PlanningScene*) input_ptr;

  std::string urdf_file = readFileFromPath(resourceUrlToPath((char*) PL_av[1]));
  std::string srdf_file = readFileFromPath(resourceUrlToPath((char*) PL_av[2]));
  try
  {
    std::vector<moveit_msgs::ContactInformation> contacts =
      p->checkCollisions(urdf_file, srdf_file, sensor_msgs::JointState(), 20);
    return PL_av[3] = (long) contacts.size();
  }
  catch(const std::exception& e)
  {
    std::cout << "ERROR: " << e.what() << std::endl;
    return FALSE;
  }

  return TRUE;
}
