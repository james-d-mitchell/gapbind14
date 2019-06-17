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
//

#ifndef GAPBIND14_SRC_GAPBIND14_HPP_
#define GAPBIND14_SRC_GAPBIND14_HPP_

#include <functional>     // for equal_to
#include <iostream>       // for debugging, FIXME remove this
#include <sstream>        // for ostringstream
#include <string>         // for string
#include <tuple>          // for tuple
#include <type_traits>    // for result_of_t
#include <unordered_map>  // for unordered_map
#include <unordered_set>  // for unordered_set
#include <utility>        // for pair
#include <vector>         // for vector

#include "gap-include.hpp"
#include "to_gap.hpp"    // for to_gap, to_cpp
#include "typename.hpp"  // for debugging, FIXME remove

////////////////////////////////////////////////////////////////////////
// Helper macros
////////////////////////////////////////////////////////////////////////

#define GAPBIND14_CONCAT(first, second) first##second
#define GAPBIND14_CONCAT3(first, second, third) first##second##third

#define GAPBIND14_STRINGIFY(x) #x
#define GAPBIND14_TO_STRING(x) GAPBIND14_STRINGIFY(x)

#define GAPBIND14_TOKENPASTE(x, y) GAPBIND14_CONCAT(x, y)
#define GAPBIND14_UNIQUE_ID \
  int GAPBIND14_TOKENPASTE(gapbind14_hack_, __COUNTER__) =

////////////////////////////////////////////////////////////////////////
// Assertions
////////////////////////////////////////////////////////////////////////

#ifdef GAPBIND14_DEBUG
#define GAPBIND14_ASSERT(x) assert(x)
#else
#define GAPBIND14_ASSERT(x)
#endif

////////////////////////////////////////////////////////////////////////
// Exceptions
////////////////////////////////////////////////////////////////////////

#define GAPBIND14_TRY(something)      \
  try {                               \
    something;                        \
  } catch (std::exception const& e) { \
    ErrorQuit(e.what(), 0L, 0L);      \
  }

////////////////////////////////////////////////////////////////////////
// Add function to list of function pointers to be initialised
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Create module
////////////////////////////////////////////////////////////////////////

#define GAPBIND14_MODULE(name, module)                       \
  gapbind14::Module module(GAPBIND14_TO_STRING(name));       \
                                                             \
  using gapbind14_init_func = void (*)(gapbind14::Module&);  \
  std::vector<gapbind14_init_func> gapbind14_init_functions; \
                                                             \
  int gapbind14_push_back(gapbind14_init_func func) {        \
    gapbind14_init_functions.push_back(func);                \
    return 0;                                                \
  }                                                          \
                                                             \
  gapbind14::Module gapbind14::GAPBIND14_MODULE_IMPL() {     \
    try {                                                    \
      for (auto init : gapbind14_init_functions) {           \
        init(module);                                        \
      }                                                      \
    } catch (...) {                                          \
    }                                                        \
    module.finalize();                                       \
    return module;                                           \
  }

////////////////////////////////////////////////////////////////////////
// Create function wrapper
////////////////////////////////////////////////////////////////////////

