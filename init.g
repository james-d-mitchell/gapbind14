#
# gapbind14: Seamless operability between C++14 and GAP
#
# Reading the declaration part of the package.
#
_PATH_SO := Filename(DirectoriesPackagePrograms("gapbind14"), "gapbind14.so");
if _PATH_SO <> fail then
    LoadDynamicModule(_PATH_SO);
fi;
Unbind(_PATH_SO);

DeclareCategory("IsTGapBind14Obj", IsObject);
BindGlobal("TheTypeTGapBind14Obj",
           NewType(NewFamily("TGapBind14ObjFamily"), IsTGapBind14Obj));

ReadPackage("gapbind14", "gap/gapbind14.gd");
