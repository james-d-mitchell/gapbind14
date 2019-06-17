#############################################################################
##
#W  demo.tst
#Y  Copyright (C) 2018                                   James D. Mitchell
##
##  Licensing information can be found in the README file of this package.
##
#############################################################################
##
gap> START_TEST("demo.tst");
gap> LoadPackage("gapbind14", false);;

# demo record tests
gap> demo;
rec( 
  Pet := rec( getName := function( arg1 ) ... end, 
      getName2 := function( arg1, arg2 ) ... end, 
      new := function( arg1 ) ... end, 
      setName := function( arg1, arg2 ) ... end, 
      terminate := function( arg1 ) ... end ), 
  testing1 := function(  ) ... end, testing2 := function(  ) ... end, 
  testing3 := function( arg1 ) ... end, testing4 := function( arg1 ) ... end )
gap> IsMutable(demo);
false
gap> demo.new_Pet := 3;
Error, Record Assignment: <rec> must be a mutable record

# new_Pet
gap> tilio := demo.Pet.new(1);
Error, expected string but got integer!
gap> tilio := demo.Pet.new("tilio");;
gap> demo.Pet.getName(tilio);
"tilio"
gap> demo.Pet.setName(tilio, "rocket");
gap> demo.Pet.setName(tilio, 1);
Error, expected string but got integer!
gap> demo.Pet.getName(tilio);
"rocket"
gap> demo.Pet.terminate(tilio);
gap> demo.Pet.getName(tilio);
"dead"
gap> TNAM_OBJ(tilio);
"gapbind14 C++ object wrapper"
gap> TypeObj(tilio) = TheTypeTGapBind14Obj; 
true
gap> demo.Pet.getName2(tilio, " is a cat");
"dead is a cat"

# testing*
gap> demo.testing2();
"it works\n"
gap> demo.testing3(1);
Error, expected string but got integer!
gap> demo.testing4("really, it does");
"it works really, it does\n"
gap> demo.testing4(1);
Error, expected string but got integer!

#
gap> STOP_TEST( "demo.tst", 1);