#define GAPBIND14_FUNCTION_NON_OVERLOAD(module, c_func_var, gap_func_var)   \
  template <typename TFunctionType = decltype(c_func_var),                  \
            typename TSFINAE       = Obj>                                   \
  auto GAPBIND14_CONCAT(c_func_var, _wrapper)(Obj self)                     \
      ->std::enable_if_t<gapbind14::returns_void<TFunctionType>::value      \
                             && gapbind14::arg_count<TFunctionType>::value  \
                                    == 0,                                   \
                         TSFINAE> {                                         \
    GAPBIND14_TRY(gapbind14::CppFunction<TFunctionType>()(c_func_var));     \
    return 0L;                                                              \
  }                                                                         \
                                                                            \
  template <typename TFunctionType = decltype(c_func_var),                  \
            typename TSFINAE       = Obj>                                   \
  auto GAPBIND14_CONCAT(c_func_var, _wrapper)(Obj self)                     \
      ->std::enable_if_t<!gapbind14::returns_void<TFunctionType>::value     \
                             && gapbind14::arg_count<TFunctionType>::value  \
                                    == 0,                                   \
                         TSFINAE> {                                         \
    using to_gap_type = gapbind14::to_gap<                                  \
        typename gapbind14::CppFunction<TFunctionType>::return_type>;       \
    GAPBIND14_TRY(return to_gap_type()(                                     \
        gapbind14::CppFunction<TFunctionType>()(c_func_var)));              \
  }                                                                         \
  template <typename TFunctionType = decltype(c_func_var),                  \
            typename TSFINAE       = Obj>                                   \
  auto GAPBIND14_CONCAT(c_func_var, _wrapper)(Obj self, Obj arg1)           \
      ->std::enable_if_t<gapbind14::returns_void<TFunctionType>::value      \
                             && gapbind14::arg_count<TFunctionType>::value  \
                                    == 1,                                   \
                         TSFINAE> {                                         \
    using to_cpp_0_type = typename gapbind14::CppFunction<                  \
        TFunctionType>::params_type::template get<0>;                       \
    GAPBIND14_TRY(gapbind14::CppFunction<TFunctionType>()(                  \
        c_func_var, gapbind14::to_cpp<to_cpp_0_type>()(arg1)));             \
    return 0L;                                                              \
  }                                                                         \
  template <typename TFunctionType = decltype(c_func_var),                  \
            typename TSFINAE       = Obj>                                   \
  auto GAPBIND14_CONCAT(c_func_var, _wrapper)(Obj self, Obj arg1)           \
      ->std::enable_if_t<!gapbind14::returns_void<TFunctionType>::value     \
                             && gapbind14::arg_count<TFunctionType>::value  \
                                    == 1,                                   \
                         TSFINAE> {                                         \
    using to_cpp_0_type = typename gapbind14::CppFunction<                  \
        TFunctionType>::params_type::template get<0>;                       \
    using to_gap_type = gapbind14::to_gap<                                  \
        typename gapbind14::CppFunction<TFunctionType>::return_type>;       \
    GAPBIND14_TRY(                                                          \
        return to_gap_type()(gapbind14::CppFunction<TFunctionType>()(       \
            c_func_var, gapbind14::to_cpp<to_cpp_0_type>()(arg1))));        \
  }                                                                         \
  template <typename TFunctionType = decltype(c_func_var),                  \
            typename TSFINAE       = Obj>                                   \
  auto GAPBIND14_CONCAT(c_func_var, _wrapper)(Obj self, Obj arg1, Obj arg2) \
      ->std::enable_if_t<gapbind14::returns_void<TFunctionType>::value      \
                             && gapbind14::arg_count<TFunctionType>::value  \
                                    == 2,                                   \
                         TSFINAE> {                                         \
    using to_cpp_0_type = typename gapbind14::CppFunction<                  \
        TFunctionType>::params_type::template get<0>;                       \
    using to_cpp_1_type = typename gapbind14::CppFunction<                  \
        TFunctionType>::params_type::template get<1>;                       \
    GAPBIND14_TRY(gapbind14::CppFunction<TFunctionType>()(                  \
        c_func_var,                                                         \
        gapbind14::to_cpp<to_cpp_0_type>()(arg1),                           \
        gapbind14::to_cpp<to_cpp_1_type>()(arg2)));                         \
    return 0L;                                                              \
  }                                                                         \
  template <typename TFunctionType = decltype(c_func_var),                  \
            typename TSFINAE       = Obj>                                   \
  auto GAPBIND14_CONCAT(c_func_var, _wrapper)(Obj self, Obj arg1, Obj arg2) \
      ->std::enable_if_t<!gapbind14::returns_void<TFunctionType>::value     \
                             && gapbind14::arg_count<TFunctionType>::value  \
                                    == 2,                                   \
                         TSFINAE> {                                         \
    using to_cpp_0_type = typename gapbind14::CppFunction<                  \
        TFunctionType>::params_type::template get<0>;                       \
    using to_cpp_1_type = typename gapbind14::CppFunction<                  \
        TFunctionType>::params_type::template get<1>;                       \
    using to_gap_type = gapbind14::to_gap<                                  \
        typename gapbind14::CppFunction<TFunctionType>::return_type>;       \
    GAPBIND14_TRY(                                                          \
        return to_gap_type()(gapbind14::CppFunction<TFunctionType>()(       \
            c_func_var,                                                     \
            gapbind14::to_cpp<to_cpp_0_type>()(arg1),                       \
            gapbind14::to_cpp<to_cpp_1_type>()(arg2))));                    \
  }                                                                         \
  void GAPBIND14_CONCAT3(gapbind14_init_, c_func_var, _wrapper)(            \
      gapbind14::Module & gapbind14_module) {                               \
    gapbind14_module.add_func(__FILE__,                                     \
                              GAPBIND14_TO_STRING(gap_func_var),            \
                              &GAPBIND14_CONCAT(c_func_var, _wrapper) < >); \
  }                                                                         \
  GAPBIND14_UNIQUE_ID gapbind14_push_back(                                  \
      &GAPBIND14_CONCAT3(gapbind14_init_, c_func_var, _wrapper));

