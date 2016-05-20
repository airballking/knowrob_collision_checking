:- module(knowrob_moveit,
    [hello/1]).

:- rdf_meta
   hello(?).

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

