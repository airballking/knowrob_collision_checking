// IMPORTANT: without the next define SWI-CPP and BOOST-MATH will clash
#define PL_SAFE_ARG_MACROS
#include <SWI-cpp.h>

#include <memory>
#include <iostream>
#include <ros/ros.h>
#include <knowrob_moveit/planning_scene.hpp>

using namespace knowrob_moveit;

PREDICATE(hello_cpp, 1)
{
   return PL_av[0] = "cpp";
}

PREDICATE(new_planning_scene, 1)
{
//  shared_ptr<PlanningScene>* p = new shared_ptr<PlanningScene>(new PlanningScene());
  PlanningScene* p = new PlanningScene();
  return PL_av[0] = (void*) p;
}

PREDICATE(check_collisions, 1)
{
  void* input_ptr = (void*) PL_av[0];
//  shared_ptr<PlanningScene>* p = (shared_ptr<PlanningScene>*) input_ptr;
  PlanningScene* p = (PlanningScene*) input_ptr;
  p->checkCollisions("", "", sensor_msgs::JointState(), 20);

  return TRUE;
}