#define GAPBIND14_FUNCTION_OVERLOAD(module, cfunc, gfunc, vrld) \
  auto GAPBIND14_CONCAT(gfunc, _overload) = vrld(cfunc);        \
  GAPBIND14_FUNCTION_NON_OVERLOAD(                              \
      module, GAPBIND14_CONCAT(gfunc, _overload), gfunc)

#define GAPBIND14_FUNCTION_GET(_1, _2, _3, _4, NAME, ...) NAME
#define GAPBIND14_FUNCTION(...)                           \
  GAPBIND14_FUNCTION_GET(__VA_ARGS__,                     \
                         GAPBIND14_FUNCTION_OVERLOAD,     \
                         GAPBIND14_FUNCTION_NON_OVERLOAD) \
  (__VA_ARGS__)

////////////////////////////////////////////////////////////////////////
// Create class wrapper
////////////////////////////////////////////////////////////////////////

#define GAPBIND14_CLASS(module, class_var)           \
  GAPBIND14_UNIQUE_ID module.add_subtype<class_var>( \
      GAPBIND14_TO_STRING(class_var));

////////////////////////////////////////////////////////////////////////
// Create class constructor
////////////////////////////////////////////////////////////////////////

#define GAPBIND14_CLASS_CONSTRUCTOR(                                           \
    module, class_var, gap_func_var, to_cpp_0_type)                            \
  Obj GAPBIND14_CONCAT(class_var, gap_func_var)(Obj self, Obj arg1) {          \
    return module.create<class_var, to_cpp_0_type>(                            \
        GAPBIND14_TO_STRING(class_var), arg1);                                 \
  }                                                                            \
  void GAPBIND14_CONCAT3(gapbind14_init_, class_var, gap_func_var)(            \
      gapbind14::Module & gapbind14_module) {                                  \
    gapbind14_module.add_mem_func(GAPBIND14_TO_STRING(class_var),              \
                                  __FILE__,                                    \
                                  GAPBIND14_TO_STRING(gap_func_var),           \
                                  &GAPBIND14_CONCAT(class_var, gap_func_var)); \
  }                                                                            \
  GAPBIND14_UNIQUE_ID gapbind14_push_back(                                     \
      &GAPBIND14_CONCAT3(gapbind14_init_, class_var, gap_func_var));

////////////////////////////////////////////////////////////////////////
// Create class method wrapper
////////////////////////////////////////////////////////////////////////

