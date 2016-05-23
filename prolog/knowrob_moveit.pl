:- module(knowrob_moveit,
    [hello/1,
     planning_scene/1,
     test/1]).

:- use_module(library('semweb/rdf_db')).
:- use_module(library('semweb/rdfs')).
:- use_module(library('kb-collision')).

:- rdf_meta hello(?),
            planning_scene(+),
            test(+).

hello(prolog).

hello(A) :- hello_cpp(A).

planning_scene :-
  planning_scene(_).
  
planning_scene(PlanningScene) :-
  (\+ current_predicate(private_planning_scene_predicate, _)),
  new_planning_scene(PlanningScene),
  assert(private_planning_scene_predicate(PlanningScene)),!.

planning_scene(PlanningScene) :-
  current_predicate(private_planning_scene_predicate, _),
  private_planning_scene_predicate(PlanningScene).

test(NumContacts) :-
  planning_scene(P),
  owl_individual_of(_R,knowrob:'Robot'), 
  owl_has(_R, knowrob:'UrdfDescriptionFile', literal(UrdfFile)),
  owl_has(_R, knowrob:'SrdfDescriptionFile', literal(SrdfFile)),
  check_collisions(P, UrdfFile, SrdfFile, NumContacts).
