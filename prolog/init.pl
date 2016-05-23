% what does this do?
:- register_ros_package(knowrob_moveit).
:- register_ros_package(kb_collisions).

% what does this do?
:- use_module(library('knowrob_moveit')).
:- use_module(library('kb-collision')).

% what does this do?
:- load_foreign_library('libknowrob_moveit_predicates.so').