// Add the wrapper member function to the module.
#define GAPBIND14_ADD_INIT_MEM_FUNC(module, class_var, gap_func_var)           \
  void GAPBIND14_CONCAT3(gapbind14_init_, class_var, gap_func_var)(            \
      gapbind14::Module & gapbind14_module) {                                  \
    gapbind14_module.add_mem_func(GAPBIND14_TO_STRING(class_var),              \
                                  __FILE__,                                    \
                                  GAPBIND14_TO_STRING(gap_func_var),           \
                                  &GAPBIND14_CONCAT(class_var, gap_func_var)<>); \
  }                                                                            \
  GAPBIND14_UNIQUE_ID gapbind14_push_back(                                     \
      &GAPBIND14_CONCAT3(gapbind14_init_, class_var, gap_func_var));

// The repeated start of every method.
#define GAPBIND14_MEM_FN_START(module, class_var)                           \
  if (TNUM_OBJ(arg1) != gapbind14::T_GAPBIND14_OBJ) {                       \
    ErrorQuit(                                                              \
        "expected gapbind14 object but got %s!", (Int) TNAM_OBJ(arg1), 0L); \
  }                                                                         \
  if (module.subtype(arg1)                                                  \
      != module.subtype(GAPBIND14_TO_STRING(class_var))) {                  \
    ErrorQuit("expected %s but got %s!",                                    \
              (Int) GAPBIND14_TO_STRING(class_var),                         \
              (Int) module.name(arg1));                                     \
  }                                                                         \
  class_var* ptr = gapbind14::SubTypeSpec<class_var>::obj_cpp_ptr(arg1);

#define GAPBIND14_CLASS_MEM_FN(module, class_var, mem_fn_var, gap_fn_var)      \
  template <typename TFunctionType = decltype(&class_var::mem_fn_var),         \
            typename TSFINAE       = Obj>                                      \
  auto GAPBIND14_CONCAT(class_var, gap_fn_var)(Obj self, Obj arg1)             \
      ->std::enable_if_t<gapbind14::returns_void<TFunctionType>::value         \
                             && gapbind14::arg_count<TFunctionType>::value     \
                                    == 0,                                      \
                         TSFINAE> {                                            \
    GAPBIND14_MEM_FN_START(module, class_var);                                 \
    GAPBIND14_TRY(                                                             \
        gapbind14::CppFunction<TFunctionType>()(&class_var::mem_fn_var, ptr)); \
    return 0L;                                                                 \
  }                                                                            \
  template <typename TFunctionType = decltype(&class_var::mem_fn_var),         \
            typename TSFINAE       = Obj>                                      \
  auto GAPBIND14_CONCAT(class_var, gap_fn_var)(Obj self, Obj arg1)             \
      ->std::enable_if_t<!gapbind14::returns_void<TFunctionType>::value        \
                             && gapbind14::arg_count<TFunctionType>::value     \
                                    == 0,                                      \
                         TSFINAE> {                                            \
    GAPBIND14_MEM_FN_START(module, class_var);                                 \
    using to_gap_type = gapbind14::to_gap<                                     \
        typename gapbind14::CppFunction<TFunctionType>::return_type>;          \
    GAPBIND14_TRY(                                                             \
        return to_gap_type()(gapbind14::CppFunction<TFunctionType>()(          \
            &class_var::mem_fn_var, ptr)));                                    \
  }                                                                            \
  GAPBIND14_ADD_INIT_MEM_FUNC(module, class_var, gap_fn_var)

////////////////////////////////////////////////////////////////////////
// Typdefs for GAP
////////////////////////////////////////////////////////////////////////

typedef Obj (*GVarFunc)(/*arguments*/);

namespace gapbind14 {
  ////////////////////////////////////////////////////////////////////////
  // Typdefs
  ////////////////////////////////////////////////////////////////////////

  using gapbind14_sub_type = size_t;

  ////////////////////////////////////////////////////////////////////////
  // Constants
  ////////////////////////////////////////////////////////////////////////

  extern UInt T_GAPBIND14_OBJ;

  ////////////////////////////////////////////////////////////////////////
  // Function templates
  ////////////////////////////////////////////////////////////////////////

