//
// gapbind14 package for GAP
// Copyright (C) 2018 James D. Mitchell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "gapbind14.hpp"
#include <iostream>

gapbind14::Module MODULE = gapbind14::GAPBIND14_MODULE_IMPL();

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

Obj  TheTypeTGapBind14Obj;
UInt gapbind14::T_GAPBIND14_OBJ = 0;

////////////////////////////////////////////////////////////////////////
// Required kernel functions
////////////////////////////////////////////////////////////////////////

Obj TGapBind14ObjTypeFunc(Obj o) {
  return TheTypeTGapBind14Obj;
}

void TGapBind14ObjPrintFunc(Obj o) {
  MODULE.print(o);
}

void TGapBind14ObjSaveFunc(Obj o) {
  MODULE.save(o);
}

void TGapBind14ObjLoadFunc(Obj o) {
  MODULE.load(o);
}

Obj TGapBind14ObjCopyFunc(Obj o, Int mut) {
  return o;
}

void TGapBind14ObjCleanFunc(Obj o) {}

void TGapBind14ObjFreeFunc(Obj o) {
  MODULE.free(o);
}

////////////////////////////////////////////////////////////////////////
// Copied from gap/src/modules.c, should be exposed in header TODO
////////////////////////////////////////////////////////////////////////

static Obj ValidatedArgList(const char *name, int nargs, const char *argStr) {
  Obj args = ArgStringToList(argStr);
  int len  = LEN_PLIST(args);
  if (nargs >= 0 && len != nargs)
    fprintf(stderr,
            "#W %s takes %d arguments, but argument string is '%s'"
            " which implies %d arguments\n",
            name,
            nargs,
            argStr,
            len);
  return args;
}

static void SetupFuncInfo(Obj func, const Char *cookie) {
  // The string <cookie> usually has the form "PATH/TO/FILE.c:FUNCNAME".
  // We check if that is the case, and if so, split it into the parts before
  // and after the colon. In addition, the file path is cut to only contain
  // the last two '/'-separated components.
  const Char *pos = strchr(cookie, ':');
  if (pos) {
    Obj location = MakeImmString(pos + 1);

    Obj  filename;
    char buffer[512];
    Int  len = 511 < (pos - cookie) ? 511 : pos - cookie;
    memcpy(buffer, cookie, len);
    buffer[len] = 0;

    Char *start = strrchr(buffer, '/');
    if (start) {
      while (start > buffer && *(start - 1) != '/')
        start--;
    } else {
      start = buffer;
    }
    filename = MakeImmString(start);

    Obj body_bag = NewBag(T_BODY, sizeof(BodyHeader));
    SET_FILENAME_BODY(body_bag, filename);
    SET_LOCATION_BODY(body_bag, location);
    SET_BODY_FUNC(func, body_bag);
    CHANGED_BAG(body_bag);
    CHANGED_BAG(func);
  }
}

////////////////////////////////////////////////////////////////////////
// Initialise kernel data structures
////////////////////////////////////////////////////////////////////////

static Int InitKernel(StructInitInfo *module) {
  InitHdlrFuncsFromTable(MODULE.funcs());

  for (auto ptr : MODULE) {
    InitHdlrFuncsFromTable(MODULE.mem_funcs(ptr->name()));
  }

  UInt &T_GAPBIND14_OBJ = gapbind14::T_GAPBIND14_OBJ;
  T_GAPBIND14_OBJ = RegisterPackageTNUM("TGapBind14", TGapBind14ObjTypeFunc);

  // InfoBags[T_GAPBIND14_OBJ].name = "gapbind14 C++ object wrapper";

  PrintObjFuncs[T_GAPBIND14_OBJ] = TGapBind14ObjPrintFunc;
  SaveObjFuncs[T_GAPBIND14_OBJ]  = TGapBind14ObjSaveFunc;
  LoadObjFuncs[T_GAPBIND14_OBJ]  = TGapBind14ObjLoadFunc;

  CopyObjFuncs[T_GAPBIND14_OBJ]      = &TGapBind14ObjCopyFunc;
  CleanObjFuncs[T_GAPBIND14_OBJ]     = &TGapBind14ObjCleanFunc;
  IsMutableObjFuncs[T_GAPBIND14_OBJ] = &AlwaysNo;

  InitMarkFuncBags(T_GAPBIND14_OBJ, &MarkOneSubBags);
  InitFreeFuncBag(T_GAPBIND14_OBJ, &TGapBind14ObjFreeFunc);

  InitCopyGVar("TheTypeTGapBind14Obj", &TheTypeTGapBind14Obj);

  // return success
  return 0;
}

////////////////////////////////////////////////////////////////////////
// Initialise library data structures
////////////////////////////////////////////////////////////////////////

static Int InitLibrary(StructInitInfo *module) {
  // InitGVarFuncsFromTable(MODULE.gvar_funcs());
  StructGVarFunc const *tab = MODULE.funcs();

  // init functions from MODULE in the record named MODULE.module_name()
  // This is done to avoid polluting the global namespace
  Obj global_rec = NEW_PREC(0);
  SET_LEN_PREC(global_rec, 0);

  for (Int i = 0; tab[i].name != 0; i++) {
    UInt gvar = GVarName(tab[i].name);
    Obj  name = NameGVar(gvar);
    Obj  args = ValidatedArgList(tab[i].name, tab[i].nargs, tab[i].args);
    Obj  func = NewFunction(name, tab[i].nargs, args, tab[i].handler);
    SetupFuncInfo(func, tab[i].cookie);
    AssPRec(global_rec, RNamName(tab[i].name), func);
  }
  for (auto ptr : MODULE) {
    tab           = MODULE.mem_funcs(ptr->name());
    Obj class_rec = NEW_PREC(0);
    SET_LEN_PREC(class_rec, 0);

    for (Int i = 0; tab[i].name != 0; i++) {
      UInt gvar = GVarName(tab[i].name);
      Obj  name = NameGVar(gvar);
      Obj  args = ValidatedArgList(tab[i].name, tab[i].nargs, tab[i].args);
      Obj  func = NewFunction(name, tab[i].nargs, args, tab[i].handler);
      SetupFuncInfo(func, tab[i].cookie);
      AssPRec(class_rec, RNamName(tab[i].name), func);
    }
    AssPRec(global_rec, RNamName(ptr->name().c_str()), class_rec);
  }

  MakeImmutable(global_rec);
  AssReadOnlyGVar(GVarName(MODULE.module_name()), global_rec);

  // return success
  return 0;
}

////////////////////////////////////////////////////////////////////////

static StructInitInfo module = {
    .type        = MODULE_DYNAMIC,
    .name        = MODULE.module_name(),
    .initKernel  = InitKernel,
    .initLibrary = InitLibrary,
};

extern "C" StructInitInfo *Init__Dynamic(void) {
  return &module;
}
