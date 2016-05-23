/*
 * Copyright (c) 2016, Georg Bartels <georg.bartels@cs.uni-bremen.de>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * * Neither the name of Institute for Artificial Intelligence nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef KNOWROB_MOVEIT_UTILS_HPP 
#define KNOWROB_MOVEIT_UTILS_HPP 

#include <std_msgs/Header.h>
#include <moveit_msgs/ContactInformation.h>
#include <moveit/collision_detection/collision_tools.h>
#include <ros/package.h>
#include <fstream>

namespace knowrob_moveit
{
  // TODO: make argument const
  inline std::vector<moveit_msgs::ContactInformation> collisionResultToMsg(
      collision_detection::CollisionResult& collision_result, const std_msgs::Header& header)
  {
    std::vector<moveit_msgs::ContactInformation> result;
  
    for (collision_detection::CollisionResult::ContactMap::iterator it=collision_result.contacts.begin(); 
         it!=collision_result.contacts.end(); ++it)
      for (std::vector<collision_detection::Contact>::iterator it2=it->second.begin(); 
           it2 != it->second.end(); ++it2)
      {
        moveit_msgs::ContactInformation msg;
        msg.header = header;
        collision_detection::contactToMsg(*it2, msg);
        result.push_back(msg);
      }
  
    ROS_INFO("Finished copying contacts.");
  
    return result;
  }
  
  // TODO: turn into a function to generate error string, and use std::run_time
  class Exception : public std::runtime_error
  {
  public:
    Exception(const std::string& file, const std::string& error_msg)
    : std::runtime_error("Error retrieving file [" + file + "]: " + error_msg)
    {}
  };
  
  std::string resourceUrlToPath(const std::string& url)
  {
    // TODO: use a real parser (like the boost one I used for the KMS40 driver).
    std::string path = url;
    if (!url.find("package://") == 0)
      throw Exception(url, "Given url does not start with 'package://'.");
    
    path.erase(0, strlen("package://"));
    size_t pos = path.find("/");
    if (pos == std::string::npos)
      throw Exception(url, "Could not find package name.");
  
    std::string package = path.substr(0, pos);
    path.erase(0, pos);
    std::string package_path = ros::package::getPath(package);
    if (package_path.empty())
      throw Exception(url, "Package '" + package + "' does not exist.");
  
    return package_path + path;
  }

  std::string readFileFromPath(const std::string& path)
  {
    std::string line, result;
    std::ifstream file(path);
    if (!file.is_open())
      throw std::runtime_error("Could not open file at '" + path + "'.");

    while ( std::getline (file,line) )
      result += line;
    file.close();

    return result;
  }
}

#endif // KNOWROB_MOVEIT_UTILS_HPP