  // Convert object to string
  template <typename T> std::string to_string(T const& n) {
    std::ostringstream stm;
    stm << n;
    return stm.str();
  }

  // Count number of params in non-const class member function
  template <class C, typename R, typename... Types>
  constexpr std::integral_constant<unsigned, sizeof...(Types)> meth_arg_count(
      R (C::*f)(Types...)) {
    return std::integral_constant<unsigned, sizeof...(Types)>{};
  }

  // Count number of params in const class member function
  template <class C, typename R, typename... Types>
  constexpr std::integral_constant<unsigned, sizeof...(Types)> meth_arg_count(
      R (C::*f)(Types...) const) {
    return std::integral_constant<unsigned, sizeof...(Types)>{};
  }

  // Class for calling a function or method, for SNIFAE
  template <typename TSignature, typename = void> struct Caller {};

  template <typename TClass, typename TReturnType, typename... TArgs>
  struct Caller<TReturnType (TClass::*)(TArgs...),
                std::enable_if_t<sizeof...(TArgs) != 0>> {
    TReturnType operator()(TReturnType (TClass::*mem_fn)(TArgs...),
                           TClass* ptr,
                           TArgs... args) {
      return (ptr->*mem_fn)(args...);
    }
    void operator()(TReturnType (TClass::*)(TArgs...), TClass*) {}
  };

  template <typename TClass, typename TReturnType>
  struct Caller<TReturnType (TClass::*)()> {
    TReturnType operator()(TReturnType (TClass::*mem_fn)(), TClass* ptr) {
      return (ptr->*mem_fn)();
    }
  };

  template <typename TClass, typename TReturnType, typename... TArgs>
  struct Caller<TReturnType (TClass::*)(TArgs...) const,
                std::enable_if_t<sizeof...(TArgs) != 0>> {
    TReturnType operator()(TReturnType (TClass::*mem_fn)(TArgs...) const,
                           TClass* ptr,
                           TArgs... args) {
      return (ptr->*mem_fn)(args...);
    }
    void operator()(TReturnType (TClass::*)(TArgs...), TClass*) {}
  };

  template <typename TClass, typename TReturnType>
  struct Caller<TReturnType (TClass::*)() const> {
    TReturnType operator()(TReturnType (TClass::*mem_fn)() const, TClass* ptr) {
      return (ptr->*mem_fn)();
    }
  };

  ////////////////////////////////////////////////////////////////////////
  // Overloading
  ////////////////////////////////////////////////////////////////////////

  template <typename... TArgs> struct overload_cast_impl {
    constexpr overload_cast_impl() {}

    template <typename TReturn>
    constexpr auto operator()(TReturn (*pf)(TArgs...)) const noexcept
        -> decltype(pf) {
      return pf;
    }

    template <typename TReturn, typename TClass>
    constexpr auto operator()(TReturn (TClass::*pmf)(TArgs...),
                              std::false_type = {}) const noexcept
        -> decltype(pmf) {
      return pmf;
    }

    template <typename TReturn, typename TClass>
    constexpr auto operator()(TReturn (TClass::*pmf)(TArgs...) const,
                              std::true_type) const noexcept -> decltype(pmf) {
      return pmf;
    }
  };

  template <typename... TArgs>
  static constexpr overload_cast_impl<TArgs...> overload_cast = {};

  ////////////////////////////////////////////////////////////////////////
  // Function return type and parameter type info
  ////////////////////////////////////////////////////////////////////////

  // For parameter packs . . .
  template <typename... TArgs> struct Pack {
    template <size_t i>
    using get = std::tuple_element_t<i, std::tuple<TArgs...>>;
  };

  template <typename TReturnType, typename... TArgs> struct CppFunctionBase {
    using arg_count   = std::integral_constant<unsigned, sizeof...(TArgs)>;
    using return_type = TReturnType;
    using params_type = Pack<TArgs...>;

    // Function pointers . . .
    template <typename SNIFAE = TReturnType>
    auto operator()(TReturnType (*f)(), TArgs... args)
        -> std::enable_if_t<sizeof...(TArgs) == 0, SNIFAE> {
      return f();
    }

    template <typename SNIFAE = TReturnType>
    auto operator()(TReturnType (*f)(TArgs...), TArgs... args)
        -> std::enable_if_t<sizeof...(TArgs) != 0, SNIFAE> {
      return f(args...);
    }
  };

  // Member functions . . .
  template <typename TClass, typename TReturnType, typename... TArgs>
  struct CppMemFnBase {
    using arg_count   = std::integral_constant<unsigned, sizeof...(TArgs)>;
    using return_type = TReturnType;
    using params_type = Pack<TArgs...>;
    using class_type  = TClass;

    template <typename SNIFAE = TReturnType>
    auto operator()(TReturnType (TClass::*mem_fn)(TArgs...),
                    TClass* ptr,
                    TArgs... args)
        -> std::enable_if_t<sizeof...(TArgs) != 0, SNIFAE> {
      return (ptr->*mem_fn)(args...);
    }

    template <typename SNIFAE = TReturnType>
    auto operator()(TReturnType (TClass::*mem_fn)(TArgs...),
                    TClass* ptr,
                    TArgs... args)
        -> std::enable_if_t<sizeof...(TArgs) == 0, SNIFAE> {
      return (ptr->*mem_fn)();
    }

    template <typename SNIFAE = TReturnType>
    auto operator()(TReturnType (TClass::*mem_fn)(TArgs...) const,
                    TClass* ptr,
                    TArgs... args)
        -> std::enable_if_t<sizeof...(TArgs) != 0, SNIFAE> {
      return (ptr->*mem_fn)(args...);
    }

    template <typename SNIFAE = TReturnType>
    auto operator()(TReturnType (TClass::*mem_fn)(TArgs...) const,
                    TClass* ptr,
                    TArgs... args)
        -> std::enable_if_t<sizeof...(TArgs) == 0, SNIFAE> {
      return (ptr->*mem_fn)();
    }
  };

  // Base declaration . . .
  template <typename TSignature> struct CppFunction {};

  // Free functions . . .
  template <typename TReturnType, typename... TArgs>
  struct CppFunction<TReturnType(TArgs...)>
      : CppFunctionBase<TReturnType, TArgs...> {};

  // Function pointers . . .
  template <typename TReturnType, typename... TArgs>
  struct CppFunction<TReturnType (*)(TArgs...)>
      : CppFunctionBase<TReturnType, TArgs...> {};

  // Member functions . . .
  template <typename TClass, typename TReturnType, typename... TArgs>
  struct CppFunction<TReturnType (TClass::*)(TArgs...)>
      : CppMemFnBase<TClass, TReturnType, TArgs...> {};

  // Const member functions
  template <typename TClass, typename TReturnType, typename... TArgs>
  struct CppFunction<TReturnType (TClass::*)(TArgs...) const>
      : CppMemFnBase<TClass, TReturnType, TArgs...> {};

  // For convenience . . .
  template <typename TFunctionType>
  using returns_void
      = std::is_void<typename CppFunction<TFunctionType>::return_type>;

  template <typename TFunctionType>
  using arg_count = typename CppFunction<TFunctionType>::arg_count;

  ////////////////////////////////////////////////////////////////////////
  // Subtype class - for polymorphism
  ////////////////////////////////////////////////////////////////////////

  class Subtype {
   public:
    Subtype(std::string nm, gapbind14_sub_type sbtyp)
        : _name(nm), _subtype(sbtyp) {
      static std::unordered_set<gapbind14_sub_type> defined;
      if (defined.find(sbtyp) != defined.end()) {
        throw std::runtime_error("somethng");
      } else {
        defined.insert(sbtyp);
      }
    }

    virtual ~Subtype() {}

    std::string const& name() const {
      return _name;
    }

    static gapbind14_sub_type obj_subtype(Obj o) {
      return reinterpret_cast<gapbind14_sub_type>(ADDR_OBJ(o)[2]);
    }

    gapbind14_sub_type obj_subtype() {
      return _subtype;
    }

    static void save(Obj o) {
      SaveUInt(obj_subtype(o));
      SaveSubObj(obj_gap_args(o));
    }

    static Obj obj_gap_args(Obj o) {
      return ADDR_OBJ(o)[0];
    }

    virtual void load(Obj o) = 0;
    virtual void free(Obj o) = 0;

   private:
    std::string        _name;
    gapbind14_sub_type _subtype;
  };

  ////////////////////////////////////////////////////////////////////////
  // SubtypeSpec class
  ////////////////////////////////////////////////////////////////////////

  template <typename TClass> struct SubTypeSpec : public Subtype {
    SubTypeSpec(std::string nm, gapbind14_sub_type sbtyp)
        : Subtype(nm, sbtyp) {}

    template <typename TToCpp> Obj create_obj(Obj args) {
      Obj o          = NewBag(T_GAPBIND14_OBJ, 3 * sizeof(Obj));
      ADDR_OBJ(o)[0] = args;
      ADDR_OBJ(o)[1] = reinterpret_cast<Obj>(new TClass(TToCpp()(args)));
      ADDR_OBJ(o)[2] = reinterpret_cast<Obj>(obj_subtype());
      CHANGED_BAG(o);
      return o;
    }

    static TClass* obj_cpp_ptr(Obj o) {
      // if (ADDR_OBJ(o)[1] == nullptr) {// TODO remove this if-clause
      //  ADDR_OBJ(o)
      //  [1] = reinterpret_cast<Obj>(new TClass(TToCpp()(obj_gap_args(o))));
      //}
      return reinterpret_cast<TClass*>(ADDR_OBJ(o)[1]);
    }

    void free(Obj o) override {
      GAPBIND14_ASSERT();
      delete reinterpret_cast<TClass*>(ADDR_OBJ(o)[1]);
    }

    void load(Obj o) override {
      GAPBIND14_ASSERT(obj_subtype(o) == obj_subtype());
      // Cannot actually reinstantiate the cpp object because its args might
      // not have been loaded yet (just the place where they are supposed to be
      // is returned by LoadSubObj().
      ADDR_OBJ(o)[0] = LoadSubObj();  // args
      ADDR_OBJ(o)[1] = nullptr;       // cpp class instance pointer
    }
  };

  ////////////////////////////////////////////////////////////////////////
  // Module class
  ////////////////////////////////////////////////////////////////////////

  class Module {
   public:
    explicit Module(std::string module_name)
        : _funcs(), _mem_funcs(), _module_name(module_name), _next_subtype(0) {}
    // delete other constructors

    void print(Obj o) {
      Pr("<class %s at %s>",
         (Int)(_subtypes.at(Subtype::obj_subtype(o))->name().c_str()),
         (Int) to_string(o).c_str());
    }

    void save(Obj o) {
      Subtype::save(o);
    }

    gapbind14_sub_type subtype(std::string const& subtype_name) const {
      // FIXME handle the case when there is no such subtype
      return _subtype_names.find(subtype_name)->second;
    }

    const char* name(Obj o) {
      // TODO assert
      gapbind14_sub_type sbtyp = Subtype::obj_subtype(o);
      return _subtypes.at(sbtyp)->name().c_str();
    }

    gapbind14_sub_type subtype(Obj o) {
      return Subtype::obj_subtype(o);
    }

    void load(Obj o) {
      gapbind14_sub_type sbtyp = LoadUInt();
      _subtypes.at(sbtyp)->load(o);
    }

    void free(Obj o) {
      gapbind14_sub_type sbtyp = Subtype::obj_subtype(o);
      _subtypes.at(sbtyp)->free(o);
    }

    template <typename TClass, typename TToCpp>
    Obj create(std::string const& subtype_name, Obj arg1) {
      return static_cast<SubTypeSpec<TClass>*>(
                 _subtypes.at(subtype(subtype_name)))
          ->template create_obj<TToCpp>(arg1);
    }

    const char* module_name() const {
      return _module_name.c_str();
    }

    StructGVarFunc const* funcs() const {
      return &_funcs[0];
    }

    StructGVarFunc const* mem_funcs(std::string const& nm) const {
      return &_mem_funcs[subtype(nm)][0];
    }

    template <typename TClass> gapbind14_sub_type add_subtype(std::string nm) {
      _subtype_names.insert(std::make_pair(nm, _next_subtype));
      _subtypes.push_back(new SubTypeSpec<TClass>(nm, _next_subtype));
      _next_subtype++;
      _mem_funcs.push_back(std::vector<StructGVarFunc>());
      return _subtypes.back()->obj_subtype();
    }

    template <typename... TArgs>
    void add_func(std::string        fnm,
                  std::string const& nm,
                  Obj (*func)(TArgs...)) {
      static_assert(sizeof...(TArgs) > 0,
                    "there must be at least 1 parameter: Obj self");
      GAPBIND14_ASSERT(sizeof...(TArgs) <= 7);
      _funcs.push_back({copy_c_str(nm),
                        sizeof...(TArgs) - 1,
                        params(sizeof...(TArgs) - 1),
                        (GVarFunc) func,
                        copy_c_str(fnm + ":Func" + nm)});
    }

    template <typename... TArgs>
    void add_mem_func(std::string const& sbtyp,
                      std::string        flnm,
                      std::string const& nm,
                      Obj (*func)(TArgs...)) {
      static_assert(sizeof...(TArgs) > 1,
                    "there must be at least 1 parameter: Obj self, Obj arg1");
      GAPBIND14_ASSERT(sizeof...(TArgs) <= 7);
      _mem_funcs.at(subtype(sbtyp))
          .push_back({copy_c_str(nm),
                      sizeof...(TArgs) - 1,
                      params(sizeof...(TArgs) - 1),
                      (GVarFunc) func,
                      copy_c_str(flnm + ":Func" + sbtyp + "::" + nm)});
    }

    void finalize() {
      for (auto& x : _mem_funcs) {
        x.push_back(StructGVarFunc({0, 0, 0, 0, 0}));
      }
      _funcs.push_back(StructGVarFunc({0, 0, 0, 0, 0}));
    }

    std::vector<Subtype*>::const_iterator begin() const {
      return _subtypes.cbegin();
    }

    std::vector<Subtype*>::const_iterator end() const {
      return _subtypes.cend();
    }

   private:
    static char const* copy_c_str(std::string const& str) {
      char* out = new char[str.size() + 1];  // we need extra char for NUL
      memcpy(out, str.c_str(), str.size() + 1);
      return out;
    }

    char const* params(size_t nr) {
      GAPBIND14_ASSERT(nr <= 6);
      if (nr == 0) {
        return "";
      }
      static std::string params = "arg1, arg2, arg3, arg4, arg5, arg6";
      std::string source(params.cbegin(), params.cbegin() + (nr - 1) * 6);
      source += std::string(params.cbegin() + (nr - 1) * 6,
                            params.cbegin() + (nr - 1) * 6 + 4);
      return copy_c_str(source);
    }

    std::vector<StructGVarFunc>                         _funcs;
    std::vector<std::vector<StructGVarFunc>>            _mem_funcs;
    std::string                                         _module_name;
    gapbind14_sub_type                                  _next_subtype;
    std::unordered_map<std::string, gapbind14_sub_type> _subtype_names;
    std::vector<Subtype*>                               _subtypes;
  };

  ////////////////////////////////////////////////////////////////////////
  // Forward declaration
  ////////////////////////////////////////////////////////////////////////

  Module GAPBIND14_MODULE_IMPL();

}  // namespace gapbind14
#endif  // GAPBIND14_SRC_GAPBIND14_HPP_
